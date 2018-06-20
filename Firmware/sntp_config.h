//comment when compiling
//#include "user_config.h"

uint32 current_time; //this variable will store the current time
uint16 old_days = 0, current_days = 1; //will store the number of days since 1st jan 1970
uint8 date_change_flag = 0; //flag used to indicate a change in date. Cleared in the timer_functions.h file, in the per minute timer

//this is a timer function, it will execute every 15 minutes
void ICACHE_FLASH_ATTR
sntp_timer_function()
{
    current_time = sntp_get_current_timestamp() + 1800; //1800 is added because the time fetched is UTC +5, and i live in UTC + 5.5
#ifdef DEBUG
    os_printf("TIME CHECK - %s\r\n", sntp_get_real_time(current_time));
#endif
    old_days = current_days; //setting last number of days as old value
    current_days = current_time / (3600*24); //getting new number of days
    if(old_days != current_days) //if the date has changed
        date_change_flag = 1; //set the date_change_flag, this will be acted upon in the timer_functions.h file
#ifdef DEBUG
    os_printf("OLD_DAYS - %d\r\nCURRENT_DAYS - %d\r\n",old_days,current_days);
#endif
}

//sntp setup, everything is self explanatory
void ICACHE_FLASH_ATTR
setup_sntp()
{
    sntp_stop();
    sntp_setservername(0, "0.in.pool.ntp.org");
    sntp_setservername(1, "1.asia.pool.ntp.org");
    sntp_set_timezone(+5);
    sntp_init();
    LOCAL os_timer_t timme;
    os_timer_disarm(&timme);
    os_timer_setfn(&timme, sntp_timer_function, NULL);
    os_timer_arm(&timme, 900000, 1);
}

