/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_hrtim1_m;
extern HRTIM_HandleTypeDef hhrtim;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

	// cyclic update the PWM
	uint32_t m_period_middle_ticks = hhrtim.Instance->sMasterRegs.MPER / 2;
	static volatile uint32_t count = 0;

	static uint32_t dma_buffr[2] = {100, 200};

	if(uwTick % 100 == 0)
	{
		// TIMER_A is updated directly at the moment, because it works! - ref for oscilloscope
		hhrtim.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR =  m_period_middle_ticks - count - 200;
		hhrtim.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR =  m_period_middle_ticks + count + 100;

		// TIMER_x should NOT be updated directly
		hhrtim.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR =  m_period_middle_ticks - count - 200;
		hhrtim.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP2xR =  m_period_middle_ticks + count + 200;

		// TIMER_x should NOT be updated directly
		hhrtim.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR =  m_period_middle_ticks - count - 200;
		hhrtim.Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP2xR =  m_period_middle_ticks + count + 200;

		// TIMER_C should be updated over DMA

		count += 10; // watch it with oscilloscope
		if( count > 800 )
		{
			count = 0;
		}
	}

	if(uwTick % 500 == 0)
	{
		static uint16_t do_it = 1; // chnge the value with debugger

		if( do_it )
		{
			// TIMER_C should be updated over DMA
			HAL_StatusTypeDef volatile ret =
					HAL_HRTIM_BurstDMATransfer( &hhrtim, HRTIM_TIMERINDEX_MASTER, (uint32_t)dma_buffr, 1 );
			if( ret != HAL_OK)
			{
				Error_Handler();
			}
//			uint32_t tmp = dma_buffr[0];
//			dma_buffr[0] = dma_buffr[1];
//			dma_buffr[1] = tmp;
		}
	}




	  HAL_IncTick();

  /* USER CODE END SysTick_IRQn 0 */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_hrtim1_m);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 global interrupts.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */

  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles DMAMUX1 overrun interrupt.
  */
void DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMAMUX1_OVR_IRQn 0 */

  /* USER CODE END DMAMUX1_OVR_IRQn 0 */
  // Handle DMA1_Stream2
  HAL_DMAEx_MUX_IRQHandler(&hdma_hrtim1_m);
  /* USER CODE BEGIN DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMAMUX1_OVR_IRQn 1 */
}

/**
  * @brief This function handles HRTIM timer D global interrupt.
  */
void HRTIM1_TIMD_IRQHandler(void)
{
  /* USER CODE BEGIN HRTIM1_TIMD_IRQn 0 */

  /* USER CODE END HRTIM1_TIMD_IRQn 0 */
  HAL_HRTIM_IRQHandler(&hhrtim,HRTIM_TIMERINDEX_TIMER_D);
  /* USER CODE BEGIN HRTIM1_TIMD_IRQn 1 */

  /* USER CODE END HRTIM1_TIMD_IRQn 1 */
}

/**
  * @brief This function handles HRTIM timer E global interrupt.
  */
void HRTIM1_TIME_IRQHandler(void)
{
  /* USER CODE BEGIN HRTIM1_TIME_IRQn 0 */

  /* USER CODE END HRTIM1_TIME_IRQn 0 */
  HAL_HRTIM_IRQHandler(&hhrtim,HRTIM_TIMERINDEX_TIMER_E);
  /* USER CODE BEGIN HRTIM1_TIME_IRQn 1 */

  /* USER CODE END HRTIM1_TIME_IRQn 1 */
}

/* USER CODE BEGIN 1 */


void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  //set the breakpoint here
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  //set the breakpoint
}

/* USER CODE END 1 */
