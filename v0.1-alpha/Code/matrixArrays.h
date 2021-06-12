
#define CHIP_A 0
#define CHIP_B 1
#define CHIP_C 2
#define CHIP_D 3
#define CHIP_E 4
#define CHIP_F 5
#define CHIP_G 6
#define CHIP_H 7
#define CHIP_I 8

extern int dataline;
char chipToChar (int chip);

byte Xaddr1 = 0;
byte Yaddr1 = 0;

byte Xaddr2 = 0;
byte Yaddr2 = 0;

int breadRow1 = 0;
int breadRow2 = 0;


char matrixArrayX [8][16] =      //1st dimension is the chip (0=A,1=B...)
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
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void findPath (int breadRow1, int breadRow2) {

  int Xpin1 = 0;
  int Ypin1 = breadRow1 % 8;

  int Xpin2 = 0;
  int Ypin2 = breadRow2 % 8;

  int chip1 = breadRow1 / 8;
  int chip2 = breadRow2 / 8;

  for (int i = 0; i < 16; i++) {

    if ((matrixArrayX[chip1][i] - 'A') == chip2 && availableConnections[chip1][i] == dataLine) {
      Xpin1 = i;
      Xpin2 = Xpin1;
      
      if (dataLine == 1) {
        availableConnections[chip1][i] = 0;
        availableConnections[chip2][i] = 0;
      } else if (dataLine == 0) {
        availableConnections[chip1][i] = 1;
        availableConnections[chip2][i] = 1;
      }

      break;
    }
    
    if (i == 15) {
      Serial1.println("Those chips don't have a direct connection");
    }
  }

  Xaddr1 = Xpin1;
  Yaddr1 = Ypin1;


  Serial1.print(chipToChar(chip1));
  Serial1.print("\t");
  Serial1.print(Xaddr1);
  Serial1.print("\t");
  Serial1.println(Yaddr1);


  Xaddr2 = Xpin2;
  Yaddr2 = Ypin2;


  Serial1.print(chipToChar(chip2));
  Serial1.print("\t");
  Serial1.print(Xaddr2);
  Serial1.print("\t");
  Serial1.println(Yaddr2);


}

char chipToChar (int chip) {

  switch (chip) {
    case 0:
      return 'A';
    case 1:
      return 'B';
    case 2:
      return 'C';
    case 3:
      return 'D';
    case 4:
      return 'E';
    case 5:
      return 'F';
    case 6:
      return 'G';
    case 7:
      return 'H';
    case 8:
      return 'I';
  }
}
