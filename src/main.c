#include <stdio.h>
#define BYTE unsigned char
#define BOOL unsigned char

static const BYTE RIGHT = 64*0;
static const BYTE UP    = 64*1;
static const BYTE LEFT  = 64*2;
static const BYTE DOWN  = 64*3;

static const BYTE CCW = 64;
static const BYTE CW  = -CCW;

static BOOL CONTINUE  = 0;
static BOOL COMPLETED = 1;

#define ADR(x,y) (0x2000+(320 * (BYTE)(y/8)) + (y & 7)+8 * (BYTE)(x/8))

struct Ant {
  short x;
  BYTE y;
  BYTE direction;
};

void setHiRes() {
    *(BYTE*)0xd011 |= 0xb0 ; // Graphics on
    *(BYTE*)0xd016 &= 240; //Multi color off
    *(BYTE*)0xd018 |= 8 ; // Graphics to $2000
}

void clearHiRes() {
    short i;
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

BYTE isPositionWhite(short x, BYTE y) {
    return *(short*)(ADR(x,y)) & 1 << ((7-(x & 7)));
}

void setPositionWhite(short x, BYTE y) {
    *(short*)(ADR(x,y)) |= 1 << ((7-(x & 7)));
}

void setPositionBlack(short x, BYTE y) {
    *(short*)(ADR(x,y)) &= ~(1 << ((7-(x & 7))));
}

BOOL moveForward(short *x, BYTE *y, BYTE direction) {
    if (direction == RIGHT) {
        if (*x==319) return COMPLETED;
        *x++;
    } else if (direction == UP) {
        if (*y==199) return COMPLETED;
        *y++;
    } else if (direction == LEFT) {
        if (*x==0) return COMPLETED;
        *x--;
    } else if (direction == DOWN) {
        if (*y==0) return COMPLETED;
        *y--;
    }
    return CONTINUE;
}

BOOL makeMove(short *x, BYTE *y, BYTE *direction) {
    if (isPositionWhite(*x, *y)) {
        direction += CW;
        setPositionBlack(*x, *y);
   } else {
        direction += CCW;
        setPositionWhite(*x, *y);
    }
    return moveForward(x, y, *direction);
}

int main(void) {
    struct Ant ants[] = {{160, 100, RIGHT},
                         {150, 100, DOWN}};
    
    short i;
    for (i = 0;i<12;i++) { printf("\n"); }
    printf("Please wait for ant ...\n");
    for (i = 0;i<12;i++) { printf("\n"); }
    setAndClearHiRes();
    BOOL completed = CONTINUE;
    while(!completed)
    {
      for (i = 0;i<sizeof(ants)/sizeof(ants[0]);i++)
      {
        completed |= makeMove(&ants[i].x, &ants[i].y, &ants[i].direction);
      }
    }
    return 0;
}
