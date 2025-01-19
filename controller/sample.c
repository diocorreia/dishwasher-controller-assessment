#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dishwasher.h>

static void get_machine_info(void){
    int rc = 0;
    dishwasher_info_t info = {0};

    if((rc = dishwasher_get_machine_info(&info)) < 0){
        printf("Failed to get machine info. ERR(%d)\n", rc);
        return;
    };

    printf("Machine type:\t\t%s\n", info.machine_type);
    printf("Hardware version:\t%s\n", info.hw_version);
    printf("Firmware version:\t%s\n", info.fw_version);
    printf("Serial number:\t\t%s\n", info.serial);
}

static void get_status(void){
    int rc = 0;
    dishwasher_status_t status = {0};

    if((rc = dishwasher_get_status(&status)) < 0){
        printf("Failed to get machine status. ERR(%d)\n", rc);
        return;
    };

    printf("Current program:\t");
    switch(status.program){
        case DW_PROG_INVALID:
            printf("PROG_INVALID");
            break;
        case DW_PROG_50DEG_ECO:
            printf("PROG_50DEG_ECO");
            break;
        case DW_PROG_60DEG_NORMAL:
            printf("PROG_60DEG_NORMAL");
            break;
        case DW_PROG_60DEG_HIGH:
            printf("PROG_60DEG_HIGH");
            break;
        case DW_PROG_70DEG_HIGH:
            printf("PROG_70DEG_HIGH");
            break;
        default:
            printf("UNKOWN");
            break;
    }
    printf("\n");

    printf("Current step:\t\t");
    switch(status.step){
        case DW_STEP_INVALID:
            printf("STEP_INVALID");
            break;
        case DW_STEP_PRESOAK:
            printf("STEP_PRESOAK");
            break;
        case DW_STEP_WARMUP:
            printf("STEP_WARMUP");
            break;
        case DW_STEP_MAIN_WASH:
            printf("STEP_MAIN_WASH");
            break;
        case DW_STEP_CLEAN_WASH:
            printf("STEP_CLEAN_WASH");
            break;
        case DW_STEP_DRY:
            printf("STEP_DRY");
            break;
        default:
            printf("UNKOWN");
            break;
    }
    printf("\n");

    printf("Current state:\t\t");
    switch(status.state){
        case DW_STATE_IDLE:
            printf("STATE_IDLE");
            break;
        case DW_STATE_PROG_RUNNING:
            printf("STATE_PROG_RUNNING");
            break;
        case DW_STATE_ERROR:
            printf("STATE_ERROR");
            break;
        default:
            printf("UNKOWN");
            break;
    }
    printf("\n");

    printf("Warnings:\t\t");
    if(status.warnings.rinse_agent_level_low){
        printf("RINSE_AGENT_LOW ");
        if(status.warnings.salt_level_low){
            printf(", ");
        }
    }
    if(status.warnings.salt_level_low){
        printf("SALT_LEVEL_LOW ");
    }
    printf("\n");

    printf("Errors:\t\t\t");
    if(status.errors.pump_jammed){
        printf("PUMP_JAMMED");
        if(status.errors.water_supply_closed){
            printf(", ");
        }
    }
    if(status.errors.water_supply_closed){
        printf("WATER_SUPPLY_CLOSED ");
    }
    printf("\n");
}

static void start_program(int program){
   dishwasher_start_program(program);
}

static void abort_program(void){
   dishwasher_abort_program();
}

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Too few arguments.\n");
        return -1;
    }

    if(dishwasher_init() < 0){
        printf("Failed to start dishwasher module.\n");
        return -1;
    };

    if(strcmp("info", argv[1]) == 0){
        get_machine_info();
    }

    if(strcmp("status", argv[1]) == 0){
        get_status();
    }

    if(strcmp("start", argv[1]) == 0 && argc > 2){
        int prog = 0;
        if(strcmp("50DEG_ECO", argv[2]) == 0 && argc == 3){
            prog = DW_PROG_50DEG_ECO;
        }else if(strcmp("60DEG_NORMAL", argv[2]) == 0 && argc == 3){
            prog = DW_PROG_60DEG_NORMAL;
        }else if(strcmp("60DEG_HIGH", argv[2]) == 0 && argc == 3){
            prog = DW_PROG_60DEG_HIGH;
        }else if(strcmp("70DEG_HIGH", argv[2]) == 0 && argc == 3){
            prog = DW_PROG_70DEG_HIGH;
        }else{
            printf("Invalid program. Valid values: 50DEG_ECO, 60DEG_NORMAL, 60DEG_HIGH, 70DEG_HIGH\n");
        }
        start_program(prog);
    } else if(strcmp("start", argv[1]) == 0) {
            printf("Invalid program. Valid values: 50DEG_ECO, 60DEG_NORMAL, 60DEG_HIGH, 70DEG_HIGH\n");
    }

    if(strcmp("abort", argv[1]) == 0){
        abort_program();
    }

    if(strcmp("-h", argv[1]) == 0){
        printf("Commands: start, abort, info, status\n");
    }

    dishwasher_uninit();

    return 0;
}