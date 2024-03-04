#ifndef GAME_H
#define GAME_H

// extern values
extern int score;
extern int lives;
extern unsigned short backgroundColor;

// player struct
typedef struct {
	int pX;
	int pY;
    int oldPY;
    int pWidth; 
    int pHeight;
    unsigned short pColor;
} PLAYER;

// player and pointer
PLAYER player1;
PLAYER* player;

// player function prototypes
void initPlayer(); 
void drawPlayer(); 
void updatePlayer(); 






// bullet struct
typedef struct {
    int bX;
    int bY;
    int oldBX;
    int oldBY;
    int bWidth;
    int bHeight;
    int bXVel;
    int bYVel;
    int active;
    int erased; 
    unsigned short bColor; 
} BULLET;

// bullet properties
#define BULLETCOUNT 25
BULLET bullets[BULLETCOUNT];

// bullet function prototypes
void initBullets();
void drawBullets();
void updateBullets();
void newBullet(int x, int y, int xVel, int yVel);






// enemy struct
typedef struct {
    int eX;
    int eY;
    int oldEX;
    int oldEY;
    int eWidth;
    int eHeight;
    int active;
    unsigned short eColor;

} ENEMY;

// enemy properties
#define ENEMYCOUNT 18
ENEMY enemies[ENEMYCOUNT];

// enemy function prototypes
void initEnemies();
void updateEnemies();
void drawEnemies();






// life block struct
typedef struct {
    int lX;
    int lY;
    int lWidth;
    int lHeight;
    unsigned short lColor;
} LIFEBLOCK;

// life block and pointer
LIFEBLOCK lifeBlock1;
LIFEBLOCK* lifeBlock;

// life block function prototypes
void initLifeBlock();
void updateLifeBlock();
void drawLifeBlock();

#endif