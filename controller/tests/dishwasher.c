#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#include <dishwasher.h>
#include <dishwasher/error.h>

void test_dishwasher_init_success(void){
    assert(dishwasher_init() == 0);
}

void test_dishwasher_uninit_success(void){

    dishwasher_init();
    assert(dishwasher_uninit() == 0);
    assert(dishwasher_init() == 0);
    dishwasher_uninit();
}

void test_dishwasher_get_machine_info_success(void){
    dishwasher_info_t m_info = {0};

    dishwasher_init();
    assert(dishwasher_get_machine_info(&m_info) == 0);
    assert(strcmp(m_info.machine_type, "Octomat Pro IV\0") == 0);
    assert(strcmp(m_info.hw_version, "Octo Base 2 Rev. 3\0") == 0);
    assert(strcmp(m_info.fw_version,"01.0.15\0") == 0);
    assert(strcmp(m_info.serial,"01987639\0") == 0);
    dishwasher_uninit();
}

void test_dishwasher_get_status_success(void){
    dishwasher_status_t m_status = {0};

    dishwasher_init();
    dishwasher_abort_program();
    assert(dishwasher_get_status(&m_status) == 0);
    assert(m_status.program == DW_PROG_INVALID);
    assert(m_status.step == DW_STEP_INVALID);
    assert(m_status.state == DW_STATE_IDLE);
    assert(m_status.warnings.salt_level_low == false);
    assert(m_status.warnings.rinse_agent_level_low == false);
    assert(m_status.errors.pump_jammed == false);
    assert(m_status.errors.water_supply_closed == false);
    dishwasher_uninit();
}

void test_dishwasher_start_program_success(void){
    dishwasher_status_t m_status = {0};

    dishwasher_init();
    dishwasher_abort_program();
    dishwasher_start_program(DW_PROG_50DEG_ECO);
    dishwasher_get_status(&m_status);
    assert(m_status.program == DW_PROG_50DEG_ECO);
    assert(m_status.step == DW_STEP_PRESOAK);
    assert(m_status.state == DW_STATE_PROG_RUNNING);
    assert(m_status.warnings.salt_level_low == false);
    assert(m_status.warnings.rinse_agent_level_low == false);
    assert(m_status.errors.pump_jammed == false);
    assert(m_status.errors.water_supply_closed == false);
    dishwasher_uninit();
}

void test_dishwasher_abort_program_success(void){
    dishwasher_status_t m_status = {0};

    dishwasher_init();
    dishwasher_start_program(DW_PROG_50DEG_ECO);
    dishwasher_get_status(&m_status);
    dishwasher_abort_program();
    dishwasher_get_status(&m_status);
    assert(m_status.program == DW_PROG_INVALID);
    assert(m_status.step == DW_STEP_INVALID);
    assert(m_status.state == DW_STATE_IDLE);
    assert(m_status.warnings.salt_level_low == false);
    assert(m_status.warnings.rinse_agent_level_low == false);
    assert(m_status.errors.pump_jammed == false);
    assert(m_status.errors.water_supply_closed == false);
    dishwasher_uninit();
}


int main(){
    test_dishwasher_init_success();
    test_dishwasher_uninit_success();
    test_dishwasher_get_machine_info_success();
    test_dishwasher_get_status_success();
    test_dishwasher_start_program_success();
    test_dishwasher_abort_program_success();
    printf("ALL TESTS PASSED!\n");
}