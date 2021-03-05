#//include <Wire.h>
#include "breadWareDefines.h"
//#include "matrixArrays.h"
#include "Globals.h"



void setup() {

  Serial1.begin (115200);
  PORTMUX.TWISPIROUTEA = 0b00100000;
  PORTMUX.USARTROUTEA = 0b11000011;

  //Serial1.println (PORTMUX.TWISPIROUTEA,BIN);
  //Serial.print (PORTMUX.TWISPIROUTEA,BIN);
  /* PORTA.DIR = 0b01111111;
    PORTA.OUT    = 0b00000000;

    PORTB.DIR = 0b00111111;
    PORTB.OUT    = 0b00000000;

    PORTC.DIR |= 0b01110000; //so I dont mess with I2C or UART
    PORTC.OUT    &= 0b00001111;

    PORTD.DIR = 0b10101000;
    PORTD.OUT    = 0b00001000; //enable the MT dual rail power supply

    PORTE.DIR = 0b00000111;
    PORTE.OUT    = 0b00000000;

    PORTF.DIR |= 0b00000000; //leave PF0-3 as inputs, dont mess with /Reset
    PORTF.OUT    &= 0b01000000;
  */


  pinMode(AY0, OUTPUT);
  digitalWrite(AY0, LOW);
  pinMode(AY1, OUTPUT);
  digitalWrite(AY1, LOW);
  pinMode(AY2, OUTPUT);
  digitalWrite(AY2, LOW);

  pinMode(AX0, OUTPUT);
  digitalWrite(AX0, LOW);
  pinMode(AX1, OUTPUT);
  digitalWrite(AX1, LOW);
  pinMode(AX2, OUTPUT);
  digitalWrite(AX2, LOW);
  pinMode(AX3, OUTPUT);
  digitalWrite(AX3, LOW);


  pinMode(CS_A, OUTPUT);
  digitalWrite(CS_A, HIGH);
  pinMode(CS_B, OUTPUT);
  digitalWrite(CS_B, HIGH);
  pinMode(CS_C, OUTPUT);
  digitalWrite(CS_C, HIGH);
  pinMode(CS_D, OUTPUT);
  digitalWrite(CS_D, HIGH);
  pinMode(CS_E, OUTPUT);
  digitalWrite(CS_E, HIGH);
  pinMode(CS_F, OUTPUT);
  digitalWrite(CS_F, HIGH);
  pinMode(CS_G, OUTPUT);
  digitalWrite(CS_G, HIGH);
  pinMode(CS_H, OUTPUT);
  digitalWrite(CS_H, HIGH);
  pinMode(CS_I, OUTPUT);
  digitalWrite(CS_I, HIGH);

  pinMode(RESET, OUTPUT);   //all of this just sets up all these pins as outputs and drives them LOW
  digitalWrite(RESET, HIGH); //I'm pretty sure Arduino IDE does this automatically but I wouldn't count on it
  delayMicroseconds(40);
  digitalWrite(RESET, LOW);
  


  pinMode(PWR_ENABLE1, OUTPUT);
  digitalWrite(PWR_ENABLE1, HIGH);

  pinMode(DATA, OUTPUT);
  digitalWrite(DATA, HIGH);
  pinMode(STROBE, OUTPUT);
  //digitalWrite(STROBE, LOW);

  pinMode(GP_PF0, OUTPUT);
  digitalWrite(GP_PF0, LOW);

  clearConnections();

}


void loop() {
  
  //clearBuffers();

  if (stayInConnectionMenu == 1) {
    getX();
  } else {
if (stayInConnectionMenu == -1) {
  showSpecialFunctionsMenu();
  selectMenuItem();
}else{
    showMenuOptions();
    selectMenuItem();
  }

}
}

int findPath (int breadRow1, int breadRow2) {



  if (breadRow1 == breadRow2) {
    return -1;
  }

  if (breadRow1 > breadRow2) {
    int breadRowTemp1 = breadRow1;
    breadRow1 = breadRow2;
    breadRow2 = breadRowTemp1;

  }

  chip1 = breadRow1 / 8;
  chip2 = breadRow2 / 8;


  if (chip2 - chip1 == 5) {
    int breadRowTemp1 = breadRow1;
    breadRow1 = breadRow2;
    breadRow2 = breadRowTemp1;
  }

  byte Xpin1 = 0;
  byte Ypin1 = breadRow1 % 8;

  byte Xpin2 = 0;
  byte Ypin2 = breadRow2 % 8;

  altPath = 0;
 
  int breadRowTemp2 = breadRow2;

  for (int i = 0; i < 17; i++) {
    //Serial1.println(i);
    if (i == 16) {

      breadRow2 = (breadRow1 + 16) % 64;
      altPath = 1;


      chip1 = breadRow1 / 8;
      Ypin1 = breadRow1 % 8;
      chip2 = breadRow2 / 8;
      Ypin2 = breadRow2 % 8;
      Serial1.println("Those chips don't have a direct connection,");
      Serial1.print("connecting through intermediate row ");
      Serial1.println(breadRow2);
      i = 0;

    }

    if (0) {        //only runs from the goto label
redoIt:

      breadRow1 = breadRow2;
      breadRow2 = breadRowTemp2;
      Serial1.println(breadRow1);
      Serial1.println(breadRow2);
      chip1 = breadRow1 / 8;
      Ypin1 = breadRow1 % 8;
      Ypin2 = breadRow2 % 8;
      chip2 = breadRow2 / 8;
      altPath = 0;
      i = 0;
    }


    if ((((matrixArrayX[chip1][i] - 'A') == chip2) && (availableConnections[chip1][i] == dataLine)) || ((chip1 == chip2) && (availableConnections[chip1][i % 16] == 1))) {

      Xpin1 = i;
      Xpin2 = Xpin1;


      if (dataLine == 1) {
        availableConnections[chip1][i % 16] = 0;
        availableConnections[chip2][i % 16] = 0;

        for (int l = 0; l < 4; l++) {
          if (connections[chip1][Ypin1][l] == -1) {
            connections[chip1][Ypin1][l] = breadRow2;
            break;
          }
        }
        for (int k = 0; k < 4; k++) {
          if (connections[chip2][Ypin2][k] == -1) {
            connections[chip2][Ypin2][k] = breadRow1;
            break;
          }
        }

      } else if (dataLine == 0) {


        availableConnections[chip1][i % 16] = 1;
        availableConnections[chip2][i % 16] = 1;

        for (int k = 0; k < 4; k++) {
          if (connections[chip1][Ypin1][k] == breadRow2) {
            connections[chip1][Ypin1][k] = -1;
            break;
          }
        }
        for (int l = 0; l < 4; l++) {
          if (connections[chip2][Ypin2][l] == breadRow1) {
            connections[chip2][Ypin2][l] = -1;
            break;
          }
        }
      }

      if (Xpin1 >= 8) {
        Xaddr1 = Xpin1 + 2;
      } else {
        Xaddr1 = Xpin1;
      }
      
      Yaddr1 = breadRow1 % 8;



      // Serial1.println(altPath);
      //Serial1.print("??????");
      Serial1.print(chipToChar(chip1));
      Serial1.print("\t");
      Serial1.print(Xaddr1);
      Serial1.print(" ");
      Serial1.print(Xpin1);
      Serial1.print("\t");
      Serial1.print(Yaddr1);
      Serial1.print(" ");
      Serial1.println(Ypin1);

      setAddress(Xaddr1, Yaddr1, chipToChar(breadRow1 / 8));

      if (Xpin2 >= 8) {         //this is a hack but it works until i can figure out why the x connections that are 2 chips away only work the 3rd time you send the command
        Xaddr2 = Xpin2 + 2;
      } else {
        Xaddr2 = Xpin2;
      }


      Yaddr2 = breadRow2 % 8;
      setAddress(Xaddr2, Yaddr2, chipToChar(chip2));

      Serial1.print(chipToChar(chip2));
      Serial1.print("\t");
      Serial1.print(Xaddr2);
      Serial1.print(" ");
      Serial1.print(Xpin2);
      Serial1.print("\t");
      Serial1.print(Yaddr2);
      Serial1.print(" ");
      Serial1.println(Ypin2);


      if (altPath >= 1) {
        i = 0;
        goto redoIt;
      } else {
        break;
      }

    }

  }

}


void showMenuOptions(void) {

  Serial1.println(" ");

  Serial1.println("Connect Nodes \t\t\tc");
  Serial1.println("Disconnect Nodes \t\td");
  Serial1.println("Show Frontend Connections \ty");
  Serial1.println("Show Backend Connections \tx");
  Serial1.println("Reset Connections\t\tr");
  Serial1.println("Special Funtions Menu\t\tf");
  Serial1.println("Back to Menu\t\t\tm");
  Serial1.println(" ");


}

void selectMenuItem(void) {

  //altPath = -1;
  while (!Serial1.available());    //this just keeps looping if there's nothing in the serial buffer, so it waits for you choose an item

  for (int i = 0; i < bufLength; i++) {   //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
  }
  Serial1.readBytesUntil('\n', serialBuffer, bufLength);

  if (isDigit(serialBuffer[0]) == true) {
    dataLine = 1;
    stayInConnectionMenu = 1;
    getX();
  }

  switch (tolower(serialBuffer[0])) {

    case 'm':
        stayInConnectionMenu = 0;
        return;
        
    case 'c':
      dataLine = 1;
      stayInConnectionMenu = 1;
      getX();
      break;

    case 'd':
      dataLine = 0;
      stayInConnectionMenu = 1;
      getX();
      break;

    case 'r':
      clearConnections();
      Serial1.print ("Cleared!\n");
      break;

    case 'x':
      showArrays();
      break;
      
    case 'y':
      showConnections();
      break;

    case 'f':
      stayInConnectionMenu = -1;
      break;  

      
  }
  clearBuffers();
}

void showSpecialFunctionsMenu (void) {

  Serial1.println(" ");
  Serial1.println("Power Supplies ON \t\ti");
  Serial1.println("Power Supplies OFF \t\to");
  Serial1.println("Potentiometer \t\t\tp");
  Serial1.println("Analog Read\t\t\tv");
  Serial1.println("GPIO \t\t\t\tg");
  Serial1.println("Back to Menu\t\t\tm");
  Serial1.println(" ");

  
}


void getX() {     //this is just to get values from the serial monitor (which isnt a particularly nice way to set the switches)
  //                  dealing with inputs to the serial monitor is almost always a shit show, as you will see below
  Serial1.println(" ");
  if (dataLine == 0) {
    Serial1.print("Disconnect ");
  } else {
    Serial1.print("Connect ");
  }
  Serial1.println("rows");


  clearBuffers();

  int shift = 0;
  

  while (!Serial1.available());    //this just keeps looping if there's nothing in the serial buffer, so it waits for you to send some data

  Serial1.readBytesUntil('\n', serialBuffer, bufLength); //reads chars one at a time and puts them into the array serialBuffer
  //                                                        until it reads an \n or it reaches the end (bufLength)
  if (isDigit(serialBuffer[0]) != true) {


    switch (tolower(serialBuffer[0])) {
      case 'm':
        Serial1.println("\n");
        stayInConnectionMenu = 0;
        return;

      case 'd':
        dataLine = 0;
        return;

      case 'c':
        dataLine = 1;
        return;

      case 'y':
        showConnections();
        return;
        
      case 'x':
        showArrays();
        break;
        
      case 'r':
       clearConnections();
        Serial1.println ("Cleared!\n");
        break;
        
      case't':    
        break;
        
      case'b':
        break;
        
      default:
        Serial1.println ("\nPlease enter a number");
        return;
    }
  }

  comma = -1;
  int bottomX = 0;
  int bottomY = 0;
  
  for (int i = shift; i < bufLength; i++) {  //this loop lets is enter values as "X,Y" instead of one at a time (it was super annoying to do it that way when testing)
    //                                         by taking one serialBuffer and then splitting it into separate X and Y serialBuffers

    if (serialBuffer[i] == ',') {     //loops through serialBuffer[] to look for a comma and sets "comma" to the position in the array
      comma = i;
      
      i++;
    }
    
    if (serialBuffer[i] == 'b') {     //loops through serialBuffer[] to look for a comma and sets "comma" to the position in the array
      if (comma == -1) {
        bottomX = 1;
      } else {
        bottomY = 1;
      }
      
      i++;
    }

    if (serialBuffer[i] == 't') {
      i++;
    }

    if (comma == -1) {                    //if it hasnt found a comma, then this part of the buffer is for the X address buffer

      serialBufferX[i] = serialBuffer[i];
      
    } else {
      
      serialBufferY[i] = serialBuffer[i];
      
    }
  }

  int incomingValueX = atoi(serialBufferX);   //atoi converts a char array to an integer (Array TO Int)

  if (bottomX == 1) {
    incomingValueX += 32;
  }
  
  int incomingValueY = atoi(serialBufferY);

  if (bottomY == 1) {
    incomingValueY += 32;
  }

  if (incomingValueX >= Xsize) {              //this checks to make sure the values you entered aren't above the maximum size specified in Xsize
    Serial1.print ("\nPlease enter a number for X between 0 and ");
    Serial1.println (Xsize - 1);
    return;
  }
  if (incomingValueY >= Ysize && comma > 0) { //the logical AND makes it only check if we put in a Y address separated by a comma
    Serial1.print ("\nPlease enter a number for Y between 0 and ");
    Serial1.println (Ysize - 1);
    return;
  }

  GbreadRow1 = incomingValueX;  //now that we know our inputs are good, we can put them into Xaddr and Yaddr

  GbreadRow2 = incomingValueY;

  if (findPath(GbreadRow1, GbreadRow2) == -1) {
    return;
  }

  if (comma > 0) {          //only prints a Y if you entered it here
    //Serial1.print("Y = ");
    //Serial1.println(Yaddr1);
    // Serial1.println(' ');
  }



  if (comma < 0) {        //if a Y wasnt entered comma will still be -1, call getY() to ask for one
    comma = -1;          //set the comma back to -1 for the next entry
  }



}

void setAddress (byte Xaddr, byte Yaddr, char chip) {



  digitalWrite(AX3, LOW);
  digitalWrite(AX2, LOW);
  digitalWrite(AX1, LOW);
  digitalWrite(AX0, LOW);

  digitalWrite(AY2, LOW);
  digitalWrite(AY1, LOW);
  digitalWrite(AY0, LOW);


  selectChip(chip);            //datasheet says CS_ should be set while setting up address lines, not sure why though
  delayMicroseconds(40);

  digitalWrite(AX3, HIGH && (Xaddr & B00001000));   //this only writes the line high if that bit is set in binary value of Xaddr
  Serial1.print (HIGH && Xaddr & B00001000);
  digitalWrite(AX2, HIGH && (Xaddr & B00000100));   //for example Xaddr = 6 or B00000110          //note: && is logical AND, and & is bitwise AND
  Serial1.print (HIGH && Xaddr & B00000100);
  digitalWrite(AX1, HIGH && (Xaddr & B00000010));   //this bitwise ANDs Xaddr and a binary value with just one bit set
  Serial1.print (HIGH && Xaddr & B00000010);
  digitalWrite(AX0, HIGH && (Xaddr & B00000001));   //so we get          Xaddr   00000110
  Serial1.println (HIGH && Xaddr & B00000001);                                    //                                                          AND bit selector   00001000 = 0
  digitalWrite(AY2, HIGH && (Yaddr & B00000100));   //then we AND that again with HIGH (which is just interpreted as 1)

  digitalWrite(AY1, HIGH && (Yaddr & B00000010));   //to get 1(HIGH) && 0(the result from above) = 0 (which is LOW)

  digitalWrite(AY0, HIGH && (Yaddr & B00000001));   //we do that for each bit to end up with the address lines LLLLLHHL

  delayMicroseconds(50);      //allow the lines to settle

  strobeItIn();   //we could probably do this without passing "chip" and just do all the strobing on both together,
  //                    but I don't necessarily want to make MT8816 ignore that many garbage signals with CS_ LOW
  //deselectChip();


}

void strobeItIn (void) {

  if (dataLine == 0) {
    digitalWrite(DATA, LOW);
  } else {
    digitalWrite(DATA, HIGH);
  }

  //pinMode(STROBE, OUTPUT);
  delayMicroseconds(100);        //Hold time in the datasheet for the MT8816 says this only needs to be 10 nanoseconds
  digitalWrite(STROBE, HIGH);  //but we're not super concerned with speed so I'll give it 1000X more just to be safe
  delayMicroseconds(400);        //Strobe has to be a minimum of 20 nanoseconds, but I dont want to think about the
  //Serial1.println("!!!!!!!!!!!!");
  digitalWrite(STROBE, LOW);   //fact that light only travels like 4 meters in that time through copper
  //pinMode(STROBE, OUTPUT);
  delayMicroseconds(80);
  //digitalWrite(DATA, LOW);
  //delayMicroseconds(30);


}

void selectChip (char chip)  {        //asserts whichever chip select line we send it

  chip = chipToChar(chip);

  digitalWrite(CS_A, LOW);
  digitalWrite(CS_B, LOW);
  digitalWrite(CS_C, LOW);
  digitalWrite(CS_D, LOW);
  digitalWrite(CS_E, LOW);
  digitalWrite(CS_F, LOW);
  digitalWrite(CS_G, LOW);
  digitalWrite(CS_H, LOW);
  digitalWrite(CS_I, LOW);

  delayMicroseconds(30);
  Serial1.print(chipToChar(chip));

  if (chip == 'A' || chip == 'a') {
    digitalWrite(CS_A, HIGH);      //datasheet says CS_ should be set while setting up address lines, not sure why though
    //Serial1.print(chipToChar(chip));
    return;
  }
  if (chip == 'B' || chip == 'b') {
    digitalWrite(CS_B, HIGH);
    return;
  }
  if (chip == 'C' || chip == 'c') {
    digitalWrite(CS_C, HIGH);
    return;
  }
  if (chip == 'D' || chip == 'd') {
    digitalWrite(CS_D, HIGH);
    return;
  }
  if (chip == 'E' || chip == 'e') {
    digitalWrite(CS_E, HIGH);
    return;
  }
  if (chip == 'F' || chip == 'f') {
    digitalWrite(CS_F, HIGH);
    return;
  }
  if (chip == 'G' || chip == 'g') {
    digitalWrite(CS_G, HIGH);
    return;
  }
  if (chip == 'H' || chip == 'h') {
    digitalWrite(CS_H, HIGH);
    return;
  }
  if (chip == 'I' || chip == 'i') {
    digitalWrite(CS_I, HIGH);
    return;
  }

}

void deselectChip (void)  {         //this should be fairly obvious

  digitalWrite(CS_A, LOW);
  digitalWrite(CS_B, LOW);
  digitalWrite(CS_C, LOW);
  digitalWrite(CS_D, LOW);
  digitalWrite(CS_E, LOW);
  digitalWrite(CS_F, LOW);
  digitalWrite(CS_G, LOW);
  digitalWrite(CS_H, LOW);
  digitalWrite(CS_I, LOW);
  return;
}

void clearConnections (void)  {    //when you send a reset pulse, all previous connections are cleared on whichever chip is CS_ed but we'll do both for now

  digitalWrite(CS_A, HIGH);
  digitalWrite(CS_B, HIGH);
  digitalWrite(CS_C, HIGH);
  digitalWrite(CS_D, HIGH);
  digitalWrite(CS_E, HIGH);
  digitalWrite(CS_F, HIGH);
  digitalWrite(CS_G, HIGH);
  digitalWrite(CS_H, HIGH);
  digitalWrite(CS_I, HIGH);

  digitalWrite(RESET, HIGH);
  delayMicroseconds(200);     //datasheet says 40 nanoseconds minimum, this is a lot more than that
  digitalWrite(RESET, LOW);

  digitalWrite(CS_A, LOW);
  digitalWrite(CS_B, LOW);
  digitalWrite(CS_C, LOW);
  digitalWrite(CS_D, LOW);
  digitalWrite(CS_E, LOW);
  digitalWrite(CS_F, LOW);
  digitalWrite(CS_G, LOW);
  digitalWrite(CS_H, LOW);
  digitalWrite(CS_I, LOW);

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 4; k++) {
        connections[i][j][k] = -1;
      }
    }
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 16; j++) {
      availableConnections[i][j] = 1;
    }
  }


}

void clearBuffers (void) {

  for (int i = 0; i < bufLength; i++) {   //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
    serialBufferX[i] = ' ';
    serialBufferY[i] = ' ';
  }

  /* while (Serial1.available() > 0) {    //this is just a trick to clear the serial buffer (if we typed in a really long string it would just keep entering them one after another)
     Serial1.print(Serial1.read());                    //it reads the serial buffer and then throws away the values, just to clear it
     delay(100);
    }*/
  Serial1.end();         //maybe it's just easier to stop and start the serial connection
  Serial1.begin(115200);

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

void showArrays (void) {

  for (int i = 0; i < 8; i++) {

    Serial1.print ("Chip ");
    Serial1.print (chipToChar(i));
    Serial1.print ("\t");

    for (int j = 0; j < 16; j++) {
      if (chipToChar(i) < matrixArrayX[i][j]) {
        Serial1.print (chipToChar(i));
        Serial1.print (matrixArrayX[i][j]);
      } else {
        Serial1.print (matrixArrayX[i][j]);
        Serial1.print (chipToChar(i));
      }
      Serial1.print (" ");
      Serial1.print (!availableConnections [i][j]);
      Serial1.print ("\t");
    }
    Serial1.println(" ");
  }
}

void showConnections (void) {

  int count = 0;
  int countConn = 0;
  int countDig = 0;
  char chipLetter = 'A';
  for (int i = 0; i < 8; i++) {
    Serial1.print("Chip ");
    Serial1.print(chipLetter);
    Serial1.print("\t");
    chipLetter++;

    for (int j = 0; j < 8; j++) {
      countDig = 0;
      countConn = 0;
      Serial1.print(count);
      if (count <= 9) Serial1.print (" ");

      count++;
      Serial1.print(" ");
      if (connections[i][j][0] == -1 && connections[i][j][1] == -1 && connections[i][j][2] == -1 && connections[i][j][3] == -1) {
        Serial1.print("X\t");
      } else {
        countDig = 0;
        for (int k = 0; k < 4; k++) {
          if (connections[i][j][k] != -1) {

            if (countConn >= 1 && countConn < 4) {
              Serial1.print(',');

              countDig++;
            }
            countConn++;
            Serial1.print(connections[i][j][k]);

            if (connections[i][j][k] >= 9) {
              countDig++;
            }
            countDig++;
          }

        }
        if (countDig <= 4) {
          Serial1.print("\t");
          //Serial1.print(countDig);
          countDig = 0;
        }
      }
      Serial1.print("\t");
    }
    //if (count == 32) Serial1.print ("\n");
    Serial1.println(" ");
  }


}
