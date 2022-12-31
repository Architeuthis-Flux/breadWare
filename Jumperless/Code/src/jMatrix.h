
#ifndef JMATRIX_H
#define JMATRIX_H
#define CSI Serial.write("\x1B\x5B");

class jMatrix{

public:
jMatrix();

int connect(int row1, int row2, int hop = 0);
int connectDirectly(int x,int y, int chip, int );
void disconnect(int);
int findHopLocation(int row1, int row2);
void setAddress(int, int);
void strobeItIn(int connectDisconnect);
void selectChip(int);
void deselectChip(void);
void clearConnections(void);
char chipToChar(int);

struct connection { //I can't decide whether to store both hops in the same struct but for now I will

bool used;

int row1;
int row2;

int chip1;
int chip2;

int chip1Xaddr;
int chip1Yaddr;

int chip2Xaddr;
int chip2Yaddr;

int lane;
int hop;

int hopChip;
int hopChipXaddr1; //we know the Y address will be 0
int hopChipXaddr2;


};


struct connection conn[100];
int numberOfConnections = 1;

private:


};



#endif