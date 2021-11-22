

#define CHIP_A 0
#define CHIP_B 1
#define CHIP_C 2
#define CHIP_D 3
#define CHIP_E 4
#define CHIP_F 5
#define CHIP_G 6
#define CHIP_H 7
#define CHIP_I 8

#define UNCONN_A 0
#define UNCONN_D 31
#define UNCONN_E 63
#define UNCONN_H 32

#define CS_A PIN_PB0
#define CS_B PIN_PB1
#define CS_C PIN_PB2
#define CS_D PIN_PB3
#define CS_E PIN_PB4
#define CS_F PIN_PB5
#define CS_G PIN_PD4
#define CS_H PIN_PD5
#define CS_I PIN_PD6

#define DATA PIN_PE0
#define RESET PIN_PE1
#define STROBE PIN_PE2

#define AX0 PIN_PA4
#define AX1 PIN_PA5
#define AX2 PIN_PA6
#define AX3 PIN_PA7

#define AY0 PIN_PC4
#define AY1 PIN_PC5
#define AY2 PIN_PC6


#define GP_PC0 PIN_PC0    //anything that starts with GP_ goes to
#define GP_PC1 PIN_PC1    //the matrix chip on the control board
#define GP_PC2 PIN_PC2    //for "General Purpose"
#define GP_PC3 PIN_PC3

#define ADC_VDIVO PIN_PD0    //for sensing voltages (after getting voltage
#define ADC_VDIV1 PIN_PD1    //divided by 5)
#define ADC_BIAS PIN_PD7     //sets the bias so you can read negative voltages

#define PWR_NEG_EN PIN_PD2   //senses dual power supply voltage / 5
#define PWR_LDAC PIN_PD3  //turns on dual power rails for matrix chips
//#define PWR_SENSE2 PIN_PD4   //senses power supply voltage / 5
//#define PWR_ENABLE2 PIN_PD5  //turns on adjustable power supply


#define USB_RX PIN_PA0
#define USB_TX PIN_PA1

#define IN_RX PIN_PF0
#define IN_TX PIN_PF1

#define OUT_RX PIN_PF5
#define OUT_TX PIN_PF4

#define I2C_SDA PIN_PA2
#define I2C_SCL PIN_PA3

#define POT_MAX 100

#define mcpAddress         0x28
#define volatileWiper0     0x00
#define volatileWiper1     0x01
#define nonvolatileWiper0  0x02
#define nonvolatileWiper1  0x03
#define TCON               0x04
#define mcpSTATUS          0x05 


#define ADC0 0
#define ADC1 1
#define POT0_A 2
#define POT0_W 3
#define POT0_B 4
#define POT1_A 5
#define POT1_W 6
#define POT1_B 7
#define PC0 8
#define PC1 9
#define PC2 10
#define PC3 11
#define PE3 12
#define POS_TOP_MT 13
#define NEG_BOTTOM_MT 14
#define GROUND_MT 15