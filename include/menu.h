#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum{
    INIT,
    PLAY,
    CONFIGURATIONS,
    DEVELOPERS,
    VICTORY,
    DEFEAT,
}GameScreen;

typedef struct{
    GameScreen currentScreen;
    int score;
    bool exit;
}GameState;

bool clickedIn(const Rectangle ButtonArea, const Vector2 mousePosition);
void showInitScreen(GameState *const state);
void showConfigScreen(GameState *const state, float *volume);
void showDevelopScreen(GameState *const state);
void showGameoverScreen(GameState *const state);
void showVictoryScreen(GameState *const state);

#endif