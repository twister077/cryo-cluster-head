#include "power.h"
#include "stm32l0xx_hal.h"

void Power_Init(void) {
    __HAL_RCC_PWR_CLK_ENABLE();
}

void Power_EnterSleepMode(void) {
    /* CLEARSLEEP not set: normal Sleep mode, wakes on SysTick interrupt */
    SCB->SCR = (SCB->SCR & ~SCB_SCR_SLEEPDEEP_Msk) | SCB_SCR_SLEEPONEXIT_Msk;
    __WFI();
    /* after wake: re-disable sleep-on-exit so main loop keeps running */
    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    __DSB();
    __ISB();
}