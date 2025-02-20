/*
 * Uart.c
 *
 *  Created on: 2023��8��27��
 *      Author: MiniSforum
 */
#include "Uart.h"
#include "IO.h"
interrupt void SCIRXINTA_IRQn(void);
void uartInit(Uint32 baud, Uint16 port)
{
    //1.enable colck

    //2.count baudrate
    Uchar8 scihbaud, scilbaud;
    Uint16 scibaud;
    scibaud = 37500000 / (8 * baud) - 1;
    scihbaud = scibaud >> 8;
    scilbaud = scibaud & 0x00ff;

    //3.initialize gpio of uart
    InitSciGpio();
    //InitScibGpio();

    EALLOW;
    //4.config uart mode
    //Initalize the SCI FIFO
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x2061;
    SciaRegs.SCIFFCT.all = 0x0;
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    SciaRegs.SCICCR.all = 0x0007; // 1stop bit, No loopback
    // No parity,8 char bits,
    // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003; // enable TX, RX, internal SCICLK,
    // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;
    SciaRegs.SCIHBAUD = scihbaud; // 9600 baud @LSPCLK = 37.5MHz.
    SciaRegs.SCILBAUD = scilbaud;
    //SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
    SciaRegs.SCICTL1.all = 0x0023; // Relinquish SCI from Reset

    //5.config receive interrupt
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
    PieVectTable.SCIRXINTA = &SCIRXINTA_IRQn;
    IER |= M_INT9;
    EINT;
    ERTM;

    EDIS;
}
//write byte
void uartSendByte(Uchar8 data)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
        ;
    SciaRegs.SCITXBUF = data;
}
void uartSendString(Uchar8 *stringData)
{
//write string
    while (*stringData != '\0')
        uartSendByte(*stringData++);
}
//read byte
Uchar8 uartReceiveByte(void)
{
    while (SciaRegs.SCIFFRX.bit.RXFFST != 1)
        ; // wait for XRDY =1 for empty state
    return SciaRegs.SCIRXBUF.all;
}

//read byte by interrupt
/**
 * bug:execute interrupt all the time,influence main thread.
 * solution:make sure fifo triggle interrupt byte more than zero.
 */
interrupt void SCIRXINTA_IRQn(void)
{
    static Uchar8 length = 0;
    while (SciaRegs.SCIFFRX.bit.RXFFST != 1); // wait for XRDY =1 for empty state
    receiveBuffer[length] = SciaRegs.SCIRXBUF.all;
    if (receiveBuffer[length++] == CHAR_ENTER)
    {
        receiveOKFlag = 1;
        receiveBuffer[length] = '\0';
        length = 0;
    }
    //don't use need lots of time in here,because it only receive one byte data.
    EALLOW;
    SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1; //must clear flag to receive next data
    PieCtrlRegs.PIEACK.bit.ACK9 = 1;
    EDIS;
}

