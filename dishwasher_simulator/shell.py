from prompt_toolkit import PromptSession
from prompt_toolkit.formatted_text import HTML
from dishwasher import Machine, Program, Warning, Error
from comProtocol import comProtocol

class Shell():

    def __init__(self, machine:Machine):
        self.session = PromptSession(message='dishwasher> ')
        self.machine = machine
        self.commands = {
            'start' : self.start,
            'program' : self.set_program,
            'next' : self.next_step,
            'n' : self.next_step,
            'restart' : self.restart_machine,
            'error' : self.set_error,
            'warning' : self.set_warning,
            'clear' : self.clear_warnings_errors,
            'info' : self.print_info,
            'help' : self.help,
            'h' : self.help
        }

    def get_toolbar(self):
        warnings = self.machine.getWarnings()
        errors = self.machine.getErrors()

        text = ' <b>Program: </b>'
        text += self.machine.program.name
        text += ' <b>Step: </b>'
        text += self.machine.step.name
        text += ' <b>State: </b>'
        text += self.machine.state.name
        text += ' <b><style bg="yellow">Warnings:</style></b> ['
        for e in warnings:
            text += e.name
            text += ', '
        if(len(warnings) > 0):
            text = text[:-2]
        text += ']'
        text += ' <b><style bg="red">Errors:</style></b> ['
        for e in errors:
            text += e.name
            text += ', '
        if(len(errors) > 0):
            text = text[:-2]
        text += ']'

        return HTML(text)

    async def run(self):
        while True:
            try:
                input = await self.session.prompt_async(bottom_toolbar=self.get_toolbar, refresh_interval=0.5)
            except (KeyboardInterrupt or EOFError):
                break
            else:
                ret = self.parse(input.split())
                if(ret == -1):
                    break
    
    def parse(self, input:list):

        if(len(input) == 0):
            self.help(None)
            return

        cmd = input[0]

        if(cmd == 'q' or cmd == 'quit'):
            return -1

        try:
            self.commands[cmd](input)
        except:
            print("Invalid command")

    def start(self, input:list):
        res = self.machine.start()
        if(res != 0):
            print("ERROR " + str(res))

    def set_program(self, input:list):
        try:
            program = Program[input[1]]
        except:
            options = [program.name for program in Program]
            options.pop(Program.INVALID.value)
            print("program " + str(options))
        else:
            return self.machine.setProgram(program)
        
    def next_step(self, input:list):
        res = self.machine.nextStep()
        if(res != 0):
            print("ERROR " + str(res))

    def restart_machine(self, input:list):
        self.machine.restart()

    def print_info(self, input:list):
        print(self.machine.getInfo())

    def set_error(self, input:list):
        try:
            error = Error[input[1]]
        except:
            options = [error.name for error in Error]
            options.pop(Error.INVALID.value)
            print("error " + str(options))
        else:
            return self.machine.setError(error)
    
    def set_warning(self, input:list):
        try:
            warning = Warning[input[1]]
        except:
            options = [warning.name for warning in Warning]
            options.pop(Warning.INVALID.value)
            print("warning " + str(options))
        else:
            return self.machine.setWarning(warning)
        
    def clear_warnings_errors(self, input:list):
        if(len(input) == 1):
            self.machine.clearAllErrors()
            self.machine.clearAllWarnings()
            return
        if(input[1] == 'errors'):
            self.machine.clearAllErrors()
        elif(input[1] == 'warnings'):
            self.machine.clearAllWarnings()

    def help(self, input:list):
        help = "\nCommand\t\t\t\tDescription\n" + \
        "================================================================\n"  + \
        "info\t\t\t\tPrints dishwasher info\n" + \
        "start\t\t\t\tStarts dishwasher program\n" + \
        "program\tECO_50DEG\t\tSets dishwasher program\n" + \
        "\tNORMAL_60DEG\n" + \
        "\tHIGH_60DEG\n" + \
        "\tHIGH_70DEG\n" + \
        "restart\t\t\t\tRestarts dishwasher\n" + \
        "error\tPUMP_JAMMED\t\tSets error\n" + \
        "\tWATER_SUPPLY_CLOSED\n" + \
        "warning\tSALT_LEVEL_LOW\t\tSets warning\n" + \
        "\tRINSE_AGENT_LOW\n" + \
        "clear\t\t\t\tClears all warnings and errors\n" + \
        "\twarnings\t\tClears all warnings\n" + \
        "\terrors\t\t\tClears all errors\n"
        print(help)
