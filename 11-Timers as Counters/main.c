

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_drv_timer.h"
#include "bsp.h"
#include "app_error.h"
#include "nrf_delay.h"

// Create a timer instance - here in this case we will use Hardware timer 0
const nrf_drv_timer_t TIMER_LED = NRF_DRV_TIMER_INSTANCE(0);

/**
 * @brief Handler for timer events.
 * This handler will not be used as the counter does not generate the events we will just 
 * pass it to the function as the function cannot accept null values.
 */
 
void timer_led_event_handler(nrf_timer_event_t event_type, void* p_context)
{
   
}




/**
 * @brief Function for main application entry.
 */
int main(void)
{
   
   // Create a variable and assign it Success value.
    uint32_t err_code = NRF_SUCCESS;

    //Configure all leds pins as output pins on the board. 
    bsp_board_init(BSP_INIT_LEDS);



    //Configure TIMER in counting mode so that you can use it as a counter
	
	// Configure the timer with default values first. 
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
	// Now change the mode of timer to counter by configuring the .mode value
    timer_cfg.mode = NRF_TIMER_MODE_COUNTER;
	// Initialize the timer with above configuration values.
    err_code = nrf_drv_timer_init(&TIMER_LED, &timer_cfg, timer_led_event_handler);
    APP_ERROR_CHECK(err_code);

//Create a variable to read the counter value
    uint32_t timVal = 0;


/*Note:
* We are doing all the work here manually, but we can automate this all
* using PPI and other techniques which are not discussed in this tutorial
* here we are just seeing how the timer works as counter, which can be used in 
* a lot of applications like counting the objects passing through a point in the 
* assembly line of a factory etc
* we can directly connect PPI event with the counter and automate everything.
 */   

    
//enable the timer so that it can increment the counter value.
    nrf_drv_timer_enable(&TIMER_LED);

// inifinite loop 
    while (1)
    {
		// A function to increment the value in the timer register manually
        nrfx_timer_increment(&TIMER_LED); 
		
		// A function to read the register value in the counter
		// we cannot read the register directly so we have to 
		// call this function.
        timVal = nrfx_timer_capture(&TIMER_LED, NRF_TIMER_CC_CHANNEL0);

		// print the value on the debugger which is read by the above function
        printf("Counter vale: %d \r\n", timVal);

		// Delay for 1 second
        nrf_delay_ms(1000);


    }
}

/** @} */
