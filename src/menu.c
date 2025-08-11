#include "raylib.h"
#include "menu.h"

bool clickedIn(const Rectangle ButtonArea, const Vector2 mousePosition){
	if (CheckCollisionPointRec(mousePosition, ButtonArea) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
		return true;
	}
	return false;
}

void updateGameState(GameState *const state, const int fontSize){

	// ------------ armazena as áreas dos botões ----------------
	Rectangle playButtonArea = {(GetScreenWidth() - MeasureText("JOGAR", fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + 100, MeasureText("JOGAR", fontSize), fontSize};
	Rectangle configButtonArea = {(GetScreenWidth() - MeasureText("CONFIGURAÇÕES", fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + 150, MeasureText("CONFIGURAÇÕES", fontSize), fontSize};
	Rectangle devButtonArea = {(GetScreenWidth() - MeasureText("DESENVOLVEDORES", fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + 200, MeasureText("DESENVOLVEDORES", fontSize), fontSize};
	Rectangle exitButtonArea = {(GetScreenWidth() - MeasureText("SAIR", fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + 250, MeasureText("SAIR", fontSize), fontSize};
	Rectangle audioLeftButtonArea = {GetScreenWidth() / 2, (GetScreenHeight() - fontSize) / 2, MeasureText("<", fontSize), fontSize};
	Rectangle audioRightButtonArea = {GetScreenWidth() / 2 + MeasureText(state->audio ? "< ON >" : "< OFF >", fontSize) - MeasureText(">", fontSize), (GetScreenHeight() - fontSize) / 2, MeasureText(">", fontSize), fontSize};
	Rectangle backButtonArea = {(GetScreenWidth() - MeasureText("VOLTAR", fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + 250, MeasureText("VOLTAR", fontSize), fontSize};
	
	// armazena a posição do mouse
	Vector2 mousePosition = GetMousePosition();

		// ------------ atualiza a tela ----------------
	switch (state->currentScreen){	// verifica se um dos botões foi pressionado, de acordo com a tela atual
		case INIT:
			if (clickedIn(playButtonArea, mousePosition)){ // se usuário apertou "JOGAR"
				state->currentScreen = PLAY;

			} else if (clickedIn(configButtonArea, mousePosition)){ // se usuário apertou "CONFIGURAÇÕES"
				state->currentScreen = CONFIGURATIONS;

			} else if (clickedIn(devButtonArea, mousePosition)){ // se usuário apertou "DESENVOLVEDORES"
				state->currentScreen = DEVELOPERS;

			} else if (clickedIn(exitButtonArea, mousePosition)){ // se usuário apertou "SAIR"
				state->exit = true;
			}
			break;

		case PLAY:
			// adicione aqui a transição da tela de jogo para a tela de vitória/derrota
			break;

		case CONFIGURATIONS:
			if (clickedIn(audioLeftButtonArea, mousePosition) || clickedIn(audioRightButtonArea, mousePosition)){
				state->audio = !state->audio;
				if (state->audio == true){

					// Carrega a música nova e atualiza o estado
    				state->music = LoadMusicStream("music/menumusic.wav");

					PlayMusicStream(state->music);
				} else{
					StopMusicStream(state->music);
					UnloadMusicStream(state->music);
				}
			} else if (clickedIn(backButtonArea, mousePosition)){ // se usuário apertou "VOLTAR"
				state->currentScreen = INIT;
			}
			break;

		case DEVELOPERS:
			if (clickedIn(backButtonArea, mousePosition)){ // se usuário apertou "VOLTAR"
				state->currentScreen = INIT;
			}
			break;
		
		default:
			break;
	}
}

void inputAudioConfig(const int fontSize, const bool audio){

	int configOffset = 0;

	const char *configAudioName = "AUDIO ";
	DrawText(configAudioName, GetScreenWidth() / 2 - MeasureText(configAudioName, fontSize), (GetScreenHeight() - fontSize) / 2 + configOffset, fontSize, WHITE);

	if (audio == true){
		DrawText("< ON >", GetScreenWidth() / 2, (GetScreenHeight() - fontSize) / 2 + configOffset, fontSize, WHITE);
	} else{
		DrawText( "< OFF >", GetScreenWidth() / 2, (GetScreenHeight() - fontSize) / 2 + configOffset, fontSize, WHITE);
	}
}

void showInitScreen(const int fontSize){
	// tamanhos da fonte
	int titleFontSize = fontSize + 20;

	// deslocamento em relação ao centro (eixo y)
	int titleOffset = -100;
	int optOffset = 100;

	const char *title = "UNTITLED GAME";
	DrawText(title, (GetScreenWidth() - MeasureText(title, titleFontSize)) / 2, (GetScreenHeight() - titleFontSize) / 2 + titleOffset, titleFontSize, WHITE);

	const char *const options[] = {"JOGAR", 
					   			   "CONFIGURAÇÕES",
					               "DESENVOLVEDORES",
					               "SAIR"};
	for (int i = 0; i < 4; i++){
		DrawText(options[i], (GetScreenWidth() - MeasureText(options[i], fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + optOffset, fontSize, WHITE);
		optOffset += 50;
	}
}

void showConfigScreen(const int fontSize, const bool audio){
	// tamanho da fonte para o titulo
	int titleFontSize = fontSize + 10;

	// deslocamento em relação ao centro (eixo y)
	int titleOffset = -250;
	int optOffset = 250;

	const char *title = "CONFIGURAÇÕES";
	DrawText(title, (GetScreenWidth() - MeasureText(title, titleFontSize)) / 2, (GetScreenHeight() - titleFontSize) / 2 + titleOffset, titleFontSize, WHITE);

	inputAudioConfig(fontSize, audio);

	const char *option = "VOLTAR";
	DrawText(option, (GetScreenWidth() - MeasureText(option, fontSize)) / 2, (GetScreenHeight() - fontSize) / 2 + optOffset, fontSize, WHITE);
}

void showDevelopScreen(const int fontSize){
	// tamanhos da fonte
	int titleFontSize = fontSize + 10;

	// deslocamento em relação ao centro (eixo y)
	int titleOffset = -250;
	int textOffset = -150;
	int optOffset = 250;
	
	const char *title = "DESENVOLVEDORES";
	DrawText(title, (GetScreenWidth() - MeasureText(title, titleFontSize)) / 2,  (GetScreenHeight() - titleFontSize) / 2 + titleOffset, titleFontSize, WHITE);

	char *developers[] = {"EDIVALDO AMBROZIO DA SILVA FILHO",
						  "GUILHERME DE CARVALHO FABRI",
						  "GUSTAVO CRAVO TEIXEIRA FILHO",
						  "HEITOR DE ASSIS MACHADO",
						  "LUCAS MENDES PRIMO AZEVEDO",
						  "TITHO LÍVIO DUARTE MELO"};

	for (int i = 0; i < 6; i++){
		DrawText(developers[i], (GetScreenWidth() - MeasureText(developers[i], fontSize)) / 2,  (GetScreenHeight() - fontSize) / 2 + textOffset, fontSize, WHITE);
		textOffset += 50;
	}

	const char *option = "VOLTAR";
	DrawText(option, (GetScreenWidth() - MeasureText(option, fontSize)) / 2,  (GetScreenHeight() - fontSize) / 2 + optOffset, fontSize, WHITE);
}