
//#include "mcp4728.h"

#include <Adafruit_MCP4728.h>
#include <Wire.h>
#include "breadWareDefines.h"
#include "Arduino.h"
#include "Globals.h"
#include "MCP466_DigitalPot.h"

Adafruit_MCP4728 mcp;
//mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

MCP466_DigitalPot pot(mcpAddress);

int findPath(int, int);
int lookForPointedConns(void);
void showMenuOptions(void);
void selectMenuItem(void);
void potentiometer(void);
void showSpecialFunctionsMenu(void);
void powerSupply(int, int, float);
void powerSupplyMenu(void);
void getX(void);
void setAddress(byte, byte, char);
void strobeItIn(void);
void selectChip(char);
void deselectChip(void);
void clearConnections(void);
void clearBuffers(void);
char chipToChar(int);
void showArrays(void);
void showConnections(void);

void setup()
{
  //Wire.swap(0);
  //Wire.begin();
  //dac.begin();  // initialize i2c interface
  // dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  Serial.begin(115200);
  mcp.begin(0x61);

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
  digitalWrite(CS_A, LOW);
  pinMode(CS_B, OUTPUT);
  digitalWrite(CS_B, LOW);
  pinMode(CS_C, OUTPUT);
  digitalWrite(CS_C, LOW);
  pinMode(CS_D, OUTPUT);
  digitalWrite(CS_D, LOW);
  pinMode(CS_E, OUTPUT);
  digitalWrite(CS_E, LOW);
  pinMode(CS_F, OUTPUT);
  digitalWrite(CS_F, LOW);
  pinMode(CS_G, OUTPUT);
  digitalWrite(CS_G, LOW);
  pinMode(CS_H, OUTPUT);
  digitalWrite(CS_H, LOW);
  pinMode(CS_I, OUTPUT);
  digitalWrite(CS_I, LOW);

  pinMode(RESET, OUTPUT);    //all of this just sets up all these pins as outputs and drives them LOW
  digitalWrite(RESET, HIGH); //I'm pretty sure Arduino IDE does this automatically but I wouldn't count on it
  delayMicroseconds(40);
  digitalWrite(RESET, LOW);

  pinMode(PIN_PF2, OUTPUT);
  digitalWrite(PIN_PF2, HIGH);
  pinMode(PIN_PF3, OUTPUT);
  digitalWrite(PIN_PF3, HIGH);

  pinMode(PIN_PA2, OUTPUT);

  pinMode(PIN_PA3, OUTPUT);

  pinMode(PIN_PD3, OUTPUT);
  digitalWrite(PIN_PD3, LOW);

  pinMode(PWR_NEG_EN, OUTPUT);
  digitalWrite(PWR_NEG_EN, HIGH);

  pinMode(DATA, OUTPUT);
  digitalWrite(DATA, HIGH);
  pinMode(STROBE, OUTPUT);
  //digitalWrite(STROBE, LOW);

  clearConnections();

powerSupply (0,0,0);

//Serial.print("Yeet!");
}

void loop()
{
//Serial.print("Yeet!");

  if (stayInConnectionMenu == 1)
  {
    getX();
  }
  else
  {
    if (stayInConnectionMenu == -1)
    {
      showSpecialFunctionsMenu();
      selectMenuItem();
    }
    else
    {
      showMenuOptions();
      selectMenuItem();
    }
  }
}

int findPath(int breadRow1, int breadRow2)
{

  //Add a way to search through existing connections to disconnect stuff

  if (breadRow1 == breadRow2)
  {
    return -1;
  }

  if (breadRow1 > breadRow2)
  {
    int breadRowTemp1 = breadRow1;
    breadRow1 = breadRow2;
    breadRow2 = breadRowTemp1;
  }

  int breadRow1Unchanged = breadRow1;
  int breadRow2Unchanged = breadRow2;

  int pathChip1 = chipToBreadboardMap[breadRow1][0];
  int pathChip2 = chipToBreadboardMap[breadRow2][0];
  int tempPathChip = 0;

  byte Xpin = 0;
  byte Ypin1 = chipToBreadboardMap[breadRow1][1];

  byte Ypin2 = chipToBreadboardMap[breadRow2][1];

  int repeatedConnectionFlag = 0;
  int altPathCounter = 0;
  int altPathChoice = 0;

  int XofChip1 = 0;
  int XofChip2 = 0;

  int hopLocation[2][7] = {{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};

  // {UNCONNchip, connected chip 1, X pin to chip 1, used X connections chip 1, connected chip 2, X pin to chip 2, used X connections chip 2}

  for (int i = 0; i < 17; i++)
  {

    if (pathChip1 == pathChip2)
    {
      for (int k = 1; k < 16; k += 2)
      {

        if (availableConnections[pathChip1][k] == 1 && availableConnections[pathChip2][k] == 1)
        {
          Xpin = k;
          availableConnections[pathChip1][k] = 0;
          availableConnections[pathChip1][k] = 0;
          connections[pathChip1][Ypin1][k] = breadRow2;
          connections[pathChip2][Ypin2][k] = breadRow1;
          Serial.print("Connected on same chip via X pin ");
          Serial.println(k);
          break;
        }
      }
      break;
    }

    if (matrixArrayX[pathChip1][i] == chipToChar(pathChip2))
    {

      for (int k = 0; k < 4; k++)
      {

        if (connections[pathChip1][Ypin1][k] == breadRow2)
        {
          Xpin = i;
          repeatedConnectionFlag = 1;
          //Serial.println("Same");
          break;
        }
        else if (connections[pathChip1][Ypin1][k] == -1 && connections[pathChip2][Ypin2][k] == -1)
        {
          connections[pathChip1][Ypin1][k] = breadRow2;

          connections[pathChip2][Ypin2][k] = breadRow1;

          //Serial.println("Different");
          break;
        }
      }

      if (availableConnections[pathChip1][i] == dataLine || repeatedConnectionFlag == 1)
      {

        Xpin = i;
        availableConnections[pathChip1][i] = !dataLine;
        availableConnections[pathChip2][i] = !dataLine;

        //Serial.println("*");
        break;
      }
      //Serial.println();

      //Serial.print("?");
    }
    //Serial.print("!");

    if (i >= 16)
    {

      for (int l = 0; l < 4; l++)
      {

        int found = 0;

        for (int m = 0; m < 17; m += 4)
        {
          if (matrixArrayX[unconnChips[l]][m] == chipToChar(pathChip2))
          {
            XofChip1 = m;
            //Serial.println(chipToChar(unconnChips[l]));
            found += 1;
          }
          if (matrixArrayX[unconnChips[l]][m] == chipToChar(pathChip1))
          {
            XofChip2 = m;
            //Serial.println(chipToChar(unconnChips[l]));
            found += 1;
          }

          if (found == 2)
          {

            hopLocation[altPathCounter][0] = unconnChips[l];
            hopLocation[altPathCounter][1] = pathChip1;
            hopLocation[altPathCounter][4] = pathChip2;

            for (int n = 0; n < 4; n++)
            {
              if (UNCONNconnections[l][XofChip1 + n] == -1)
              { //make it the X pin in UCONNconnections
                hopLocation[altPathCounter][2] = XofChip2 + n;
                hopLocation[altPathCounter][3] = n;
                break;
              }
            }
            for (int p = 0; p < 4; p++)
            {
              if (UNCONNconnections[l][XofChip2 + p] == -1)
              { //make it the X pin in UCONNconnections
                hopLocation[altPathCounter][5] = XofChip1 + p;
                hopLocation[altPathCounter][6] = p;
                //Serial.println(p);
                break;
              }
            }
            found = 0;
            //for (int hop = 0; hop < 7; hop++) {
            //  Serial.print(hopLocation[altPathCounter][hop]);
            //  Serial.print("\t");
            //} Serial.println("\n");
            altPathCounter += 1;
          }
        }
      }
      if (altPathCounter >= 2)
      {
        if (hopLocation[0][3] <= hopLocation[1][3] && hopLocation[0][6] <= hopLocation[1][6])
        {
          altPathChoice = 0;
        }
        else if (hopLocation[0][3] >= hopLocation[1][3] && hopLocation[0][6] >= hopLocation[1][6])
        {
          altPathChoice = 1;
        }
        else if (hopLocation[0][3] == 4 || hopLocation[0][6] == 4)
        {
          altPathChoice = 1;
        }
        else if (hopLocation[1][3] == 4 || hopLocation[1][6] == 4)
        {
          altPathChoice = 0;
        }
        else if ((hopLocation[1][3] + hopLocation[1][6]) < (hopLocation[0][3] + hopLocation[0][6]))
        {
          altPathChoice = 1;
        }
        else
        {
          altPathChoice = 0;
        }
        //Serial.print(hopLocation[altPathChoice][3]);
        // Serial.println(hopLocation[altPathChoice][6]);
      }

      int chipBackToUNCONNchip = 0;
      for (int u = 0; u < 4; u++)
      {
        if (hopLocation[altPathChoice][0] == unconnChips[u])
        {
          chipBackToUNCONNchip = u;
        }
      }

      availableConnections[hopLocation[altPathChoice][0]][hopLocation[altPathChoice][2]] = 0;
      availableConnections[hopLocation[altPathChoice][0]][hopLocation[altPathChoice][5]] = 0;
      availableConnections[pathChip1][hopLocation[altPathChoice][2]] = 0;
      availableConnections[pathChip2][hopLocation[altPathChoice][5]] = 0;

      for (int q = 0; q < 4; q++)
      {

        if (connections[pathChip1][Ypin1][q] == -1 && connections[pathChip2][Ypin2][q] == -1)
        {

          connections[pathChip1][Ypin1][q] = breadRow2Unchanged;

          connections[pathChip2][Ypin2][q] = breadRow1Unchanged;
          break;
        }
      }

      for (int r = 0; r < 4; r++)
      {

        if (connections[hopLocation[altPathChoice][0]][unconnYpins[hopLocation[altPathChoice][0]]][r] == -1)
        {

          connections[hopLocation[altPathChoice][0]][unconnYpins[hopLocation[altPathChoice][0]]][r] = breadRow1Unchanged;

          connections[hopLocation[altPathChoice][0]][unconnYpins[hopLocation[altPathChoice][0]]][r + 1] = breadRow2Unchanged;
          break;
        }
      }

      if (dataLine == 1)
      {
        UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][2]] = breadRow1Unchanged;
        UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][5]] = breadRow2Unchanged;
      }
      else
      {
        UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][2]] = -1;
        UNCONNconnections[chipBackToUNCONNchip][hopLocation[altPathChoice][5]] = -1;
      }

      //Serial.println (altPathChoice);

      //Serial.println (altPathCounter);

      Serial.print(chipToChar(pathChip1));
      Serial.print("\t");
      Serial.print(hopLocation[altPathChoice][2]);
      Serial.print("\t");
      Serial.println(Ypin1);

      Serial.print(chipToChar(hopLocation[altPathChoice][0]));
      Serial.print("\t");
      Serial.print(hopLocation[altPathChoice][2]);
      Serial.print("\t");
      Serial.println(unconnYpins[hopLocation[altPathChoice][0]]);

      Serial.println(" ");

      Serial.print(chipToChar(hopLocation[altPathChoice][0]));
      Serial.print("\t");
      Serial.print(hopLocation[altPathChoice][5]);
      Serial.print("\t");
      Serial.println(unconnYpins[hopLocation[altPathChoice][0]]);

      Serial.print(chipToChar(pathChip2));
      Serial.print("\t");
      Serial.print(hopLocation[altPathChoice][5]);
      Serial.print("\t");
      Serial.println(Ypin2);


      setAddress(hopLocation[altPathChoice][2], unconnYpins[hopLocation[altPathChoice][0]], chipToChar(hopLocation[altPathChoice][0]));
      setAddress(hopLocation[altPathChoice][2], Ypin1, chipToChar(pathChip1));

      setAddress(hopLocation[altPathChoice][5], unconnYpins[hopLocation[altPathChoice][0]], chipToChar(hopLocation[altPathChoice][0]));
      setAddress(hopLocation[altPathChoice][5], Ypin2, chipToChar(pathChip2));

      //Serial.println(altPathChoice);
      //break;
    }
  }

  if (altPathCounter == 0)
  {
    Serial.print(chipToChar(pathChip1));
    Serial.print("\t");
    //Serial.print(Xaddr1);
    Serial.print(" ");
    Serial.print(Xpin);
    Serial.print("\t");
    //Serial.print(Yaddr1);
    Serial.print(" ");
    Serial.println(Ypin1);

    setAddress(Xpin, Ypin1, chipToChar(pathChip1));

    setAddress(Xpin, Ypin2, chipToChar(pathChip2));

    Serial.print(chipToChar(pathChip2));
    Serial.print("\t");
    //Serial.print(Xaddr2);
    Serial.print(" ");
    Serial.print(Xpin);
    Serial.print("\t");
    //Serial.print(Yaddr2);
    Serial.print(" ");
    Serial.println(Ypin2);
  }
}

int lookForPointedConns(void)
{

  int testFreq = 20000;
  unsigned long duration;

  tone(PIN_PC0, testFreq);

  int probePath[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  int probePathChoice[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //
  int testXpin = 0;

  for (int i = 0; i < 8; i++)
  {

    for (int k = 0; k < 4; k++)
    {

      if (connections[i][unconnYpins[i]][k] != -1)
      {
        probePath[i]++;
      }
    }

    Serial.print(probePath[i]);
  }
  Serial.println(" ");

  for (int i = 0; i < 8; i++)
  {

    if (i == 0 || i == 3 || i == 4 || i == 7)
    {

      if (probePath[i] == 0 && probePath[(i + 4) % 8] == 0)
      {

        for (int j = 0; j < 4; j++)
        {
          probePathChoice[j] = i;
          Serial.print(probePathChoice[j]);
        }
        for (int j = 4; j < 8; j++)
        {
          probePathChoice[j] = (i + 4) % 8;
          Serial.print(probePathChoice[j]);
        }
        break;
      }
      else if (probePath[i] == 0 && probePath[(i + 3) % 8] == 0)
      {
        for (int j = 0; j < 4; j++)
        {
          probePathChoice[j] = i;
          Serial.print(probePathChoice[j]);
        }
        for (int j = 4; j < 8; j++)
        {
          probePathChoice[j] = (i + 3) % 8;
          Serial.print(probePathChoice[j]);
        }
        Serial.println(" ");
        break;
      }
    }
  }

  /*for (int i = 0; i < 8; i++) {

    if (i == 1 || i == 2 || i == 5 || i == 6) {

      if (probePath [i] == 0 && i < 4) {
        probePathChoice [8] = i;
      } else if (probePath [i] == 0 && i >= 4) {
        probePathChoice [9] = i;
      }
    }
    }*/

  //setAddress(8, 6, 'I'); // PC0 to breadboard pin B16

  for (int i = 0; i < 8; i++)
  {
    int offset = 0;
    if (i == 0)
    {
      dataLine = 1;
      setAddress(8, probePathChoice[0], 'I');
      setAddress(12, probePathChoice[4], 'I');

      Serial.println(" ");
      Serial.print(probePathChoice[0]);
      Serial.print(" ");
      Serial.println(probePathChoice[4]);
      Serial.println(" ");
    }
    else if (i == 4)
    {

      dataLine = 0;
      setAddress(8, probePathChoice[0], 'I');
      setAddress(12, probePathChoice[4], 'I');

      dataLine = 1;
      setAddress(8, probePathChoice[4], 'I');
      setAddress(12, probePathChoice[0], 'I');

      Serial.println(" ");
      Serial.print(probePathChoice[4]);
      Serial.print(" ");
      Serial.println(probePathChoice[0]);
      Serial.println(" ");
    }
    /*
      for (int k = 0; k < 4; k++) {
      if (availableConnections [probePathChoice[i]][k*(i%4)] == 1) {
      offset = k;
      } else if (availableConnections [probePathChoice[i]][k * 4] == 1)
    */

    for (int j = 0; j < 8; j++)
    {

      dataLine = 1;
      setAddress((i % 4) * 4, 7, chipToChar(probePathChoice[i % 4]));

      setAddress((i % 4) * 4, j, chipToChar(probePathChoice[((i % 4) + 4) % 8]));

      duration = pulseIn(PIN_PE3, HIGH, 200);
      //if (duration > 5) {
      Serial.print(i);
      Serial.print(' ');
      Serial.print(j);
      Serial.print(' ');
      Serial.println(duration);
      //}
      dataLine = 0;
      setAddress((i % 4) * 4, 7, chipToChar(probePathChoice[i % 4]));

      setAddress((i % 4) * 4, j, chipToChar(probePathChoice[((i % 4) + 4) % 8]));
    }
  }
  dataLine = 0;
  setAddress(8, probePathChoice[0], 'I');
  setAddress(12, probePathChoice[4], 'I');

  setAddress(8, probePathChoice[4], 'I');
  setAddress(12, probePathChoice[0], 'I');
  //noTone(PIN_PE3);
}

void showMenuOptions(void)
{

  Serial.println(" ");

  Serial.println("Connect Nodes \t\t\tc");
  Serial.println("Disconnect Nodes \t\td");
  Serial.println("Show Frontend Connections \ty");
  Serial.println("Show Backend Connections \tx");
  Serial.println("Reset Connections\t\tr");
  Serial.println("Special Funtions Menu\t\ts");
  Serial.println("Back to Menu\t\t\tm");
  Serial.println(" ");
}

void selectMenuItem(void)
{

  //altPath = -1;
  while (!Serial.available())
    ; //this just keeps looping if there's nothing in the serial buffer, so it waits for you choose an item

  for (int i = 0; i < bufLength; i++)
  { //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
  }
  Serial.readBytesUntil('\n', serialBuffer, bufLength);

  if (isDigit(serialBuffer[0]) == true)
  {
    dataLine = 1;
    stayInConnectionMenu = 1;
    getX();
  }

  switch (tolower(serialBuffer[0]))
  {

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
    Serial.print("Cleared!\n");
    break;

  case 'x':
    showArrays();
    break;

  case 'y':
    showConnections();
    break;

  case 's':
    stayInConnectionMenu = -1;
    break;

  case 'i':
    powerSupplyMenu();
    break;

  case 'o':
   powerSupply(0,0,0);
    break;

  case 'p':
    potentiometer();
    break;

  case 'f':
    lookForPointedConns();
    break;
  }
  clearBuffers();
}

void potentiometer(void)
{

  Wire.begin();
  char serialBuffer2[bufLength];

  Serial.println("Select Pot a or b");
  while (!Serial.available())
    ;
  Serial.readBytesUntil('\n', serialBuffer2, bufLength);
  char potLetter = serialBuffer2[0];
  for (int i = 0; i < bufLength; i++)
  { //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
  }
  Serial.println("Select resistance between pin A and the wiper in Kohms (50K max)");
  while (!Serial.available())
    ;
  Serial.readBytesUntil('\n', serialBuffer2, bufLength);

  int wiperPosition = atoi(serialBuffer2);

  if (tolower(potLetter == 'a'))
  {

    pot.mcpWrite(volatileWiper0, wiperPosition * 0.194);
  }
  else if (tolower(potLetter == 'b'))
  {

    pot.mcpWrite(volatileWiper1, wiperPosition * 0.194);
  }

  for (int i = 0; i < bufLength; i++)
  { //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
  }
  Serial.println("Choose row for wiper (p for Power, g for Ground, x for unconnected)");

  while (!Serial.available())
    ;
  Serial.readBytesUntil('\n', serialBuffer2, bufLength);

  int potWiper = 0;
  int wiperPin = 5;
  if (potLetter == 'b')
  {
    wiperPin = 8;
  }

  if (isDigit(serialBuffer2[0]) == true)
  {
    int potWiper = atoi(serialBuffer2);
  }
  else
  {
    switch (serialBuffer2[0])
    {
    case 'p':
      potWiper = -1;
      setAddress(wiperPin, 56, 'I');

    case 'g':
      potWiper = -1;
      setAddress(wiperPin, 32, 'I');
    case 'X':
      potWiper = -1;
    }
  }
  if (potWiper >= 0)
  {
    findPath(48, potWiper);
  }

  for (int i = 0; i < bufLength; i++)
  { //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
  }
  Serial.println("Choose row for pin A");
  while (!Serial.available())
    ;
  Serial.readBytesUntil('\n', serialBuffer2, bufLength);

  int APin = 4;
  if (potLetter == 'b')
  {
    APin = 7;
  }
  int potPinA = atoi(serialBuffer2);

  if (isDigit(serialBuffer2[0]) == true)
  {
    int potPinA = atoi(serialBuffer2);
  }
  else
  {
    switch (serialBuffer2[0])
    {
    case 'p':
      potPinA = -1;
      setAddress(APin, 56, 'I');

    case 'g':
      potPinA = -1;
      setAddress(APin, 32, 'I');
    case 'X':
      potPinA = -1;
    }
  }
  if (potPinA >= 0)
  {
    findPath(40, potPinA);
  }

  for (int i = 0; i < bufLength; i++)
  { //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
  }
  Serial.println("Choose row for pin B");
  while (!Serial.available())
    ;
  Serial.readBytesUntil('\n', serialBuffer2, bufLength);

  int BPin = 5;

  if (potLetter == 'b')
  {
    BPin = 9;
  }
  int potPinB = atoi(serialBuffer2);

  if (isDigit(serialBuffer2[0]) == true)
  {
    int potPinB = atoi(serialBuffer2);
  }
  else
  {
    switch (serialBuffer2[0])
    {
    case 'p':
      potPinB = -1;
      setAddress(BPin, 56, 'I');

    case 'g':
      potPinB = -1;
      setAddress(BPin, 32, 'I');
    case 'X':
      potPinB = -1;
    }
  }
  if (potPinB >= 0)
  {
    findPath(16, potPinB);
  }
}

void showSpecialFunctionsMenu(void)
{

  Serial.println(" ");
  Serial.println("Power Supplies ON \t\ti");
  Serial.println("Power Supplies OFF \t\to");
  Serial.println("Potentiometer \t\t\tp");
  Serial.println("Analog Read\t\t\tv");
  Serial.println("GPIO \t\t\t\tg");
  Serial.println("Back to Menu\t\t\tm");
  Serial.println(" ");
}

void powerSupply(int onOff, int channel, float voltage)
{
  int setting = abs(voltage*(4095/5));

  if (abs(powerSupplyVoltages[0]) > 0.1 || abs(powerSupplyVoltages[2]) > 0.1 ){
    digitalWrite(PIN_PF2, LOW);
  } else {
    digitalWrite(PIN_PF2, HIGH);
  }
  if (abs(powerSupplyVoltages[1]) > 0.1 || abs(powerSupplyVoltages[3]) > 0.1 ){
    digitalWrite(PIN_PF3, LOW);
  } else {
    digitalWrite(PIN_PF3, HIGH);
  }


  if (onOff == 1)
  {
    //dataLine = 1;
    //digitalWrite(PWR_ENABLE2, HIGH);
    //digitalWrite(PIN_PF2, LOW);
   //digitalWrite(PIN_PF3, LOW);

    digitalWrite(PIN_PD3, LOW);

    //Serial.println("Adafruit MCP4728 test!");

    // Try to initialize!
    if (!mcp.begin(0x61))
    {
      Serial.println("Failed to find MCP4728 chip");

      mcp.begin(0x61);
    }
    if (channel == 0) {
      mcp.setChannelValue(MCP4728_CHANNEL_C, 0);
      mcp.setChannelValue(MCP4728_CHANNEL_A, setting-82);

    } else if (channel == 1) {
      mcp.setChannelValue(MCP4728_CHANNEL_B, setting-550);

    } else if (channel == 2) {
      mcp.setChannelValue(MCP4728_CHANNEL_A, 0);
      mcp.setChannelValue(MCP4728_CHANNEL_C, (setting/1.604)-100);

    } else if (channel == 3) {
      mcp.setChannelValue(MCP4728_CHANNEL_D, (setting/1.604)-164);
    }

  }
  else if (onOff == 0)
  {
        powerSupplyVoltages[0] = 0;
        powerSupplyVoltages[1] = 0;
        powerSupplyVoltages[2] = 0;
        powerSupplyVoltages[3] = 0;
    mcp.setChannelValue(MCP4728_CHANNEL_A, 0);
    mcp.setChannelValue(MCP4728_CHANNEL_B, 0);
    mcp.setChannelValue(MCP4728_CHANNEL_C, 0);
    mcp.setChannelValue(MCP4728_CHANNEL_D, 0);
    //dataLine = 0;
    //digitalWrite(PWR_ENABLE2, LOW);
    digitalWrite(PIN_PF2, HIGH);
    digitalWrite(PIN_PF3, HIGH);
  }
  else
  {
    return;
  }
}

void powerSupplyMenu(void)
{
  float enteredVal = 0;
tryagain:
while(Serial.available()){Serial.read();}

  Serial.println("\n\tCurrent Values\n");
  Serial.print("Top Negative \t\t\t");
  Serial.print("0.00");
  Serial.println("V");

  Serial.print("Top Positive \t\t\t");
  Serial.print(powerSupplyVoltages[0] + powerSupplyVoltages[2]);
  Serial.println("V");

  Serial.print("\nBottom Negative \t\t");
  if (powerSupplyVoltages[3] < 0) {Serial.print("\b");}
  Serial.print(powerSupplyVoltages[3]);
  Serial.println("V");

  Serial.print("Bottom Positive \t\t");
  Serial.print(powerSupplyVoltages[1]);
  Serial.println("V");
  Serial.println(" ");

  Serial.println("Choose rail to edit\t\ttp bp bn or d to set both as 5v and Gnd");

  while (!Serial.available()); //this just keeps looping if there's nothing in the serial buffer, so it waits for you to send some data

  Serial.readBytesUntil('\n', serialBuffer, bufLength);

    switch (tolower(serialBuffer[0]))
    {
      case 'd':
        powerSupplyVoltages[0] = 5;
        powerSupplyVoltages[1] = 5;
        powerSupplyVoltages[2] = 0;
        powerSupplyVoltages[3] = 0;

        powerSupply(1,0,powerSupplyVoltages[0]);
        powerSupply(1,1,powerSupplyVoltages[1]);
        //powerSupply(1,2,powerSupplyVoltages[2]);
        powerSupply(1,3,powerSupplyVoltages[3]);

        break;
      
      case 't':
        if (tolower(serialBuffer[1])=='p')
        {
          
            Serial.println("Enter voltage for Top Positive rail (0 - 8.0)");
            
            while (!Serial.available());
            enteredVal = Serial.parseFloat();

            if (enteredVal >= 0.0 && enteredVal <= 5.0) {
              powerSupplyVoltages[0] = enteredVal;
              powerSupplyVoltages[2] = 0;
              powerSupply(1,0,powerSupplyVoltages[0]);
              goto tryagain;
            } else if (enteredVal >= 5.0 && enteredVal <= 8.0){           
              powerSupplyVoltages[2] = enteredVal;
              powerSupplyVoltages[0] = 0;
               powerSupply(1,2,powerSupplyVoltages[2]);
               goto tryagain;
              } else {
                Serial.println ("Please enter a voltage between 0 - 8.0");
                goto tryagain;
              }

              

          if (tolower(serialBuffer[1])=='n')
        {
            Serial.println("Top Negative rail is currently tied to Gnd (change jumpers to get negative voltages)");
        goto tryagain;
        }     
        }
        goto tryagain;
      case 'b':
              if (tolower(serialBuffer[1])=='p')
              {
            Serial.println("Enter voltage for Bottom Positive rail (0 - 5.0)");
            
            while (!Serial.available());
            enteredVal = Serial.parseFloat();

            if (enteredVal >= 0.0 && enteredVal <= 5.0) {
              powerSupplyVoltages[1] = enteredVal;
              powerSupply(1,1,powerSupplyVoltages[1]);
              goto tryagain;
            
              } else {
                Serial.println ("Please enter a voltage between 0 - 5.0");
                goto tryagain;
              }
              }
              
              
              if (tolower(serialBuffer[1])=='n')
              {

            Serial.println("Enter voltage for Bottom Negative rail (0 - -8.0)");
            
            while (!Serial.available());
            enteredVal = Serial.parseFloat();

            if (abs(enteredVal) >= 0.0 && abs(enteredVal) <= 8.0) {
              powerSupplyVoltages[3] =  0 - abs(enteredVal);
              powerSupply(1,3,powerSupplyVoltages[3]);
              goto tryagain;
              } else {
                Serial.println ("Please enter a voltage between 0 - -8.0");
                goto tryagain;
              }
              }
              goto tryagain;
              
      case 'm':
      stayInConnectionMenu = 0;
        return;
      case 'f':
      stayInConnectionMenu = -1;
        return;  
      default:
        goto tryagain;


    }

       // powerSupply(1,0,powerSupplyVoltages[0]);
       /// powerSupply(1,1,powerSupplyVoltages[1]);
       // powerSupply(1,2,powerSupplyVoltages[2]);
       // powerSupply(1,3,powerSupplyVoltages[3]);
}

void getX(void)
{ //this is just to get values from the serial monitor (which isnt a particularly nice way to set the switches)
  //                  dealing with inputs to the serial monitor is almost always a shit show, as you will see below
  Serial.println(" ");
  if (dataLine == 0)
  {
    Serial.print("Disconnect ");
  }
  else
  {
    Serial.print("Connect ");
  }
  Serial.println("rows");

  clearBuffers();

  int shift = 0;

  while (!Serial.available())
    ; //this just keeps looping if there's nothing in the serial buffer, so it waits for you to send some data

  Serial.readBytesUntil('\n', serialBuffer, bufLength); //reads chars one at a time and puts them into the array serialBuffer
  //                                                        until it reads an \n or it reaches the end (bufLength)
  if (isDigit(serialBuffer[0]) != true)
  {

    switch (tolower(serialBuffer[0]))
    {
    case 'm':
      Serial.println("\n");
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
      Serial.println("Cleared!\n");
      break;

    case 't':
      break;

    case 'b':
      break;

    default:
      Serial.println("\nPlease enter a number");
      return;
    }
  }

  comma = -1;
  int bottomX = 0;
  int bottomY = 0;
  int pwr = 0;
  int gnd = 0;

  for (int i = shift; i < bufLength; i++)
  { //this loop lets is enter values as "X,Y" instead of one at a time (it was super annoying to do it that way when testing)
    //                                         by taking one serialBuffer and then splitting it into separate X and Y serialBuffers

    if (serialBuffer[i] == ',')
    { //loops through serialBuffer[] to look for a comma and sets "comma" to the position in the array
      comma = i;

      i++;
    }

    if (serialBuffer[i] == 'b')
    { //loops through serialBuffer[] to look for a bottom row
      if (comma == -1)
      {
        bottomX = 1;
      }
      else
      {
        bottomY = 1;
      }

      i++;
    }

    if (serialBuffer[i] == 'p')
    {
      pwr = 1;
      comma = -1;
    }
    if (serialBuffer[i] == 'g')
    {
      gnd = 1;
      comma = -1;
    }

    if (serialBuffer[i] == 't')
    {
      i++;
    }

    if (comma == -1)
    { //if it hasnt found a comma, then this part of the buffer is for the X address buffer

      serialBufferX[i] = serialBuffer[i];
    }
    else
    {

      serialBufferY[i] = serialBuffer[i];
    }
  }

  int incomingValueX = atoi(serialBufferX); //atoi converts a char array to an integer (Array TO Int)

  if (bottomX == 1)
  {
    incomingValueX += 30;
  }

  int incomingValueY = atoi(serialBufferY);

  if (bottomY == 1)
  {
    incomingValueY += 30;
  }

  if (pwr == 1 && gnd == 1)
  {
    Serial.println("Don't short power directly to ground!");
    return;
  }

  /*if (pwr == 1) {

    if (incomingValueX >= 8 && incomingValueX <= 39) {
      incomingValueY = 31;
    } else {
      incomingValueY = 63;
    }
    }

    if (gnd == 1) {

    if (incomingValueX >= 24 && incomingValueX <= 55) {
      incomingValueY = 32;
    } else {
      incomingValueY = 0;
    }
    }
  */

  if (incomingValueX >= Xsize)
  { //this checks to make sure the values you entered aren't above the maximum size specified in Xsize
    Serial.print("\nPlease enter a number for X between 0 and ");
    Serial.println(Xsize - 1);
    return;
  }
  if (incomingValueY >= Ysize && comma > 0)
  { //the logical AND makes it only check if we put in a Y address separated by a comma
    Serial.print("\nPlease enter a number for Y between 0 and ");
    Serial.println(Ysize - 1);
    return;
  }

  GbreadRow1 = incomingValueX; //now that we know our inputs are good, we can put them into Xaddr and Yaddr

  GbreadRow2 = incomingValueY;

  if (findPath(GbreadRow1, GbreadRow2) == -1)
  {
    return;
  }

  if (comma > 0)
  { //only prints a Y if you entered it here
    //Serial.print("Y = ");
    //Serial.println(Yaddr1);
    // Serial.println(' ');
  }

  if (comma < 0)
  {             //if a Y wasnt entered comma will still be -1, call getY() to ask for one
    comma = -1; //set the comma back to -1 for the next entry
  }
}

void setAddress(byte Xaddr, byte Yaddr, char chip)
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

  selectChip(chip); //datasheet says CS_ should be set while setting up address lines, not sure why though
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

  strobeItIn(); //we could probably do this without passing "chip" and just do all the strobing on both together,
  //                    but I don't necessarily want to make MT8816 ignore that many garbage signals with CS_ LOW
  //deselectChip();
}

void strobeItIn(void)
{

  if (dataLine == 0)
  {
    digitalWrite(DATA, LOW);
  }
  else
  {
    digitalWrite(DATA, HIGH);
  }

  //pinMode(STROBE, OUTPUT);
  delayMicroseconds(2);       //Hold time in the datasheet for the MT8816 says this only needs to be 10 nanoseconds
  digitalWrite(STROBE, HIGH); //but we're not super concerned with speed so I'll give it 1000X more just to be safe
  delayMicroseconds(5);       //Strobe has to be a minimum of 20 nanoseconds, but I dont want to think about the
  //Serial.println("!!!!!!!!!!!!");
  digitalWrite(STROBE, LOW); //fact that light only travels like 4 meters in that time through copper
  //pinMode(STROBE, OUTPUT);
  delayMicroseconds(2);
  //digitalWrite(DATA, LOW);
  //delayMicroseconds(30);
}

void selectChip(char chip)
{ //asserts whichever chip select line we send it

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

  delayMicroseconds(2);
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
}

void deselectChip(void)
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
  return;
}

void clearConnections(void)
{ //when you send a reset pulse, all previous connections are cleared on whichever chip is CS_ed but we'll do both for now

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
  delayMicroseconds(20); //datasheet says 40 nanoseconds minimum, this is a lot more than that
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

void clearBuffers(void)
{

  for (int i = 0; i < bufLength; i++)
  { //clears the serial buffers by filling them with spaces
    serialBuffer[i] = ' ';
    serialBufferX[i] = ' ';
    serialBufferY[i] = ' ';
  }

  /* while (Serial.available() > 0) {    //this is just a trick to clear the serial buffer (if we typed in a really long string it would just keep entering them one after another)
     Serial.print(Serial.read());                    //it reads the serial buffer and then throws away the values, just to clear it
     delay(100);
    }*/
  Serial.end(); //maybe it's just easier to stop and start the serial connection
  Serial.begin(115200);
}

char chipToChar(int chip)
{

  switch (chip)
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
  }
}

void showArrays(void)
{

  for (int i = 0; i < 8; i++)
  {

    Serial.print("Chip ");
    Serial.print(chipToChar(i));
    Serial.print("\t");

    for (int j = 0; j < 16; j++)
    {
      if (chipToChar(i) < matrixArrayX[i][j])
      {
        Serial.print(chipToChar(i));
        Serial.print(matrixArrayX[i][j]);
      }
      else
      {
        Serial.print(matrixArrayX[i][j]);
        Serial.print(chipToChar(i));
      }
      Serial.print(" ");
      Serial.print(!availableConnections[i][j]);
      Serial.print("\t");
    }
    Serial.println(" ");
  }
}

void showConnections(void)
{

  int chipOrder[9] = {0, 1, 2, 3, 7, 6, 5, 4};
  int count = 0;
  int countConn = 0;
  int countDig = 0;

  for (int i = 0; i < 8; i++)
  {

    (i == 4) ? Serial.println("") : Serial.print("");

    Serial.print("Chip ");
    Serial.print(chipToChar(chipOrder[i]));
    Serial.print("\t");

    for (int j = 0; j < 8; j++)
    {
      countDig = 0;
      countConn = 0;

      if (count <= 6 && count >= 0)
      { //this is a nightmare
        //Serial.print(" ");
        Serial.print(count + 1);
      }
      else if (count == 7)
      {
        Serial.print("0 *");
      }
      else if (count <= 38 && count >= 32)
      {
        Serial.print("\b"); //backspace
        Serial.print("b");
        Serial.print(count - 31);
      }
      else if (count == 39)
      {
        Serial.print("\bb0 *");
      }
      else if (count >= 32)
      {
        Serial.print("\b");
        Serial.print("b");
        Serial.print(count - 32);
      }
      else
      {
        /// Serial.print(" ");
        Serial.print(count);
      }

      if (count == 31 || count == 63)
      {

        Serial.print("*");
      }
      else if (count != 7 && count != 39)
      {
        Serial.print(" ");
      }

      if ((count <= 9 || (count >= 32 && count <= 41)) && count != 7 && count != 39)
        Serial.print(" ");

      count++;
      Serial.print(" ");
      if (connections[chipOrder[i]][j][0] == -1 && connections[chipOrder[i]][j][1] == -1 && connections[chipOrder[i]][j][2] == -1 && connections[chipOrder[i]][j][3] == -1)
      {
        Serial.print(".\t");
      }
      else
      {
        countDig = 0;
        for (int k = 0; k < 4; k++)
        {
          if (connections[chipOrder[i]][j][k] != -1)
          {

            if (countConn >= 1 && countConn < 4)
            {
              Serial.print(',');

              countDig++;
            }
            countConn++;

            //Serial.print(connections[chipOrder[i]][j][k]);

            if (connections[chipOrder[i]][j][k] >= 32)
            {
              Serial.print("b");
              Serial.print(connections[chipOrder[i]][j][k] - 30);
              // if ((connections[chipOrder[i]][j][k] - 30) > 9) {
              //  countDig++;
              //  }
            }
            else
            {
              //Serial.print(" ");
              Serial.print(connections[chipOrder[i]][j][k]);
            }

            if (connections[chipOrder[i]][j][k] > 9)
            {
              countDig++;
            }
            countDig++;
          }
        }
        if (countDig <= 3)
        {
          Serial.print("\t");
          //Serial.print(countDig);
          countDig = 0;
        }
      }
      Serial.print("\t");
    }
    //if (count == 32) Serial.print ("\n");
    Serial.println(" ");
  }
}
