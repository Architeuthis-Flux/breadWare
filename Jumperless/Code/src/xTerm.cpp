#ifndef xTerm_h
#define xTerm_h
#include "xTerm.h"
#include <Arduino.h>

#include "menus.h"

#define MENUOFFSET 10



char asciiCursor[4] = {0, 0, 0, 0};

int8_t selectedColumn[60] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int selectionLength = 0;

int net = 3;

uint8_t highlightedMenuItem[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

int inBounds = 0;
int menuPos = 0;
int menuSelectedFlag = 0;

xTerm::xTerm(void)
{
}

void xTerm::clear(void)
{
  Serial.write("\e G");
  Serial.write("\x1B\x5B 2J");
}

int xTerm::getInput(void)
{

  if (Serial.available())
  {
    char input = Serial.read();

    switch (input)
    {
    case 'w':

      if (currentMenu != 3)
        highlightMenu(0);
      Serial.write("\x1B\x5B A");

      if (currentMenu != 3)
        highlightMenu(1);
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0;

    case 'a':
      Serial.write("\x1B\x5B D");
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0;

    case 's':

      if (currentMenu != 3)
        highlightMenu(0);
      Serial.write("\x1B\x5B B");

      if (currentMenu != 3)
        highlightMenu(1);
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0;

    case 'd':
      Serial.write("\x1B\x5B C");

#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0;

    case '\t':
      Serial.write("\x1B\x5B I");
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0;

    case '\r':
      // Serial.write("\x1B\x5B I");
      getCursorPosition();
      if (currentMenu != 3)
        highlightMenu(2);
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor - MENUOFFSET);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return xCursor - MENUOFFSET + 1;

    case 'p':
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      // tempMoveCursor(1, 1);
      getCursorPosition();
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      // tempMoveCursor(-1,-1);
      return 0;

    default:
      // Serial.print(input);
      CSI
          Serial.write(input);
      return -1;
    }
  }
  else
  {
    return;
  }
}

int xTerm::getBreadboardInput(void)
{

  if (Serial.available())
  {
    // getCursorPosition();
    int cursorBBlocation = 1;

    cursorBBlocation = ((yCursor - 14) / 2);

    char input = Serial.read();

    if (input == '\x1B')
    {
      char buffer[5] = {0, 0, 0, 0, 0};

      Serial.readBytes(buffer, 5);
      for (int i = 0; i < 5; i++)
      {

        if (buffer[i] == 'A')
        {
          input = 'w';
           break;
        }
        else if (buffer[i] == 'B')
        {
          input = 's';
           break;
        }
        else if (buffer[i] == 'C')
        {
          input = 'd';
           break;
        }
        else if (buffer[i] == 'D')
        {
          input = 'a';
           break;
        }
      }
      //while (Serial.available())
        //Serial.read();
    }

    switch (input)
    {
    case 'w':

      if (getCursorXPosition() >= 22&& getCursorYPosition() <= 72 && getCursorYPosition() >= 14)
      {
        Serial.write("\x1B\x5B 5A");
        getCursorPosition();
   
       
        yCursor = yCursor / 2;
        yCursor = yCursor * 2;
drawBBarrow();
      }
      else
      {
        Serial.write("\x1B\x5B A");
      }

#ifdef TOPCURSOR
      getCursorPosition();
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0 - cursorBBlocation;

    case 'a':

      if (getCursorXPosition() >= 22 )
      {
        Serial.write("\x1B\x5B 2D");
        getCursorPosition();
      
       
        yCursor = yCursor / 2;
        yCursor = yCursor * 2;
        if(yCursor < 14) yCursor = 14;
drawBBarrow();
      }
      else
      {
        Serial.write("\x1B\x5B D");
      }

#ifdef TOPCURSOR
      getCursorPosition();
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0 - cursorBBlocation;

    case 's':

      // highlightColumn(1);
      if (getCursorXPosition() >= 22 && getCursorXPosition() <= 25)
      {
        Serial.write("\x1B\x5B 5B");
        getCursorPosition();

          
        yCursor = yCursor / 2;
        yCursor = yCursor * 2;
drawBBarrow();
        if (xCursor > 26) xCursor = 27;
       
      } else if (getCursorXPosition() > 30)
      {

      }
      else if (getCursorXPosition() < 22)
      {
        Serial.write("\x1B\x5B B");
      }
      // surroundColumn(0);

#ifdef TOPCURSOR
      getCursorPosition();
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0 - cursorBBlocation;

    case 'd':
      // highlightColumn(1);
      if (getCursorXPosition() >= 22)
      {
        Serial.write("\x1B\x5B 2C");
        getCursorPosition();

       
        yCursor = yCursor / 2;
        yCursor = yCursor * 2;
        if(yCursor > 72) yCursor = 72;
drawBBarrow();
      }
      else
      {
        Serial.write("\x1B\x5B C");
      }
      // Serial.write("\x1B\x5B C");

#ifdef TOPCURSOR
      getCursorPosition();
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0 - cursorBBlocation;

    case '\t':
      Serial.write("\x1B\x5B I");
#ifdef TOPCURSOR
      getCursorPosition();
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return 0 - cursorBBlocation;

    case '\r':
      // Serial.write("\x1B\x5B I");
      getCursorPosition();
      yCursor = yCursor / 2;
      yCursor = yCursor * 2;
      tempMoveCursor(-1, -1);
      if (xCursor > 26 && yCursor > 12)
      {
        cursorBBlocation = ((yCursor - 12) / 2) + 32;
      }
      else
      {
        cursorBBlocation = ((yCursor - 12) / 2);
      }
      // highlightColumn(cursorBBlocation);
      drawBBarrow();
/*
      tempMoveCursor(26, 0);
        Serial.print("                                                                      ");
      tempMoveCursor(26, yCursor);
      if (xCursor < 26)
      {
        Serial.print("^");
      }
      else
      {
        Serial.print("v");
      }

      tempMoveCursor(-1, -1);
*/
      // cursorBBlocation =

#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor - MENUOFFSET);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      return cursorBBlocation;

    case 'c':
      net++;
      return 0 - cursorBBlocation;

          case 'm':
      net++;
      return -100;

    case 'p':
#ifdef TOPCURSOR
      tempMoveCursor(1, 1);
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      tempMoveCursor(-1, -1);
#endif
      // tempMoveCursor(1, 1);
      getCursorPosition();
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      // tempMoveCursor(-1,-1);
      return 0 - cursorBBlocation;

    case 'v':
      // Serial.write("\x1B\x5B I");
      getCursorPosition();
      yCursor = yCursor / 2;
      yCursor = yCursor * 2;
      tempMoveCursor(-1, -1);
      if (xCursor > 26 && yCursor > 12)
      {
        cursorBBlocation = ((yCursor - 12) / 2) + 32;
      }
      else
      {
        cursorBBlocation = ((yCursor - 12) / 2);
      }
      // highlightColumn(cursorBBlocation);


      return cursorBBlocation + 100;

    default:
      //CSI
          //Serial.write(input);
      return 0 - cursorBBlocation;
    }
  }
}

int xTerm::getNetInput(void)
{
  
 
  if (Serial.available())
  {
     //getCursorPosition();
   int cursorNetlocation = 1;

    //cursorNetlocation = (xCursor + 3);
//Serial.read();
    char input2 = Serial.read();
  

    if (input2 == '\x1B')
    {
      char buffer2[5] = {0, 0, 0, 0, 0};

      Serial.readBytes(buffer2, 5);
      for (int i = 0; i < 5; i++)
      {

        if (buffer2[i] == 'A')
        {
          input2 = 'w';
           break;
        }
        else if (buffer2[i] == 'B')
        {
          input2 = 's';
           break;
        }
        else if (buffer2[i] == 'C')
        {
          input2 = 'd';
           break;
        }
        else if (buffer2[i] == 'D')
        {
          input2 = 'a';
           break;
        }
      }
      while (Serial.available())
        Serial.read();
    }

    
//Serial.print(input2);

    switch (input2)
    {
    case 'w':
Serial.write("\x1B\x5B D");
      Serial.print (' ');

        Serial.write("\x1B\x5B A");
        Serial.write("\x1B\x5B D");
      Serial.print ('>');

      return 0 - cursorNetlocation;

    

    case 's':

     
Serial.write("\x1B\x5B D");
      Serial.print (' ');

        Serial.write("\x1B\x5B B");
        Serial.write("\x1B\x5B D");
      Serial.print ('>');

      return 0 - cursorNetlocation;

    

    case '\r':
     
      getCursorPosition();
        
        cursorNetlocation = xCursor - 3;
        Serial.write("\x1B\x5B D");
        Serial.print("                  ");

      return cursorNetlocation;

    case 'c':
      //net++;
      getCursorPosition();
      Serial.write("\x1B\x5B D");
      Serial.print(" ");
      return 200;

    case 'm':
    Serial.write("\x1B\x5B D");
    Serial.print(" ");
      //net++;
      return 100;

    case 'p':

     
      getCursorPosition();
      Serial.print(xCursor);
      Serial.print(',');
      Serial.print(yCursor);
      Serial.print(' ');
      // tempMoveCursor(-1,-1);
      return 0 - cursorNetlocation;



    default:
      //CSI
         // Serial.write(input);
      return -1;
    }
  } else {
    //Serial.read();
    return -1;
  }
}
void xTerm::drawBBarrow(void)
{
      tempMoveCursor(26, 0);
        Serial.print("                                                                      ");
      tempMoveCursor(26, yCursor);
      if (xCursor < 26)
      {
        Serial.print("^");
      }
      else
      {
        Serial.print("v");
      }

      tempMoveCursor(-1, -1);

}
void xTerm::checkSelectedColumns(void)
{

  for (int i = 0; i < selectionLength; i++)
  {
  }
}

void xTerm::colorColumn (int column, int addRemove, int net1 = 0)
{
    int topBottom = 2;
    int tempColumnColors[4]= {2,2,2,2};
    int uniqueColumnColors[4]= {-1,-1,-1,-1};
    int uniqueNets = 0;
tempMoveCursor (1,1);
  if (column == 1 || (column >= 30 && column <= 33) || column >= 62)
    return;

  if (column >= 33)
      {
        column = column - 32;
        topBottom = 7;
      }
      Serial.print("Column = ");
Serial.print(column);
      Serial.print("\tNet = ");
Serial.println(net1);

//Serial.print("tempColumnColors \t");
  for (int i = 0; i < 4; i++) // load array (if were removing don't load revoved net)
  {
    if (addRemove == 0)
    {
      if (breadboardNetColors[topBottom + i][column - 1] != net1+2 && breadboardNetColors[topBottom + i][column - 1] != 2 && breadboardNetColors[topBottom + i][column - 1] < net)
      {
        tempColumnColors[i] = breadboardNetColors[topBottom + i][column - 1];
      } else {
        tempColumnColors[i] = -2;
      }
    } else {

      tempColumnColors[i] = breadboardNetColors[topBottom + i][column - 1];


    }

    

//Serial.print(tempColumnColors[i]);
//Serial.print(",");

  }

    int temp;
  for (int i = 0; i < 4; i++) // sort
  {
   
    for(int k = i+1; k<4;k++)
    {
      if (tempColumnColors[i]>tempColumnColors[k] )
      {
          temp = tempColumnColors[i];
          tempColumnColors[i] = tempColumnColors[k];
          tempColumnColors[k] = temp;
      }
		
    }

  }
  //Serial.println(" ");

//uniqueColumnColors [0] = tempColumnColors [0];

int uniqueIndex = 0;

  for (int i = 0; i < 4; i++) // get unique values
  {
      if(tempColumnColors[i] != uniqueColumnColors[uniqueIndex-1] && tempColumnColors[i] != -2&& tempColumnColors[i] != 2)
        {
          uniqueNets++;
          uniqueColumnColors[uniqueIndex] = tempColumnColors[i];

          uniqueIndex++;

        } else {

        }

  }

 //Serial.print("uniqueColumnColors \t");
  //for (int i = 0; i < 4; i++) // load array
  //{
    
//Serial.print(uniqueColumnColors[i]);
//Serial.print(",");

 // }
 // Serial.println(" ");
//Serial.print("uniqueNets = ");
//Serial.print(uniqueNets);
//Serial.println(" ");


if (uniqueNets == 4 && addRemove == 1)
{
    uniqueColumnColors[0] = net1;
} else if (addRemove == 1) {

uniqueColumnColors[uniqueNets] = net1;
uniqueNets++;

}


switch (uniqueNets)
{
  case 0:
  tempColumnColors[0] = 2;
  tempColumnColors[1] = 2;
  tempColumnColors[2] = 2;
  tempColumnColors[3] = 2;
  break;

  case 1:
  tempColumnColors[0] = uniqueColumnColors[0];
  tempColumnColors[1] = uniqueColumnColors[0];
  tempColumnColors[2] = uniqueColumnColors[0];
  tempColumnColors[3] = uniqueColumnColors[0];
  break;

  case 2:
  tempColumnColors[0] = uniqueColumnColors[0];
  tempColumnColors[1] = uniqueColumnColors[0];
  tempColumnColors[2] = uniqueColumnColors[1];
  tempColumnColors[3] = uniqueColumnColors[1];
  break;

  case 3:
  tempColumnColors[0] = uniqueColumnColors[0];
  tempColumnColors[1] = uniqueColumnColors[1];
  tempColumnColors[2] = uniqueColumnColors[2];
  tempColumnColors[3] = uniqueColumnColors[2];
  break;

  case 4:
  tempColumnColors[0] = uniqueColumnColors[0];
  tempColumnColors[1] = uniqueColumnColors[1];
  tempColumnColors[2] = uniqueColumnColors[2];
  tempColumnColors[3] = uniqueColumnColors[3];
  break;

}
 //Serial.print("newColumnColors \t");
  //for (int i = 0; i < 4; i++) // load array
  //{
    
//Serial.print(tempColumnColors[i]);
///Serial.print(",");

 // }

      for (int i = 0; i < 4; i++)
    {
      breadboardNetColors[topBottom + i][column - 1] = tempColumnColors[i];
    }
showMenu(3, column);

}

void xTerm::highlightColumn(int column, int undo = 0, int net = 2)
{

  int numberOfNetsConnected = 0;
  //int connectedNets[4] = {-1, -1, -1, -1};
  int lastNet = 2;

  int tempColumnColors[4]= {2,2,2,2};
  int otherNets[4]= {-1,-1,-1,-1};
  int otherIndex = 0;

  
  static int historyNet;
  static int historyColumn;

  int topBottom = 2;
  int tempNet = net; // so we can undo if we want

int uniqueNets = 1;


  if (column == 1 || (column >= 30 && column <= 33) || column >= 62)
    return;

  if (column >= 33)
  {
    column = column - 32;
    topBottom = 7;
  }

  for (int i = 0; i < 4; i++)
  {
    tempColumnColors[i] = breadboardNetColors[topBottom + i][column - 1];
  }





   for (int i = 0; i < 3; i++) // count unique nets
  {
    if (tempColumnColors[i] != tempColumnColors[i+1])
    {
      uniqueNets++;
    }
  }

if (undo == 1) //undo
  {
    int tempOther = otherIndex;

      for (int i = 0; i < 4; i++)
        {
          
          if( tempColumnColors[i] == net)
          {
              for (int k = i; k < 4;k++)
              {
                if( tempColumnColors[k] != 2 && tempColumnColors[k] != net)
                {
                  tempColumnColors[i] = tempColumnColors[k];
                  break;
                }

              }
              
              
            }
            
          }
        
    uniqueNets -= 1;
}

  if (tempColumnColors[0] == 2 && tempColumnColors[1] == 2 && tempColumnColors[2] == 2 && tempColumnColors[3] == 2 )
  {
    uniqueNets = 0;
  }
 // } else {

  //uniqueNets = 1;

  //}
//Serial.print(uniqueNets);
//Serial.print(uniqueNets);
//Serial.print(uniqueNets);

if (undo == 0){


  switch(uniqueNets)
  {
    case 0:
      for (int i = 0; i < 4; i++)
        {
          tempColumnColors[i] = net;
        }
      break;
    case 1:
      for (int i = 2; i < 4; i++)
        {
          tempColumnColors[i] = net;
        }
      break;
    case 2:
      for (int i = 3; i < 4; i++)
        {
          tempColumnColors[i] = net;
        }
      break;
    case 3:

          tempColumnColors[0] = net;
        
      break;


  }

  int temp;
  for (int i = 0; i < 4; i++) // sort
  {
   
    for(int k = i+1; k<4;k++)
    {
      if (tempColumnColors[i]>tempColumnColors[k] )
      {
          temp = tempColumnColors[i];
          tempColumnColors[i] = tempColumnColors[k];
          tempColumnColors[k] = temp;
      }
		
    }

  }
}
//Serial.print(uniqueNets);



/*
  for (int j = 0; j < 4; j++)
  {
    if (breadboardNetColors[topBottom + j][column - 1] != 2 && breadboardNetColors[topBottom + j][column - 1] != lastNet)
    {
      if (undo == 1 && breadboardNetColors[topBottom + j][column - 1] == historyNet)
      {
        breadboardNetColors[topBottom + j][column - 1] = breadboardNetColors[topBottom + j-1][column - 1]; //unconnected color
        //Serial.print(historyColumn);
      } else {
        
      numberOfNetsConnected++;
       //Serial.print(numberOfNetsConnected);
      }
     // if (breadboardNetColors[topBottom + j][column - 1] == netToRemove)
      //{
     //   breadboardNetColors[topBottom + j][column - 1] = 2;
     // }
    }
    //connectedNets[j] = breadboardNetColors[topBottom + j][column - 1];
    lastNet = breadboardNetColors[topBottom + j][column - 1];
  }
if (undo >= 1) numberOfNetsConnected--;//this is a hack

  for (int i = 0; i < 4; i++)
  {
  Serial.print(tempColumnColors[i]);
  Serial.print(' ');
  }
*/

    for (int i = 0; i < 4; i++)
    {
      breadboardNetColors[topBottom + i][column - 1] = tempColumnColors[i];
    }
   

historyNet = net;
historyColumn = column;
  // net++;

  showMenu(3, column);
}

void xTerm::surroundColumn(int setUnset)
{
  static int lastSurroundedX = -1;
  static int lastSurroundedY = -1;

  int setUnsetFlag = 0;

  getCursorPosition();
  if (yCursor == 1 || yCursor > 91)
    return;
  yCursor = yCursor / 3;
  yCursor = yCursor * 3;
  tempMoveCursor(-1, yCursor);

  if (lastSurroundedX == -1 && lastSurroundedY == -1)
  {
    lastSurroundedX = xCursor;
    lastSurroundedY = yCursor;
  }

  // xCursor = lastSurroundedX;
  // yCursor = lastSurroundedY;
  setForegroundColor(0, 0, 0);

  Serial.write("\x1B(0");

  if (xCursor < 16)
  {
    CSI
        Serial.write("12d");
  }
  else
  {
    CSI
        Serial.write("17d");
  }
surround:
  // tempMoveCursor(tempXcursor,tempYcursor);

  CSI
      Serial.write("D"); // back cursor
  for (int i = 0; i < 4; i++)
  {
    Serial.print('\x61');

    CSI
        Serial.write("D"); // back cursor
    CSI
        Serial.write("B");
  }
  CSI
      Serial.write("4A");
  CSI
      Serial.write("3C");
  for (int i = 0; i < 4; i++)
  {
    Serial.print('\x61');

    CSI
        Serial.write("D"); // back cursor
    CSI
        Serial.write("B");
  }
  CSI
      Serial.write("4A");
  CSI
      Serial.write("2D");

  if (setUnsetFlag == 0)
  {
    setUnsetFlag = 1;
    setForegroundColor(32, 32, 32);

    lastSurroundedX = xCursor;
    lastSurroundedY = yCursor;

    Serial.print(xCursor);
    Serial.print(yCursor);
    goto surround;
  }

  Serial.write("\x1B(B");
}

void xTerm::showMenu(int menuNumber, int column = 0)
{
  static int BBalreadyDrawn;
  switch (menuNumber)
  {
    getCursorPosition();
  case 0:
  BBalreadyDrawn = 0;
    for (int i = 0; i < 7; i++)
    {

      tempMoveCursor(MENUOFFSET + i, 9);

      Serial.print(mainMenu[i]);
    }
    tempMoveCursor(10, 12);
    highlightMenu(1);
    break;

  case 1:
  BBalreadyDrawn = 0;
    for (int i = 0; i < 7; i++)
    {
      tempMoveCursor(MENUOFFSET + i, 9);
      Serial.print(SpecialFunctionsMenu[i]);
    }
    highlightMenu(1);
    tempMoveCursor(10, 12);
    break;

  case 3:
  
if (column < 0){
  

column = abs(column);
//Serial.print("!!!!!!!!!!");

    tempMoveCursor(3, 83);
    Serial.print("Connections");
    for (int i = 0; i < 7; i++)
    {
      tempMoveCursor(MENUOFFSET + i, 9);
      if (column-1 ==  i)
      {
        setBackgroundColor(100,28,0);
      } else {
      setBackgroundColor(0,0,0);
      }
      int backFlag = 0;
      for (int j = 0; j < 50; j++)
      {

        if(connectNodesMenu[i][j] == ' ' && backFlag == 0)
        {

        setBackgroundColor(0,0,0);
        backFlag = 1;
        }
        Serial.print(connectNodesMenu[i][j]);

      }
      
    }
    column = 0;
}


    tempMoveCursor(20, 12);
    Serial.write("\x1B(0"); /// technical drawing set
    Serial.print('\r');
    // setNetColor(2);
    int lastNetColor = -1;
    if (BBalreadyDrawn != 1)
    {

      for (int i = 0; i < 13; i++)
      {
        Serial.print("\t     ");
        // x.showMenu(0);
        for (int j = 0; j < 70; j++)
        {

          if (lastNetColor != breadboardNetColors[i][j / 2])
          {

            setNetColor(breadboardNetColors[i][j / 2]);

            lastNetColor = breadboardNetColors[i][j / 2];
          }
          if ((j + 2) % 10 == 0 && j > 2 && j < 58)
          {
            setNetColor(31);
            lastNetColor = 31;
            // Serial.print('!');
          }

          if (breadBoard[i][j] == '#')
          {
            Serial.print('\x61');
            //Serial.print(net);
          }
          else
          {
            Serial.print(breadBoard[i][j]);
          }
        }
        Serial.println(" ");
      }
    }
    else
    {
      if (column > 32)
      {
        column = column - 32;
      }
      int colLoc = column;
      colLoc = (colLoc * 2) + 12;
      //getCursorPosition();
      tempMoveCursor(20, colLoc);
      for (int i = 0; i < 11; i++)
      {
          if (i != 6) {
        // x.showMenu(0);

        if (lastNetColor != breadboardNetColors[i][column - 1])
        {
          setNetColor(netColor[(breadboardNetColors[i][column - 1])]);
          lastNetColor = breadboardNetColors[i][column - 1];
        }
         

        if (breadBoard[i][(column - 1) * 2] == '#')
        {
          Serial.print('\x61');
           //Serial.print(breadboardNetColors[i][column-1]);
        }
        else
        {
          Serial.print(breadBoard[i][(column - 1) * 2]);
        }
        Serial.write("\x1B\x5B B");
        Serial.write("\x1B\x5B D");
        
        // if (breadboardNetColors[i][column-1] >= 10) Serial.write("\x1B\x5B D");
      } else {
         Serial.write("\x1B\x5B B");
      }
      }
      tempMoveCursor(-1,-1);
    }

    Serial.write("\x1B(B"); /// technical drawing set off
BBalreadyDrawn = 1;
    break;
  }
}

void xTerm::highlightMenu(int setUnset,int selection = -1)
{
  inBounds = 0;
  menuPos = 0;
if (selection == -1) {
  getCursorPosition();

  
  if (xCursor > MENUOFFSET - 1 && xCursor < MENUOFFSET + 6)
  {
    inBounds = 1;

    for (int i = 0; i < 9; i++)
    {

      if (i == xCursor - MENUOFFSET)
      {
        if (highlightedMenuItem[i] == 2)
        {
          highlightedMenuItem[i] = 1;
        }
        else
        {
          highlightedMenuItem[i] = setUnset;
          if (highlightedMenuItem[i] == 2)
          {
            menuSelectedFlag = 1;
          }
          else
          {
            menuSelectedFlag = 0;
          }
        }
        menuPos = i;
      }
      else
      {

        highlightedMenuItem[i] = 0;
      }
    }

  } 
  }else {
    for (int i = 0; i < 9; i ++)
    {
      if (i == selection)
      {
        highlightedMenuItem[i] = 1;
      } else {
        highlightedMenuItem[i] = 0;
      }
      //Serial.print(highlightedMenuItem[i]);
    }
  }      
  
  CSI
  Serial.write("?25l"); // hide Cursor
    for (int i = 0; i < 7; i++)
    {

      tempMoveCursor(i + MENUOFFSET, 10);


      if (highlightedMenuItem[i] == 1)
      {

        setBackgroundColor(140 - ((xCursor - 10) * 14), 60, 75 + (xCursor * 3));
      }
      else if (highlightedMenuItem[i] == 2)
      {

        setBackgroundColor(180 - ((xCursor - 9) * 14), 120, 15 + (xCursor * 2));
      }
      else
      {

        setBackgroundColor(0, 0, 0);
      }

      switch (currentMenu)
      {
      case 0:
        Serial.print(mainMenu[i]);
        break;

      case 1:
        Serial.print(SpecialFunctionsMenu[i]);
        break;

        case 3:
        Serial.print(connectNodesMenu[i]);
        break;
      }
      setBackgroundColor(0, 0, 0);
    }
    CSI
        Serial.write("?25h"); /// show Cursor

    tempMoveCursor(-1, -1);
  

  setBackgroundColor(0, 0, 0);

}

void xTerm::splashScreen(void)
{

  Serial.write("\x1B\x5B 1;1;H");
  Serial.write("\x1B\x5B 1;36m");

  setBackgroundColor(0, 0, 0);
  Serial.print("\t\t\t\t\t\t\n\r\t ");
  setBackgroundColor(130, 0, 70);

  Serial.print(splash[0]);

  setBackgroundColor(0, 0, 0);
  Serial.print(" \t\t\t\t\t\t\n\r\t");
  setBackgroundColor(130, 10, 60);

  Serial.print(splash[1]);

  setBackgroundColor(0, 0, 0);
  Serial.print("\t\t\t\t\t\t\n\r\t");
  setBackgroundColor(130, 20, 50);

  Serial.print(splash[2]);

  setBackgroundColor(0, 0, 0);
  Serial.print("\t\t\t\t\t\t\n\r\t");
  setBackgroundColor(130, 30, 40);

  Serial.print(splash[3]);

  setBackgroundColor(0, 0, 0);
  Serial.print("\t\t\t\t\t\t\n\r\t");
  setBackgroundColor(130, 40, 30);

  Serial.print(splash[4]);
  setBackgroundColor(0, 0, 0);
  Serial.print("\t\t\t\t\t\t\n\r\t ");
  setBackgroundColor(130, 50, 20);
  Serial.print(splash[5]);
  setBackgroundColor(0, 0, 0);
  Serial.print(" \t\t\t\t\t\t\n\r\t ");
  // setBackgroundColor(130,0,70);
  Serial.print(splash[6]);
  setBackgroundColor(0, 0, 0);
  Serial.print("\t\t\t\t\t\t\n\r\t");
  CSI
      Serial.write("0m");
  Serial.write("\x1B\x5B B");
}

void xTerm::setBackgroundColor(int red, int green, int blue)
{
  Serial.printf("\x1B\x5B 48:2:%d:%d:%dm", red + '0', green + '0', blue + '0');
}

void xTerm::setForegroundColor(int red, int green, int blue)
{
  Serial.printf("\x1B\x5B 38:2:%d:%d:%dm", red + '0', green + '0', blue + '0');
}

void xTerm::setNetColor(int paletteNumber)
{
  if (paletteNumber < 34){ //if I run out of colors it starts swapping rgb values

  Serial.printf("\x1B\x5B 38:2:%d:%d:%dm", palette[paletteNumber][0] + '0', palette[paletteNumber][1] + '0', palette[paletteNumber][2] + '0');
  } else if (paletteNumber >= 34 && paletteNumber >= 164)
  {
    paletteNumber = paletteNumber % 30 + 3;
      Serial.printf("\x1B\x5B 38:2:%d:%d:%dm", palette[paletteNumber][2] + '0', palette[paletteNumber][1] + '0', palette[paletteNumber][0] + '0');
  } else {
    paletteNumber = paletteNumber % 30 + 3;
    Serial.printf("\x1B\x5B 38:2:%d:%d:%dm", palette[paletteNumber][0] + '0', palette[paletteNumber][2] + '0', palette[paletteNumber][1] + '0');
  }

}


void xTerm::getCursorPosition(void)
{
  char response[6] = {'.', '.', '.', '.', '.', '.'};
  CSI
      Serial.write("6n");

  Serial.readBytesUntil('\x5B', response, 6);
  xCursor = Serial.parseInt();
  Serial.readBytesUntil(';', response, 6);
  yCursor = Serial.parseInt();

//while(Serial.available())Serial.read();

}

int xTerm::getCursorYPosition(void)
{
  char response[6] = {'.', '.', '.', '.', '.', '.'};
  CSI
      Serial.write("6n");

  Serial.readBytesUntil('\x5B', response, 6);
  int x = Serial.parseInt();
  Serial.readBytesUntil(';', response, 6);
  return Serial.parseInt();
}

int xTerm::getCursorXPosition(void)
{
  char response[6] = {'.', '.', '.', '.', '.', '.'};
  CSI
      Serial.write("6n");

  Serial.readBytesUntil('\x5B', response, 6);
  return Serial.parseInt();
}

void xTerm::tempMoveCursor(int xTarget, int yTarget)
{

  if (xTarget >= 0 && yTarget >= 0)
  {

    itoa(xTarget, asciiCursor, 10);
    CSI
        Serial.write(asciiCursor);
    Serial.write(';');
    itoa(yTarget, asciiCursor, 10);
    Serial.write(asciiCursor);
    Serial.write(';');
    Serial.write('H');
  }
  else if (xTarget >= 0 && yTarget < 0)
  {
    //;
    itoa(xTarget, asciiCursor, 10);
    CSI
        Serial.write(asciiCursor);
    Serial.write(';');
    itoa(getCursorYPosition(), asciiCursor, 10);
    Serial.write(asciiCursor);
    Serial.write(';');
    Serial.write('H');
  }
  else if (yTarget >= 0 && xTarget < 0)
  {

    itoa(yTarget, asciiCursor, 10);
    CSI
        Serial.write(asciiCursor);
    Serial.write(';');
    Serial.write('G');
  }

  else
  {
    CSI
        itoa(xCursor, asciiCursor, 10);
    Serial.write(asciiCursor);
    Serial.write(';');
    itoa(yCursor, asciiCursor, 10);
    Serial.write(asciiCursor);
    Serial.write(';');
    Serial.write('H');
  }
}

void xTerm::printPalette(void)
{
  int color = -2;
  Serial.write("\x1B(0"); /// technical drawing set
  for (int i = 0; i < 6; i++)
  {

    // x.showMenu(0);
    Serial.print('\r');
    for (int j = 0; j < 62; j++)
    {

      if (j % 2 == 0)
      {
        color++;
        setForegroundColor(palette[color][0], palette[color][1], palette[color][2]);
      }
      else
      {
        Serial.print(color);
      }

      // Serial.print(breadBoard[i][j]);
      Serial.print('\x61');
    }
    color = -2;
    Serial.println(' ');
  }
  Serial.write("\x1B(B"); /// technical drawing set off
  Serial.println("\tColors in Net Order");
  Serial.write("\x1B(0"); /// technical drawing set
  for (int i = 0; i < 8; i++)
  {

    // x.showMenu(0);
    Serial.print('\r');
    for (int j = 0; j < 62; j++)
    {

      if (j % 2 == 0)
      {
        color++;
        setNetColor(netColor[color]);
      }
      else
      {
        Serial.print(netColor[color]);
      }

      // Serial.print(breadBoard[i][j]);
      Serial.print('\x61');
    }
    color = -2;
    Serial.println(' ');
  }

  Serial.write("\x1B(B"); /// technical drawing set off

  while (1)
  {
    Serial.read();
    if (Serial.available())
    {
      clear();
      splashScreen();
      return;
    }
  }
}
#endif