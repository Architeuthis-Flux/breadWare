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

/*
  char connectionsChip[100]; //stores which chip each connection was made on
  int connectionsX [100];   //
  int connectionsY [100];   //these should match each other by index so connectionsX[0][5] should be connected to connectionsY[0][5]
  int connIndex = 0; //this stores the index of last position (we only need one because X and Y should be the same)
*/
byte Xaddr1 = 0;
byte Yaddr1 = 0;

byte Xaddr2 = 0;
byte Yaddr2 = 0;

int GbreadRow1 = 0;
int GbreadRow2 = 0;


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
{ //2nd dimension is the whether or not that line is connected to something
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
