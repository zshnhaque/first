/********************************************************************
 *                Copyright Simcom(shanghai)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   main_customer.c
 * version       :   0.10
 * Description   :   
 * Authors       :   maobin
 * Notes         :
 *---------------------------------------------------------------------
 *
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2012-09-24, maobin, create originally.
 *
 *--------------------------------------------------------------------
 * File Description
 * AT+CEAT=parma1,param2
 * param1 param2 
 *   1      1   Turn on PowerKey shut down and turn on KeyPad Led
 *   1      2   Turn off PowerKey shut down and turn off KeyPad Led
 *
 *--------------------------------------------------------------------
 ********************************************************************/
/********************************************************************
 * Include Files
 ********************************************************************/
#include <stdio.h>
#include <string.h>
#include "eat_modem.h"
#include "eat_interface.h"
#include "eat_uart.h"
#include "eat_timer.h" 
#include "key.h"
#include "eat_clib_define.h" //only in main.c

/********************************************************************
 * Macros
 ********************************************************************/

/********************************************************************
* Types
 ********************************************************************/
typedef void (*app_user_func)(void*);
/********************************************************************
 * Extern Variables (Extern /Global)
 ********************************************************************/
 
/********************************************************************
 * Local Variables:  STATIC
 ********************************************************************/
static const char * key_name[KEY_NUM] = {
    "0"
        ,"1"
        ,"2"
        ,"3"
        ,"4"
        ,"5"
        ,"6"
        ,"7"
        ,"8"
        ,"9"
        ,"STAR"
        ,"HASH"
        ,"UP"
        ,"DOWN"
        ,"LEFT"
        ,"RIGHT"
        ,"OK"
        ,"POWER"
};
/********************************************************************
* External Functions declaration
 ********************************************************************/
extern void APP_InitRegions(void);

/********************************************************************
* Local Function declaration
 ********************************************************************/
void app_main(void *data);
void app_func_ext1(void *data);
/********************************************************************
* Local Function
 ********************************************************************/
#pragma arm section rodata = "APP_CFG"
APP_ENTRY_FLAG  
#pragma arm section rodata

#pragma arm section rodata="APPENTRY"
	const EatEntry_st AppEntry = 
	{
		app_main,
		app_func_ext1,
		(app_user_func)EAT_NULL,//app_user1,
		(app_user_func)EAT_NULL,//app_user2,
		(app_user_func)EAT_NULL,//app_user3,
		(app_user_func)EAT_NULL,//app_user4,
		(app_user_func)EAT_NULL,//app_user5,
		(app_user_func)EAT_NULL,//app_user6,
		(app_user_func)EAT_NULL,//app_user7,
		(app_user_func)EAT_NULL,//app_user8,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL,
		EAT_NULL
	};
#pragma arm section rodata

void app_func_ext1(void *data)
{
	/*This function can be called before Task running ,configure the GPIO,uart and etc.
	   Only these api can be used:
		 eat_uart_set_debug: set debug port
		 eat_pin_set_mode: set GPIO mode
		 eat_uart_set_at: set AT port
	*/
	eat_uart_set_debug(EAT_UART_2);
    eat_uart_set_at_port(EAT_UART_1);// UART1 is as AT PORT

	eat_pin_set_mode(EAT_PIN20_COL0, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN25_COL1, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN22_COL2, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN21_COL3, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN24_COL4, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN62_ROW0, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN60_ROW1, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN61_ROW2, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN23_ROW3, EAT_PIN_MODE_KEY);
	eat_pin_set_mode(EAT_PIN63_ROW4, EAT_PIN_MODE_KEY);

}
eat_bool eat_modem_data_parse(u8* buffer, u16 len, u8* param1, u8* param2)
{
    eat_bool ret_val = EAT_FALSE;
    u8* buf_ptr = NULL;
    /*param:%d,extern_param:%d*/
     buf_ptr = (u8*)strstr((const char *)buffer,"param");
    if( buf_ptr != NULL)
    {
        sscanf((const char *)buf_ptr, "param:%d,extern_param:%d",(int*)param1, (int*)param2);
        eat_trace("data parse param1:%d param2:%d",*param1, *param2);
        ret_val = EAT_TRUE;
    }
    return ret_val;
}

static void key_test_handler(Key_enum key, eat_bool pressed)
{
    eat_trace("Key value = %s,Key press = %d",key_name[key],pressed);
}
static void key_reg_handle(void)
{
    key_register_hander(KEY_0, key_test_handler);
    key_register_hander(KEY_1, key_test_handler);
    key_register_hander(KEY_2, key_test_handler);
    key_register_hander(KEY_3, key_test_handler);
    key_register_hander(KEY_4, key_test_handler);
    key_register_hander(KEY_5, key_test_handler);
    key_register_hander(KEY_6, key_test_handler);
    key_register_hander(KEY_7, key_test_handler);
    key_register_hander(KEY_8, key_test_handler);
    key_register_hander(KEY_9, key_test_handler);
    key_register_hander(KEY_STAR, key_test_handler);
    key_register_hander(KEY_HASH, key_test_handler);
    key_register_hander(KEY_UP, key_test_handler);
    key_register_hander(KEY_DOWN, key_test_handler);
    key_register_hander(KEY_LEFT, key_test_handler);
    key_register_hander(KEY_RIGHT, key_test_handler);
    key_register_hander(KEY_OK, key_test_handler);
}

/****************************************************
 * Key testing module
 *****************************************************/
eat_bool eat_module_test_key(u8 param1, u8 param2)
{
    /***************************************
     * example 1
     ***************************************/
    if( 1 == param1 )
    {
       if( 1 == param2)
       {
           eat_trace("Key test start"); 
           eat_poweroff_key_sw(EAT_TRUE);
           eat_kpled_sw(EAT_TRUE); 
           eat_trace("PowerKey shut down on and KeyPad Led on");
       }
       else if(2 == param2)
       {
           eat_poweroff_key_sw(EAT_FALSE);
           eat_kpled_sw(EAT_FALSE); 
           eat_trace("PowerKey shut down off and KeyPad Led off");
       }
    }    
    return EAT_TRUE;
}


void app_main(void *data)
{
    EatEvent_st event;
    u8 buf[2048];
    u16 len = 0;

    APP_InitRegions();//Init app RAM
    APP_init_clib();

    eat_trace(" app_main ENTRY");
    key_reg_handle();
    while(EAT_TRUE)
    {
        eat_get_event(&event);
        eat_trace("MSG id%x", event.event);
        switch(event.event)
        {
            case EAT_EVENT_KEY :
                {
                    //Key EVENT Proc
                    key_proc(&event);
                }
                break;
            case EAT_EVENT_MDM_READY_RD:
                {
                    u8 param1,param2;
                    len = 0;
                    len = eat_modem_read(buf, 2048);
                    if(len > 0)
                    {
                        //Get the testing parameter
                        if(eat_modem_data_parse(buf,len,&param1,&param2))
                        {
                            //Entry key test module
                            eat_module_test_key(param1, param2);
                        }
                        else
                        {
                            eat_trace("From Mdm:%s",buf);
                        }
                    }

                }
                break;
            case EAT_EVENT_MDM_READY_WR:
            case EAT_EVENT_UART_READY_RD:
                break;
            case EAT_EVENT_UART_SEND_COMPLETE :
                break;
            default:
                break;
        }

    }

}


