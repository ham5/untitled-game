#ifndef MENU_H
#define MENU_H

#include "raylib.h"

typedef enum{
	INIT,
	PLAY,
	CONFIGURATIONS,
	DEVELOPERS
}GameScreen;

typedef struct{
	GameScreen currentScreen;
	Music music;
	bool audio;
	int score;
	bool exit;
}GameState;

bool clickedIn(const Rectangle ButtonArea, const Vector2 mousePosition);
void updateGameState(GameState *const state, const int fontSize);
void inputAudioConfig(const int fontSize, const bool audio);
void showInitScreen(const int fontSize);
void showConfigScreen(const int fontSize, const bool audio);
void showDevelopScreen(const int fontSize);

#endif