extern struct _timekeeping_ timekeeper;
//#include "user_config.h"
void ICACHE_FLASH_ATTR
gpio_interrupt_handler() //this is the handler function for GPIO interrupts
{
//    os_printf("GPIO Handler\r\n");
    uint32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS); //reads gpio register status
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status); //writes to the interrupt clear register
    timekeeper.pulses_per_second++; //increments pulse counter
}

void ICACHE_FLASH_ATTR
gpio_interrupt_setup(void) //setups GPIO interrupts
{
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0RXD_U, FUNC_GPIO3);
    gpio_output_set(0,0,0,BIT3);
    ETS_GPIO_INTR_DISABLE(); //disables GPIO interrupts
    gpio_pin_intr_state_set(GPIO_ID_PIN(3), GPIO_PIN_INTR_NEGEDGE); //trigger interrupt on a positive edge
    ETS_GPIO_INTR_ATTACH(gpio_interrupt_handler, NULL);//this attaches "gpio_interrupt_handler" as the gpio interrupt handler function
    ETS_GPIO_INTR_ENABLE(); //gpio interrupt enable
}
