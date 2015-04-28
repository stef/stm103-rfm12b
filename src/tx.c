#include <stdint.h>
#include <string.h>
#include "inc/stm32f10x_map.h"
#include "inc/stm32f10x_rcc.h"
#include "inc/stm32f10x_gpio.h"
#include "inc/stm32f10x_spi.h"
#include "delay.h"

#define RFM12_CMD_RESET         0xFE00  // performs software reset of RFM12 Module
#define BITRATE ((uint32_t) ((1000000/4800)*(4.0/3))) // 1bit / BITRATE us - adjusted a bit

uint8_t rf12_xfer(uint16_t data){
  uint16_t res;
  GPIO_ResetBits(GPIOA, GPIO_Pin_4);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1, data);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  res = SPI_I2S_ReceiveData(SPI1);
  GPIO_SetBits(GPIOA, GPIO_Pin_4);
  return res;
}

void rfm_hw_init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

  SPI_InitTypeDef SPI_InitTypeDefStruct;

  SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
  SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;

  SPI_Init(SPI1, &SPI_InitTypeDefStruct);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitTypeDefStruct;

  GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
  GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);

  GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitTypeDefStruct);

  GPIO_SetBits(GPIOA, GPIO_Pin_4);

  SPI_Cmd(SPI1, ENABLE);
}

void rfm_init(uint16_t freq) {
  rf12_xfer(0x0000);//just in case
  rf12_xfer(0x8205);//sleep mode
  uDelay(100);
  rf12_xfer(0x8098);//EL,!ef,433band,12.5pF
  rf12_xfer(0x8239);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
  rf12_xfer(0xA000|(freq&0xfff));
  rf12_xfer(0xC647);//4.8kbps
  rf12_xfer(0x94A2);//VDI,FAST,134kHz,0dBm,-91Bm
  rf12_xfer(0xC2AC);//AL,!ml,DIG,DQD4
  rf12_xfer(0xCA81);//FIFO8,SYNC,!ff,DR
  rf12_xfer(0xCED4);//SYNC=2DD4;
  //rf12_xfer(0xC483);//@PWR,NO RSTRIC,!st,!fi,OE,EN
  rf12_xfer(0xC400);//!auto,NO RSTRIC,!st,!fi,!oe,!en
  rf12_xfer(0x9857);//!mp,9810=30kHz,MAX OUT
  rf12_xfer(0xCC77);//OB1,OB0,!lpx,!ddy,DDIT,BW0
  rf12_xfer(0xE000);//NOT USE
  rf12_xfer(0xC800);//NOT USE
  rf12_xfer(0xC040);//1.66MHz,2.2V
}

void hello(void) {
  const uint8_t hello[] = "\x2d\xe9Hello World";
  uint16_t status;
  (void)status;
  uint16_t i,j;
  for(i=0;i<8;i++) {
    rf12_xfer(0xb8aa);
    uDelay(BITRATE);
  }
  for(j=0;hello[j]!=0;j++) {
    rf12_xfer(0xb800 | hello[j]);
    uDelay(BITRATE);
  }
  for(i=0;i<(128-21);i++) {
    rf12_xfer(0xb800 | i);
    uDelay(BITRATE);
  }
  for(i=0;i!=32;i++) {
    rf12_xfer(0xb800);
    uDelay(BITRATE);
  }
  for(i=0;i!=32;i++) {
    rf12_xfer(0xb8ff);
    uDelay(BITRATE);
  }
  for(i=0;i!=64;i++) {
    rf12_xfer(0xb855);
    uDelay(BITRATE);
  }
}

void test(void) {
  rfm_hw_init();

  rfm_init(0x640);

  while(1) {
    rf12_xfer(0x8239);//!er,!ebb,ET,ES,EX,!eb,!ew,DC
    uDelay(250);
    hello();
    rf12_xfer(0x8205);//sleep mode
    mDelay(500);
  }
}
