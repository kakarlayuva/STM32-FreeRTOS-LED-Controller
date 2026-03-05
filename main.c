#include "main.h"
#include "cmsis_os.h"
#include "freertos.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void LedSeqTask(void *argument);

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  /* Init scheduler */
  osKernelInitialize();

  /* Create LED Task */
  osThreadNew(LedSeqTask, NULL, NULL);

  /* Start scheduler */
  osKernelStart();

  while (1)
  {
  }
}

void LedSeqTask(void *argument)
{
    const uint16_t leds[] = {
        GPIO_PIN_12,   // Green
        GPIO_PIN_13,   // Orange
        GPIO_PIN_14,   // Red
        GPIO_PIN_15    // Blue
    };

    while(1)
    {
        for(int i = 0; i < 4; i++)
        {
            /* Turn OFF all LEDs */
            HAL_GPIO_WritePin(GPIOD,
                GPIO_PIN_12 | GPIO_PIN_13 |
                GPIO_PIN_14 | GPIO_PIN_15,
                GPIO_PIN_RESET);

            /* Turn ON one LED */
            HAL_GPIO_WritePin(GPIOD, leds[i], GPIO_PIN_SET);

            osDelay(300);   // RTOS delay
        }
    }
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOD_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 |
                        GPIO_PIN_14 | GPIO_PIN_15;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;

  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
