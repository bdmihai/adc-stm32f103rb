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
 |  Author: Mihai Baneu                           Last modified: 26.Nov.2020  |
 |                                                                            |
 |___________________________________________________________________________*/

/* 
  Text drawing is based on the :
  u8x8 original code from https://github.com/olikraus/u8g2/
  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.
*/

#include "stm32f1xx.h"
#include "ssd1339.h"

#define CS_HIGH        (GPIOB->BSRR = GPIO_BSRR_BS5)
#define CS_LOW         (GPIOB->BSRR = GPIO_BSRR_BR5)
#define RES_HIGH       (GPIOB->BSRR = GPIO_BSRR_BS6)
#define RES_LOW        (GPIOB->BSRR = GPIO_BSRR_BR6)
#define DC_HIGH        (GPIOB->BSRR = GPIO_BSRR_BS7)
#define DC_LOW         (GPIOB->BSRR = GPIO_BSRR_BR7)
#define WR_HIGH        (GPIOB->BSRR = GPIO_BSRR_BS8)
#define WR_LOW         (GPIOB->BSRR = GPIO_BSRR_BR8)
#define RD_HIGH        (GPIOB->BSRR = GPIO_BSRR_BS9)
#define RD_LOW         (GPIOB->BSRR = GPIO_BSRR_BR9)
#define CS_WR_HIGH     (GPIOB->BSRR = GPIO_BSRR_BS5 | GPIO_BSRR_BS8)
#define CS_WR_LOW      (GPIOB->BSRR = GPIO_BSRR_BR5 | GPIO_BSRR_BR8)
#define CS_RD_HIGH     (GPIOB->BSRR = GPIO_BSRR_BS5 | GPIO_BSRR_BS9)
#define CS_RD_LOW      (GPIOB->BSRR = GPIO_BSRR_BR5 | GPIO_BSRR_BR9)
#define DC_CS_WR_HIGH  (GPIOB->BSRR = GPIO_BSRR_BS5 | GPIO_BSRR_BS8 | GPIO_BSRR_BS7)
#define DC_CS_WR_LOW   (GPIOB->BSRR = GPIO_BSRR_BR5 | GPIO_BSRR_BR8 | GPIO_BSRR_BR7)
#define DATA_WR(d)     (GPIOA->ODR = d)
#define DATA_RD        (GPIOA->IDR)

static void config_data_out()
{
    /* set the pis 0-7 as output low speed (max 2MHz) */
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF0_Msk | GPIO_CRL_MODE0_Msk, GPIO_CRL_MODE0_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF1_Msk | GPIO_CRL_MODE1_Msk, GPIO_CRL_MODE1_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF2_Msk | GPIO_CRL_MODE2_Msk, GPIO_CRL_MODE2_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF3_Msk | GPIO_CRL_MODE3_Msk, GPIO_CRL_MODE3_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF4_Msk | GPIO_CRL_MODE4_Msk, GPIO_CRL_MODE4_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk, GPIO_CRL_MODE5_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk, GPIO_CRL_MODE6_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk, GPIO_CRL_MODE7_1);
}

static void config_data_in()
{
    /* set the pis 0-7 as input floating*/
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF0_Msk | GPIO_CRL_MODE0_Msk, GPIO_CRL_CNF0_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF1_Msk | GPIO_CRL_MODE1_Msk, GPIO_CRL_CNF1_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF2_Msk | GPIO_CRL_MODE2_Msk, GPIO_CRL_CNF2_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF3_Msk | GPIO_CRL_MODE3_Msk, GPIO_CRL_CNF3_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF4_Msk | GPIO_CRL_MODE4_Msk, GPIO_CRL_CNF4_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk, GPIO_CRL_CNF5_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk, GPIO_CRL_CNF6_1);
    MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk, GPIO_CRL_CNF7_1);
}

static void config_control_out()
{
    /* set the pis B5-9 as output low speed (max 2MHz) */
    MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF5_Msk | GPIO_CRL_MODE5_Msk, GPIO_CRL_MODE5_1);
    MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF6_Msk | GPIO_CRL_MODE6_Msk, GPIO_CRL_MODE6_1);
    MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF7_Msk | GPIO_CRL_MODE7_Msk, GPIO_CRL_MODE7_1);
    MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF8_Msk | GPIO_CRH_MODE8_Msk, GPIO_CRH_MODE8_1);
    MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF9_Msk | GPIO_CRH_MODE9_Msk, GPIO_CRH_MODE9_1);
}

void ssd1339_init()
{
    config_control_out();
    config_data_out();
    DC_CS_WR_HIGH;
    RES_HIGH;
    RD_HIGH;
}

void ssd1339_reset_on()
{
    RES_LOW;
}

void ssd1339_reset_off()
{
    RES_HIGH;
}

void ssd1339_command(uint8_t command)
{
    DC_CS_WR_LOW;
    DATA_WR(command);
    DC_CS_WR_HIGH;
}

void ssd1339_write8(uint8_t data)
{
    CS_WR_LOW;
    DATA_WR(data);
    CS_WR_HIGH;
}

void ssd1339_write16(uint16_t data)
{
    CS_WR_LOW;
    DATA_WR((data >> 8) & 0xFF);
    CS_WR_HIGH;
    CS_WR_LOW;
    DATA_WR(data & 0xFF);
    CS_WR_HIGH;
}

void ssd1339_write(const uint8_t *buffer, int size)
{
    while (size--) {
        CS_WR_LOW;
        DATA_WR(*(buffer++));
        CS_WR_HIGH;
    }
}

uint8_t ssd1339_read8()
{
    uint8_t data;
    config_data_in();
    CS_LOW;
    RD_LOW;
    data = DATA_RD;
    RD_HIGH;
    CS_HIGH;
    config_data_out();
    return data;
}

void ssd1339_set_column_address(uint8_t start, uint8_t end)
{
    ssd1339_command(SSD1339_CMD_SET_COLUMN_ADDRESS);
    ssd1339_write8(start);
    ssd1339_write8(end);
}

void ssd1339_set_row_address(uint8_t start, uint8_t end)
{
    ssd1339_command(SSD1339_CMD_SET_ROW_ADDRESS);
    ssd1339_write8(start);
    ssd1339_write8(end);
}

void ssd1339_set_write_ram()
{
    ssd1339_command(SSD1339_CMD_SET_WRITE_RAM);
}

void ssd1339_set_read_ram()
{
    ssd1339_command(SSD1339_CMD_SET_READ_RAM);
}

void ssd1339_set_map_and_color_depth(uint8_t mode)
{
    ssd1339_command(SSD1339_CMD_SET_MAPAND_COLOR_DEPTH);
    ssd1339_write8(mode);
}

void ssd1339_set_display_start_line(uint8_t line)
{
    ssd1339_command(SSD1339_CMD_SET_DISPLAY_START_LINE);
    ssd1339_write8(line);
}

void ssd1339_set_display_offset(uint8_t offset)
{
    ssd1339_command(SSD1339_CMD_SET_DISPLAY_OFFSET);
    ssd1339_write8(offset);
}

void ssd1339_set_display_mode(uint8_t mode)
{
    ssd1339_command(SSD1339_CMD_SET_DISPLAY_MODE | mode);
}

void ssd1339_set_master_configuration(uint8_t config)
{
    ssd1339_command(SSD1339_CMD_SET_MASTER_CONFIGURATION);
    ssd1339_write8(config);
}

void ssd1339_set_sleep_mode(uint8_t mode)
{
    ssd1339_command(SSD1339_CMD_SET_SLEEP_MODE | mode);
}

void ssd1339_set_power_saving_mode(uint8_t mode)
{
    ssd1339_command(SSD1339_CMD_SET_POWER_SAVING_MODE);
    ssd1339_write8(mode);
}

void ssd1339_set_reset_and_precharge(uint8_t reset, uint8_t precharge)
{
    ssd1339_command(SSD1339_CMD_SET_RESET_AND_PRECHARGE);
    ssd1339_write8(reset | (precharge << 4));
}

void ssd1339_set_clockdiv_and_oscilator(uint8_t clockdiv, uint8_t oscilator)
{
    ssd1339_command(SSD1339_CMD_SET_CLOCKDIV_AND_OSCILATOR);
    ssd1339_write8(clockdiv | (oscilator << 4));
}

void ssd1339_set_use_buildin_lut()
{
    ssd1339_command(SSD1339_CMD_SET_USE_BUILTIN_LUT);
}

void ssd1339_set_precharge_voltage(uint8_t a, uint8_t b, uint8_t c)
{
    ssd1339_command(SSD1339_CMD_SET_PRECHARGE_VOLTAGE);
    ssd1339_write8(a);
    ssd1339_write8(b);
    ssd1339_write8(c);
}

void ssd1339_set_vcomh(uint8_t vcomh)
{
    ssd1339_command(SSD1339_CMD_SET_VCOMH);
    ssd1339_write8(vcomh);
}

void ssd1339_set_contrast_curent(uint8_t a, uint8_t b, uint8_t c)
{
    ssd1339_command(SSD1339_CMD_SET_CONTRAST_CURRENT);
    ssd1339_write8(a);
    ssd1339_write8(b);
    ssd1339_write8(c);
}

void ssd1339_set_master_contrast_curent(uint8_t contrast)
{
    ssd1339_command(SSD1339_CMD_SET_MASTER_CONTRAST_CURRENT);
    ssd1339_write8(contrast);
}

void ssd1339_set_mux_ratio(uint8_t ratio)
{
    ssd1339_command(SSD1339_CMD_SET_SET_MUX_RATIO);
    ssd1339_write8(ratio);
}

void ssd1339_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    ssd1339_command(SSD1339_CMD_DRAW_LINE);
    ssd1339_write8(x1);
    ssd1339_write8(y1);
    ssd1339_write8(x2);
    ssd1339_write8(y2);
    ssd1339_write16(color);
}

void ssd1339_draw_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color, uint16_t fill)
{
    ssd1339_command(SSD1339_CMD_DRAW_RECTANGLE);
    ssd1339_write8(x1);
    ssd1339_write8(y1);
    ssd1339_write8(x2);
    ssd1339_write8(y2);
    ssd1339_write16(color);
    ssd1339_write16(fill);
}

void ssd1339_draw_circle(uint8_t x1, uint8_t y1, uint8_t r, uint16_t color, uint16_t fill)
{
    ssd1339_command(SSD1339_CMD_DRAW_CIRCLE);
    ssd1339_write8(x1);
    ssd1339_write8(y1);
    ssd1339_write8(r);
    ssd1339_write16(color);
    ssd1339_write16(fill);
}

void ssd1339_draw_copy(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t x3, uint16_t y3)
{
    ssd1339_command(SSD1339_CMD_DRAW_COPY);
    ssd1339_write8(x1);
    ssd1339_write8(y1);
    ssd1339_write8(x2);
    ssd1339_write8(y2);
    ssd1339_write8(x3);
    ssd1339_write8(y3);
}

void ssd1339_draw_dim(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    ssd1339_command(SSD1339_CMD_DRAW_DIM);
    ssd1339_write8(x1);
    ssd1339_write8(y1);
    ssd1339_write8(x2);
    ssd1339_write8(y2);
}

void ssd1339_draw_clear(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    ssd1339_command(SSD1339_CMD_DRAW_CLEAR);
    ssd1339_write8(x1);
    ssd1339_write8(y1);
    ssd1339_write8(x2);
    ssd1339_write8(y2);
}

void ssd1339_set_fill_copy_mode(uint8_t mode)
{
    ssd1339_command(SSD1339_CMD_SET_FILL_COPY_MODE);
    ssd1339_write8(mode);
}

void ssd1339_draw_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    ssd1339_set_column_address(x1, x2-1);
    ssd1339_set_row_address(y1, y2-1);

    ssd1339_set_write_ram();
    for (int i = 0; i < x2-x1; i++) {
        for (int j = 0; j < y2-y1; j++) {
            ssd1339_write16(color);
        }
    }
}

void ssd1339_draw_gradient_hfill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    uint16_t masked;

    ssd1339_set_column_address(x1, x2-1);
    ssd1339_set_row_address(y1, y2-1);
    ssd1339_set_write_ram();
    for (int i = 0; i < x2-x1; i++) {
        for (int j = 0; j < y2-y1; j++) {
            masked = 0;
            masked += (((i/4) % 32) << 11);
            masked += (((i/2) % 64) <<  5);
            masked += ((i/4) % 32);
            ssd1339_write16(color & masked);
        }
    }
}

void ssd1339_draw_gradient_vfill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color)
{
    uint16_t masked;

    ssd1339_set_column_address(x1, x2-1);
    ssd1339_set_row_address(y1, y2-1);
    ssd1339_set_write_ram();
    for (int i = 0; i < x2-x1; i++) {
        for (int j = 0; j < y2-y1; j++) {
            masked = 0;
            masked += (((j/4) % 32) << 11);
            masked += (((j/2) % 64) <<  5);
            masked += ((j/4) % 32);
            ssd1339_write16(color & masked);
        }
    }
}

void ssd1339_draw_image(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, const uint16_t *img)
{
    ssd1339_set_column_address(x1, x2-1);
    ssd1339_set_row_address(y1, y2-1);
    ssd1339_set_write_ram();
    ssd1339_write((const uint8_t *)img, (x2-x1) * (y2-y1) * 2);
}

static uint8_t get_font_data(const uint8_t *adr)
{
    return (*(const uint8_t *)(adr));
}

static void get_glyph_data(const uint8_t *font, uint8_t th, uint8_t tv, uint8_t encoding, uint8_t *buf, uint8_t tile_offset)
{
    uint8_t first, last;
    uint16_t offset;

    first = get_font_data(font + 0);
    last = get_font_data(font + 1);
    if (first <= encoding && encoding <= last) {
        offset = encoding;
        offset -= first;
        offset *= th * tv;
        offset += tile_offset;
        offset *= 8;
        offset += 4;
        for (uint8_t i = 0; i < 8; i++) {
            buf[i] = get_font_data(font + offset);
            offset++;
        }
    } else {
        for (uint8_t i = 0; i < 8; i++) {
            buf[i] = 0;
        }
    }
}

static void draw_tile(const uint8_t *buffer, uint8_t x, uint8_t y, uint16_t color, uint16_t background)
{
    const uint8_t colormask[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
    ssd1339_set_column_address(x, x + 8 - 1);
    ssd1339_set_row_address(y, y + 8 - 1);
    ssd1339_set_write_ram();
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            ssd1339_write16((buffer[j] & colormask[i]) ? color : background);
        }
    }
}

static void draw_glyph(const uint8_t *font, uint8_t x, uint8_t y, uint8_t th, uint8_t tv, uint16_t color, uint16_t background, uint8_t encoding)
{
    uint8_t buffer[8];
    uint8_t tile = 0;;

    for (uint8_t yy = 0; yy < tv; yy++) {
        for (uint8_t xx = 0; xx < th; xx++) {
            get_glyph_data(font, th, tv, encoding, buffer, tile);
            draw_tile(buffer, xx * 8 + x, (tv-yy-1) * 8 + y, color, background);
            tile++;
        }
    }
}

void ssd1339_draw_char(const uint8_t *font, uint8_t x, uint8_t y, uint16_t color, uint16_t background, const char c)
{
    uint8_t th = get_font_data(font + 2);
    uint8_t tv = get_font_data(font + 3);

    draw_glyph(font, x, y, th, tv, color, background, c);
}

void ssd1339_draw_string(const uint8_t *font, uint8_t x, uint8_t y, uint16_t color, uint16_t background, const char *s)
{
    uint8_t th = get_font_data(font + 2);
    uint8_t tv = get_font_data(font + 3);

    while (*s != 0) {
        draw_glyph(font, x, y, th, tv, color, background, *s);
        x += th * 8;
        s++;
    }
}
