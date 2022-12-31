
//#define Serial
#define CHIP_A 0
#define CHIP_B 1
#define CHIP_C 2
#define CHIP_D 3
#define CHIP_E 4
#define CHIP_F 5
#define CHIP_G 6
#define CHIP_H 7
#define CHIP_I 8
#define CHIP_J 9

#define UNCONN_A 0
#define UNCONN_D 31
#define UNCONN_E 63
#define UNCONN_H 32

#define CS_A PIN_PG0
#define CS_B PIN_PG1
#define CS_C PIN_PG2
#define CS_D PIN_PG3
#define CS_E PIN_PG4
#define CS_F PIN_PG5
#define CS_G PIN_PG6
#define CS_H PIN_PG7
#define CS_I PIN_PF4
#define CS_J PIN_PF5

#define DATAPIN PIN_PA4
#define RESET PIN_PA5
#define STROBE PIN_PA6

#define AX0 PIN_PB0
#define AX1 PIN_PB1
#define AX2 PIN_PB2
#define AX3 PIN_PB3

#define AY0 PIN_PB4
#define AY1 PIN_PB5
#define AY2 PIN_PB6


#define GP_PC0 PIN_PC0    //anything that starts with GP_ goes to
#define GP_PC1 PIN_PC1    //the matrix chip on the control board
#define GP_PC2 PIN_PC2    //for "General Purpose"
#define GP_PC3 PIN_PC3
#define GP_PC4 PIN_PC4
#define GP_PC5 PIN_PC5
#define GP_PC6 PIN_PC6

#define GP_PE0 PIN_PE0    //anything that starts with GP_ goes to
#define GP_PE1 PIN_PE1    //the matrix chip on the control board
#define GP_PE2 PIN_PE2    //for "General Purpose"
#define GP_PE3 PIN_PE3
#define GP_PE4 PIN_PE4
#define GP_PE5 PIN_PE5
#define GP_PE6 PIN_PE6
#define GP_PE7 PIN_PE7


#define PROBE_2_1 PIN_PD0    
#define PROBE_1_1 PIN_PD4    
#define PROBE_1_2 PIN_PD7     //ADC_BIAS sets the bias so you can read negative voltages


//#define USB_RX PIN_PA0
//#define USB_TX PIN_PA1

#define IN_RX PIN_PF0
#define IN_TX PIN_PF1

#define I2C_SDA PIN_PA2
#define I2C_SCL PIN_PA3

#define mcpAddress         0x2F
#define volatileWiper0     0x00
#define volatileWiper1     0x01
#define nonvolatileWiper0  0x02
#define nonvolatileWiper1  0x03
#define TCON               0x04
#define mcpSTATUS          0x05 



#define PWR_NEG_EN PIN_PE2   //senses dual power supply voltage / 5
#define PWR_LDAC PIN_PC3  //turns on dual power rails for matrix chips

