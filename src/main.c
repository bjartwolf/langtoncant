#include <stdio.h>
#define BYTE unsigned char

short i;
short j;
short x;
BYTE y;
BYTE res;
short ad;
short tmpY;
short nrOfAlive;

void setHiRes() {
    *(BYTE*)0xd011 = *(BYTE*)0xd011 | 0xb0 ; // Graphics on
    *(BYTE*)0xd016 = *(BYTE*)0xd016 & 240; //Multi color off
    *(BYTE*)0xd018 = *(BYTE*)0xd018 | 8 ; // Graphics to $2000
}
void clearHiRes() {
    for (i =0;i< 8000; i++)
    {
        *(BYTE*)(0x2000+i) = 0; 
    }

    for (i =0;i< 1000; i++)
    {
        *(BYTE*)(0x400+i) = 0xf0; 
    }
}

void setAndClearHiRes(){
    clearHiRes();
    setHiRes();
}

void calcAdress(){
    /// Refactored it a bit, it had stuff like 320*FLOOR(Y/8) which is made into
    /// 40*8*FLOOR(Y/8) which is the same as (32+8)* (Y and remove last three bits)
    /// which ends up being (4+1)*Y_with_three bits removed
    /// which is split into a temp variable... and summed in the last line
    tmpY = (y & 0xf8) << 3;
    ad = 0x2000+tmpY + (tmpY << 2) + (y & 7)+ (x&(0xfff8));
}

BYTE isPositionWhite() {
    calcAdress();
    return *(short*)(ad) & 1 << ((7-(x & 7)));
}

BYTE isNWhite() {
    y++;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    y--;
    calcAdress();
    return res;
}

BYTE isNEWhite() {
    y++;
    x++;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    y--;
    x--;
    calcAdress();
    return res;
}

BYTE isEWhite() {
    x++;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    x--;
    calcAdress();
    return res;
}

BYTE isSEWhite() {
    y--;
    x++;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    y++;
    x--;
    calcAdress();
    return res;
}

BYTE isSWhite() {
    y--;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    y++;
    calcAdress();
    return res;
}

BYTE isSWWhite() {
    y--;
    x--;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    x++;
    y++;
    calcAdress();
    return res;
}


BYTE isWWhite() {
    x--;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    x++;
    calcAdress();
    return res;
}

BYTE isNWWhite() {
    y++;
    x--;
    calcAdress();
    res = *(short*)(ad) & 1 << ((7-(x & 7)));
    y--;
    x++;
    calcAdress();
    return res;
}


// https://archive.org/details/The_Graphics_Book_for_the_Commodore_65/page/n129/
void setPositionWhite() {
    calcAdress();
    *(short*)(ad) = *(short*)(ad) | 1 << ((7-(x & 7)));
}

void setPositionBlack() {
    calcAdress();
    *(short*)(ad) = (*(short*)(ad)) & ~(1 << ((7-(x & 7))));
}

int countAliveNeighbors(){
    nrOfAlive = 0;
    if (isNWhite()) {
        nrOfAlive++;
    }
    if (isNEWhite()) {
        nrOfAlive++;
    }
    if (isEWhite()){
        nrOfAlive++;
    }
    if (isSEWhite()) {
        nrOfAlive++;
    }
    if (isSWhite()) {
        nrOfAlive++;
    }
    if (isSWWhite()) {
        nrOfAlive++;
    }
    if (isWWhite()){
        nrOfAlive++;
    }
    if (isNWWhite()){
        nrOfAlive++;
    }
    return nrOfAlive;
}

int main(void) {
    for (i = 0;i<12;i++) { printf("\n"); }
    printf("Please wait for ant ...\n");
    for (i = 0;i<12;i++) { printf("\n"); }
    setAndClearHiRes();
    x = 100;
    y = 100;
    setPositionWhite();
    x++;
    setPositionWhite();
    x++;
    setPositionWhite();

    /*
    x = 100;
    y = 100;
    setPositionWhite();
    x++;
    y--;
    setPositionWhite(); 
    x++; 
    setPositionWhite();
    y++;
    setPositionWhite();
    y++;
    setPositionWhite();
*/
    //while(x > 0 && x < 320 && y > 0 && y < 200)
    while(1)
    {
        for (i =90;i< 110; i++) {
            for (j =90;j< 110; j++) {
                x = i;
                y = j;
                countAliveNeighbors();
                if (!isPositionWhite() && (nrOfAlive == 2 || nrOfAlive == 3)) {
                } else if ((isPositionWhite()) && nrOfAlive == 3){
                    setPositionWhite(); // spawning
                } else {
                    setPositionBlack();
                }
            }
        }
    }
    return 0;
}