#include "IIC.h"
#define WAIT_TIME 0xFFF
#define SCL PBout(5)
#define SDA PBout(6)
#define ReadSDABit PBin(6)
#define SDAInMode() GPIO_InitTypeDef GPIO_InitStructure;\
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;\
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;\
	GPIO_Init(GPIOB, &GPIO_InitStructure)
#define SDAOutMode()  GPIO_InitTypeDef GPIO_InitStructure;\
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;\
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;\
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
	GPIO_Init(GPIOB, &GPIO_InitStructure)
void IICInit(void){
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	SCL = 0;
	SDA = 0;
}
//start signal
void IICStart(void){
    SDAOutMode();
    SDA = 1;
    SCL = 1;
    rt_hw_us_delay(5);
    SDA =0;
    rt_hw_us_delay(6);
    SCL = 0;
}
//stop signal
void IICStop(void){
    SDAOutMode();
    SCL = 0;
    SDA =0;
    SCL = 1;
    rt_hw_us_delay(6);
    SDA = 1;
    rt_hw_us_delay(6);
}
//wait ack
uint8_t IICWaitACK(void){
   uint16_t timeCount = 0;
    SDA = 1;
    rt_hw_us_delay(3);
    SDAInMode();
    SCL = 1;
    rt_hw_us_delay(5);
    while(ReadSDABit){
        if(++timeCount>WAIT_TIME){
            IICStop();
            return 1;
        }
    }
    SCL = 0;
    return 0;
}
void IICACK(void){
    SCL = 0;
    SDAOutMode();
    SDA = 0;
    rt_hw_us_delay(3);
    SCL = 1;
    rt_hw_us_delay(5);
    SCL = 0;
}
void IICNOACK(void){
    SCL = 0;
    SDAOutMode();
    SDA = 1;
    rt_hw_us_delay(3);
    SCL = 1;
    rt_hw_us_delay(5);
    SCL = 0;
}
//send bytes
void IICWriteByte(uint8_t data){
    uint8_t i;
    SDAOutMode();
    for(i=0;i<8;i++){
        if(data&(0x80>>i)) SDA = 1;
        else SDA = 0;
        rt_hw_us_delay(3);
        SCL = 1;
        rt_hw_us_delay(5);
        SCL = 0;
        rt_hw_us_delay(5);
    }
}
//receive bytes
uint8_t IICReadByte(uint8_t ack){
    uint8_t i,data = 0x00;
    SDAInMode();
    for(i=0;i<8;i++){
        SCL = 0;
        rt_hw_us_delay(5);
        SCL = 1;
        if(ReadSDABit) data |= (0x80>>i);
        rt_hw_us_delay(3);
    }
    if(!ack) IICACK();
    else IICNOACK();
    return data;
}
