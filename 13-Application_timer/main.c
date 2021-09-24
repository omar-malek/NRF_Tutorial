
#include "nrf.h"
#include "nordic_common.h"
#include "boards.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"

// LED pin 
#define LED_Pin1 17

// use a function to calculate the number of tick needed to 
// create a delay of 100 millisec and store this value in LED_INTERVAL constant
#define LED_INTERVAL APP_TIMER_TICKS(100)



// For each application timer we need to create a handle which points to that instance
// so for evey application timer (Software timer) you create, you need to define a handle
// create a handle and name it as:  m_app_timer_id 
APP_TIMER_DEF(m_app_timer_id);



// its really important to initialize the clock other wise the app timer will not work
// so make sure you initialize it once in your code and if you are using a soft device
// then you don't need to initialize the clock as the soft device automatically initializes it.
static void lfclk_config(void)
{
	// initialize the low power low frequency clock
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

	// request the lf clock to not to generate any events on ticks
	// One tick =  1 value increment in the counter register
  nrf_drv_clock_lfclk_request(NULL);

}






// create a simple handler function which will be called once the timer reaches its 
// desired number of ticks value
static void app_timer_handler(void * p_context)
{
	// Toggle the LED 
    nrf_gpio_pin_toggle(LED_Pin1);
}



// a function to initialize the Application timers
static void timers_init(void)
{
	// a variable to hold error value
    ret_code_t err_code;

	// Initialize the timer 
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

	// Create an application timer with the handle, mode and interrupt event handle function
    err_code = app_timer_create(&m_app_timer_id, APP_TIMER_MODE_REPEATED, app_timer_handler);
    APP_ERROR_CHECK(err_code);

}




/**
 * @brief Function for application main entry.
 */
int main(void)
{

// initialize the gpio for led
    nrf_gpio_cfg_output(LED_Pin1);

// call the function to initialize the clock
    lfclk_config();
// initialize the timer by calling this function which is performing all the basic steps
    timers_init();

// start the timer so that it can generate events on the desired tick value
    uint32_t err_code = app_timer_start(m_app_timer_id, LED_INTERVAL, NULL);



    while (true)
    {
        // Do nothing.
    }
}
/** @} */
