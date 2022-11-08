#include <Arduino.h>
#include "BoardDefinitions.h"
#include <Wire.h>
#ifndef MENUS_H
#include "menus.h"
#endif
#include "jMatrix.h"
#include "xTerm.h"


#include <Adafruit_MCP4728.h>
#include "MCP466_DigitalPot.h"

#define CSI Serial.write("\x1B\x5B");
#define TOPCURSOR 1

Adafruit_MCP4728 mcp;
xTerm x;
jMatrix j;

int deletedNumRows = 0;

// jConnection first(15,35);
// jConnection second(3,45);
// jConnection third(11,58);

void setup()
{

  Serial0.begin(115200);
  Wire.begin();

  // Serial.begin(115200);

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

  pinMode(RESET, OUTPUT);    // all of this just sets up all these pins as outputs and drives them LOW
  digitalWrite(RESET, HIGH); // I'm pretty sure Arduino IDE does this automatically but I wouldn't count on it
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

  pinMode(DATAPIN, OUTPUT);
  // digitalWrite(DATAPIN, HIGH);
  pinMode(STROBE, OUTPUT);
  // digitalWrite(STROBE, LOW);

  DAC0.CTRLA |= (DAC_OUTEN_bm | DAC_ENABLE_bm);
  DAC0.DATA = (512 << 6);

  mcp.begin(0x61);
  mcp.setChannelValue(MCP4728_CHANNEL_A, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K, false);
  mcp.setChannelValue(MCP4728_CHANNEL_B, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K, false);
  mcp.setChannelValue(MCP4728_CHANNEL_C, 0, MCP4728_VREF_VDD, MCP4728_GAIN_1X, MCP4728_PD_MODE_GND_1K, false);
  /*
  for(int i = 0; i <8;i++)
  {
    jchip[i].letter = (i+0x41);
  }
  */
  j.clearConnections();
  x.clear();
}

void loop()
{

startFresh:
  Serial.write("\x1B(B"); /// technical drawing set off
  x.splashScreen();
  x.currentMenu = 0;
  x.showMenu(x.currentMenu);

  int selectedMenuItem = 0;
  while (1)
  {
    selectedMenuItem = x.getInput();

    if (selectedMenuItem > 0)
    {
      switch (x.currentMenu)
      {
      case 0:

        switch (selectedMenuItem)
        {
        case 3:
          x.printPalette();
          goto startFresh;
          break;

        case CONNECTNODES:
          x.currentMenu = 3;
          connectNodesScreen();
          break;

        case SPECIALFUNCTIONSMENU:
          x.currentMenu = 1;
          x.showMenu(x.currentMenu);
          x.highlightMenu(1);
          break;
        }
        break;

      case 1:
        switch (selectedMenuItem)
        {
        case BACKTOMENU:
          x.currentMenu = 0;
          x.showMenu(x.currentMenu);
          x.highlightMenu(1);
          break;
        }
        break;
      }

      selectedMenuItem = 0;
    }
  }
}

void connectNodesScreen(void)
{

  x.showMenu(3, -1);
  x.tempMoveCursor(22, 42);
  x.getCursorPosition();
   x.drawBBarrow();
connectagain:

  int bbInput = -1;
  int netInput = -1;
//x.tempMoveCursor(22, 42);
  x.getCursorPosition();
  if (x.yCursor < 14)
    x.yCursor = 14;
    //x.getCursorPosition();
    x.drawBBarrow();
  x.setNetColor(0);
  while (bbInput <= 0)
  {
    bbInput = x.getBreadboardInput();

    if (bbInput == 1 || bbInput == 33 || bbInput == 30 || bbInput == 62)
    {
      bbInput = 0 - bbInput;
    }
    if (bbInput == -100)
    {
      return;
    }
  }

  if (bbInput > 100) // entered 'v'
  {
    x.setForegroundColor(180, 180, 180);
    x.showMenu(3, -2);
    netInput = -1;
    x.tempMoveCursor(4, 80);
    x.getCursorPosition();
    while (Serial.available())
      Serial.read();

    while (netInput > -80)
    {
      netInput = -1;

      while (netInput <= 0)
      {
        netInput = x.getNetInput();
      }

      if (netInput == 200) // entered 'c'
      {
        //x.tempMoveCursor(-1, -1);
        
        redrawNetList();
        x.tempMoveCursor(22, 42);
        bbInput = -1;
        deletedNumRows = 0;
        x.setForegroundColor(180, 180, 180);
        x.showMenu(3, -1);
        x.tempMoveCursor(22, 42);
        goto connectagain;
      }

      x.tempMoveCursor(31, 80);

 x.net--;
      j.disconnect(netInput);
deletedNumRows ++;
      
         x.colorColumn(j.conn[netInput].row1,0,netInput);
    x.tempMoveCursor(-1, -1);
       x.colorColumn(j.conn[netInput].row2,0,netInput);
    x.tempMoveCursor(-1, -1);
      

     // x.highlightColumn(j.conn[netInput].row1, 0, netInput);
      ///x.highlightColumn(j.conn[netInput].row2, 0, netInput);

    }
  }
  x.colorColumn(bbInput,1,x.net);
  //x.highlightColumn(bbInput,0,x.net);
  x.tempMoveCursor(15, 43);
  x.setNetColor(netColor[x.net]);
  Serial.printf("%d     ", bbInput);

  int row1Int = bbInput;

  int bbInput2 = -1;
  x.tempMoveCursor(-1, -1);
  x.getCursorPosition();

  while (bbInput2 <= 0)
  {
    bbInput2 = x.getBreadboardInput();
    if (bbInput2 == 1 || bbInput2 == 33 || bbInput2 == 30 || bbInput2 == 62 || bbInput2 == bbInput)
    {
      bbInput2 = 0 - bbInput2;
    }
    if (bbInput == -100)
    {
      return;
    }
  }
  int row2Int = bbInput2;

  x.tempMoveCursor(35, 10);
  if (j.connect(row1Int, row2Int) <= 0)
  {
    x.tempMoveCursor(-1, -1);
    x.colorColumn(row1Int,0,x.net-2);
    x.tempMoveCursor(-1, -1);
   // x.highlightColumn(row1Int, 1);
    // Serial.print("can't connect");
  }
  else
  {
   x.colorColumn(row2Int,1,x.net);
    x.tempMoveCursor(-1, -1);
    //x.highlightColumn(bbInput2,0,x.net);
    x.tempMoveCursor(16, 43);
    x.setNetColor(netColor[x.net]);
    Serial.printf("%d     ", bbInput2);

    drawNetList(x.net, row1Int, row2Int);

    x.net++;
  }
  goto connectagain;
}

void redrawNetList(void) // do I change the net colors on the board or the netcolor order?
{
  int newNumberOfConnections = 1;
x.getCursorPosition();
  // Serial.printf("\n\n\n\n\n\n\r");
  // for (int i = 1; i <= j.numberOfConnections; i++)
  //{

  // Serial.printf("Net %d\t%d,%d\n\r",i,j.conn[i].row1,j.conn[i].row2);

  ///}
  //x.net = j.numberOfConnections;
  syncNet();


   
  for (int i = 0; i <= j.numberOfConnections + deletedNumRows + 3; i++)
  {
      x.colorColumn(j.conn[i].row1, 0, i);
      x.colorColumn(j.conn[i].row2, 0, i);
    if (j.conn[i].used == 1)
    { 
      j.conn[newNumberOfConnections] = j.conn[i];

      
      newNumberOfConnections++;
    }

  }
//newNumberOfConnections++;

  for (int k = 0; k <= (deletedNumRows + 3); k++)
  {
    j.conn[j.numberOfConnections + k].used = 0;
  }

  //j.numberOfConnections = newNumberOfConnections;
//x.tempMoveCursor(4 , 81);
//x.getCursorPosition();

  for (int i = 1; i <= (j.numberOfConnections + deletedNumRows +3); i++)
  {

    // Serial.printf("Net %d\t%d,%d\t%d\n\r",i,j.conn[i].row1,j.conn[i].row2,j.conn[i].used);
    
    if (j.conn[i].used == 1)
    {
      x.colorColumn(j.conn[i].row2, 1, i+2);
      x.colorColumn(j.conn[i].row1, 1, i+2);
      drawNetList(i+2 , j.conn[i].row1, j.conn[i].row2);
      //Serial.print(i);

    }
    else
    {
      //

    //Serial.write("\x1B\x5B 20D");
      //Serial.write("\x1B\x5B B");
      x.tempMoveCursor(3 + i, 81);
      Serial.print("                    ");
      //Serial.print(newNumberOfConnections);
    }
  }
 //x.net = j.numberOfConnections;
}

void drawNetList(int netDraw, int row1Int, int row2Int)
{


//Serial.print(j.conn[net-2].used);
  x.setNetColor(netColor[netDraw]);
  x.tempMoveCursor(1 + netDraw, 81);
  Serial.printf("Net %d\t", netDraw );
  Serial.write("\x1B(0"); /// technical drawing set
  Serial.print('\x61');

  Serial.write("\x1B(B"); /// technical drawing set off
  Serial.printf(" %d,%d \t ", row1Int, row2Int);

  x.tempMoveCursor(-1, -1);
}

void syncNet (void)
{

  int newNet = 0;
for(int i = 0; i < 100; i++)
{

if (j.conn[i].used == 1)
{
newNet++;

}




}

x.net = newNet + 3;
j.numberOfConnections = newNet + 1;



}