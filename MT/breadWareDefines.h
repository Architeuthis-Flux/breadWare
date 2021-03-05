
#define CS_A PIN_PB0
#define CS_B PIN_PB1
#define CS_C PIN_PB2
#define CS_D PIN_PB3
#define CS_E PIN_PB4
#define CS_F PIN_PB5
#define CS_G PIN_PC4
#define CS_H PIN_PC5
#define CS_I PIN_PC6

#define DATA PIN_PE0
#define RESET PIN_PE1
#define STROBE PIN_PE2

#define AX0 PIN_PA0
#define AX1 PIN_PA1
#define AX2 PIN_PA2
#define AX3 PIN_PA3

#define AY0 PIN_PA4
#define AY1 PIN_PA5
#define AY2 PIN_PA6


#define GP_PF0 PIN_PF0    //anything that starts with GP_ goes to
#define GP_PF1 PIN_PF1    //the matrix chip on the control board
#define GP_PF2 PIN_PF2    //for "General Purpose"
#define GP_PF3 PIN_PF3

#define ADC_VDIVO PIN_PD0    //for sensing voltages (after getting voltage
#define ADC_VDIV1 PIN_PD1    //divided by 5)
#define ADC_BIAS PIN_PD7     //sets the bias so you can read negative voltages

#define PWR_SENSE1 PIN_PD2   //senses dual power supply voltage / 5
#define PWR_ENABLE1 PIN_PD3  //turns on dual power rails for matrix chips
#define PWR_SENSE2 PIN_PD4   //senses power supply voltage / 5
#define PWR_ENABLE2 PIN_PD5  //turns on adjustable power supply


#define USB_RX PIN_PC0
#define USB_TX PIN_PC1

#define I2C_SDA PIN_PC2
#define I2C_SCL PIN_PC3
