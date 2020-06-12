//DON'T USE! NOT FUNCTIONAL, FAILS COMPILE

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "py/mpconfig.h"
#include "py/nlr.h"
#include "py/misc.h"
#include "py/qstr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mpprint.h"

#include "uart.h"
#include "rwip.h"
#include "arch_system.h"
// #include "arch_console.h"
#include "arch.h"
// #include "gpio.h"
// #include "da_gpio.h"
#include "uart_utils.h"
#include "user_periph_setup.h"
// #include "user_callback_config.h"

const mp_obj_type_t BLE_type;


#include "ke_task.h"
#include "ke_msg.h"

#include "arch_api.h"

#include "app_task.h"

#define arch_printf_process() {}

extern last_ble_evt arch_rwble_last_event;
extern void read_rcx_freq(uint16_t cal_time);
extern bool ea_sleep_check(uint32_t *sleep_duration, uint32_t wakeup_delay);
extern const struct arch_main_loop_callbacks user_app_main_loop_callbacks;

// /* Specifies the message handlers that are common to all states. */
// struct ke_state_handler app_default_handler = KE_STATE_HANDLER(app_default_state);

// /* Defines the place holder for the states of all the task instances. */
// ke_state_t app_state[APP_IDX_MAX] __SECTION_ZERO("retention_mem_area0"); //RETENTION MEMORY

// static struct ke_task_desc TASK_DESC_APP = {NULL,
//                                             &app_default_handler,
//                                             app_state,
//                                             APP_STATE_MAX,
//                                             APP_IDX_MAX};


// #############################################################
// BLE funcs from arch_main.c

/**
 ****************************************************************************************
 * @brief Check if the BLE module is powered on.
 * @return void
 ****************************************************************************************
 */
static inline bool ble_is_powered()
{
    return ((GetBits16(CLK_RADIO_REG, BLE_ENABLE) == 1) &&
            (GetBits32(BLE_DEEPSLCNTL_REG, DEEP_SLEEP_STAT) == 0) &&
            !(rwip_prevent_sleep_get() & RW_WAKE_UP_ONGOING));
}

/**
 ****************************************************************************************
 * @brief Used for sending messages to kernel tasks generated from
 *        asynchronous events that have been processed in app_asynch_proc.
 * @return KEEP_POWERED to force calling of schedule_while_ble_on(), else GOTO_SLEEP
 ****************************************************************************************
 */
static inline arch_main_loop_callback_ret_t app_asynch_trm(void)
{
    if (user_app_main_loop_callbacks.app_on_ble_powered != NULL)
    {
        return user_app_main_loop_callbacks.app_on_ble_powered();
    }
    else
    {
        return GOTO_SLEEP;
    }
}

/**
 ****************************************************************************************
 * @brief Call the scheduler if the BLE module is powered on.
 * @return void
 ****************************************************************************************
 */

static inline void schedule_while_ble_on(void)
{
    // BLE clock is enabled
    while (ble_is_powered())
    {
        //execute messages and events
        rwip_schedule();
#if defined (__DA14531__)
        rcx20_read_freq(false);
#endif

#if !defined(__FPGA__)
        if (arch_rwble_last_event == BLE_EVT_END)
        {
#if defined (__DA14531__)
            rcx20_read_freq(true);
#else
            rcx20_read_freq();
#endif

            uint32_t sleep_duration = 0;
            //if you have enough time run a temperature calibration of the radio
            if (ea_sleep_check(&sleep_duration, 4)) //6 slots -> 3.750 ms
            {
                // check time and temperature to run radio calibrations.
                conditionally_run_radio_cals();
            }
        }
#endif

        //grant control to the application, try to go to sleep
        //if the application returns GOTO_SLEEP
        if (app_asynch_trm() != GOTO_SLEEP)
        {
            continue; // so that rwip_schedule() is called again
        }
        else
        {
            arch_printf_process();
            break;
        }
    }
}


// // #############################################################
// // Type definitions


// struct mnf_specific_data_ad_structure
// {
//     uint8_t ad_structure_size;
//     uint8_t ad_structure_type;
//     uint8_t company_id[APP_AD_MSD_COMPANY_ID_LEN];
//     uint8_t proprietary_data[APP_AD_MSD_DATA_LEN];
// };


// // #############################################################
// // GLOBAL VARIABLE DEFINITIONS


// uint8_t app_connection_idx                    __SECTION_ZERO("retention_mem_area0"); // @RETENTION MEMORY
// timer_hnd app_adv_data_update_timer_used      __SECTION_ZERO("retention_mem_area0"); // @RETENTION MEMORY
// timer_hnd app_param_update_request_timer_used __SECTION_ZERO("retention_mem_area0"); // @RETENTION MEMORY

// // Retained variables
// // Manufacturer Specific Data
// struct mnf_specific_data_ad_structure mnf_data  __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
// // Index of manufacturer data in advertising data or scan response data (when MSB is 1)
// uint8_t mnf_data_index                          __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
// uint8_t stored_adv_data_len                     __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
// uint8_t stored_scan_rsp_data_len                __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
// uint8_t stored_adv_data[ADV_DATA_LEN]           __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY
// uint8_t stored_scan_rsp_data[SCAN_RSP_DATA_LEN] __SECTION_ZERO("retention_mem_area0"); //@RETENTION MEMORY


// // #############################################################
// // dependant funcs

// /**
//  ****************************************************************************************
//  * @brief Initialize Manufacturer Specific Data
//  * @return void
//  ****************************************************************************************
//  */
// static void mnf_data_init()
// {
//     mnf_data.ad_structure_size = sizeof(struct mnf_specific_data_ad_structure ) - sizeof(uint8_t); // minus the size of the ad_structure_size field
//     mnf_data.ad_structure_type = GAP_AD_TYPE_MANU_SPECIFIC_DATA;
//     mnf_data.company_id[0] = APP_AD_MSD_COMPANY_ID & 0xFF; // LSB
//     mnf_data.company_id[1] = (APP_AD_MSD_COMPANY_ID >> 8 )& 0xFF; // MSB
//     mnf_data.proprietary_data[0] = 0;
//     mnf_data.proprietary_data[1] = 0;
// }

// /**
//  ****************************************************************************************
//  * @brief Update Manufacturer Specific Data
//  * @return void
//  ****************************************************************************************
//  */
// static void mnf_data_update()
// {
//     uint16_t data;

//     data = mnf_data.proprietary_data[0] | (mnf_data.proprietary_data[1] << 8);
//     data += 1;
//     mnf_data.proprietary_data[0] = data & 0xFF;
//     mnf_data.proprietary_data[1] = (data >> 8) & 0xFF;

//     if (data == 0xFFFF) {
//          mnf_data.proprietary_data[0] = 0;
//          mnf_data.proprietary_data[1] = 0;
//     }
// }

// void user_app_init(void)
// {
//     app_param_update_request_timer_used = EASY_TIMER_INVALID_TIMER;

//     // Initialize Manufacturer Specific Data
//     mnf_data_init();

//     // Initialize Advertising and Scan Response Data
//     memcpy(stored_adv_data, USER_ADVERTISE_DATA, USER_ADVERTISE_DATA_LEN);
//     stored_adv_data_len = USER_ADVERTISE_DATA_LEN;
//     memcpy(stored_scan_rsp_data, USER_ADVERTISE_SCAN_RESPONSE_DATA, USER_ADVERTISE_SCAN_RESPONSE_DATA_LEN);
//     stored_scan_rsp_data_len = USER_ADVERTISE_SCAN_RESPONSE_DATA_LEN;

//     // default_app_on_init();
//     app_set_prf_srv_perm(TASK_ID_CUSTS1, APP_CUSTS1_SEC_REQ);
//     app_set_prf_srv_perm(TASK_ID_SUOTAR, APP_SUOTA_SEC_REQ);
//     // Fetch bond data from the external memory
//     app_easy_security_bdb_init();
// }


// #############################################################
//BLE.BLE

STATIC mp_obj_t mp_schedule_while_ble_on()
{
    schedule_while_ble_on();
    return mp_const_none;
}


STATIC MP_DEFINE_CONST_FUN_OBJ_0(schedule_ble, mp_schedule_while_ble_on);

typedef struct _mymodule_hello_obj_t {
    // base represents some basic information, like type
    mp_obj_base_t base;
    // a member created by us
    uint8_t pin;
    uint8_t port;
} ble_module_vars;

//Obj init
STATIC mp_obj_t BLE_make_new( const mp_obj_type_t *type,
                                  size_t n_args,
                                  size_t n_kw,
                                  const mp_obj_t *args ) {
    // this checks the number of arguments (min 1, max 1);
    // on error -> raise python exception
    mp_arg_check_num(n_args, n_kw, 1, 1, true);
    // create a new object of our C-struct type
    ble_module_vars *self = m_new_obj(ble_module_vars);
    // give it a type
    self->base.type = &BLE_type;

    return MP_OBJ_FROM_PTR(self);
}

//print(obj)
STATIC void ble_obj_print( const mp_print_t *print,
                                  mp_obj_t self_in,
                                  mp_print_kind_t kind ) {
    printf_string(UART1, "BLE OBJ");
}

// creating the table of global members
STATIC const mp_rom_map_elem_t BLE_locals_dict_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_BLE) },

    //Constants

    //Funcs
    { MP_OBJ_NEW_QSTR(MP_QSTR_schedule_ble), (mp_obj_t)&schedule_ble }

};
STATIC MP_DEFINE_CONST_DICT(BLE_locals_dict,
                            BLE_locals_dict_table);

// create the class-object itself
const mp_obj_type_t BLE_type = {
    // "inherit" the type "type"
    { &mp_type_type },
     // give it a name
    .name = MP_QSTR_bleObj,
     // give it a print-function
    .print = ble_obj_print,
     // give it a constructor
    .make_new = BLE_make_new,
     // and the global members
    .locals_dict = (mp_obj_dict_t*)&BLE_locals_dict,
};


// #############################################################
// BLE

// class funcs
STATIC mp_obj_t BLE_hello(void) {
    printf_string(UART1, "I'm BLE!\n\r");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(BLE_hello_obj, BLE_hello);


extern void default_advertise_operation(void);

STATIC mp_obj_t default_advertise(void) {
    // printf_string(UART1, "Advertising!\n\r");
    default_advertise_operation();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(BLE_default_adv, default_advertise);


//table of class funcs & vars
STATIC const mp_map_elem_t ble_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_BLE) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_hello), (mp_obj_t)&BLE_hello_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_adv), (mp_obj_t)&BLE_default_adv },
    { MP_OBJ_NEW_QSTR(MP_QSTR_BLE), (mp_obj_t)&BLE_type },
};

STATIC MP_DEFINE_CONST_DICT ( mp_module_ble_globals, ble_globals_table );

//module
const mp_obj_module_t mp_module_ble = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ble_globals,
};