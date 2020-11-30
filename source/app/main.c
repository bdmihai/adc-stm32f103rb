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
 |  Author: Mihai Baneu                           Last modified: 24.Nov.2020  |
 |                                                                            |
 |___________________________________________________________________________*/

#include "stm32f1xx.h"
#include "stm32rtos.h"
#include "task.h"
#include "queue.h"
#include "system.h"
#include "gpio.h"
#include "isr.h"
#include "utils.h"
#include "ssd1339.h"
#include "printf.h"

typedef struct AdcEvent_t {
    uint16_t digitalValue;
} AdcEvent_t;

extern const uint8_t u8x8_font_courB18_2x3_r[];

/* Queue used to send and receive complete struct AMessage structures. */
QueueHandle_t xAdcQueue = NULL;

static void vTaskLED(void *pvParameters)
{
    (void)pvParameters;

    /* led OFF */
    vGPIOSetLed();

    for (;;) {
        vGPIOSetLed();
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        vGPIOResetLed();
        vTaskDelay(100 / portTICK_PERIOD_MS);

        vGPIOSetLed();
        vTaskDelay(100 / portTICK_PERIOD_MS);

        vGPIOResetLed();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

static void vTaskDisplay(void *pvParameters)
{
    AdcEvent_t adcEvent;
    (void)pvParameters;

    ssd1339_init();
    ssd1339_set_column_address(0, SSD1339_128_COLS-1);
    ssd1339_set_row_address(0, SSD1339_128_RWOS-1);
    ssd1339_set_map_and_color_depth(SSD1339_REMAP_MODE_ODD_EVEN | SSD1339_COLOR_MODE_65K);
    ssd1339_set_use_buildin_lut();
    ssd1339_set_sleep_mode(SSD1339_SLEEP_OFF);
    ssd1339_set_display_mode(SSD1339_MODE_ALL_OFF);
    ssd1339_set_master_contrast_curent(100);
    ssd1339_set_contrast_curent(1,200,200);
    ssd1339_set_precharge_voltage(0b00000011, 0b01100000, 0b10000000);
    ssd1339_set_display_mode(SSD1339_MODE_RESET_TO_NORMAL_DISPLAY);
    ssd1339_draw_clear(0, 0, SSD1339_128_COLS-1, SSD1339_128_COLS-1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    for (;;) {
        if (xQueueReceive(xAdcQueue, &adcEvent, portMAX_DELAY) == pdPASS) {
            char txt[6];
            float voltage = adcEvent.digitalValue * 3.3f / 4096.0f;
            sprintf(txt, "%4.2f V", voltage);
            ssd1339_draw_string(u8x8_font_courB18_2x3_r, 20, 50, SSD1339_WHITE, SSD1339_BLACK, txt);
        }
    }
}

static void vTaskAdc(void *pvParameters)
{
    AdcEvent_t adcEvent;
    (void)pvParameters;

    for (;;) {
        adcEvent.digitalValue = xAdcAnalogRead();
        xQueueSend(xAdcQueue, &adcEvent, (TickType_t) 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

int main(void)
{
    /* initialize the system */
    vSystemInit();

    /* initialize the gpio */
    vGPIOInit();

    /* initialize the interupt service routines */
    vISRInit();

    /* initialize utility module */
    vUtilsInit();

    /* initialize the adc */
    vAdcInit();

    /* create the queues */
    xAdcQueue = xQueueCreate(1, sizeof(AdcEvent_t));

    /* create the tasks specific to this application. */
    xTaskCreate(vTaskLED, (const portCHAR *)"vTaskLED", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
    xTaskCreate(vTaskDisplay, (const portCHAR *)"vTaskDisplay", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vTaskAdc, (const portCHAR *)"vTaskAdc", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    /* start the scheduler. */
    vTaskStartScheduler();

    /* should never get here ... */
    vBlink(10);
    return 0;
}
