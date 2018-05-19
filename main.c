
#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>



UART_HandleTypeDef UARTHandle2;

void set_GPIO(){
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.Pin = GPIO_PIN_3;
//	GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
//	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void set_UART(){
	UARTHandle2.Instance        = USART2;
	UARTHandle2.Init.BaudRate   = 115200;
	UARTHandle2.Init.WordLength = UART_WORDLENGTH_8B;
	UARTHandle2.Init.StopBits   = UART_STOPBITS_1;
	UARTHandle2.Init.Parity     = UART_PARITY_NONE;
	UARTHandle2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
//	UARTHandle2.Init.Mode       = UART_MODE_TX_RX;
	UARTHandle2.Init.Mode       = UART_MODE_TX;
	if (HAL_UART_Init(&UARTHandle2) != HAL_OK)
	        asm("bkpt 255");
}
int main(void)
{

    HAL_Init();

    //enable clock
    __USART2_CLK_ENABLE();
    __GPIOA_CLK_ENABLE();

    //config GPIO and setting UART
    set_GPIO();
    set_UART();

    for (;;)
    {
        uint8_t buffer[] = "1234\n";;
//        HAL_UART_Receive(&s_UARTHandle, buffer, sizeof(buffer), HAL_MAX_DELAY);
        HAL_UART_Transmit(&UARTHandle2, buffer, sizeof(buffer), HAL_MAX_DELAY);
        HAL_Delay(1000);
    }
}
