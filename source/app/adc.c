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
#include "adc.h"

void vAdcInit()
{
    /* set the pis B0 as input ADC */
    MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF0_Msk | GPIO_CRL_MODE0_Msk, 0);

    /* SW triggered ADC*/
    MODIFY_REG(ADC1->CR2, ADC_CR2_EXTTRIG_Msk, ADC_CR2_EXTTRIG);
    MODIFY_REG(ADC1->CR2, ADC_CR2_EXTSEL_Msk, ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1 | ADC_CR2_EXTSEL_2);

    /* select B0 channel for conversion */
    MODIFY_REG(ADC1->SQR1, ADC_SQR1_L_Msk, 0);
    MODIFY_REG(ADC1->SQR3, ADC_SQR3_SQ1_Msk, 8);

    /* ADC 1 ON */
    MODIFY_REG(ADC1->CR2, ADC_CR2_ADON_Msk, ADC_CR2_ADON);


    /* enable interupt */
    //NVIC_SetPriority(ADC1_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 11 /* PreemptPriority */, 0 /* SubPriority */));
    //NVIC_EnableIRQ(ADC1_2_IRQn);
}

uint16_t xAdcAnalogRead()
{
  CLEAR_BIT(ADC1->SR, ADC_SR_STRT);
  MODIFY_REG(ADC1->CR2, ADC_CR2_SWSTART_Msk, ADC_CR2_SWSTART);

  /* wait for start of conversion */
  do {
  } while (!(ADC1->SR & ADC_SR_STRT));

  /* wait for end of conversion */
  do {
  } while (!(ADC1->SR & ADC_SR_EOC));

  return ADC1->DR;
}

//void ADC1_2_IRQHandler(void)
//{
//  /* USER CODE BEGIN ADC1_2_IRQn 0 */
//
//  /* USER CODE END ADC1_2_IRQn 0 */
//  HAL_ADC_IRQHandler(&hadc1);
//  /* USER CODE BEGIN ADC1_2_IRQn 1 */
//
//  /* USER CODE END ADC1_2_IRQn 1 */
//}