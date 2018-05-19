//////////////////////////////////////////////////////////////////////////////
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stm32_hal_legacy.h>

static void MX_ADC_Init(void);
ADC_HandleTypeDef hadc1;

static void MX_UART_Init(void);
UART_HandleTypeDef UARTHandle2;

static void SystemClock_Config(void);


char value[10];

uint16_t g_ADCValue;
int g_MeasurementNumber;

int main(void)
{
    HAL_Init();
//    SystemClock_Config();
    MX_UART_Init();
    MX_ADC_Init();

    HAL_ADC_Start(&hadc1);
    for (;;)
    {
        if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)
        {
            g_ADCValue = HAL_ADC_GetValue(&hadc1);
            sprintf(value, "%d\r\n",g_ADCValue);
            HAL_UART_Transmit(&UARTHandle2,(uint8_t *)value,sizeof(value), HAL_MAX_DELAY);
            g_MeasurementNumber++;
        }
        HAL_Delay(1000);
    }
}

static void MX_ADC_Init(void)
{
	    ADC_ChannelConfTypeDef adcChannel;

	    hadc1.Instance = ADC1;

	    hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
	    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	    hadc1.Init.ScanConvMode = DISABLE;
	    hadc1.Init.ContinuousConvMode = ENABLE;
	    hadc1.Init.DiscontinuousConvMode = DISABLE;
	    hadc1.Init.NbrOfDiscConversion = 0;
	    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	    hadc1.Init.NbrOfConversion = 1;
	    hadc1.Init.DMAContinuousRequests = ENABLE;
	    hadc1.Init.EOCSelection = DISABLE;

	    HAL_ADC_Init(&hadc1);

	    adcChannel.Channel = ADC_CHANNEL_0;
	    adcChannel.Rank = 1;
	    adcChannel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	    adcChannel.Offset = 0;

	    if (HAL_ADC_ConfigChannel(&hadc1, &adcChannel) != HAL_OK)
	    {
	        asm("bkpt 255");
	    }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStructure;
  if(hadc->Instance==ADC1)
  {
	  __ADC1_CLK_ENABLE();
	  __GPIOA_CLK_ENABLE();

	  GPIO_InitStructure.Pin = GPIO_PIN_0;
	  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	  GPIO_InitStructure.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
//  HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(ADC_IRQn);

}


static void MX_UART_Init(void){

	UARTHandle2.Instance        = USART2;
	UARTHandle2.Init.BaudRate   = 115200;
	UARTHandle2.Init.WordLength = UART_WORDLENGTH_8B;
	UARTHandle2.Init.StopBits   = UART_STOPBITS_1;
	UARTHandle2.Init.Parity     = UART_PARITY_NONE;
	UARTHandle2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UARTHandle2.Init.Mode       = UART_MODE_TX;
	HAL_UART_Init(&UARTHandle2);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart){

	GPIO_InitTypeDef GPIO_InitStructure;

	if(huart->Instance==USART2)
	 {
		__GPIOA_CLK_ENABLE();
		__USART2_CLK_ENABLE();

		GPIO_InitStructure.Pin = GPIO_PIN_2;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	 }
}


static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 288;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 6;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
    SystemCoreClockUpdate();

    if (HAL_GetREVID() == 0x1001)
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

