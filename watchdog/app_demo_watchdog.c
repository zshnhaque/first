/********************************************************************
 *                Copyright Simcom(shanghai)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   app_demo_gpio.c
 * version       :   0.10
 * Description   :   
 * Authors       :   fangshengchang
 * Notes         :
 *---------------------------------------------------------------------
 *
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2012-09-24, fangshengchang, create originally.
 *0.20  2013-03-26, maobin, modify the PIN definition, to adapt to the SIM800W and SIM800V
 *
 *--------------------------------------------------------------------
 * File Description
 * AT+CEAT=parma1,param2
 * param1 param2 
 *   1      1    start watchdog
 *   1      2    feed dog
 *   1      3    stop watchdog
 *--------------------------------------------------------------------
 ********************************************************************/
/********************************************************************
 * Include Files
 ********************************************************************/
#include <stdio.h>
#include <string.h>
#include "eat_interface.h"
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

/********************************************************************
 * External Functions declaration
 ********************************************************************/
extern void APP_InitRegions(void);

/********************************************************************
 * Local Function declaration
 ********************************************************************/
void app_main(void *data);
void app_func_ext1(void *data);
void app_user1(void *data);
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
		(app_user_func)app_user1,//app_user1,
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


u32 g_pin_set_result = 0;
void app_func_ext1(void *data)
{
   u32 ret_val=0;
	/*This function can be called before Task running ,configure the GPIO,uart and etc.
	   Only these api can be used:
		 eat_uart_set_debug: set debug port
		 eat_pin_set_mode: set GPIO mode
		 eat_uart_set_at_port: set AT port
	*/
    eat_uart_set_debug(EAT_UART_USB);
    eat_uart_set_at_port(EAT_UART_1);// UART1 is as AT PORT     
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


/****************************************************
 * GPIO testing module
 *****************************************************/
eat_bool eat_module_test_watchdog(u8 param1, u8 param2)
{
    s32 ret;
    u8 read_buffer[10]=0;
    u8 write_buffer[10]=0;
    /***************************************
     * example 1
     * used eat_gpio_write test EAT_GPIO_TEST1,EAT_GPIO_TEST2
     ***************************************/
     
    eat_trace("Watchdog test param1=%d,param2=%d",param1,param2);
    if( 1 == param1 )
    {

        if( 1 == param2 )
        {
            ret=eat_watchdog_start(10000,0); //10s, reboot if over time
            if(ret)
            {
                eat_trace("open watchdog success");
            }
            else
            {
                eat_trace("open watchdog fail");
            }
           
        }else if( 2 == param2)
        {  
            eat_watchdog_feed();

        }else if(3 == param2)
        {
            eat_watchdog_stop();
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

    eat_watchdog_start(10000,0);
    eat_trace(" app_main ENTRY");
    while(EAT_TRUE)
    {
        eat_get_event(&event);
        eat_trace("MSG id%x", event.event);
        switch(event.event)
        {
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
                            //Entry watchdog test module
                            eat_module_test_watchdog(param1, param2);
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


void app_user1(void *data)
{
    while(1);
}
