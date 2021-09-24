

#include <stdbool.h>
#include <stdint.h>

#include "boards.h"
#include "bsp.h"
#include "app_timer.h"
#include "nordic_common.h"
#include "nrf_error.h"   
#include "nrf_drv_clock.h"


#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"



// for handling the bsp events we use the application timers at the backend
// so if you are not using a soft device then its necessary to initialize the 
// low frequency clock 

// 
void clock_initialize(void)
{
// initialize the low power low frequency clock
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code);

// request the lf clock to not to generate any events on ticks
// One tick =  1 value increment in the counter register
  nrf_drv_clock_lfclk_request(NULL);


  }





// Create a Board support package event handler
// this handler will be called when ever an interrupt event is generated
// by the input buttons on the board
void bsp_event_handler(bsp_event_t evt)
{
	// Create a switch statement to see which variable 
	// matches and execute that case.
    switch(evt)
    {
// variable to hold the error value
    uint32_t err_code;

// Case for the Input Button 1 which is defined as BSP KEY 0 in BSP library
      case BSP_EVENT_KEY_0:

      bsp_board_led_on(1); // turn on the led 1
      NRF_LOG_INFO("Led Turned on!!!"); // print log msg
	  
      break;

// Case for the Input Button 2 which is defined as BSP KEY 1 in BSP library
      case BSP_EVENT_KEY_1:
	  
      bsp_board_led_off(1); // turn off the led 1
      NRF_LOG_INFO("Led Turned off!!!"); // print log msg
	  
      break;

// Case for the Input Button 3 which is defined as BSP KEY 2 in BSP library
      case BSP_EVENT_KEY_2:
	  // A function to set the LEDs to a specific indication value
      err_code = bsp_indication_set(BSP_INDICATE_FATAL_ERROR);
      APP_ERROR_CHECK(err_code);
	  
      NRF_LOG_INFO("BSP_INDICATE_FATAL_ERROR"); // print log msg

      break;

// Case for the Input Button 4 which is defined as BSP KEY 3 in BSP library
      case BSP_EVENT_KEY_3:
	  // A function to set the LEDs to a specific indication value
      err_code = bsp_indication_set(BSP_INDICATE_IDLE);
      APP_ERROR_CHECK(err_code);
	  
      NRF_LOG_INFO("BSP_INDICATE_IDLE"); // print log msg

      break;

      
// default statement if none of above matches 
      default:
          return;
    
    }
  

}



// create  a function to configure all the leds and buttons 
void bsp_configure(void)
{

	// call a built in library function to initialize all the leds and all the buttons
	// present on the board and also attach interrupt for port events on the buttons
	// and connect an event handler to deal with the interrupts
    uint32_t err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_event_handler);
    APP_ERROR_CHECK(err_code);

}




/**
 * @brief Function for application main entry.
 */
int main(void)
{
	// call the clock initialization function
    clock_initialize();

	// initialize the application timers, make sure they are only initialized once in your code
	// otherwise you will get errors
    uint32_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

	// Initialize the Log with bakcends
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

// call the function created above to init all the leds and buttons
   bsp_configure();

// print a log msg 
   NRF_LOG_INFO("Application code started!!!");
// clear the log buffer
   NRF_LOG_FLUSH();

 // infinite loop   
    while (true)
    {
        // Do nothing.
    }
}
/** @} */
