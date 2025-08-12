#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum{
    INIT,
    PLAY,
    CONFIGURATIONS,
    DEVELOPERS,
    LORE,
    VICTORY,
    DEFEAT,
}GameScreen;

typedef struct{
    GameScreen currentScreen;
    int score;
    bool exit;
}GameState;

bool clickedIn(Rectangle ButtonArea, Vector2 mousePosition);
void showInitScreen(GameState *state);
void showConfigScreen(GameState *state, float *volume);
void showDevelopScreen(GameState *state);
void showGameoverScreen(GameState *state);
void showVictoryScreen(GameState *state);
void showLoreScreen(GameState *state);

#endif