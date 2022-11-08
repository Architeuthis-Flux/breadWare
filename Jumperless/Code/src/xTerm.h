

#include <Arduino.h>
//#include "menus.h"
#define CSI Serial.write("\x1B\x5B");
//#define TOPCURSOR 1




class xTerm 
{
    public:
xTerm();
int getInput (void);
int getBreadboardInput(void);
void drawBBarrow(void);
int getNetInput(void);
void checkSelectedColumns(void);
void colorColumn(int, int,int);
void highlightColumn( int, int undo = 0,int net1 = 2);
void surroundColumn(int);
void splashScreen(void);
void setBackgroundColor(int red, int green, int blue);
void setForegroundColor(int red, int green, int blue);
void xTerm::setNetColor(int paletteNumber);
void getCursorPosition(void);
int getCursorXPosition(void);
int getCursorYPosition(void);
void tempMoveCursor(int xTarget, int yTarget);
void clear (void);
void showMenu(int, int column = 0);
void highlightMenu(int,int selection = -1);
void printPalette(void);
int currentMenu = 0;
int net = 3;
int xCursor = 1;
int yCursor = 1;
};




//#endif