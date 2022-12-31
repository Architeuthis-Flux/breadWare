#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>



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

const byte bufLength = 8;

char serialBuffer[bufLength];
char serialBufferX[bufLength];
char serialBufferY[bufLength];

float powerSupplyVoltages [4] = {0,0,0,0};




byte Xaddr1 = 0;
byte Yaddr1 = 0;

byte Xaddr2 = 0;
byte Yaddr2 = 0;

int GbreadRow1 = 0;
int GbreadRow2 = 0;

const byte MTfuckedUpTruthTable [16] = {0,1,2,3,4,5,8,9,10,11,12,13,6,7,14,15}; //apparently X12 and X13 needed to be crammed in between X5 and X6



const int8_t chipToBreadboardMap [64][2] = //index is the breadBoard Row
 //2nd dimension is the chip it's connected to (0=A, 1=B, etc) , then the Y pin on that chip , -1 MEANS UNCONNECTED
{
  {-1,-1},{-1,-1},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},//
  {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},
  {2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},
  {3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},
  {-1,-1},{-1,-1},{-1,-1},{-1,-1},
  {4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},
  {5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},
  {6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},
  {7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7}
};

char matrixArrayX [8][17] =      //1st dimension is the chip (0=A,1=B...)
{ //2nd dimension is the chip that X pin connects to (index is the X value)
  {'I','J','B','B','C','C','D','D','E','E','F','F','G','G','H','H'},
  {'A','A','I','J','C','C','D','D','E','E','F','F','G','G','H','H'},
  {'A','A','B','B','I','J','D','D','E','E','F','F','G','G','H','H'},
  {'A','A','B','B','C','C','I','J','E','E','F','F','G','G','H','H'},
  {'A','A','B','B','C','C','D','D','I','J','F','F','G','G','H','H'},
  {'A','A','B','B','C','C','D','D','E','E','I','J','G','G','H','H'},
  {'A','A','B','B','C','C','D','D','E','E','F','F','I','J','H','H'},
  {'A','A','B','B','C','C','D','D','E','E','F','F','G','G','I','J'}


};

int availableConnections [8][16] =      //1st dimension is the chip (0=A,1=B...)
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

int8_t UNCONNconnections [8][16] =      //1st dimension is the UNCONN (0=UNCONN_A, 1=UNCONN_B, 2=UNCONN_C, 3=UNCONN_D...)
{ //2nd dimension is the which pin is connected (Y values)  //this will save a maximum of 16 connections per row
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }, 
   { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }



 };
/*
const char breadBoard [13][70] = {

{"                   1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 "},
{" 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{"                                                               "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # "},
{" 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 6 6 6 "},
{" 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 "}

};

int breadboardNetColors [13][30] = {
{  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  },
{  3,4,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2  },
{  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  },
{  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0  }

};*/


const int sine256 [257] = {0x200,0x20c,0x219,0x225,0x232,0x23e,0x24b,0x257,
0x263,0x270,0x27c,0x288,0x294,0x2a0,0x2ac,0x2b8,
0x2c3,0x2cf,0x2da,0x2e5,0x2f1,0x2fc,0x306,0x311,
0x31c,0x326,0x330,0x33a,0x344,0x34e,0x357,0x360,
0x369,0x372,0x37a,0x383,0x38b,0x393,0x39a,0x3a2,
0x3a9,0x3b0,0x3b6,0x3bd,0x3c3,0x3c8,0x3ce,0x3d3,
0x3d8,0x3dd,0x3e1,0x3e5,0x3e9,0x3ec,0x3f0,0x3f3,
0x3f5,0x3f7,0x3f9,0x3fb,0x3fd,0x3fe,0x3fe,0x3ff,
0x3ff,0x3ff,0x3fe,0x3fe,0x3fd,0x3fb,0x3f9,0x3f7,
0x3f5,0x3f3,0x3f0,0x3ec,0x3e9,0x3e5,0x3e1,0x3dd,
0x3d8,0x3d3,0x3ce,0x3c8,0x3c3,0x3bd,0x3b6,0x3b0,
0x3a9,0x3a2,0x39a,0x393,0x38b,0x383,0x37a,0x372,
0x369,0x360,0x357,0x34e,0x344,0x33a,0x330,0x326,
0x31c,0x311,0x306,0x2fc,0x2f1,0x2e5,0x2da,0x2cf,
0x2c3,0x2b8,0x2ac,0x2a0,0x294,0x288,0x27c,0x270,
0x263,0x257,0x24b,0x23e,0x232,0x225,0x219,0x20c,
0x200,0x1f3,0x1e6,0x1da,0x1cd,0x1c1,0x1b4,0x1a8,
0x19c,0x18f,0x183,0x177,0x16b,0x15f,0x153,0x147,
0x13c,0x130,0x125,0x11a,0x10e,0x103,0xf9,0xee,
0xe3,0xd9,0xcf,0xc5,0xbb,0xb1,0xa8,0x9f,
0x96,0x8d,0x85,0x7c,0x74,0x6c,0x65,0x5d,
0x56,0x4f,0x49,0x42,0x3c,0x37,0x31,0x2c,
0x27,0x22,0x1e,0x1a,0x16,0x13,0xf,0xc,
0xa,0x8,0x6,0x4,0x2,0x1,0x1,0x0,
0x0,0x0,0x1,0x1,0x2,0x4,0x6,0x8,
0xa,0xc,0xf,0x13,0x16,0x1a,0x1e,0x22,
0x27,0x2c,0x31,0x37,0x3c,0x42,0x49,0x4f,
0x56,0x5d,0x65,0x6c,0x74,0x7c,0x85,0x8d,
0x96,0x9f,0xa8,0xb1,0xbb,0xc5,0xcf,0xd9,
0xe3,0xee,0xf9,0x103,0x10e,0x11a,0x125,0x130,
0x13c,0x147,0x153,0x15f,0x16b,0x177,0x183,0x18f,
0x19c,0x1a8,0x1b4,0x1c1,0x1cd,0x1da,0x1e6,0x1f3};

const int sine128 [129] = {0x200,0x219,0x232,0x24b,0x263,0x27c,0x294,0x2ac,
0x2c3,0x2da,0x2f1,0x306,0x31c,0x330,0x344,0x357,
0x369,0x37a,0x38b,0x39a,0x3a9,0x3b6,0x3c3,0x3ce,
0x3d8,0x3e1,0x3e9,0x3f0,0x3f5,0x3f9,0x3fd,0x3fe,
0x3ff,0x3fe,0x3fd,0x3f9,0x3f5,0x3f0,0x3e9,0x3e1,
0x3d8,0x3ce,0x3c3,0x3b6,0x3a9,0x39a,0x38b,0x37a,
0x369,0x357,0x344,0x330,0x31c,0x306,0x2f1,0x2da,
0x2c3,0x2ac,0x294,0x27c,0x263,0x24b,0x232,0x219,
0x200,0x1e6,0x1cd,0x1b4,0x19c,0x183,0x16b,0x153,
0x13c,0x125,0x10e,0xf9,0xe3,0xcf,0xbb,0xa8,
0x96,0x85,0x74,0x65,0x56,0x49,0x3c,0x31,
0x27,0x1e,0x16,0xf,0xa,0x6,0x2,0x1,
0x0,0x1,0x2,0x6,0xa,0xf,0x16,0x1e,
0x27,0x31,0x3c,0x49,0x56,0x65,0x74,0x85,
0x96,0xa8,0xbb,0xcf,0xe3,0xf9,0x10e,0x125,
0x13c,0x153,0x16b,0x183,0x19c,0x1b4,0x1cd,0x1e6};

const int sine64 [65] = {0x200,0x232,0x263,0x294,0x2c3,0x2f1,0x31c,0x344,
0x369,0x38b,0x3a9,0x3c3,0x3d8,0x3e9,0x3f5,0x3fd,
0x3ff,0x3fd,0x3f5,0x3e9,0x3d8,0x3c3,0x3a9,0x38b,
0x369,0x344,0x31c,0x2f1,0x2c3,0x294,0x263,0x232,
0x200,0x1cd,0x19c,0x16b,0x13c,0x10e,0xe3,0xbb,
0x96,0x74,0x56,0x3c,0x27,0x16,0xa,0x2,
0x0,0x2,0xa,0x16,0x27,0x3c,0x56,0x74,
0x96,0xbb,0xe3,0x10e,0x13c,0x16b,0x19c,0x1cd};


const int sine32 [33] = {0x200,0x263,0x2c3,0x31c,0x369,0x3a9,0x3d8,0x3f5,
0x3ff,0x3f5,0x3d8,0x3a9,0x369,0x31c,0x2c3,0x263,
0x200,0x19c,0x13c,0xe3,0x96,0x56,0x27,0xa,
0x0,0xa,0x27,0x56,0x96,0xe3,0x13c,0x19c};


const int sine16 [17] = {0x200,0x2c3,0x369,0x3d8,0x3ff,0x3d8,0x369,0x2c3,
0x200,0x13c,0x96,0x27,0x0,0x27,0x96,0x13c};
#endif