#include "Button.h"
static uint8_t keyValue;
static uint8_t preKeyValue;
#define KEY_UP_PORT PAin(0)
#define KEY_DOWN_PORT PAin(1)
#define KEY_RETURN_PORT PAin(2)
#define KEY_CONFIRM_PORT PAin(3)
void buttonInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    keyValue = preKeyValue = 0x00;
}
uint8_t scanButton(uint8_t longPressEnable)
{
    /**
     * longPressEnable = 1 long press enable
     */
    preKeyValue = keyValue;
    keyValue = (KEY_UP_PORT << 3) | (KEY_DOWN_PORT << 2) | (KEY_RETURN_PORT << 1) | KEY_CONFIRM_PORT;
    if ((keyValue != preKeyValue) || longPressEnable)
    {
        switch (keyValue)
        {
        case 0x08:
            return KEY_UP;
        case 0x04:
            return KEY_DOWN;
        case 0x02:
            return KEY_RETURN;
        case 0x01:
            return KEY_CONFIRM;
        default:
            return KEY_NO;
        }
    }
    return KEY_NO;
}
