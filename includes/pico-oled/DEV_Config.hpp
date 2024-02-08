#ifndef DEV_CONFIG_H_
#define DEV_CONFIG_H_
#include "types.hpp"

#include <cstdio>
#include <hardware/i2c.h>
#include <hardware/pwm.h>
#include <hardware/spi.h>
#include <pico/stdlib.h>

/**
 * GPIOI config
 **/
extern uint EPD_RST_PIN;
extern uint EPD_DC_PIN;
extern uint EPD_CS_PIN;
extern uint EPD_BL_PIN;
extern uint EPD_CLK_PIN;
extern uint EPD_MOSI_PIN;
extern uint EPD_SCL_PIN;
extern uint EPD_SDA_PIN;

/*------------------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void DEV_Digital_Write(u16 Pin, u8 Value);
u8 DEV_Digital_Read(u16 Pin);

void DEV_GPIO_Mode(u16 Pin, u16 Mode);
void DEV_KEY_Config(u16 Pin);
void DEV_Digital_Write(u16 Pin, u8 Value);
u8 DEV_Digital_Read(u16 Pin);

void DEV_SPI_WriteByte(u8 Value);
void DEV_SPI_Write_nByte(uint8_t *pData, uint32_t Len);

void DEV_Delay_ms(u32 xms);
void DEV_Delay_us(u32 xus);

void DEV_I2C_Write(uint8_t addr, uint8_t reg, uint8_t Value);
void DEV_I2C_Write_nByte(uint8_t addr, uint8_t *pData, uint32_t Len);
uint8_t DEV_I2C_ReadByte(uint8_t addr, uint8_t reg);

void DEV_SET_PWM(uint8_t Value);

u8 DEV_Module_Init(void);
void DEV_Module_Exit(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
