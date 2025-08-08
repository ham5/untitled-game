/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"	// biblioteca auxiliar criada para para garantir que arquivos como imagens, sons e fontes possam ser carregados corretamente, mesmo que o executável esteja em outro diretório

typedef enum{
	INIT,
	PLAY,
	CONFIGURATIONS,
	DEVELOPERS
}GameScreen;

int main (){
	// informa à janela para usar V-Sync e funcionar em telas com alta densidade de pixels (HiDPI)
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// define largura e altura da tela
	const int screenWidth = 1280;
	const int screenHeight = 720;
	
	// inicia a janela e o contexto OpenGL
	InitWindow(screenWidth, screenHeight, "Untitled Game");

	// função utilitária de resource_dir.h para encontrar a pasta de recursos e defini-la como diretório de trabalho atual, assim podemos carregar as imagens a partir dela
	SearchAndSetResourceDir("resources");

	GameScreen currentScreen = INIT;	// programa inicia no menu principal
	bool exit = false;	// flag para sair

	// executa o loop até o usuário pressionar ESC, clicar no botão de fechar da janela ou clicar na opção SAIR
	while (!WindowShouldClose() && !exit){

		int MenuFontSize = 30;

		// ------------ desenha a tela ----------------
		BeginDrawing();
			ClearBackground(RAYWHITE); // limpa o background

			switch(currentScreen){
				case INIT:
					DrawText("JOGAR", (GetScreenWidth() - MeasureText("JOGAR", MenuFontSize)) / 2, (GetScreenHeight() - MenuFontSize) / 2 + 100, MenuFontSize, LIGHTGRAY);

					DrawText("CONFIGURAÇÕES", (GetScreenWidth() - MeasureText("CONFIGURAÇÕES", MenuFontSize)) / 2, (GetScreenHeight() - MenuFontSize) / 2 + 150, MenuFontSize, LIGHTGRAY);

					DrawText("DESENVOLVEDORES", (GetScreenWidth() - MeasureText("DESENVOLVEDORES", MenuFontSize)) / 2,  (GetScreenHeight() - MenuFontSize) / 2 + 200, MenuFontSize, LIGHTGRAY);

					DrawText("SAIR", (GetScreenWidth() - MeasureText("SAIR", MenuFontSize)) / 2,  (GetScreenHeight() - MenuFontSize) / 2 + 250, MenuFontSize, LIGHTGRAY);
					break;
						
				case PLAY:
					DrawText("TELA DE JOGO", (GetScreenWidth() - MeasureText("TELA DE JOGO", MenuFontSize)) / 2, (GetScreenHeight() - MenuFontSize) / 2, MenuFontSize, LIGHTGRAY);
					break;
					
				case CONFIGURATIONS:
					DrawText("CONFIGURAÇÕES", (GetScreenWidth() - MeasureText("CONFIGURAÇÕES", MenuFontSize)) / 2, (GetScreenHeight() - MenuFontSize) / 2, MenuFontSize, LIGHTGRAY);
					break;
					
				case DEVELOPERS:
					DrawText("DESENVOLVEDORES", (GetScreenWidth() - MeasureText("DESENVOLVEDORES", MenuFontSize)) / 2,  (GetScreenHeight() - MenuFontSize) / 2, MenuFontSize, LIGHTGRAY);
					break;
				
				default:
					break;
			}

		EndDrawing();
	}
	// destrói a janela e limpa o contexto OpenGL
	CloseWindow();
	
	return 0;
}
