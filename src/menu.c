#include "raylib.h"
#include "menu.h"
#include <stdio.h>

bool clickedIn(Rectangle ButtonArea, Vector2 mousePosition){
	if (CheckCollisionPointRec(mousePosition, ButtonArea) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
		return true;
	}
	return false;
}

void showInitScreen(GameState *state){
	// Áreas base dos botões
    Rectangle playButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 - 50, 250, 50 };
    Rectangle optionsButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 20, 250, 50 };
    Rectangle creditsButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 90, 250, 50 };
    Rectangle LoreButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 160, 250, 50 };
    Rectangle exitButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 230, 250, 50 };
    

    Vector2 mousePoint = GetMousePosition();

    // Checa clique nos botões 
    if (clickedIn(playButton, mousePoint)) state->currentScreen = PLAY;
    if (clickedIn(optionsButton, mousePoint)) state->currentScreen = CONFIGURATIONS;
    if (clickedIn(creditsButton, mousePoint)) state->currentScreen = DEVELOPERS;
    if (clickedIn(LoreButton, mousePoint)) state->currentScreen = LORE;
    if (clickedIn(exitButton, mousePoint)) state->exit = true;

    DrawText("Coliseu", GetScreenWidth()/2 - MeasureText("Coliseu", 80)/2, 100, 80, WHITE);
    // Botão Jogar
    if (CheckCollisionPointRec(mousePoint, playButton)) {
        DrawRectangleRec(playButton, WHITE);
        DrawText("Jogar", playButton.x + 75, playButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(playButton, LIGHTGRAY);
        DrawText("Jogar", playButton.x + 75, playButton.y + 10, 30, DARKGRAY);
    }

    // Botão Opções
    if (CheckCollisionPointRec(mousePoint, optionsButton)) {
        DrawRectangleRec(optionsButton, WHITE);
        DrawText("Opções", optionsButton.x + 65, optionsButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(optionsButton, LIGHTGRAY);
        DrawText("Opções", optionsButton.x + 65, optionsButton.y + 10, 30, DARKGRAY);
    }

    // Botão Créditos
    if (CheckCollisionPointRec(mousePoint, creditsButton)) {
        DrawRectangleRec(creditsButton, WHITE);
        DrawText("Créditos", creditsButton.x + 55, creditsButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(creditsButton, LIGHTGRAY);
        DrawText("Créditos", creditsButton.x + 55, creditsButton.y + 10, 30, DARKGRAY);
    }

    // Botão Lore
    if (CheckCollisionPointRec(mousePoint, LoreButton)) {   
        DrawRectangleRec(LoreButton, WHITE);
        DrawText("Lore", LoreButton.x + 85, LoreButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(LoreButton, LIGHTGRAY);
        DrawText("Lore", LoreButton.x + 85, LoreButton.y + 10, 30, DARKGRAY);
    }

    // Botão Sair
    if (CheckCollisionPointRec(mousePoint, exitButton)) {
        DrawRectangleRec(exitButton, WHITE);
        DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(exitButton, LIGHTGRAY);
        DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, DARKGRAY);
    }
}

void showConfigScreen(GameState *state, float *volume){
// Elementos da tela
    Rectangle volumeSliderBar = { GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 10, 300, 20 };
    Rectangle volumeSliderHandle = { volumeSliderBar.x + (*volume * volumeSliderBar.width) - 10, GetScreenHeight()/2 - 20, 20, 40 };
    Rectangle backButton = { GetScreenWidth()/2 - 100, GetScreenHeight()/2 + 100, 200, 50 };
    Vector2 mousePoint = GetMousePosition();

    // Lógica da barra de volume
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, volumeSliderBar))
    {
        // A posição do handle depende do volume recebido pelo mastervolume
        *volume = (mousePoint.x - volumeSliderBar.x) / volumeSliderBar.width;
        if (*volume < 0.0) *volume = 0.0;
        if (*volume > 1.0) *volume = 1.0;
        SetMasterVolume(*volume);
    }
    
    // checa click do mouse
    if (clickedIn(backButton, mousePoint)) state->currentScreen = INIT;
    
    DrawText("Opções", GetScreenWidth()/2 - MeasureText("Opções", 40)/2, 150, 40, WHITE);
    DrawText("Volume", GetScreenWidth()/2 - MeasureText("Volume", 30)/2, GetScreenHeight()/2 - 60, 30, WHITE);
    
    // barra de volume
    DrawRectangleRec(volumeSliderBar, LIGHTGRAY);
    DrawRectangleRec(volumeSliderHandle, SKYBLUE);
    
    // botao voltar
    if (CheckCollisionPointRec(mousePoint, backButton)) {
        DrawRectangleRec(backButton, WHITE);
        DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(backButton, LIGHTGRAY);
        DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, DARKGRAY);
    }

}

void showDevelopScreen(GameState *state){
	// tamanhos da fonte
	int titleFontSize = 40;

	// deslocamento em relação ao centro (eixo y)
	int titleOffset = -250;
	int textOffset = -150;
	
	char *title = "DESENVOLVEDORES";
	DrawText(title, (GetScreenWidth() - MeasureText(title, titleFontSize)) / 2,  (GetScreenHeight() - titleFontSize) / 2 + titleOffset, titleFontSize, WHITE);

	char *developers[] = {"EDIVALDO AMBROZIO DA SILVA FILHO",
						  "GUILHERME DE CARVALHO FABRI",
						  "GUSTAVO CRAVO TEIXEIRA FILHO",
						  "HEITOR DE ASSIS MACHADO",
						  "LUCAS MENDES PRIMO AZEVEDO",
						  "TITHO LÍVIO DUARTE MELO"};

	for (int i = 0; i < 6; i++){
		DrawText(developers[i], (GetScreenWidth() - MeasureText(developers[i], 30)) / 2,  (GetScreenHeight() - 30) / 2 + textOffset, 30, WHITE);
		textOffset += 50;
	}

	Rectangle backButton = { GetScreenWidth()/2 - 100, GetScreenHeight() - 100, 200, 50 };
    Vector2 mousePoint = GetMousePosition();

    // checa click do mouse
    if (clickedIn(backButton, mousePoint)) state->currentScreen = INIT;
    
    //botao voltar
    if (CheckCollisionPointRec(mousePoint, backButton)) {
        DrawRectangleRec(backButton, WHITE);
        DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(backButton, LIGHTGRAY);
        DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, DARKGRAY);
    }

}

void showLoreScreen(GameState *state){

	Rectangle backButton = { GetScreenWidth()/2 - 100, GetScreenHeight() - 100, 200, 50 };
    Vector2 mousePoint = GetMousePosition();

    // checa click do mouse
    if (clickedIn(backButton, mousePoint)) state->currentScreen = INIT;
    
    //botao voltar
    if (CheckCollisionPointRec(mousePoint, backButton)) {
        DrawRectangleRec(backButton, WHITE);
        DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(backButton, LIGHTGRAY);
        DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, DARKGRAY);
    }

}

void showGameoverScreen(GameState *state)
{
    Rectangle menuButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 90, 250, 50 };
    Rectangle exitButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 160, 250, 50 };
    Vector2 mousePoint = GetMousePosition();

    // checa click do mouse
    if (clickedIn(menuButton, mousePoint)) state->currentScreen = INIT;
    if (clickedIn(exitButton, mousePoint)) state->exit = true;

    char scoreText[20];
    sprintf(scoreText, "Score: %d", state->score);
    DrawText(scoreText, GetScreenWidth()/2 - MeasureText(scoreText, 25)/2, 470, 25, WHITE);
    DrawText("VOCÊ MORREU", GetScreenWidth()/2 - MeasureText("VOCÊ MORREU", 100)/2, 350, 100, RED);
    
    //botao voltar
    if (CheckCollisionPointRec(mousePoint, menuButton)) {
        DrawRectangleRec(menuButton, GRAY);
        DrawText("Voltar ao Menu", menuButton.x + 7, menuButton.y + 10, 30, WHITE);
    } else {
        DrawRectangleRec(menuButton, DARKGRAY);
        DrawText("Voltar ao Menu", menuButton.x + 7, menuButton.y + 10, 30, RED);
    }
    // botao sair 
    if (CheckCollisionPointRec(mousePoint, exitButton)) {
        DrawRectangleRec(exitButton, GRAY);
        DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, WHITE);
    } else {
        DrawRectangleRec(exitButton, DARKGRAY);
        DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, RED);
    }

}

void showVictoryScreen(GameState *state)
{
    Rectangle menuButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 90, 250, 50 };
    Rectangle exitButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 160, 250, 50 };
    Vector2 mousePoint = GetMousePosition();



    // checa click do mouse
    if (clickedIn(menuButton, mousePoint)) state->currentScreen = INIT;
    if (clickedIn(exitButton, mousePoint)) state->exit = true;

    char scoreText[20];
    sprintf(scoreText, "Score: %d", state->score);
    DrawText(scoreText, GetScreenWidth()/2 - MeasureText(scoreText, 25)/2, 470, 25, WHITE); 
    DrawText("VOCÊ VENCEU", GetScreenWidth()/2 - MeasureText("VOCÊ VENCEU", 100)/2, 350, 100, GREEN);
    
    // botao menu
    if (CheckCollisionPointRec(mousePoint, menuButton)) {
        DrawRectangleRec(menuButton, WHITE);
        DrawText("Voltar ao Menu", menuButton.x + 7, menuButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(menuButton, LIGHTGRAY);
        DrawText("Voltar ao Menu", menuButton.x + 7, menuButton.y + 10, 30, DARKGRAY);
    }
    //botao sair
    if (CheckCollisionPointRec(mousePoint, exitButton)) {
        DrawRectangleRec(exitButton, WHITE);
        DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, BLACK);
    } else {
        DrawRectangleRec(exitButton, LIGHTGRAY);
        DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, DARKGRAY);
    }

}
