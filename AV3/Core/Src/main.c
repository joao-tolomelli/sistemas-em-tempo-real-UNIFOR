/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h"
#include "semphr.h"
#include "queue.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
/* REMOVEMOS A DEFAULT TASK PARA USAR A API NATIVA */
/*
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
*/
/* USER CODE BEGIN PV */
// Handles dos objetos RTOS
SemaphoreHandle_t g_MutexLCD;
SemaphoreHandle_t g_MutexCounter;
QueueHandle_t g_QueueSerialRx;

// Variáveis Globais
uint16_t g_u16Counter = 0;   // Variável global para o contador
uint8_t g_rxByte;            // Byte único para recepção UART IT
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
/* void StartDefaultTask(void *argument); */ /* <-- REMOVIDO */

/* USER CODE BEGIN PFP */
// Protótipos das Tarefas
void vTaskADC_LCD1(void *pvParameters);      // TAREFA 1
void vTaskCounter_LCD2(void *pvParameters); // TAREFA 2
void vTaskSerialRx_LCD2(void *pvParameters); // TAREFA 3
void vTaskLedToggle(void *pvParameters);     // TAREFA 4

// Protótipo do _write para printf
int _write(int file, char *ptr, int len);

// Protótipo do Callback da UART
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Inicializa o LCD
  lcd_init();

  // Criação dos Mutexes
  g_MutexLCD = xSemaphoreCreateMutex();
  g_MutexCounter = xSemaphoreCreateMutex();

  // Criação da Fila para a Serial
  // Fila de 16 bytes. Suficiente para digitação.
  g_QueueSerialRx = xQueueCreate(16, sizeof(uint8_t));

  // Verificação (Boa prática, como nas aulas)
  if(g_MutexLCD == NULL || g_MutexCounter == NULL || g_QueueSerialRx == NULL)
  {
    printf("Falha ao criar objetos RTOS!\n");
    while(1);
  }

  // Inicia a primeira recepção por Interrupção do UART
  // Isso "arma" a interrupção para o primeiro byte
  HAL_UART_Receive_IT(&huart2, &g_rxByte, 1);

  printf("--- Sistema Iniciado ---\n");

  /* USER CODE END 2 */

  /* Init scheduler */
  /* REMOVEMOS AS CHAMADAS CMSIS PARA USAR A API NATIVA (das aulas) */
  /*
  osKernelInitialize();
  */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  /* defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes); */ /* <-- REMOVIDO */

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(vTaskADC_LCD1,
              "TaskADC",
              256,        // Stack. Pode precisar de mais por causa do float/sprintf
              NULL,
              2,          // Prioridade Média
              NULL);

  xTaskCreate(vTaskCounter_LCD2,
              "TaskCounter",
              128,        // Stack
              NULL,
              2,          // Prioridade Média
              NULL);

  xTaskCreate(vTaskSerialRx_LCD2,
              "TaskSerialRx",
              256,        // Stack
              NULL,
              2,          // Prioridade Média
              NULL);

  xTaskCreate(vTaskLedToggle,
              "TaskLED",
              configMINIMAL_STACK_SIZE, //
              NULL,
              1,          // Prioridade Baixa
              NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  /* osKernelStart(); */ /* <-- REMOVIDO */
  vTaskStartScheduler();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */

/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB10 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief Implementação do _write para redirecionar printf para UART2
  */
int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
  return len;
}


/**
  * @brief TAREFA 1: Leitura ADC, Teclas, Display L1 e Serial
  */
void vTaskADC_LCD1(void *pvParameters)
{
  uint32_t adc_val;
  float voltage;
  char key_name[7] = "NONE  "; // 6 chars + \0 (para apagar "SELECT")
  char buffer[17]; // Buffer para a linha do LCD (16 chars + \0)

  for(;;)
  {
    // 1. Leitura do ADC (Usando HAL)
    HAL_ADC_Start(&hadc1);
    if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
      adc_val = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);

    voltage = (float)adc_val * (3.3f / 4095.0f);

    // 2. Lógica das Teclas (Valores adaptados para ADC raw)
        if (adc_val < 50) // RIGHT
        {
            strcpy(key_name, "RIGHT ");
        }
        else if (adc_val > 800 && adc_val < 850) // UP
        {
            strcpy(key_name, "UP    ");
            // Incrementa o contador global (Lógica original mantida)
            if (xSemaphoreTake(g_MutexCounter, portMAX_DELAY) == pdTRUE) {
                g_u16Counter++;
                xSemaphoreGive(g_MutexCounter);
            }
        }
        else if (adc_val > 1950 && adc_val < 2050) // DOWN
        {
            strcpy(key_name, "DOWN  ");
        }
        else if (adc_val > 3050 && adc_val < 3150) // LEFT
        {
            strcpy(key_name, "LEFT  ");
        }
        else if (adc_val > 4000 && adc_val < 5050) // SELECT
        {
            strcpy(key_name, "SELECT"); // "SELECT" já tem 6 chars
        }
        else // NONE
        {
            strcpy(key_name, "NONE  ");
        }

    // 3. Envia Tensão pela Serial [A]
    // Usamos printf graças ao _write()
    //printf("TENSAO=%.1fV\n", voltage);

    // 4. Escreve no LCD [1, 2]
    // Protegendo o acesso ao LCD com o Mutex
    if(xSemaphoreTake(g_MutexLCD, portMAX_DELAY) == pdTRUE)
    {
      // Formata a string: "TENSAO=X.XV KEY"
      sprintf(buffer, "TENSAO=%.1fV%s", voltage, key_name);

      lcd_put_cur(0, 0); // Linha 1, Coluna 0
      lcd_send_string(buffer);

      // Libera o Mutex do LCD
      xSemaphoreGive(g_MutexLCD);
    }

    // Aguarda 500ms
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/**
  * @brief TAREFA 2: Envia Contador para LCD L2 e Serial
  */
void vTaskCounter_LCD2(void *pvParameters)
{
  uint16_t local_counter;
  char buffer[11]; // "CONT=XXXXX "

  for(;;)
  {
    // 1. Lê o contador global de forma segura
    if(xSemaphoreTake(g_MutexCounter, portMAX_DELAY) == pdTRUE)
    {
      local_counter = g_u16Counter;
      xSemaphoreGive(g_MutexCounter);
    }

    // 2. Envia Contador pela Serial [B]
    //printf("CONTADOR=%u\n", local_counter);

    // 3. Escreve no LCD [3]
    // Protegendo o acesso ao LCD
    if(xSemaphoreTake(g_MutexLCD, portMAX_DELAY) == pdTRUE)
    {
      // Formata: "CONT=XXXXX"
      // O "%-5u" preenche com espaços à direita para apagar valores antigos
      sprintf(buffer, "CONT=%-5u", local_counter);

      lcd_put_cur(1, 0); // Linha 2, Coluna 0
      lcd_send_string(buffer);

      // Libera o Mutex
      xSemaphoreGive(g_MutexLCD);
    }

    // Aguarda 200ms
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

/**
  * @brief TAREFA 3: Recebe String Serial e mostra no LCD L2
  */
void vTaskSerialRx_LCD2(void *pvParameters)
{
  uint8_t rx_char;
  char str_buffer[6]; // Buffer para 5 caracteres + '\0'
  uint8_t index = 0;
  char lcd_output[7]; // Buffer para 6 caracteres (para padding) + '\0'

  for(;;)
  {
    // 1. Aguarda um byte chegar na fila (enviado pela Interrupção)
    if(xQueueReceive(g_QueueSerialRx, &rx_char, portMAX_DELAY) == pdPASS)
    {
      // 2. Adiciona o caractere ao buffer
      str_buffer[index] = (char)rx_char;
      index++;

      // 3. Verifica se o buffer acumulou 5 caracteres
      if (index == 5)
      {
          str_buffer[index] = '\0'; // Finaliza a string

          // Prepara a string para o LCD. O "%-6s" garante que 6 caracteres
          // sejam escritos, preenchendo com espaços se a string for menor.
          // Como str_buffer tem 5 chars, isso vira "XXXXX ".
          sprintf(lcd_output, "%-6s", str_buffer);

          // 4. Escreve no LCD [4]
          if(xSemaphoreTake(g_MutexLCD, portMAX_DELAY) == pdTRUE)
          {
            lcd_put_cur(1, 10); // Linha 2, coluna 10
            lcd_send_string(lcd_output);
            xSemaphoreGive(g_MutexLCD);
          }

          // 5. Reseta o índice para a próxima string de 5 caracteres
          index = 0;
      }
    }
  }
}

/**
  * @brief TAREFA 4: Toggle LED do Usuário
  */
void vTaskLedToggle(void *pvParameters)
{
  for(;;)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin); // LD2 é PA5
    vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda 500ms
  }
}

/**
  * @brief Callback de Interrupção da UART.
  * Chamada pela HAL sempre que um byte é recebido.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2)
  {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Envia o byte recebido (g_rxByte) para a fila
    xQueueSendFromISR(g_QueueSerialRx, &g_rxByte, &xHigherPriorityTaskWoken);

    // Rearma a interrupção para o próximo byte
    HAL_UART_Receive_IT(&huart2, &g_rxByte, 1);

    // Se a fila acordou uma tarefa de prioridade maior, força a troca de contexto
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
/*
void StartDefaultTask(void *argument)
{
  */
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
/*
  for(;;)
  {
    osDelay(1);
  }
  */
  /* USER CODE END 5 */
/*
}
*/

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint8_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
