#include <Arduino.h>

#include "jGlobals.h"
#include "BoardDefinitions.h"
#include "jMatrix.h"

#define NUMSTOREDCONNECTIONS 100
//#define SHOWALLCONNECTIONS 1


jMatrix::jMatrix(void){

}
//struct jMatrix::connectionList(int){
//return conn[]
//}


int jMatrix::connect(int row1, int row2, int hop = 0)
{

static int connectionNumber;

//if (hop != 1){
//  connectionNumber++;
//}
int firstChip = chipToBreadboardMap[row1][0];
int firstChipYaddress = chipToBreadboardMap[row1][1];

int secondChip = chipToBreadboardMap[row2][0];
int secondChipYaddress = chipToBreadboardMap[row2][1];

int firstChipFreeXline = -1;
int secondChipFreeXline = -1;

int Xlane = 0;

int foundDirect = 0; //check if connection has already been made!!

int hopFlag = hop;



if (hopFlag >= 1)
{
  if (row2 > 0) {
    Serial.print("Unable to connect rows, try making an intermediate connection through an unused row");
    return -1;
  }
  secondChip = abs(row2);
  secondChipYaddress = 0;




}

if (firstChip == secondChip){

  // you dont need to use an unconnected row, just make both go to the same x line and do nothing with the second chip
for (int i = 0; i < 16; i ++) // check for a direct connection
{

  if (availableConnections[firstChip][i] == 1 && matrixArrayX [firstChip][i] != 'I' && matrixArrayX [firstChip][i] != 'J')
   {
                    firstChipFreeXline = i;
                    secondChipFreeXline = i;

                    availableConnections[firstChip][i] = 0;
                    foundDirect = 1;
                    Serial.print("Same Chip");
                    break;

   }


}

} else {

for (int i = 0; i < 16; i ++) // check for a direct connection
{
if ( matrixArrayX [firstChip][i] == chipToChar(secondChip))
  {

  if (availableConnections[firstChip][i] == 1)
   {
      if (i%2 == 0)
      {
        Xlane = 0;
      } else {
        Xlane = 1;
      }

        for(int j = 0; j < 16; j ++)
        {
            if ( matrixArrayX [secondChip][j] == chipToChar(firstChip) )
            {
              if (availableConnections[secondChip][j] == 1)
                {
     
                  foundDirect = 1;
                  firstChipFreeXline = i;
                  secondChipFreeXline = j;
                  availableConnections[firstChip][i] = 0;
                  availableConnections[secondChip][j] = 0;
                  

                  
                  for (int k = 0; k < 4; k++){   ///store connection (not sure if I'll keep this)
                  if ( connections[firstChip][firstChipYaddress][k] != -1)
                  connections[firstChip][firstChipYaddress][k] = row2;
                  }

                  for (int k = 0; k < 4; k++){
                  if ( connections[secondChip][secondChipYaddress][k] != -1)
                  connections[secondChip][secondChipYaddress][k] = row1;
                  }
                  break;
                }

            }

        }
        if (foundDirect == 1) break;
   }
  }
}   
}


if (foundDirect <= 0 ){
  //Serial.print("no direct connection!");
  //hopFlag = 1;
  //return -1;
if (findHopLocation(row1,row2) > 0)
{
  return 2;
} else {
  return - 2;
}
//return 2;


}else {
  
  connectDirectly(firstChipFreeXline, firstChipYaddress, firstChip, 1);
  connectDirectly(secondChipFreeXline, secondChipYaddress, secondChip, 1);

  
 if (hopFlag == 0) 
 { // for the first part of the hop don't change connection number so theyre stored together

//for (int i = 0; i < NUMSTOREDCONNECTIONS; i ++)
//{
//if (conn[i].used == 0)
//{
 // connectionNumber = i;
 //// break;
//}
connectionNumber++;
  numberOfConnections ++;
 //}




  conn[connectionNumber].used = 1;
  conn[connectionNumber].row1 = row1;
  conn[connectionNumber].row2 = row2;
  conn[connectionNumber].chip1 = firstChip;
  conn[connectionNumber].chip2 = secondChip;
  conn[connectionNumber].chip1Xaddr = firstChipFreeXline;
  conn[connectionNumber].chip2Xaddr = secondChipFreeXline;
  conn[connectionNumber].chip1Yaddr = firstChipYaddress;
  conn[connectionNumber].chip2Yaddr = secondChipYaddress;
  conn[connectionNumber].lane = Xlane;

  conn[connectionNumber].hop = hopFlag;


 } else if (hopFlag == 1)
 {
connectionNumber++;
  conn[connectionNumber].used = 1;
  conn[connectionNumber].row1 = row1;
  
  conn[connectionNumber].chip1 = firstChip;
  
  conn[connectionNumber].chip1Xaddr = firstChipFreeXline;
  conn[connectionNumber].chip1Yaddr = firstChipYaddress;
  conn[connectionNumber].lane = Xlane;

  conn[connectionNumber].hop = hopFlag;
  conn[connectionNumber].hopChipXaddr1 = secondChipFreeXline;
  conn[connectionNumber].row1 = row1;
 

} 

else if (hopFlag == 2)
{
  
  numberOfConnections ++;
  conn[connectionNumber].used = 1;
  conn[connectionNumber].row2 = row1;
  conn[connectionNumber].chip2 = firstChip;
  conn[connectionNumber].hopChip = abs(row2);
  conn[connectionNumber].chip2Xaddr = firstChipFreeXline;
  conn[connectionNumber].chip2Yaddr = firstChipYaddress;
  conn[connectionNumber].hopChipXaddr2 = secondChipFreeXline;

}




  //Serial.println("                                                                                             ");
CSI 
Serial.write("0K");
Serial.write("\x1B\x5B B");
CSI 
Serial.write("0K");
Serial.write("\x1B\x5B A");


//Serial.print("                                                     ");
  
  Serial.printf("\r\t\t\tChip %c(Y%d,X%d)<-->Chip %c(Y%d,X%d) Lane %c%c%d     \n\r\t\t\t",chipToChar(firstChip),firstChipYaddress ,firstChipFreeXline, chipToChar(secondChip),secondChipYaddress,secondChipFreeXline ,chipToChar(firstChip),chipToChar(secondChip),Xlane );

//Serial.print("                                            ");
  
  //Serial.printf("\n\rStruct number %d\n\rRow 1 = %d\tRow2 = %d \n\rChip 1 = %d\tChip2 = %d\n\rChip1Xaddr = %d\tChip2Xaddr = %d\n\rChip1Yaddr = %d\tChip2Yaddr = %d\n\rLane = %d\tRedundant = %d\n\n\r",connectionNumber,  conn[connectionNumber].row1,conn[connectionNumber].row2,conn[connectionNumber].chip1,conn[connectionNumber].chip2,conn[connectionNumber].chip1Xaddr,conn[connectionNumber].chip2Xaddr,conn[connectionNumber].chip1Yaddr,conn[connectionNumber].chip2Yaddr,Xlane,conn[connectionNumber].hop);
  return 1; //1 for direct connection
}
Serial.print("this shouldn't happen");
return -1;

}

int jMatrix::findHopLocation(int row1, int row2)
{

int firstChip = chipToBreadboardMap[row1][0];
int firstChipOpenDirectConnections [8] = {0,0,0,0,0,0,0,0}; //index is chip number, value is how many open lines

int secondChip = chipToBreadboardMap[row2][0];
int secondChipOpenDirectConnections [8] = {0,0,0,0,0,0,0,0};

int freeUCONNchips [8] = {-1,-1,-1,-1,-1,-1,-1,-1};
int freeUCONNcount = 0;

int hopChip = -1;

    for (int i = 0; i < 8; i++) //check for free unconnected Y pins (or already connected to target)
    {
      if(UNCONNconnections[i][0] == -1 && i != firstChip && i != secondChip)
      {
        freeUCONNchips[i] = 1;
        freeUCONNcount ++;
      } else if (i == firstChip || i == secondChip){ //idk
        
      }

      
    }


      for (int k = 0; k < 16; k++) 
      {
        if (availableConnections[firstChip][k] == 1)
        {
          firstChipOpenDirectConnections [k/2] ++;      
        }
        if (availableConnections[secondChip][k] == 1)
        {
          secondChipOpenDirectConnections [k/2] ++;
        }
      }

    for (int i = 0; i < 8; i++)
    {
      if (firstChipOpenDirectConnections[i] == 2 && secondChipOpenDirectConnections[i] == 2 && freeUCONNchips[i] == 1)
      {
        hopChip = i;
        //Serial.print(chipToChar(hopChip));
        //Serial.print("!");
        break;

      } else if (firstChipOpenDirectConnections[i] + secondChipOpenDirectConnections[i] == 3 && freeUCONNchips[i] == 1) // if one of them is 2 and the other is 1 (none them should ever be 3)
        {
          hopChip = i;

        } else if (firstChipOpenDirectConnections[i] == 1 && secondChipOpenDirectConnections[i] == 1 && freeUCONNchips[i] == 1 && hopChip == -1)
        {
          hopChip = i;
        }
//Serial.print(chipToChar(hopChip));
//Serial.print("? ");
      
    }
//Serial.print("hop");
if (connect(row1, (0-hopChip),1) >= 0 && connect(row2, (0-hopChip),2) >= 0)
{
  return 1;
} else {
  return -1;
}



/*
      
Serial.print("\n\n\n\n\n\n\n\n\n\rfirst = {");
for (int i = 0; i < 8 ; i++){

   Serial.print(firstChipOpenDirectConnections[i]);
   Serial.print(',');
}
      Serial.println('}');
    
Serial.print("second = {");
for (int i = 0; i < 8 ; i++){

   Serial.print(secondChipOpenDirectConnections[i]);
   Serial.print(',');
}
      Serial.println('}');

Serial.print("freeUCONNchips = {");
for (int i = 0; i < 8 ; i++){

   Serial.print(freeUCONNchips[i]);
   Serial.print(',');
}
      Serial.println("}\n\n\n\r");

      for (int i = 0; i < 8; i++)
      {
        for (int j = 0; j<16;j++)
        {
Serial.print(availableConnections[i][j]);
Serial.print(' ');




        }
        Serial.println(' ');
      }
*/

}

void jMatrix::disconnect(int net)
{

int connectionNumber = net;



#ifdef SHOWALLCONNECTIONS
for (int i =1; i<=(numberOfConnections); i++)
{
  connectionNumber = i;
Serial.printf("\rNet %d\n\rRow 1 = %d\tRow2 = %d \n\rChip 1 = %c\tChip2 = %c\n\rChip1Xaddr = %d\tChip2Xaddr = %d\n\rChip1Yaddr = %d\tChip2Yaddr = %d\n\rHop = %d\n\r",connectionNumber,  conn[connectionNumber].row1,conn[connectionNumber].row2,chipToChar(conn[connectionNumber].chip1),chipToChar(conn[connectionNumber].chip2),conn[connectionNumber].chip1Xaddr,conn[connectionNumber].chip2Xaddr,conn[connectionNumber].chip1Yaddr,conn[connectionNumber].chip2Yaddr,conn[connectionNumber].hop);

if (conn[connectionNumber].hop == 1)
{
  Serial.printf("Hop Chip = %c\n\rHop Xaddr1 = %d\tHop Xaddr2 = %d\n\n\r", chipToChar(conn[connectionNumber].hopChip),conn[connectionNumber].hopChipXaddr1,conn[connectionNumber].hopChipXaddr2);
}else{
  Serial.print("\n\r");
}
}
#endif
//Serial.print(net);




//connectionNumber = net;

if (conn[connectionNumber].hop == 1)
{

conn[connectionNumber].used = 0;
availableConnections[conn[connectionNumber].chip1][conn[connectionNumber].chip1Xaddr] = 1;
availableConnections[conn[connectionNumber].hopChip][conn[connectionNumber].hopChipXaddr1] = 1;
availableConnections[conn[connectionNumber].hopChip][conn[connectionNumber].hopChipXaddr2] = 1;
availableConnections[conn[connectionNumber].chip2][conn[connectionNumber].chip2Xaddr] = 1;


connectDirectly(conn[connectionNumber].chip1Xaddr,conn[connectionNumber].chip1Yaddr, conn[connectionNumber].chip1,0);
connectDirectly(conn[connectionNumber].hopChipXaddr1,0, conn[connectionNumber].hopChip,0);
connectDirectly(conn[connectionNumber].hopChipXaddr2,0, conn[connectionNumber].hopChip,0);
connectDirectly(conn[connectionNumber].chip2Xaddr,conn[connectionNumber].chip2Yaddr, conn[connectionNumber].chip1,0);




} else {

conn[connectionNumber].used = 0;
availableConnections[conn[connectionNumber].chip1][conn[connectionNumber].chip1Xaddr] = 1;
availableConnections[conn[connectionNumber].chip2][conn[connectionNumber].chip2Xaddr] = 1;


  connectDirectly(conn[connectionNumber].chip1Xaddr,conn[connectionNumber].chip1Yaddr, conn[connectionNumber].chip1,0);
  connectDirectly(conn[connectionNumber].chip2Xaddr,conn[connectionNumber].chip2Yaddr, conn[connectionNumber].chip2,0);

}

//net--;

}



int jMatrix::connectDirectly(int x,int y, int chip, int connectDisconnect)
{

selectChip(chip); //datasheet says CS_ should be set while setting up address lines, not sure why though
setAddress(x,y);
strobeItIn(connectDisconnect);
deselectChip();


}




void jMatrix::setAddress(int Xaddr, int Yaddr)
{

  byte XaddrFixed = MTfuckedUpTruthTable[Xaddr];
  //Serial.println (XaddrFixed);

  digitalWrite(AX3, LOW);
  digitalWrite(AX2, LOW);
  digitalWrite(AX1, LOW);
  digitalWrite(AX0, LOW);

  digitalWrite(AY2, LOW);
  digitalWrite(AY1, LOW);
  digitalWrite(AY0, LOW);

  //
  delayMicroseconds(10);

  digitalWrite(AX3, HIGH && (XaddrFixed & B00001000)); //this only writes the line high if that bit is set in binary value of Xaddr
  //Serial.print (HIGH && XaddrFixed & B00001000);
  digitalWrite(AX2, HIGH && (XaddrFixed & B00000100)); //for example Xaddr = 6 or B00000110          //note: && is logical AND, and & is bitwise AND
  //Serial.print (HIGH && XaddrFixed & B00000100);
  digitalWrite(AX1, HIGH && (XaddrFixed & B00000010)); //this bitwise ANDs Xaddr and a binary value with just one bit set
  //Serial.print (HIGH && XaddrFixed & B00000010);
  digitalWrite(AX0, HIGH && (XaddrFixed & B00000001)); //so we get          Xaddr   00000110
  //Serial.println (HIGH && XaddrFixed & B00000001);

  //                                                          AND bit selector   00001000 = 0
  digitalWrite(AY2, HIGH && (Yaddr & B00000100)); //then we AND that again with HIGH (which is just interpreted as 1)

  digitalWrite(AY1, HIGH && (Yaddr & B00000010)); //to get 1(HIGH) && 0(the result from above) = 0 (which is LOW)

  digitalWrite(AY0, HIGH && (Yaddr & B00000001)); //we do that for each bit to end up with the address lines LLLLLHHL

  delayMicroseconds(10); //allow the lines to settle

  //strobeItIn(); //we could probably do this without passing "chip" and just do all the strobing on both together,
  //                    but I don't necessarily want to make MT8816 ignore that many garbage signals with CS_ LOW
  //deselectChip();
}

void jMatrix::strobeItIn(int connectDisconnect)
{

  if (connectDisconnect == 0)
  {
    digitalWrite(DATAPIN, LOW);
  }
  else
  {
    digitalWrite(DATAPIN, HIGH);
  }

  //pinMode(STROBE, OUTPUT);
  delayMicroseconds(2);       //Hold time in the datasheet for the MT8816 says this only needs to be 10 nanoseconds
  digitalWrite(STROBE, HIGH); //but we're not super concerned with speed so I'll give it 1000X more just to be safe
  delayMicroseconds(5);       //Strobe has to be a minimum of 20 nanoseconds, but I dont want to think about the
  //Serial.println("!!!!!!!!!!!!");
  digitalWrite(STROBE, LOW); //fact that light only travels like 4 meters in that time through copper
  //pinMode(STROBE, OUTPUT);
  delayMicroseconds(2);
  digitalWrite(DATAPIN, LOW);
  //delayMicroseconds(30);
}

void jMatrix::selectChip(int chipInt)
{ //asserts whichever chip select line we send it

  char chip = chipToChar(chipInt);

  digitalWrite(CS_A, LOW);
  digitalWrite(CS_B, LOW);
  digitalWrite(CS_C, LOW);
  digitalWrite(CS_D, LOW);
  digitalWrite(CS_E, LOW);
  digitalWrite(CS_F, LOW);
  digitalWrite(CS_G, LOW);
  digitalWrite(CS_H, LOW);
  digitalWrite(CS_I, LOW);
  digitalWrite(CS_J, LOW);

  //delayMicroseconds(2);
  //Serial.print(chipToChar(chip));

  if (chip == 'A' || chip == 'a')
  {
    digitalWrite(CS_A, HIGH); //datasheet says CS_ should be set while setting up address lines, not sure why though
    //Serial.print(chipToChar(chip));
    return;
  }
  if (chip == 'B' || chip == 'b')
  {
    digitalWrite(CS_B, HIGH);
    return;
  }
  if (chip == 'C' || chip == 'c')
  {
    digitalWrite(CS_C, HIGH);
    return;
  }
  if (chip == 'D' || chip == 'd')
  {
    digitalWrite(CS_D, HIGH);
    return;
  }
  if (chip == 'E' || chip == 'e')
  {
    digitalWrite(CS_E, HIGH);
    return;
  }
  if (chip == 'F' || chip == 'f')
  {
    digitalWrite(CS_F, HIGH);
    return;
  }
  if (chip == 'G' || chip == 'g')
  {
    digitalWrite(CS_G, HIGH);
    return;
  }
  if (chip == 'H' || chip == 'h')
  {
    digitalWrite(CS_H, HIGH);
    return;
  }
  if (chip == 'I' || chip == 'i')
  {
    digitalWrite(CS_I, HIGH);
    return;
  }
  if (chip == 'J' || chip == 'j')
  {
    digitalWrite(CS_J, HIGH);
    return;
  }
}

void jMatrix::deselectChip(void)
{ //this should be fairly obvious

  digitalWrite(CS_A, LOW);
  digitalWrite(CS_B, LOW);
  digitalWrite(CS_C, LOW);
  digitalWrite(CS_D, LOW);
  digitalWrite(CS_E, LOW);
  digitalWrite(CS_F, LOW);
  digitalWrite(CS_G, LOW);
  digitalWrite(CS_H, LOW);
  digitalWrite(CS_I, LOW);
  digitalWrite(CS_J, LOW);
  return;
}

void jMatrix::clearConnections(void)
{ //when you send a reset pulse, all previous connections are cleared on whichever chip is CS_ed but we'll do both for now
digitalWrite(RESET, HIGH);
  digitalWrite(CS_A, HIGH);
  digitalWrite(CS_B, HIGH);
  digitalWrite(CS_C, HIGH);
  digitalWrite(CS_D, HIGH);
  digitalWrite(CS_E, HIGH);
  digitalWrite(CS_F, HIGH);
  digitalWrite(CS_G, HIGH);
  digitalWrite(CS_H, HIGH);
  digitalWrite(CS_I, HIGH);
  digitalWrite(CS_J, HIGH);

  
  delayMicroseconds(35); //datasheet says 40 nanoseconds minimum, this is a lot more than that
  

  digitalWrite(CS_A, LOW);
  digitalWrite(CS_B, LOW);
  digitalWrite(CS_C, LOW);
  digitalWrite(CS_D, LOW);
  digitalWrite(CS_E, LOW);
  digitalWrite(CS_F, LOW);
  digitalWrite(CS_G, LOW);
  digitalWrite(CS_H, LOW);
  digitalWrite(CS_I, LOW);
  digitalWrite(CS_J, LOW);

digitalWrite(RESET, LOW);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      for (int k = 0; k < 4; k++)
      {
        connections[i][j][k] = -1;
      }
    }
  }

  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      availableConnections[i][j] = 1;
    }
  }

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      UNCONNconnections[i][j] = -1;
    }
  }
}

char jMatrix::chipToChar(int chipInt)
{

  switch (chipInt)
  {
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
  case 9:
    return 'J';
  }
}