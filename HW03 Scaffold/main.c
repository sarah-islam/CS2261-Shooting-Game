#include "gba.h"
#include "game.h"
#include "print.h"
#include <stdio.h>

// Function Prototypes
void initialize();
void updateGame();
void drawGame();
void initGame();
void start();
void game();
void pause();
void lose();
void win();
void goToStart();
void goToGame();
void goToPause();
void goToLose();
void goToWin();

unsigned short buttons;
unsigned short oldButtons;

char buffer[50];

// state enum
enum STATE {
    START,
    GAME,
    PAUSE,
    LOSE,
    WIN
} state;

int main() {

    initialize();

    while (1) {
        oldButtons = buttons;
        buttons = REG_BUTTONS;

        // state machine
        switch (state) {
            case START:
                start();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case LOSE:
                lose();
                break;
            case WIN:
                win();
                break;
        }
    }
    return 0;

}

void initialize() {
    mgba_open();

    REG_DISPCTL = MODE(3) | BG2_ENABLE;
    oldButtons = 0;
    buttons = REG_BUTTONS;

    goToStart();
}

void updateGame() {
    updateLifeBlock();
    updatePlayer();
    updateBullets();
    updateEnemies();
}

void drawGame() {
    drawLifeBlock();
    drawPlayer();
    drawBullets();
    drawEnemies();
}

void initGame() {
    initLifeBlock();
    initPlayer();
    initBullets();
    initEnemies();
    score = 0;
    lives = 3;
}

void goToStart() {
    fillScreen(WHITE);
    drawString(20, 65, "shoot 'em up!", TURQUOISE);
    drawString(20, 75, "press start to play", TEAL);

    state = START;
}

void start() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {        
        initGame();
        goToGame();
    }
}

void goToGame() {
    fillScreen(backgroundColor);
    drawString(180, 5, "score: ", WHITE);
    drawString(180, 150, "lives: ", WHITE);

    state = GAME;
}

void game() {
    updateGame();
    waitForVBlank();

    sprintf(buffer, "%d", score);
    drawRectangle(220, 5, 13, 8, backgroundColor);
    drawString(220, 5, buffer, WHITE);

    sprintf(buffer, "%d", lives);
    drawRectangle(220, 150, 10, 8, backgroundColor);
    drawString(220, 150, buffer, WHITE);

    drawGame();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }

    if (lives == 0) {
        goToLose();
    }
    if (score == 18) {
        goToWin();
    }
}

void goToPause() {
    fillScreen(WHITE);
    drawString(10, 65, "break so soon? press start to resume", TURQUOISE);
    drawString(10, 75, "or press select to restart game!", TEAL); 

    waitForVBlank();
    state = PAUSE;
}

void pause() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START))
        goToGame();
    else if (BUTTON_PRESSED(BUTTON_SELECT))
        goToStart();
}

void goToWin() {
    fillScreen(WHITE);
    drawString(20, 65, "YIPPEE YOU WON! :D", TURQUOISE);
    drawString(20, 75, "press start to replay!", TEAL);

    waitForVBlank();
    state = WIN;
}

void win() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToLose() {
    fillScreen(WHITE);
    drawString(20, 65, "UH OH, you lost :(", TURQUOISE);
    drawString(20, 75, "press start to try again!", TEAL);

    waitForVBlank();
    state = LOSE;
}

void lose() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}