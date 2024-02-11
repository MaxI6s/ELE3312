/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ARM_MATH_CM4
#include "arm_math.h"

#include "stdio.h"
#include "stdlib.h"
#include "MCUFRIEND_kbv.h"
#include "avatar.h"

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

/* USER CODE BEGIN PV */
	struct Avatar avatar;
	struct Obstacle obstacle;
	
	static int distance_de_jeu = 500;

	int gameover = 0;
	volatile int scroll = 0;
	int timer = 0;
	int full_timer = 0;
	int speed = 200;

			// Partie A
	int newPosy = 210;
	uint32_t valeur_1 = 0;
	uint32_t valeur_2 = 0;
	uint32_t distance = 0;
	int difference = 0;
	bool deuxiemeCapture = false;
	volatile bool max_distance = false;

	volatile int hand = true;
	volatile int handCounter = 0;
	static int handCounterPeriod = 3;
	uint32_t last_distance = 0;

	volatile int flagMulti = 0;
	int score = 0;
	
	static int screen_x = 320;
	static int screen_y = 240;
	static int background_color = BLACK;
	
			// Partie B
	uint8_t bufferTX[4];
	uint8_t bufferRx;		// Réception d'un octet pour analyse
	volatile int posRecu = 0;  //Variable où sera stocké temporairement les valeurs reçues avant confirmation de l'octer 254
	volatile int idRecu = 0;
	volatile int flag = 0;
	volatile int etat = 0;
	volatile int recepOk = 0; //Indique si la recpetion des données c'est fait sans problème

			//Valeurs pour l'envoie PWM avec la del infrarouge
	float valueSin[1000];
	#define TABLE_LENGTH 69
	int valueHigh[TABLE_LENGTH]; //Tableau contenant les valeurs du pwm lorsque l'on envoie un bit '1'
	int  valueLow[TABLE_LENGTH]; //Tableau contenant les valeurs du pwm lorsque l'on envoie un bit '0'
	volatile int* current_tab = valueHigh;
	volatile bool FlagT = 0;	

	//test message
	uint8_t  pos = 0;
	uint8_t  id = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void startGame();
/* USER CODE BEGIN PFP */

// TODO: Voir si la fonction est utilisée
void resetBuffer(uint8_t* array){
	for(int i = 0; i < 10; ++i){
		array[i] = 0;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifndef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


void HAL_SYSTICK_Callback(void) {
	full_timer++;
	timer++;
	
	if (timer >= speed){
		scroll = 1;
		timer = 0;
	}
}


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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_DAC_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
	
	initGame(&avatar, &obstacle);
	drawAvatar(&avatar);
	
	HAL_TIM_PWM_Init(&htim2);
  HAL_TIM_IC_Init(&htim3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
	
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)current_tab, TABLE_LENGTH, DAC_ALIGN_12B_R);
	HAL_UART_Transmit_DMA(&huart5, bufferTX, sizeof(bufferTX));
	HAL_UART_Receive_IT(&huart5,&bufferRx, 1);
	
	//Préchargement des valeurs de sinus
	float period = 1.0/1000; //period est l'intervalle entre les échantillons d'un periode
	for(int g =0; g<1000 ; g++){ 
		valueSin[g] = 1 + sin(2*PI*g*period);
	}
	//Préchargement du tableau valueHigh
	for(int g = 0; g < TABLE_LENGTH; ++g){
		valueHigh[g] = (int)4095.0 *valueSin[(int)(g/14.4762)];
	}
	
		//Préchargement du tableau valueLow
	for(int g = 0; g < TABLE_LENGTH; ++g){
		valueLow[g] = 0;
	}
	
	//Remplissage du buffer d'envoie
	bufferTX[0] = 255;
	bufferTX[1] = pos;
	bufferTX[2] = id;
	bufferTX[3] = 254;
	
	initGame(&avatar, &obstacle);
	drawAvatar(&avatar);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		// STARTING SCREEN
		LCD_FillScreen(background_color);
		LCD_SetTextColor(WHITE, WHITE);
		LCD_SetTextSize(2);
		LCD_SetCursor(0, 0);
		LCD_Printf("! PROJET !");
		LCD_SetCursor(screen_x/2 - 50, screen_y/2 - 40);
		LCD_SetTextSize(3);
		LCD_Printf("+DINO RUN+");
		LCD_SetCursor(screen_x/2 - 80, screen_y/2 + 50);
		LCD_Printf("PLAYER 2");
		
		while(1){
			printf("Buffer : %d %d %d %d \r\n",bufferTX[0] ,bufferTX[1],bufferTX[2],bufferTX[3]);
		}
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
* @brief Retargets the C library printf function to the USART.
* @param None
* @retval None
*/
PUTCHAR_PROTOTYPE
{
/* Place your implementation of fputc here */
/* e.g. write a character to the USART2 and Loop until the end
of transmission */
HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
return ch;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
		if(deuxiemeCapture == false){
			valeur_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		//	valeur_1 = TIM3->CCR3;
			deuxiemeCapture = true;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_FALLING);
		} 
		else if(deuxiemeCapture == true){
			valeur_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			//valeur_2 = TIM3->CCR3;
			__HAL_TIM_SET_COUNTER(htim, 0);
			
			if (valeur_2 > valeur_1)
			{
				difference = valeur_2 - valeur_1;
			}

			else if (valeur_1 > valeur_2)
			{
				difference = (0xffff - valeur_1) + valeur_2;
			}
			
			distance = difference *.34/2;
			
			if(distance<=distance_de_jeu){
				max_distance = 0;
				last_distance = distance;
			}
			
			if(distance>distance_de_jeu){
				max_distance = 1;
				distance = last_distance;
			}
			
			deuxiemeCapture = false;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_3, TIM_INPUTCHANNELPOLARITY_RISING);
			
			}
		
	}
	if(max_distance == 1){
		handCounter++;
		if(handCounter > handCounterPeriod){
			hand = false;
		}
	}
	else if(max_distance == 0){
		handCounter = 0;
		hand = true;
	}
	
	//printf("Value : %f   max_distance : %d   hand ? : %d \r\n",(double) distance/10, max_distance, hand);
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	bufferTX[1] = (uint8_t) (distance*0.21);
	bufferTX[2] = (uint8_t) hand;
	HAL_UART_Transmit_DMA(&huart5, bufferTX, sizeof(bufferTX));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//Machine à état
	switch (etat){
		//Reception du premier octe
		case 0:
			if(bufferRx == 255){  //reception de l'octet de start
				etat = 1;
			}
			break;
			
		//Reception des données de position
		case 1: 						//Reception de la position
			if(bufferRx == 255){  //reception de l'octet de start
				etat = 1;
			}else{
				posRecu = bufferRx;
				etat = 2;
			}
			break;
			
		//Reception des données d'id
		case 2:						//Reception de l'id
			if(bufferRx == 255){  //reception de l'octet de start
				etat = 1;	
			}else{
				idRecu = bufferRx;
				etat = 3;
			}
			break;
			
		//Reception de l'octet de stop 254
		case 3:
			if(bufferRx == 255){  //reception de l'octet de start
				etat = 1;
			}else{
				if(bufferRx == 254){ //reception de l'octet de stop
					recepOk = 1;
				}else{
					recepOk = 0;
				}
				etat = 0;
				flag = 1;
			}
			break;
			
		//Default
		default:
			etat = 0;
			break;
	}	
	HAL_UART_Receive_IT (&huart5, &bufferRx, 1);
}


//Changement du signal sur la DEL infrarouge lorsqu'il y un changement sur la pin PA12 qui est reliée à la pin TX
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		// On change entre un signal sinusoidal et un signal de zero lorsqu'il y a un changement sur la pin PA12
    if(FlagT) //HAL_GPIO_ReadPin(PA12_GPIO_Port, GPIO_Pin)== GPIO_PIN_RESET
    {
			current_tab = valueLow;
			FlagT = 0;
    }
		else{
			current_tab = valueHigh;
			FlagT = 1;
		}	
}
void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef* hdac) {
	HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_2, (uint32_t *)current_tab, TABLE_LENGTH, DAC_ALIGN_12B_R);
}

/* USER CODE END 4 */

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

void startGame() {
		LCD_FillScreen(background_color);
		LCD_SetTextColor(WHITE, WHITE);
		LCD_SetTextSize(2);
		LCD_SetCursor(0, 0);
		LCD_Printf("! PROJET !");
		LCD_SetCursor(screen_x/2 - 50, screen_y/2 - 40);
		LCD_Printf("+DINO RUN+");
		LCD_SetCursor(screen_x/2 - 80, screen_y/2 + 20);
		LCD_Printf("SINGLE PLAYER");
		LCD_SetCursor(screen_x/2 - 80, screen_y/2 + 50);
		LCD_Printf("MULTIPLAYER");
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
