#include "power.h"
#include "stm32l0xx_hal.h"

void Power_Init(void) {
    __HAL_RCC_PWR_CLK_ENABLE();
}

void Power_EnterSleepMode(void) {
    /* SLEEPDEEP not set: normal Sleep mode, wakes on SysTick */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}

void Power_EnterStopMode(void) {
    /* STOP0 mode: lowest power while retaining SRAM.
       After wake: re-enable HSI and restore SysTick. */
    HAL_SuspendTick();

    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    SystemCoreClock = 16000000U; /* 16 MHz HSI */
    HAL_ResumeTick();
}
