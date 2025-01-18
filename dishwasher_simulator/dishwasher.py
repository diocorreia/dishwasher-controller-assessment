from enum import Enum, auto

MACHINE_TYPE = "Octomat Pro IV"
HW_VERSION = "Octo Base 2 Rev. 3"
FW_VERSION = "1.0.15"
SERIAL_NUMBER = "1987639"

class State(Enum):
    IDLE = 0
    RUNNING = 1
    ERROR = 2

class Program(Enum):
    INVALID = 0
    ECO_50DEG = 1
    NORMAL_60DEG = 2
    HIGH_60DEG = 3
    HIGH_70DEG = 4

class Step(Enum):
    INVALID = 0
    PRESOAK = 1
    WARMUP = 2
    MAIN_WASH = 3
    CLEAN_WASH = 4
    DRY = 5
    DONE = auto()

class Warning(Enum):
    SALT_LEVEL_LOW = 0
    RINSE_AGENT_LOW = 1
    INVALID = auto()

class Error(Enum):
    PUMP_JAMMED = 0
    WATER_SUPPLY_CLOSED = 1
    INVALID = auto()

class Machine:

    def __init__(self):

        self.type = MACHINE_TYPE
        self.hw_version = HW_VERSION
        self.fw_version = FW_VERSION
        self.serial = SERIAL_NUMBER

        self.program = Program.INVALID
        self.step = Step.INVALID
        self.state = State.IDLE

        self.warning_salt_level_low = False
        self.warning_rinse_agent_low = False

        self.__error_pump_jammed = False
        self.__error_water_supply_closed = False

        self.error_pump_jammed = False
        self.error_water_supply_closed = False

    def restart(self):

        self.program = Program.INVALID
        self.step = Step.INVALID
        self.state = State.IDLE

        self.error_pump_jammed = False
        self.error_water_supply_closed = False

        return 0

    def start(self):
        
        if(self.state != State.IDLE):
            return -1 

        if(self.program == Program.INVALID):
            return -2

        self.state = State.RUNNING

        self.setStep(Step.PRESOAK)

        return 0
    
    def setProgram(self, program:Program):
        if(self.state != State.IDLE):
            return -1         
        self.program = program
        return 0

    def nextStep(self):

        if(self.state != State.RUNNING):
            return -1
        
        self.checkErrors()

        self.step = Step(self.step.value + 1)

        if(self.step == Step.DONE):
            self.state = State.IDLE
            self.step = Step.INVALID

        return 0

    def setStep(self, step:Step):

        if(self.state == State.IDLE and self.program != Program.INVALID):
            # Puts the machine state to RUNNING. Otherwise, the user would have to manually call start().
            self.state = State.RUNNING

        if(self.state != State.RUNNING):
            return -1
        
        self.checkErrors()
        
        self.step = step

        return 0

    def setWarning(self, warning:Warning):

        if(warning == Warning.SALT_LEVEL_LOW):
            self.warning_salt_level_low = True
            return 0
        
        if(warning == Warning.RINSE_AGENT_LOW):
            self.warning_rinse_agent_low = True
            return 0

        return -1

    def clearWarning(self, warning:Warning):

        if(warning == Warning.SALT_LEVEL_LOW):
            self.warning_salt_level_low = False
            return 0
        
        if(warning == Warning.RINSE_AGENT_LOW):
            self.warning_rinse_agent_low = False
            return 0

        return -1

    def clearAllWarnings(self):
        self.warning_rinse_agent_low = False
        self.warning_salt_level_low = False
        return 0

    def setError(self, error:Error):

        if(error.value >= Error.INVALID.value):
            return -1

        if(error == Error.PUMP_JAMMED):
            self.__error_pump_jammed = True
        
        if(error == Error.WATER_SUPPLY_CLOSED):
            self.__error_water_supply_closed = True

        if(self.state == State.RUNNING):
            self.checkErrors()

        return 0
    
    def checkErrors(self):

        # Update error flags
        self.error_pump_jammed = self.__error_pump_jammed
        self.error_water_supply_closed = self.__error_water_supply_closed

        if(self.error_pump_jammed or self.error_water_supply_closed):
            self.state = State.ERROR
            return True
        
        return False

    def clearError(self, error:Error):

        if(error.value >= Error.INVALID.value):
            return -1

        if(error == Error.PUMP_JAMMED):
            self.__error_pump_jammed = False
        
        if(error == Error.WATER_SUPPLY_CLOSED):
            self.__error_water_supply_closed = False

        return 0
    
    def clearAllErrors(self):
        self.__error_pump_jammed = False
        self.__error_water_supply_closed = False
        return 0
    
    def getInfo(self):
        return [
            self.type,
            self.hw_version,
            self.fw_version,
            self.serial
        ]
    
    def getWarnings(self):
        warnings = []

        if(self.warning_rinse_agent_low):
            warnings.append(Warning.RINSE_AGENT_LOW)

        if(self.warning_salt_level_low):
            warnings.append(Warning.SALT_LEVEL_LOW)

        return warnings
    
    def getErrors(self):
        errors = []

        if(self.error_pump_jammed):
            errors.append(Error.PUMP_JAMMED)

        if(self.error_water_supply_closed):
            errors.append(Error.WATER_SUPPLY_CLOSED)

        return errors
        
    def getStatus(self):
        warnings = self.getWarnings()
        errors = self.getErrors()
        return {
            "program" : self.program,
            "step" : self.step,
            "state" : self.state,
            "warnings" : warnings,
            "errors" : errors
        }