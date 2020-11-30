/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2020 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 15.May.2020  |
 |                                                                            |
 |___________________________________________________________________________*/

#include "stm32f1xx.h"
#include "stm32rtos.h"
#include "utils.h"

void vUtilsInit()
{
    /* enable DWT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void vDelay(const int clocks)
{
    DWT->CYCCNT = 0;
    do {
    } while (DWT->CYCCNT < clocks);
}

void vBlink(const int n)
{
    /* enable port C clock */
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

    /* set the pis as output low speed (max 2MHz) */
    MODIFY_REG(GPIOC->CRH, GPIO_CRH_CNF13_Msk | GPIO_CRH_MODE13_Msk, GPIO_CRH_MODE13_1);

    /* enable DWT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    for (;;) {
        for (int i = 0; i < n; i++) {
            GPIOC->BSRR = GPIO_BSRR_BR13;
            DWT->CYCCNT = 0;
            do {
            } while (DWT->CYCCNT < (configCPU_CLOCK_HZ / 4));

            GPIOC->BSRR = GPIO_BSRR_BS13;
            DWT->CYCCNT = 0;
            do {
            } while (DWT->CYCCNT < (configCPU_CLOCK_HZ / 4));
        }
        DWT->CYCCNT = 0;
        do {
        } while (DWT->CYCCNT < (configCPU_CLOCK_HZ * 2));
    }
}

int _write(int file, char *ptr, int len)
{
    (void)(file);
    for (int i = 0; i < len; i++) {
        ITM_SendChar((*ptr++));
    }
    return len;
}
