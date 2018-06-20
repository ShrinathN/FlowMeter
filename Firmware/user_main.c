#define DEBUG
#include "user_config.h"
#include "timer_functions.h"
#include "interrupt_functions.h"
#include "connection_functions.h"
#include "sntp_config.h"
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)
extern uint16 current_days;
extern uint32 current_time;
void ICACHE_FLASH_ATTR user_init(void)
{
    SET_BAUD(0,115200);
#ifdef DEBUG
    os_printf("\n\r\n\r\nSetting up SNTP...");
#endif
    setup_sntp();
    current_time = sntp_get_current_timestamp() + 1800; //1800 is added because the time fetched is UTC +5, and i live in UTC + 5.5
    current_days = current_time / (3600*24); //getting new number of days
#ifdef DEBUG
    os_printf("SNTP set up\n\r");
    os_printf("Setting up GPIO...");
#endif
    gpio_interrupt_setup();
#ifdef DEBUG
    os_printf("GPIO Setup Done\n\r");

    os_printf("\n\rSetting up connections...");
#endif
    connection_setup();
#ifdef DEBUG
    os_printf("Connection Setup Done\n\r");

    os_printf("Setting up timer functions...");
#endif
    timer_setup();
#ifdef DEBUG
    os_printf("Timer Setup Done\n\r");
#endif
}
