#include "game.h"
#include "gba.h"
#include "print.h"

// values
int score = 0;
int lives = 3;
unsigned short backgroundColor = LAVENDER;

// PLAYER LOGIC

// initializes player
void initPlayer() {
    player1.pX = 5;
    player1.pY = 3;
    player1.oldPY = 5;
    player1.pWidth = 20;
    player1.pHeight = 10;
    player1.pColor = HOTPINK;

    player = &player1;
}

// updates player: movement, shooting, color changes
void updatePlayer() {
    player->oldPY = player->pY;
    if (BUTTON_HELD(BUTTON_UP)) {
        player->pY -= 1;
        if (player->pY < 0) {
            player->pY = 0;
        }
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        player->pY += 1;
        if (player->pY > SCREENHEIGHT - player->pHeight) {
            player->pY = SCREENHEIGHT - player->pHeight;
        }
    }

    if (BUTTON_PRESSED(BUTTON_A)) {
       newBullet(player->pX, player->pY, 4, 0);
    }

    if (lives == 3) {
        player->pColor = HOTPINK;
    } else if (lives == 2) {
        player->pColor = VIORED;
    } else if (lives == 1) {
        player->pColor = ROGUE;
    } else if (lives == 0) {
        player->pColor = BLACK;
    }
}

// draws player
void drawPlayer() {
    drawRectangle(player->pX, player->oldPY, player->pWidth, player->pHeight, backgroundColor);
    drawRectangle(player->pX, player->pY, player->pWidth, player->pHeight, player->pColor);
}

// BULLET LOGIC

// initializes bullets
void initBullets() {
    for (int i = 0; i < BULLETCOUNT; i++) {
        bullets[i].bX = 0;
        bullets[i].bY = 0;
        bullets[i].oldBX = 0;
        bullets[i].oldBY = 0;
        bullets[i].bWidth = 4;
        bullets[i].bHeight = 3;
        bullets[i].bXVel = 4;
        bullets[i].bYVel = 0;
        bullets[i].active = 0;
        bullets[i].erased = 0;
        bullets[i].bColor = WHITE;
    }
}

// updates bullets
void updateBullets() {
    for (int i = 0; i < BULLETCOUNT; i++) {
        if (bullets[i].active) {
            bullets[i].oldBX = bullets[i].bX;
            bullets[i].oldBY = bullets[i].bY;
            bullets[i].bX += bullets[i].bXVel;
            if (bullets[i].bX >= SCREENWIDTH) {
                bullets[i].active = 0;
                bullets[i].erased = 1;
            }
        }
    }
}

// draws bullets
void drawBullets() {
    for (int i = 0; i < BULLETCOUNT; i++) {
        if (bullets[i].active) {
            drawRectangle(bullets[i].oldBX, bullets[i].oldBY, bullets[i].bWidth, bullets[i].bHeight, backgroundColor);
            drawRectangle(bullets[i].bX, bullets[i].bY, bullets[i].bWidth, bullets[i].bHeight, bullets[i].bColor);
        } else if (bullets[i].erased == 1) {
            drawRectangle(bullets[i].oldBX, bullets[i].oldBY, bullets[i].bWidth, bullets[i].bHeight, backgroundColor);
        }
    }
}

// new bullet creation
void newBullet(int x, int y, int xVel, int yVel) {
    for (int i = 0; i < BULLETCOUNT; i++) {
        if (!bullets[i].active) {
            bullets[i].active = 1;
            bullets[i].bX = x;
            bullets[i].bXVel = xVel;
            bullets[i].bY = y;
            bullets[i].bYVel = yVel;
            break;
        }
    }
}

// ENEMY LOGIC

// initializes enemies
void initEnemies() {
    for (int i = 0; i < ENEMYCOUNT; i++) {
        enemies[i].eColor = TURQUOISE;
        enemies[i].eWidth = 8;
        enemies[i].eHeight = 12;
        enemies[i].active = 1;
    }

    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 6; j++) {
            enemies[count].eX = 210 - (20 * i);
            if (i % 2 == 0) {
                enemies[count].eY = 30 + (20 * j);
                count++;
            } else {
            enemies[count].eY = 20 + (20 * j);
            count++;
            }
        }
    }
}

// updates enemies: movement (wraps around), collision with player (life lost, player position reset), collision with bullets (score increase)
void updateEnemies() {
    for (int i = 0; i < ENEMYCOUNT; i++) {
        if (enemies[i].active) {
            enemies[i].oldEX = enemies[i].eX;
            enemies[i].oldEY = enemies[i].eY;
            enemies[i].eX -= 1;

            if (enemies[i].eX + enemies[i].eWidth < 0) {
                enemies[i].eX = SCREENWIDTH;
            }

            if (collision(player->pX, player->pY, player->pWidth, player->pHeight, enemies[i].eX, enemies[i].eY, enemies[i].eWidth, enemies[i].eHeight)) {
                player->pX = 5;
                player->pY = 3;
                lives--;
            }

            for (int j = 0; j < BULLETCOUNT; j++) {
                if (bullets[j].active && collision(bullets[j].bX, bullets[j].bY, bullets[j].bWidth, bullets[j].bHeight, enemies[i].eX, enemies[i].eY, enemies[i].eWidth, enemies[i].eHeight)) {
                    bullets[j].active = 0;
                    bullets[j].erased = 1;
                    drawRectangle(bullets[j].oldBX, bullets[j].oldBY, bullets[j].bWidth, bullets[j].bHeight, backgroundColor);
                    enemies[i].active = 0;
                    drawRectangle(enemies[i].oldEX, enemies[i].oldEY, enemies[i].eWidth, enemies[i].eHeight, backgroundColor);
                    score++;
                }
            }
        }
    }
}

// draws enemies
void drawEnemies() {
    for (int i = 0; i < ENEMYCOUNT; i++) {
        if (enemies[i].active) {
            drawRectangle(enemies[i].oldEX, enemies[i].oldEY, enemies[i].eWidth, enemies[i].eHeight, backgroundColor);
            drawRectangle(enemies[i].eX, enemies[i].eY, enemies[i].eWidth, enemies[i].eHeight, enemies[i].eColor);
        }
    }
}

// LIFE BLOCK LOGIC

// initializes life block
void initLifeBlock() {
    lifeBlock1.lX = 5;
    lifeBlock1.lY = 145;
    lifeBlock1.lWidth = 7;
    lifeBlock1.lHeight = 7;
    lifeBlock1.lColor = WHITE;

    lifeBlock = &lifeBlock1;
}

// updates life block: collision with player (gain life, max 3 lives), color change
void updateLifeBlock() {
    if (collision(player->pX, player->pY, player->pWidth, player->pHeight, lifeBlock->lX, lifeBlock->lY, lifeBlock->lWidth, lifeBlock->lHeight)) {
        lifeBlock->lColor = LAVPINK;
        if (lives < 3) {
            lives++;
        }
    } else {
        lifeBlock->lColor = WHITE;
    }
}

// draws life block
void drawLifeBlock() {
    drawRectangle(lifeBlock->lX, lifeBlock->lY, lifeBlock->lWidth, lifeBlock->lHeight, lifeBlock->lColor);
}