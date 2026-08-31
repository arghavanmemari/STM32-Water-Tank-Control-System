/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
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

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t buff[100]; //rtc
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* ----------------key pad------------------ */
int k=0;
char key[16]="n";
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==GPIO_PIN_11)k=1;
}

void keyscan(void){
	if(k){
		if(((GPIOA->IDR >> 12) & 0x0F)==0x00)strcpy(key,"7");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x01)strcpy(key,"4");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x02)strcpy(key,"1");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x03)strcpy(key,"on");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x04)strcpy(key,"8");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x05)strcpy(key,"5");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x06)strcpy(key,"2");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x07)strcpy(key,"0");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x08)strcpy(key,"9");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x09)strcpy(key,"6");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x0A)strcpy(key,"3");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x0E)strcpy(key,"+");
		else if(((GPIOA->IDR >> 12) & 0x0F)==0x0F)strcpy(key,"-");

		else strcpy(key,"");
		k=0;
	}
}

/*---------------------ultrasonic------------------*/
uint32_t IC_Val1 = 0;   //لبه ی بالارونده
uint32_t IC_Val2 = 0;   //لبه ی پایین رونده
uint32_t Difference = 0; //فاصلشون
uint8_t  Is_First_Captured = 0;
int Distance = 0;   //cm

void HCSR04_Read(void)
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  HAL_Delay(1);


  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
  HAL_Delay(1);


  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
    if (Is_First_Captured == 0)
    {
      IC_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
      Is_First_Captured = 1;

      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
    }
    else
    {
      IC_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);

      if (IC_Val2 > IC_Val1) Difference = IC_Val2 - IC_Val1;
      else Difference = (0xFFFF - IC_Val1) + IC_Val2;

      Distance = Difference /61; // cm

      Is_First_Captured = 0;
      __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
    }
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
  lcd_init(GPIOA, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10);

  RTC_TimeTypeDef sTime;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
HAL_TIM_Base_Start(&htim2);
// دما
char adc1[16];
int adc=0;
int temperror;

// فاصله
char distStr[16];
int diserror;


int a,b,c;

uint8_t msg[50] ; //uart

// ست پوینت
char setp[4] = {0};
int setpoint=50 ;
char setpointc[16];

char tbuf[10] = {0};   //HHMMSS

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      //ورود به منو
	  if(strcmp(key,"on")==0){
	  lcd_gotoxy(0, 0);
	 		lcd_puts("1-SetPoint");
	 	   	lcd_gotoxy(0, 1);
	 	  	lcd_puts("2-Time       ");
	 	  	lcd_gotoxy(0, 2);
	 	  	lcd_puts("            ");
	 	  	lcd_gotoxy(0, 3);
	 	  	lcd_puts("            ");
	 	  	a=1; //یعنی به منو وارد شدیم
	 	   strcpy(key,"n");

		while(a==1){
			keyscan();
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
			sprintf((char*)buff, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
			lcd_gotoxy(0, 3); lcd_puts((char*)buff);

			//به کدوم منو وارد شیم
	  		if(strcmp(key,"+")==0)
	  			    	{
	  			    	 lcd_gotoxy(0, 0);
	  			         lcd_puts("1-SetPoint <=");
	  			    	 lcd_gotoxy(0, 1);
	  			    	 lcd_puts("2-Time        ");
	  			    	 b=1; //تغییر ست پوینت
	  			    	}
	  		if(strcmp(key,"-")==0)
	  			    	 {
	  			    	  lcd_gotoxy(0, 0);
	  			    	  lcd_puts("1-SetPoint   ");
	  			    	  lcd_gotoxy(0, 1);
	  			    	  lcd_puts("2-Time <=");
	  			    	  b=2; //تغییر زمان
	  			    	  }

	  		if(strcmp(key,"on")==0)
	  		{  //یکی از حالتا انتخاب شده
		        a=2;
		        lcd_clear();
                strcpy(key,"");
	  		}
		}

		while(a==2)
		{
		    keyscan();

		    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		   	sprintf((char*)buff, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
		   	lcd_gotoxy(0, 3); lcd_puts((char*)buff);

		    if(b==1) //تغییر ست پوینت
		    {
		    	 lcd_gotoxy(0,0);
		         lcd_puts("Enter SetPoint ");
		         lcd_gotoxy(0,1);
		         lcd_puts("SetPoint:");

		         // وقتی ان زده شه ست پوینت جدید انتخاب میشه
		         if(strcmp(key,"on")==0)
		         {
		             int sp_new = atoi(setp);
                     // اگه ست پوینت جدید مقدارش مناسب باشه انتخاب میشه بعنوان ست پوینت
		             if(sp_new < 50)
		             {
		                 setpoint = sp_new;
		                 strcpy((char*)msg, "SetPoint changed successfully\r\n");
		                 HAL_UART_Transmit(&huart3, msg, strlen((char*)msg), HAL_MAX_DELAY);
		                 HAL_Delay(1000);
		             }
		             else
		             {
		                 lcd_gotoxy(0,2);
		                 lcd_puts("out of range");
		                 HAL_Delay(1000);
		             }

		             strcpy(setp,"");
		             a=0; // خروج از منو
		             lcd_clear();
		             strcpy(key,"");
		         }
                // گرفتن ست پوینت
		        if(key[0]>='0' && key[0]<='9' && key[1]=='\0')
		        {
		        	strcat(setp, key);
		        	strcpy(key,"");

		        	lcd_gotoxy(9,1);
		        	lcd_puts(setp);

		            }
		        }

		    if(b==2)   // Change Time (سه خط جدا)
		    {
		        static int sel = 0;          // 0:Hour  1:Min  2:Sec
		        static char numbuf[3] = {0};
		        RTC_TimeTypeDef cur;

		        // نمایش مقادیر فعلی RTC روی 3 خط
		        HAL_RTC_GetTime(&hrtc, &cur, RTC_FORMAT_BIN);

		        lcd_gotoxy(0,0);
		        sprintf((char*)buff, "Hour%s",   (sel==0)?" <=":"         ");
		        lcd_puts((char*)buff);

		        lcd_gotoxy(0,1);
		        sprintf((char*)buff, "Min%s", (sel==1)?" <=":"         ");
		        lcd_puts((char*)buff);

		        lcd_gotoxy(0,2);
		        sprintf((char*)buff, "Sec%s", (sel==2)?" <=":"         ");
		        lcd_puts((char*)buff);

		        // حرکت بین خطوط با + و -
		        if(strcmp(key,"+")==0) { sel = (sel + 1) % 3; strcpy(key,""); }
		        if(strcmp(key,"-")==0) { sel = (sel + 2) % 3; strcpy(key,""); } // -1 mod 3

		        // با on وارد حالت ادیت همون  می‌شیم
		        if(strcmp(key,"on")==0)
		        {
		            strcpy(key,"");
		            strcpy(numbuf,"");
		            lcd_clear();

		            // صفحه ادیت
		            while(1)
		            {
		                keyscan();

		                if(sel==0){
		                	lcd_gotoxy(0,0);
		                	lcd_puts("Set HOUR (0-23)");
		                	lcd_gotoxy(0,1);
		                	lcd_puts("HOUR:");
		                	lcd_gotoxy(5,1);
		                	lcd_puts(numbuf);
		                }

		                if(sel==1){
		                	lcd_gotoxy(0,0);
		                	lcd_puts("Set MIN  (0-59)");
		                	lcd_gotoxy(0,1);
		                	lcd_puts("MIN:");
		                	lcd_gotoxy(4,1);
		                    lcd_puts(numbuf);
		                }
		                if(sel==2){
		                	lcd_gotoxy(0,0);
		                	lcd_puts("Set SEC  (0-59)");
		                	lcd_gotoxy(0,1);
		                	lcd_puts("SEC:");
		                	lcd_gotoxy(4,1);
		                	lcd_puts(numbuf);
		                }



		                // پاک کردن با -
		                if(strcmp(key,"-")==0) { strcpy(numbuf,""); strcpy(key,""); }

		                // گرفتن رقم
		                if(key[0]>='0' && key[0]<='9' && key[1]=='\0')
		                {
		                    if(strlen(numbuf) < 2) strncat(numbuf, key, 1);
		                    strcpy(key,"");
		                }

		                // ثبت با on
		                if(strcmp(key,"on")==0)
		                {
		                    int v = atoi(numbuf);

		                    int ok = 0;
		                    if(sel==0 && v>=0  && v<24) ok=1;
		                    if(sel!=0 && v>=0 && v<60) ok=1;

		                    if(ok)
		                    {
		                        RTC_TimeTypeDef newTime = cur;  // از مقدار فعلی شروع کن
		                        if(sel==0) newTime.Hours   = v;
		                        if(sel==1) newTime.Minutes = v;
		                        if(sel==2) newTime.Seconds = v;

		                        HAL_RTC_SetTime(&hrtc, &newTime, RTC_FORMAT_BIN);

		                        strcpy((char*)msg, "Time changed successfully\r\n");
		                        HAL_UART_Transmit(&huart3, msg, strlen((char*)msg), HAL_MAX_DELAY);

		                        a = 0;            // خروج از منو (مثل ست‌پوینت)
		                        strcpy(key,"");
		                        lcd_clear();
		                        break;
		                    }
		                    else
		                    {
		                        lcd_gotoxy(0,2);
		                        lcd_puts("Invalid!");
		                        HAL_Delay(800);
		                        strcpy(numbuf,"");
		                        strcpy(key,"");
		                        lcd_clear();
		                    }
		                }
		            }
		        }
		    }

		}


	  }

	  else // بیرون منو
	  {
	  keyscan();

	  //-------------------Distance-----------------------
	  HCSR04_Read();
	  if(Distance>50)
	  {
		  sprintf(distStr, "dist:50cm");
		  	  lcd_gotoxy(0, 0);
		  	  lcd_puts(distStr);
	  }
	  else{
		  sprintf(distStr, "dist:%dcm", Distance);
		  	  lcd_gotoxy(0, 0);
		  	  lcd_puts(distStr);
	  }
	  if(setpoint != 0)
	  {
	      if(Distance > setpoint)
	          diserror = 0;
	      else if(Distance <= 0)
	          diserror = 159;
	      else
	          diserror = 159 - (Distance * (159 - 0) / setpoint);
	  }
	  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, diserror);



//-----------------------temp-----------------------------------
//گرفتن دما
HAL_ADC_Start(&hadc1);
if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY)==HAL_OK){
		  adc=HAL_ADC_GetValue(&hadc1);
		  adc=adc*0.0806;
	  }
if(adc > 40 && adc < 60)temperror=0;
	temperror=7.5*adc-286;
while(adc<40){
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY)==HAL_OK){
			  adc=HAL_ADC_GetValue(&hadc1);
			  adc=adc*0.0806;
		  }
	lcd_clear();
	HAL_Delay(500);
	lcd_gotoxy(0, 0);
	lcd_puts("TOO COLD");
	HAL_Delay(500);

}
__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,159-temperror);

sprintf(adc1,"temp:%2d",adc);
lcd_gotoxy(0, 2);
lcd_puts(adc1);

//-----------------------set point-------------------------
sprintf(setpointc,"set point:%2d",setpoint);
lcd_gotoxy(0, 1);lcd_puts(setpointc);

//-----------------------rtc----------------------------
HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
sprintf(buff,"%02d:%02d:%02d",sTime.Hours,sTime.Minutes,sTime.Seconds);
lcd_gotoxy(0, 3);lcd_puts(buff);
  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 15;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 159;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 999;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 159;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
  HAL_TIM_MspPostInit(&htim4);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA3 PA4 PA5
                           PA7 PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA12 PA13 PA14 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
