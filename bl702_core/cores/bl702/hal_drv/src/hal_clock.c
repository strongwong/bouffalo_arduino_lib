/**
 * @file hal_clock.c
 * @brief
 *
 * Copyright (c) 2021 Bouffalolab team
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 */

#include "bl702_glb.h"
#include "bl702_pwm.h"
#include "hal_clock.h"
#include "hal_mtimer.h"

static uint32_t mtimer_get_clk_src_div(void)
{
    return ((SystemCoreClockGet() / (GLB_Get_BCLK_Div() + 1)) / 1000 / 1000 - 1);
}

void system_clock_init(void)
{
    /*select root clock*/
    GLB_Set_System_CLK(CLOCK_XTAL, BSP_ROOT_CLOCK_SOURCE - 2);
    /*set fclk/hclk and bclk clock*/
    GLB_Set_System_CLK_Div(BSP_FCLK_DIV, BSP_BCLK_DIV);
    /* Set MTimer the same frequency as SystemCoreClock */
    GLB_Set_MTimer_CLK(1, GLB_MTIMER_CLK_BCLK, mtimer_get_clk_src_div());
#ifdef BSP_AUDIO_PLL_CLOCK_SOURCE
    PDS_Set_Audio_PLL_Freq(BSP_AUDIO_PLL_CLOCK_SOURCE - ROOT_CLOCK_SOURCE_AUPLL_12288000_HZ);
#endif
}
void peripheral_clock_init(void)
{
#if defined(BSP_USING_UART0) || defined(BSP_USING_UART1)
#if BSP_UART_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_PLL_96M
    GLB_Set_UART_CLK(ENABLE, HBN_UART_CLK_96M, BSP_UART_CLOCK_DIV);
#elif BSP_UART_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_FCLK
    GLB_Set_UART_CLK(ENABLE, HBN_UART_CLK_FCLK, BSP_UART_CLOCK_DIV);
#else
#error "please select correct uart clock source"
#endif
#endif

#if defined(BSP_USING_I2C0)
#if BSP_I2C_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_BCLK
    GLB_Set_I2C_CLK(ENABLE, BSP_I2C_CLOCK_DIV);
#else
#error "please select correct i2c clock source"
#endif
#endif

#if defined(BSP_USING_SPI0)
#if BSP_SPI_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_BCLK
    GLB_Set_SPI_CLK(ENABLE, BSP_SPI_CLOCK_DIV);
#else
#error "please select correct spi clock source"
#endif
#endif

#if defined(BSP_USING_PWM_CH0) || defined(BSP_USING_PWM_CH1) || defined(BSP_USING_PWM_CH2) || defined(BSP_USING_PWM_CH3) || defined(BSP_USING_PWM_CH4) || defined(BSP_USING_PWM_CH5)
    /* Disable clock gate */
    GLB_AHB_Slave1_Clock_Gate(DISABLE, BL_AHB_SLAVE1_PWM);
    uint32_t timeoutCnt = 160 * 1000;
    uint32_t tmpVal;
    uint32_t PWMx;
#if BSP_PWM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_32K_CLK

    for (int i = 0; i < 5; i++) {
        PWMx = PWM_BASE + PWM_CHANNEL_OFFSET + (i)*0x20;
        tmpVal = BL_RD_REG(PWMx, PWM_CONFIG);
        BL_WR_REG(PWMx, PWM_CONFIG, BL_SET_REG_BIT(tmpVal, PWM_STOP_EN));

        while (!BL_IS_REG_BIT_SET(BL_RD_REG(PWMx, PWM_CONFIG), PWM_STS_TOP)) {
            timeoutCnt--;

            if (timeoutCnt == 0) {
                return;
            }
        }
        tmpVal = BL_RD_REG(PWMx, PWM_CONFIG);
        tmpVal = BL_SET_REG_BITS_VAL(tmpVal, PWM_REG_CLK_SEL, PWM_CLK_32K);
        BL_WR_REG(PWMx, PWM_CONFIG, tmpVal);
        /* Config pwm division */
        BL_WR_REG(PWMx, PWM_CLKDIV, BSP_PWM_CLOCK_DIV);
    }
#elif BSP_PWM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_BCLK

    for (int i = 0; i < 5; i++) {
        PWMx = PWM_BASE + PWM_CHANNEL_OFFSET + (i)*0x20;
        tmpVal = BL_RD_REG(PWMx, PWM_CONFIG);
        BL_WR_REG(PWMx, PWM_CONFIG, BL_SET_REG_BIT(tmpVal, PWM_STOP_EN));

        while (!BL_IS_REG_BIT_SET(BL_RD_REG(PWMx, PWM_CONFIG), PWM_STS_TOP)) {
            timeoutCnt--;

            if (timeoutCnt == 0) {
                return;
            }
        }
        tmpVal = BL_RD_REG(PWMx, PWM_CONFIG);
        tmpVal = BL_SET_REG_BITS_VAL(tmpVal, PWM_REG_CLK_SEL, PWM_CLK_BCLK);
        BL_WR_REG(PWMx, PWM_CONFIG, tmpVal);
        /* Config pwm division */
        BL_WR_REG(PWMx, PWM_CLKDIV, BSP_PWM_CLOCK_DIV);
    }
#elif BSP_PWM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK

    for (int i = 0; i < 5; i++) {
        PWMx = PWM_BASE + PWM_CHANNEL_OFFSET + (i)*0x20;
        tmpVal = BL_RD_REG(PWMx, PWM_CONFIG);
        BL_WR_REG(PWMx, PWM_CONFIG, BL_SET_REG_BIT(tmpVal, PWM_STOP_EN));

        while (!BL_IS_REG_BIT_SET(BL_RD_REG(PWMx, PWM_CONFIG), PWM_STS_TOP)) {
            timeoutCnt--;

            if (timeoutCnt == 0) {
                return;
            }
        }
        tmpVal = BL_RD_REG(PWMx, PWM_CONFIG);
        tmpVal = BL_SET_REG_BITS_VAL(tmpVal, PWM_REG_CLK_SEL, PWM_CLK_XCLK);
        BL_WR_REG(PWMx, PWM_CONFIG, tmpVal);
        /* Config pwm division */
        BL_WR_REG(PWMx, PWM_CLKDIV, BSP_PWM_CLOCK_DIV);
    }
#else
#error "please select correct pwm clock source"
#endif
#endif

#if defined(BSP_USING_IR)
    GLB_Set_IR_CLK(ENABLE, 0, BSP_IR_CLOCK_DIV);
#endif

#if defined(BSP_USING_I2S0)
    GLB_Set_I2S_CLK(ENABLE, GLB_I2S_OUT_REF_CLK_NONE);
#endif

#if defined(BSP_USING_ADC0)
#if BSP_ADC_CLOCK_SOURCE >= ROOT_CLOCK_SOURCE_AUPLL_12288000_HZ
    GLB_Set_ADC_CLK(ENABLE, GLB_ADC_CLK_AUDIO_PLL, BSP_ADC_CLOCK_DIV);
#elif BSP_ADC_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
    GLB_Set_ADC_CLK(ENABLE, GLB_ADC_CLK_XCLK, BSP_ADC_CLOCK_DIV);
#else
#error "please select correct adc clock source"
#endif
#endif

#if defined(BSP_USING_DAC0)
#if BSP_DAC_CLOCK_SOURCE >= ROOT_CLOCK_SOURCE_AUPLL_12288000_HZ
    GLB_Set_DAC_CLK(ENABLE, GLB_DAC_CLK_AUDIO_PLL, BSP_DAC_CLOCK_DIV);
#elif BSP_DAC_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
    GLB_Set_DAC_CLK(ENABLE, GLB_DAC_CLK_XCLK, BSP_DAC_CLOCK_DIV);
#else
#error "please select correct dac clock source"
#endif
#endif

#if defined(BSP_USING_CAM)
#if BSP_CAM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_PLL_96M
    GLB_Set_CAM_CLK(ENABLE, GLB_CAM_CLK_DLL96M, BSP_CAM_CLOCK_DIV);
    GLB_SWAP_EMAC_CAM_Pin(GLB_EMAC_CAM_PIN_CAM);
#elif BSP_CAM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
    GLB_Set_CAM_CLK(ENABLE, GLB_CAM_CLK_XCLK, BSP_CAM_CLOCK_DIV);
    GLB_SWAP_EMAC_CAM_Pin(GLB_EMAC_CAM_PIN_CAM);
#else
#error "please select correct camera clock source"
#endif
#endif

#if defined(BSP_USING_QDEC0) || defined(BSP_USING_QDEC1) || defined(BSP_USING_QDEC2) || defined(BSP_USING_KEYSCAN)
#if BSP_QDEC_KEYSCAN_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_32K_CLK
    GLB_Set_QDEC_CLK(GLB_QDEC_CLK_F32K, BSP_QDEC_KEYSCAN_CLOCK_DIV);
#elif BSP_QDEC_KEYSCAN_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
    GLB_Set_QDEC_CLK(GLB_QDEC_CLK_XCLK, BSP_QDEC_KEYSCAN_CLOCK_DIV);
#else
#error "please select correct qdec or keyscan clock source"
#endif
#endif

#if defined(BSP_USING_USB)
    GLB_Set_USB_CLK(1);
#endif
}
uint32_t system_clock_get(enum system_clock_type type)
{
    switch (type) {
        case SYSTEM_CLOCK_ROOT_CLOCK:
            return SystemCoreClockGet();

        case SYSTEM_CLOCK_FCLK:
            return (SystemCoreClockGet() / (GLB_Get_HCLK_Div() + 1));

        case SYSTEM_CLOCK_BCLK:
            return (SystemCoreClockGet() / ((GLB_Get_HCLK_Div() + 1) * (GLB_Get_BCLK_Div() + 1)));

        case SYSTEM_CLOCK_XCLK:
            return 32000000;
        case SYSTEM_CLOCK_32K_CLK:
            return 32000;
        case SYSTEM_CLOCK_AUPLL:
            if (BSP_AUDIO_PLL_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_AUPLL_12288000_HZ) {
                return 12288000;
            } else if (BSP_AUDIO_PLL_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_AUPLL_11289600_HZ) {
                return 11289600;
            } else if (BSP_AUDIO_PLL_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_AUPLL_5644800_HZ) {
                return 5644800;
            } else if (BSP_AUDIO_PLL_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_AUPLL_24576000_HZ) {
                return 24576000;
            } else if (BSP_AUDIO_PLL_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_AUPLL_24000000_HZ) {
                return 24000000;
            }
        default:
            break;
    }

    return 0;
}
uint32_t peripheral_clock_get(enum peripheral_clock_type type)
{
    uint32_t tmpVal;
    uint32_t div;

    switch (type) {
        case PERIPHERAL_CLOCK_UART:
#if defined(BSP_USING_UART0) || defined(BSP_USING_UART1)
#if BSP_UART_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_PLL_96M
            return 96000000;
#elif BSP_UART_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_FCLK
            return system_clock_get(SYSTEM_CLOCK_FCLK) / (GLB_Get_HCLK_Div() + 1));
#endif
#endif

        case PERIPHERAL_CLOCK_SPI:
#if defined(BSP_USING_SPI0)
#if BSP_SPI_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_BCLK
            tmpVal = BL_RD_REG(GLB_BASE, GLB_CLK_CFG3);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_SPI_CLK_DIV);
            return system_clock_get(SYSTEM_CLOCK_BCLK) / (div + 1);
#endif
#endif

        case PERIPHERAL_CLOCK_I2C:
#if defined(BSP_USING_I2C0)
#if BSP_I2C_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_BCLK
            tmpVal = BL_RD_REG(GLB_BASE, GLB_CLK_CFG3);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_I2C_CLK_DIV);
            return system_clock_get(SYSTEM_CLOCK_BCLK) / (div + 1);
#endif
#endif

        case PERIPHERAL_CLOCK_I2S:
            return system_clock_get(SYSTEM_CLOCK_AUPLL);

        case PERIPHERAL_CLOCK_ADC:
#if defined(BSP_USING_ADC0)
#if BSP_ADC_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
            tmpVal = BL_RD_REG(GLB_BASE, GLB_GPADC_32M_SRC_CTRL);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_GPADC_32M_CLK_DIV);
            return system_clock_get(SYSTEM_CLOCK_XCLK) / div;
#elif BSP_ADC_CLOCK_SOURCE >= ROOT_CLOCK_SOURCE_AUPLL_12288000_HZ
            tmpVal = BL_RD_REG(GLB_BASE, GLB_GPADC_32M_SRC_CTRL);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_GPADC_32M_CLK_DIV);
            return system_clock_get(SYSTEM_CLOCK_AUPLL) / div;
#endif
#endif

        case PERIPHERAL_CLOCK_DAC:
#if defined(BSP_USING_DAC0)
#if BSP_DAC_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
            tmpVal = BL_RD_REG(GLB_BASE, GLB_DIG32K_WAKEUP_CTRL);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_DIG_512K_DIV);
            return system_clock_get(SYSTEM_CLOCK_XCLK) / (div + 1);
#elif BSP_DAC_CLOCK_SOURCE >= ROOT_CLOCK_SOURCE_AUPLL_12288000_HZ
            tmpVal = BL_RD_REG(GLB_BASE, GLB_DIG32K_WAKEUP_CTRL);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_DIG_512K_DIV);
            return system_clock_get(SYSTEM_CLOCK_AUPLL) / div;
#endif
#endif

        case PERIPHERAL_CLOCK_PWM:
#if defined(BSP_USING_PWM_CH0) || defined(BSP_USING_PWM_CH1) || defined(BSP_USING_PWM_CH2) || defined(BSP_USING_PWM_CH3) || defined(BSP_USING_PWM_CH4) || defined(BSP_USING_PWM_CH5)
#if BSP_PWM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_32K_CLK
            div = BL_RD_REG(PWM_BASE + PWM_CHANNEL_OFFSET, PWM_CLKDIV);
            return system_clock_get(SYSTEM_CLOCK_32K_CLK) / div;
#elif BSP_PWM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_BCLK
            div = BL_RD_REG(PWM_BASE + PWM_CHANNEL_OFFSET, PWM_CLKDIV);
            return system_clock_get(SYSTEM_CLOCK_BCLK) / div;
#elif BSP_PWM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
            div = BL_RD_REG(PWM_BASE + PWM_CHANNEL_OFFSET, PWM_CLKDIV);
            return system_clock_get(SYSTEM_CLOCK_XCLK) / div;
#endif
#endif
            break;

        case PERIPHERAL_CLOCK_CAM:
#if defined(BSP_USING_CAM)
#if BSP_CAM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_PLL_96M
            tmpVal = BL_RD_REG(GLB_BASE, GLB_CLK_CFG1);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_REG_CAM_REF_CLK_DIV);
            return (96000000 / (div + 1));
#elif BSP_CAM_CLOCK_SOURCE == ROOT_CLOCK_SOURCE_XCLK
            tmpVal = BL_RD_REG(GLB_BASE, GLB_CLK_CFG1);
            div = BL_GET_REG_BITS_VAL(tmpVal, GLB_REG_CAM_REF_CLK_DIV);
            return (system_clock_get(SYSTEM_CLOCK_XCLK) / (div + 1));
#endif
#endif
            break;

        default:

            break;
    }

    (void)(tmpVal);
    (void)(div);
    return 0;
}