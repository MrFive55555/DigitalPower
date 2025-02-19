#include "Buzzer.h"
/**
*created a music table,it's frequecen is from low1 to high7:freq[200,2000]
*how to complete:change TIM_Period
*notice: if TIM_Prescaler is more small,the deviation is more big,
*then lead to music be worse.
*/
const uint16_t musicTable[36] = 
{
	//low 1 to low 7
	45801,43321,40816,38585,36363,34383,32432,30612,28915,27272,25751,24291, //0-11
	//mid
	22944,21660,20442,19292,18209,17191,16216,15306,14440,13636,12875,12145, //12-23
	//high
	11472,10820,10212,9638,9104,8589,8108,7653,7224,6818,6434,6072,	//24-35
};
//musicNote as index of musicTable;
/*notice:don't have 7b,so I lost lots time to found why the index didn't accord table to run.
*becuase 7 to 1 just is half-step!
*/
enum musicNote{
	L1 = 0,L1b,L2,L2b,L3,L4,L4b,L5,L5b,L6,L6b,L7,
	M1,M1b,M2,M2b,M3,M4,M4b,M5,M5b,M6,M6b,M7,
	H1,H1b,H2,H2b,H3,H4,H4b,H5,H5b,H6,H6b,H7,P, //P is pause note
};
#define MUSIC_SPEED 780 //ms
//format: note+timePeriod
/*uint8_t yhyl[]={
	P,2,
	M3,2,M3,2,M5,2,M6,2,M5,2,M6,2,H1,2,M7,2,M6,2,M5,1,M6,4,M5,4,M3,1
};*/
rt_uint8_t firework_chill[]={
	M3,M3,M5,M6,M5,M6,H1,M7,M6,M5,M6,M5,M3,
};
#define musicSize sizeof(firework_chill)
void buzzerInit(uint16_t period,uint16_t pulse){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1,ENABLE);
	//1.initialize GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //Time1 CH1N
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//2.initialize TimeBase
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 5; // 72M/(1+5)=12M Minimum frequence:about 1098.63hz
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = period; //period=100 -> 10KHZ/100 = 100HZ;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0 ;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	//3.intialize ouput compare
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //up mode,when cnt < cc, ouput High
	TIM_OCInitStructure.TIM_OutputNState =  TIM_OutputNState_Enable; //PB13 enable;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_Pulse = pulse; //can be adjusted volume for buzzer audio
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_Cmd(TIM1, ENABLE); //enable counter
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
//on music
/*void playMusic(void){
	uint8_t i,index;
	for(i=0;i<yhylLength;i++){
		index = yhyl[i];
		if(i%2!=0){
			delay_ms(MUSIC_SPEED/index);
		}else{
			if(yhyl[i]==P){
				TIM_SetCompare1(TIM1,0);
			}else{
				TIM_Cmd(TIM1,DISABLE);
				delay_ms(5);
				TIM_SetAutoreload(TIM1,musicTable[index]);
				TIM_SetCompare1(TIM1,musicTable[index]/2);
				TIM_SetCounter(TIM1,0);
				TIM_Cmd(TIM1,ENABLE);
			}
		}
	}	
}*/
void buttonMusic(rt_bool_t musicEnable){
	static rt_uint16_t musicIndex = 0;
	static rt_uint8_t note;
	if(musicEnable){
		note = firework_chill[musicIndex++];
		musicIndex %= musicSize; 
		TIM_SetAutoreload(TIM1,musicTable[note]);
		TIM_SetCompare1(TIM1,musicTable[note]>>6);
		TIM_SetCounter(TIM1,0);
		//TIM_Cmd(TIM1, ENABLE);
	}else{
		TIM_SetCompare1(TIM1,0);
		//TIM_Cmd(TIM1, DISABLE);
	}
}
