
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"

void LED_Init() {
  LED2_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = LED2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
}

void BUTTON_Init() {
  USER_BUTTON_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = USER_BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(USER_BUTTON_GPIO_PORT, &GPIO_InitStruct);
}

int main(void) {
  HAL_Init();
  LED_Init();
  BUTTON_Init();

  while (1) {
    if ( !HAL_GPIO_ReadPin( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN ) ) { // button is pressed
        // wait until the button is released
        while ( !HAL_GPIO_ReadPin( USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN ) ) {
           HAL_Delay(10 /* msec*/ );
        }
        HAL_GPIO_TogglePin( LED2_GPIO_PORT, LED2_PIN ); // toggle the LED
     }
  }
}
