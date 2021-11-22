char chip1 = 'A';
char chip2 = 'A';

byte Xaddr = 0;
byte Yaddr = 0;

int Xsize = 64;
int Ysize = 64;

int X = 0;
int Y = 0;

int altPath = 0;

extern int dataLine = 1;

int comma = -1;

int stayInConnectionMenu = 0;   //if this is 1, it skips the menu so we can keep adding connections

const byte bufLength = 50;

char serialBuffer[bufLength];
char serialBufferX[bufLength];
char serialBufferY[bufLength];

float powerSupplyVoltages [4] = {0,0,0,0};

//int specialMTconnections [5][4] = {}
int GPIOmode = -1; //0 = UART, 1 = I2C, 2 = SPI, 3 = Logic, 4 = PWM, 5 = TCA, -1 = unset

int logicTruth = 0b00000000;


byte Xaddr1 = 0;
byte Yaddr1 = 0;

byte Xaddr2 = 0;
byte Yaddr2 = 0;

int GbreadRow1 = 0;
int GbreadRow2 = 0;

const byte MTfuckedUpTruthTable [16] = {0,1,2,3,4,5,8,9,10,11,12,13,6,7,14,15}; //apparently X12 and X13 needed to be crammed in between X5 and X6

const int unconnYpins [8] = {7,0,0,7,7,0,0,7};
const int unconnChips [4] = {0,3,4,7};
const int connChips [4] = {1,2,5,6};

const int specialYmap [8] = {0,1,2,3,7,6,5,4};
 int specialYavailable [8] = {1,1,1,1,1,1,1,1};


int potData[2] = {-1,-1};
int potNumber = 0;

const int8_t chipToBreadboardMap [64][2] = //index is the breadBoard Row
 //1st dimension is the chip it's connected to (0=A, 1=B, etc) , then the Y pin on that chip
{
  {0,7},{0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},// the extra one on this row is because there's no 0th breadboard row
  {1,0},{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},
  {2,0},{2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},
  {3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},//
  {7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},//
  {6,0},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},
  {5,0},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},
  {4,0},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},//
};

char matrixArrayX [9][17] =      //1st dimension is the chip (0=A,1=B...)
{ //2nd dimension is the chip that X pin connects to (index is the X value)
  {'B', 'B', 'B', 'B', 'H', 'H', 'H', 'H', 'C', 'C', 'C', 'C', 'G', 'G', 'G', 'G'},
  {'A', 'A', 'A', 'A', 'C', 'C', 'C', 'C', 'H', 'H', 'H', 'H', 'D', 'D', 'D', 'D'},
  {'D', 'D', 'D', 'D', 'B', 'B', 'B', 'B', 'A', 'A', 'A', 'A', 'E', 'E', 'E', 'E'},
  {'C', 'C', 'C', 'C', 'E', 'E', 'E', 'E', 'F', 'F', 'F', 'F', 'B', 'B', 'B', 'B'},
  {'F', 'F', 'F', 'F', 'D', 'D', 'D', 'D', 'G', 'G', 'G', 'G', 'C', 'C', 'C', 'C'},
  {'E', 'E', 'E', 'E', 'G', 'G', 'G', 'G', 'D', 'D', 'D', 'D', 'H', 'H', 'H', 'H'},
  {'H', 'H', 'H', 'H', 'F', 'F', 'F', 'F', 'E', 'E', 'E', 'E', 'A', 'A', 'A', 'A'},
  {'G', 'G', 'G', 'G', 'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'F', 'F', 'F', 'F'}
};

byte availableConnections [8][16] =      //1st dimension is the chip (0=A,1=B...)
{ //2nd dimension is the whether or not that X line is connected to something
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int8_t connections [8][8][4] =      //1st dimension is the chip (0=A,1=B...)
{ //2nd dimension is the which pin is connected (Y values)  //this will save a maximum of 4 connections per row
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1} 
},        
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
},
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
},
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
},
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
},
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
},
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
},
 { { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}, { -1, -1, -1, -1}
}
};

int8_t UNCONNconnections [4][16] =      //1st dimension is the UNCONN (0=UNCONN_A, 1=UNCONN_D, 2=UNCONN_E, 3=UNCONN_H)
{ //2nd dimension is the which pin is connected (Y values)  //this will save a maximum of 16 connections per row
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
 };
