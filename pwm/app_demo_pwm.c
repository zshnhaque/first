/********************************************************************
 *                Copyright Simcom(shanghai)co. Ltd.                   *
 *---------------------------------------------------------------------
 * FileName      :   app_demo_pwm.c
 * version       :   0.10
 * Description   :   
 * Authors       :   fangshengchang
 * Notes         :
 *---------------------------------------------------------------------
 *
 *    HISTORY OF CHANGES
 *---------------------------------------------------------------------
 *0.10  2012-09-24, fangshengchang, create originally.
 *
 *--------------------------------------------------------------------
 * File Description
 * AT+CEAT=parma1,param2
 * param1 param2 
 *   1      1    Output pwm 
 *   1      2    Stop pwm output 
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

    eat_pin_set_mode(EAT_PIN26_PWM, EAT_PIN_MODE_PWM);
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
 * PWM testing module
 *****************************************************/
eat_bool eat_module_test_pwm(u8 param1, u8 param2)
{
    eat_bool result = EAT_TRUE;
    /***************************************
     * example 1
     * used eat_gpio_write test EAT_PIN53_PCM_IN,EAT_PIN57_GPIO4
     ***************************************/
    if( 1 == param1 )
    {

        if( 1 == param2 )
        {          
           eat_pwm_start(200,50);
           
           eat_trace("PWM test 1,1 start");
        }else if( 2 == param2)
        {                   
           eat_pwm_stop();
           
           eat_trace("PWM test 1,2 stop");
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

    eat_trace(" app_main ENTRY");
    while(EAT_TRUE)
    {
        eat_get_event(&event);
        eat_trace("MSG id%x", event.event);
        switch(event.event)
        {
            case EAT_EVENT_TIMER :
                {
                    //Restart timer
                    eat_timer_start(event.data.timer.timer_id, 3000);
                    eat_trace("PWM test 1, timer ID:%d", event.data.timer.timer_id);
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
                            //Entry gpio test module
                            eat_module_test_pwm(param1, param2);
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


