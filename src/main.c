/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"	// biblioteca auxiliar criada para para garantir que arquivos como imagens, sons e fontes possam ser carregados corretamente, mesmo que o executável esteja em outro diretório

#include "menu.h"

int main (){
	// informa à janela para usar V-Sync e funcionar em telas com alta densidade de pixels (HiDPI)
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// define largura e altura da tela
	const int screenWidth = 900;
	const int screenHeight = 900;
	
	// inicia a janela e o contexto OpenGL
	InitWindow(screenWidth, screenHeight, "Untitled Game");
	// inicializa o sistema de áudio
	InitAudioDevice();

	// função utilitária de resource_dir.h para encontrar a pasta de recursos e defini-la como diretório de trabalho atual, assim podemos carregar as imagens a partir dela
	SearchAndSetResourceDir("resources");

	Texture menuImage = LoadTexture("background/menu.png");

	Music menuMusic = LoadMusicStream("music/menumusic.wav");

	GameState state = {.currentScreen = INIT,
					   .audio = true,
					   .music = menuMusic,
					   .exit = false };
	int fontSize = 30;

	PlayMusicStream(state.music);

	// executa o loop até o usuário pressionar ESC, clicar no botão de fechar da janela ou clicar na opção SAIR
	while (!WindowShouldClose() && !state.exit){

		if (state.audio == true) {
        	UpdateMusicStream(state.music);
    	}

		// desenha a tela
		BeginDrawing();
			ClearBackground(RAYWHITE); // limpa o background
			DrawTexture(menuImage, 0, 0, WHITE); // carrega imagem do menu
			switch(state.currentScreen){
				case INIT:
					showInitScreen(fontSize);
					break;
						
				case PLAY:
					// rodar o jogo aqui (chamar a função para rodar o jogo)
					break;
					
				case CONFIGURATIONS:
					showConfigScreen(fontSize, state.audio);
					break;
					
				case DEVELOPERS:
					showDevelopScreen(fontSize);
					break;
				
				default:
					break;
			}
		EndDrawing();
		
		// atualiza o estado da tela
		updateGameState(&state, fontSize);

	}

	if (state.audio == true){
		StopMusicStream(state.music);
		UnloadMusicStream(state.music);
	}

	// libera a memória
	UnloadTexture(menuImage);

	CloseAudioDevice(); // fecha o sistema de áudio
	CloseWindow();	// destrói a janela e limpa o contexto OpenGL
	
	return 0;
}
