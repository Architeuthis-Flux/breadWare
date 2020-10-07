
#define chipSelectA 14
#define chipSelectB 18

#define dataA 15
#define strobeA 16
#define dataB 19
#define strobeB 20
#define rst 17

#define AX0 5
#define AX1 6
#define AX2 7
#define AX3 8

#define AY0 2
#define AY1 3
#define AY2 4

//Address lines for chip A and B will be connected together
//we can use the chipSelect lines to decide which chip to update
/*
The pinout for the dev board
Left side control lines (top to bottom)

Chip Select A
DATa A
STroBe A
ReSeT
Chip Select B
DATa B
STroBe B
VDD +
VSS GND
VEE -

Bottom address lines (left to right)

Y0 A
Y1 A
Y2 A
X0 A
X1 A
X2 A
X3 A
Y0 B
Y1 B
Y2 B
X0 B
X1 B
X2 B
X3 B

*/

char chip = 'A';

byte Xaddr = 0;
byte Yaddr = 0;

int Xsize = 16;
int Ysize = 8;

int X = 0;
int Y = 0;

int comma = -1;


const byte bufLength = 8;


char serialBuffer[bufLength];
char serialBufferX[bufLength];
char serialBufferY[bufLength];

void setup() {

  Serial.begin (9600);

  pinMode(rst, OUTPUT);   //all of this just sets up all these pins as outputs and drives them LOW
  digitalWrite(rst, LOW); //I'm pretty sure Arduino IDE does this automatically but I wouldn't count on it

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


  pinMode(chipSelectA, OUTPUT);
  digitalWrite(chipSelectA, LOW);
  pinMode(chipSelectB, OUTPUT);
  digitalWrite(chipSelectB, LOW);

  pinMode(dataA, OUTPUT);
  digitalWrite(dataA, LOW);
  pinMode(strobeA, OUTPUT);
  digitalWrite(strobeA, LOW);

  pinMode(dataB, OUTPUT);
  digitalWrite(dataB, LOW);
  pinMode(strobeB, OUTPUT);
  digitalWrite(strobeB, LOW);
}


void loop() {

  Serial.println("Connect X [0-15] to Y [0-7]");
  
  getX();           

  setAddress(Xaddr,Yaddr,chip); //these are all global variables and nothing in the function changes their value but I'm passing them anyway
  
  Serial.println("Cool\n");

}


void getX() {     //this is just to get values from the serial monitor (which isnt a particularly nice way to set the switches)
//                  dealing with inputs to the serial monitor is almost always a shit show, as you will see below

  for (int i = 0; i < bufLength; i++) {   //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
    serialBufferX[i] = ' ';
    serialBufferY[i] = ' ';
  }

  Serial.print("X = ");
  while (!Serial.available());    //this just keeps looping if there's nothing in the serial buffer, so it waits for you to send some data

  Serial.readBytesUntil('\n', serialBuffer, bufLength); //reads chars one at a time and puts them into the array serialBuffer
//                                                        until it reads an \n or it reaches the end (bufLength)

  for (int i = 0; i < bufLength; i++) {  //this loop lets is enter values as "X,Y" instead of one at a time (it was super annoying to do it that way when testing)
//                                         by taking one serialBuffer and then splitting it into separate X and Y serialBuffers
    if (serialBuffer[i] == ',') {     //loops through serialBuffer[] to look for a comma and sets "comma" to the position in the array
      comma = i;
      i++;
    }

    if (comma == -1) {                    //if it hasnt found a comma, then this part of the buffer is for the X address buffer
      serialBufferX[i] = serialBuffer[i];
    } else {
      serialBufferY[i] = serialBuffer[i]; //if it has, then it start putting them into the Y address buffer
    }
  }


  int incomingValueX = atoi(serialBufferX);   //atoi converts a char array to an integer (Array TO Int)
  int incomingValueY = atoi(serialBufferY);

  if (incomingValueX >= Xsize) {              //this checks to make sure the values you entered aren't above the maximum size specified in Xsize
    Serial.print ("\nPlease enter a number for X between 0 and ");
    Serial.println (Xsize - 1);
    return;
  }
  if (incomingValueY >= Ysize && comma > 0) { //the logical AND makes it only check if we put in a Y address separated by a comma
    Serial.print ("\nPlease enter a number for Y between 0 and ");
    Serial.println (Ysize - 1);
    return;
  }

  Xaddr = atoi(serialBufferX);  //now that we know our inputs are good, we can put them into Xaddr and Yaddr
  Yaddr = atoi(serialBufferY);

  Serial.println(Xaddr); 

  if (comma > 0) {          //only prints a Y if you entered it here
    Serial.print("Y = ");
    Serial.println(Yaddr);
  }

  Serial.println(' ');    //gives us some breathing room in the serial monitor output
  
  if (comma < 0) {        //if a Y wasnt entered comma will still be -1, call getY() to ask for one
     comma = -1;          //set the comma back to -1 for the next entry
     getY(); 
  }
 
}

void getY() {

  Serial.print("Y = ");           //so you don't forget what you were doing
  
  while (!Serial.available());    //this just keeps looping if there's nothing in the serial buffer, so it waits for you to send some data

  Serial.readBytesUntil('\n', serialBufferY, bufLength);    //reads chars one at a time and puts them into the array serialBuffer
//                                                            until it reads an \n or it reaches the end (bufLength)

  int incomingValue = atoi(serialBufferY);    //Array to int
  if (incomingValue >= Ysize) {               //bounds checking
    Serial.print ("\nPlease enter a number between 0 and ");
    Serial.println (Ysize - 1);
    return;
  }

  while (Serial.available() > 0) {    //this is just a trick to clear the serial buffer (if we typed in a really long string it would just keep entering them one after another)
    Serial.read();                    //it reads the serial buffer and then throws away the values, just to clear it
  }

  Yaddr = atoi(serialBufferY);      //put our Y input as an integer into Yaddr 

  Serial.println(Yaddr);
  Serial.println(' ');
}

void setAddress (byte Xaddr, byte Yaddr, char chip) {

  selectChip(chip);            //datasheet says chipSelect should be set while setting up address lines, not sure why though


  digitalWrite(AX3, HIGH && (Xaddr & B00001000));   //this only writes the line high if that bit is set in binary value of Xaddr
  digitalWrite(AX2, HIGH && (Xaddr & B00000100));   //for example Xaddr = 6 or B00000110          //note: && is logical AND, and & is bitwise AND
  digitalWrite(AX1, HIGH && (Xaddr & B00000010));   //this bitwise ANDs Xaddr and a binary value with just one bit set
  digitalWrite(AX0, HIGH && (Xaddr & B00000001));   //so we get          Xaddr   00000110
  //                                                          AND bit selector   00001000 = 0
  digitalWrite(AY2, HIGH && (Yaddr & B00000100));   //then we AND that again with HIGH (which is just interpreted as 1)
  digitalWrite(AY1, HIGH && (Yaddr & B00000010));   //to get 1(HIGH) && 0(the result from above) = 0 (which is LOW)
  digitalWrite(AY0, HIGH && (Yaddr & B00000001));   //we do that for each bit to end up with the address lines LLLLLHHL

  delayMicroseconds(100);      //allow the lines to settle

  strobeItIn(chip);   //we could probably do this without passing "chip" and just do all the strobing on both together,
  //                    but I don't necessarily want to make MT8816 ignore that many garbage signals with chipSelect LOW
  deselectChip();

  digitalWrite(AX3, LOW);
  digitalWrite(AX2, LOW);
  digitalWrite(AX1, LOW);
  digitalWrite(AX0, LOW);

  digitalWrite(AY2, LOW);
  digitalWrite(AY1, LOW);
  digitalWrite(AY0, LOW);

}

void strobeItIn (char chip) {

  if (chip == 'A' || chip == 'a') {
    digitalWrite(dataA, HIGH);
    delayMicroseconds(10);        //Hold time in the datasheet for the MT8816 says this only needs to be 10 nanoseconds
    digitalWrite(strobeA, HIGH);  //but we're not super concerned with speed so I'll give it 1000X more just to be safe
    delayMicroseconds(10);        //Strobe has to be a minimum of 20 nanoseconds, but I dont want to think about the
    digitalWrite(strobeA, LOW);   //fact that light only travels like 4 meters in that time through copper
    delayMicroseconds(10);
    digitalWrite(dataA, LOW);
    delayMicroseconds(30);
  }

  if (chip == 'B' || chip == 'b') {
    digitalWrite(dataB, HIGH);
    delayMicroseconds(10);
    digitalWrite(strobeB, HIGH);
    delayMicroseconds(10);
    digitalWrite(strobeB, LOW);
    delayMicroseconds(10);
    digitalWrite(dataB, LOW);
    delayMicroseconds(30);
  }



}

void selectChip (char chip)  {        //asserts whichever chip select line we send it

  if (chip == 'A' || chip == 'a') {
    digitalWrite(chipSelectA, HIGH);      //datasheet says chipSelect should be set while setting up address lines, not sure why though
    return;
  }
  if (chip == 'B' || chip == 'b') {
    digitalWrite(chipSelectB, HIGH);
    return;
  }
}

void deselectChip (void)  {         //this should be fairly obvious

  digitalWrite(chipSelectA, LOW);
  digitalWrite(chipSelectB, LOW);
  return;
}


void clearConnections (void)  {    //when you send a reset pulse, all previous connections are cleared on whichever chip is chipSelected but we'll do both for now
  
  digitalWrite(chipSelectA, HIGH);
  digitalWrite(chipSelectB, HIGH);
  
  digitalWrite(rst, HIGH);
  delayMicroseconds(10);     //datasheet says 40 nanoseconds minimum, this is a lot more than that
  digitalWrite(rst, LOW);

  digitalWrite(chipSelectA, LOW);
  digitalWrite(chipSelectB, LOW);

}
