/* This header will deal with everything timer related
 * Every time a rising edge is detected, the GPIO interrupt routine (not in this file) will increment the
 * pulse_counter
*/

extern struct espconn esp; //the espconn struct used to send data
extern uint8 isConnected; //is connected flag, used to decide if the data will be sent over or not
extern uint8 date_change_flag; //date change flag, used to reset the daily run time counter, both pulses and seconds

LOCAL os_timer_t timer_second; //timer to fire the pulses-per-second counter
LOCAL os_timer_t timer_minute; //timer to fire the pulses-per-minute counter

uint16 temp_16_bit; //used to keep a sum of pulses per minute, used for averaging
struct _timekeeping_
{
    uint8 pulses_per_second; //pulses per second
    uint8 pulses_per_minute; //average of pulses per second
    uint16 runtime_today_seconds; //total runtime today in seconds, time where pulses_per_second != 0
    uint32 total_pulses_today; //a sum of pulses today
    uint16 runtime_yesterday_seconds;
    uint32 total_pulses_yesterday;
} timekeeper; //struct to keep all that info, its 14 bytes long

//Will be executed every second
void ICACHE_FLASH_ATTR pulse_timer_function_second() //this function will execute every 1 second to clear the pulse counter
{
#ifdef DEBUG
    os_printf("PULSES PER SECOND - %d\r\nPULSES PER MINUTE - %d\r\n",timekeeper.pulses_per_second,timekeeper.pulses_per_minute);//debug info, displays pulses per second
    os_printf("RUNTIME TODAY - %d\r\nTOTAL PULSES TODAY - %d\r\n\r\n",timekeeper.runtime_today_seconds,timekeeper.total_pulses_today);
#endif
    temp_16_bit += timekeeper.pulses_per_second; //temp counter to average the number of pulses per minute, sums here, and clears counter in the per minute counter
    timekeeper.total_pulses_today += timekeeper.pulses_per_second; //
    if(isConnected == 1)
        espconn_send(&esp, (uint8 *)&timekeeper, 14);
    if(timekeeper.pulses_per_second)
        timekeeper.runtime_today_seconds++;
    timekeeper.pulses_per_second = 0;
}

void ICACHE_FLASH_ATTR
pulse_timer_function_minute()
{
    timekeeper.pulses_per_minute = temp_16_bit / 60;
    temp_16_bit = 0;
    if(date_change_flag == 1)
    {
#ifdef DEBUG
        os_printf("Date Change!\n\r");
#endif
        timekeeper.total_pulses_yesterday = timekeeper.total_pulses_today;
        timekeeper.runtime_yesterday_seconds = timekeeper.runtime_today_seconds;
        timekeeper.total_pulses_today = 0;
        timekeeper.runtime_today_seconds = 0;
        date_change_flag = 0;
    }
}

void ICACHE_FLASH_ATTR
timer_setup()
{
    os_timer_disarm(&timer_second);
    os_timer_setfn(&timer_second, pulse_timer_function_second, NULL);
    os_timer_arm(&timer_second, 1000, 1);

    os_timer_disarm(&timer_minute);
    os_timer_setfn(&timer_minute, pulse_timer_function_minute, NULL);
    os_timer_arm(&timer_minute, 60000, 1);
}
