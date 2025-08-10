<<<<<<< Updated upstream
/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("characters/wabbit_alpha.png");
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib", 200,200,20,WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
=======
#include "funcoe_utilizadas.h" // A única inclusão necessária para suas funções
#include "resource_dir.h"

int main (void)
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    Personagens** personagens = inicializar();
    InitAudioDevice();
    SearchAndSetResourceDir("resources");

    Music bossmusic = LoadMusicStream("musics/bossmusic.wav");
    Music salainimigo = LoadMusicStream("musics/salainimigo.wav");
    Music menumusic = LoadMusicStream("musics/menumusic.wav");

    // variaveis de estados e assets
    GameScreen tela_atual = MENU;
    float volume = 1.0;
    SetMasterVolume(volume);

    Texture2D menubackground = LoadTexture("background/Menu.png");
    Texture2D imagemcreditos = LoadTexture("background/creditos.png");
    Texture2D mortebackground = LoadTexture("background/morte.png");
    Texture2D vitoriabackground = LoadTexture("background/vitoria.png");
    
    Texture2D background;
    int qtd_entidades[5] = {0}; // Quantidade de entidades de cada tipo
    int stage_sequence = 0; // 0 = Primeiro estágio; 1 = Transição_Primeiro_e_Boss; 2 = Boss;
    double time = 0;
    Vector2 middle_circle;
    Color blue = {0, 100, 255, 255};
    Texture2D textura_atual_player; // Variável para guardar a textura a ser desenhada do player
    Image bala_image;
    int cooldown = 60; // Variável para controlar o cooldown do tiro
    int timer = 0; // Timer para controlar os movimentos do boss
    bool gameplayiniciada = false;
    PlayMusicStream(menumusic);

    while (WindowShouldClose() == false)
    {
        if(tela_atual == GAMEPLAY){
            if(stage_sequence <= 1){
                UpdateMusicStream(salainimigo);
            }
            else { // stage_sequence == 2
                UpdateMusicStream(bossmusic);
            }
        } else { // Se não for gameplay, é menu
            UpdateMusicStream(menumusic);
        }
        switch(tela_atual)
        {
            case MENU:
            {
                if (gameplayiniciada == true)
                {
                    destruir_personagem(personagens[0][0]);
                    if(stage_sequence==2){
                        destruir_personagem(personagens[4][0]);
                    }
                    gameplayiniciada = false;
                }
                tela_menu(&tela_atual, menubackground);
            } break;
            
            case OPTIONS:
            {
                tela_opcoes(&tela_atual, menubackground, &volume);
            } break;
            
            case CREDITOS:
            {
                tela_creditos(&tela_atual, imagemcreditos);
            } break;
            
            case GAMEPLAY:
            {
                // Inicializa as variáveis de gameplay apenas uma vez
                if (gameplayiniciada == false)
                {
                    stage_sequence = 0;
                    time = 0;
                    timer = 0;
                    StopMusicStream(menumusic);
                    PlayMusicStream(salainimigo);
                    
                    for (int i = 0; i < 5; i++)
                    {
                        qtd_entidades[i] = 0; // zera a contagem de todos os personagens
                    }
                    background = criar_background("background/Arena.png");
                    /*
                    EXPLICANDO O VETOR DE VETORES personagens
                    personagens[0] vai conter somente o vetor do player
                    personagens[1] vai conter o vetor de todos os inimigos corredores
                    personagens[2] vai conter o vetor de todos os inimigos atiradores horizontais
                    personagens[3] vai conter o vetor de todos os inimigos atiradores verticais
                    personagens[4] vai conter somente o vetor do boss 
                    */
                    personagens[0][0] = criar_personagem('E', (GetScreenWidth() / 2) - 25, (GetScreenHeight() / 2) - 50, 
                                                        LoadImage("characters/Robo_Gladiador_NORTH.png"),
                                                        LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                                                        LoadImage("characters/Robo_Gladiador_WEST.png"),
                                                        LoadImage("characters/Robo_Gladiador_EAST.png"), 
                                                        3, 3, 50, 50);

                    qtd_entidades[0] = 1;

                    bala_image = LoadImage("characters/wabbit_alpha.png");
                    gameplayiniciada = true;
                }

                cooldown++;
                //TEMPO
                if (time < 120 && stage_sequence == 0)
                {
                    time = GetTime();
                }

                //EVENT HANDLING
                mover_player(personagens[0]);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cooldown >= 60)
                {
                    cooldown = 0; // Reseta o cooldown
                    atirar(&personagens[0][0], bala_image);
                }
            
                if (time >= 10 && stage_sequence == 0)
                {
                    stage_sequence = 1;
                    middle_circle.x =(GetScreenWidth() / 2.0f);
                    middle_circle.y = (GetScreenHeight() / 2.0f) - 5;
                    StopMusicStream(salainimigo);
                }
                if (stage_sequence == 1 && CheckCollisionCircleRec(middle_circle, 30, personagens[0][0].hitbox))
                {
                    //Lembrar de limpar todos os inimigos da tela
                    stage_sequence = 2;
                    UnloadTexture(background); // Descarrega o background antigo
                    background = criar_background("background/Sala_Boss.png");
                    time = 0;

                    PlayMusicStream(bossmusic);
                }
                if (stage_sequence == 2)
                {
                    if (timer == 0) { //cria o boss
                        personagens[4][0] = criar_boss('S', (GetScreenWidth() / 2), (GetScreenHeight() / 2) + 70,
                                                LoadImage("characters/Robo_Gladiador_NORTH.png"),
                                                LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                                                LoadImage("characters/Robo_Gladiador_WEST.png"),
                                                LoadImage("characters/Robo_Gladiador_EAST.png"));

                        qtd_entidades[4] = 1; // Atualiza a quantidade de entidades do tipo boss
                    }
                    movimentacao_boss(&personagens[4][0], &personagens[0][0], bala_image, &timer);
                    timer++;
                }

                // DRAWING
                BeginDrawing();
                // Escolhe qual textura desenhar com base no sentido atual do player
                switch (personagens[0][0].sentido)
                {
                    case 'N': 
                        textura_atual_player = personagens[0][0].sprite_N; 
                        break;
                    case 'S': 
                        textura_atual_player = personagens[0][0].sprite_S; 
                        break;
                    case 'W': 
                        textura_atual_player = personagens[0][0].sprite_W; 
                        break;
                    case 'E': 
                        textura_atual_player = personagens[0][0].sprite_E; 
                        break;
                }

                ClearBackground(WHITE);
                DrawTexture(background, 0, 0, WHITE);
                switch (stage_sequence)
                {
                    case 0:
                        DrawText(TextFormat("%d : %02d", ((int)time) / 60, ((int)time) % 60), 10, 10, 50, WHITE);
                        DrawText("MISSÃO: SOBREVIVER POR 2 MINUTOS!", 10, GetScreenHeight() - 50, 30, WHITE);
                        break;
                    case 1:
                        DrawCircleV(middle_circle, 30, blue);
                        DrawText("MISSÃO: ENTRE NO PORTAL PARA ENFRENTAR O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                        break;
                    case 2:
                        if (personagens[0][0].HP == 0) {
                            DrawText("MORREU ZÉ", 10, GetScreenHeight() - 50, 30, WHITE); //só p testar
                            background = criar_background("background/foxe.png");
                        } else {
                            DrawText("MISSÃO: DERROTE O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                        }
                        desenhar_boss(&personagens[4][0]);

                        if (personagens[4][0].HP <= 0){
                            tela_atual = VITORIA;           // vitoria do personagem redireciona pra tela de vitoria
                        }
                        break;
                }
                
                DrawTextureV(textura_atual_player, (Vector2){personagens[0][0].hitbox.x, personagens[0][0].hitbox.y}, WHITE);
                mover_balas(personagens, &qtd_entidades);
                EndDrawing();

                if (personagens[0][0].HP <= 0){ // morte do personagem redireciona pra tela de gameover
                    tela_atual = MORTE;
                }

            } break;

            case MORTE:
            {
                if(IsMusicStreamPlaying(salainimigo)) StopMusicStream(salainimigo);
                if(IsMusicStreamPlaying(bossmusic)) StopMusicStream(bossmusic);
                tela_morte(&tela_atual, mortebackground);
                PlayMusicStream(menumusic);
            } break;

            case VITORIA:
            {
                if(IsMusicStreamPlaying(salainimigo)) StopMusicStream(salainimigo);
                if(IsMusicStreamPlaying(bossmusic)) StopMusicStream(bossmusic);
                tela_vitoria(&tela_atual, vitoriabackground);
                PlayMusicStream(menumusic);
            } break;
            
            default: break;
        }
    }
    
    // LIMPEZA FINAL (é bom fazer uma função disso no futuro)
    if (gameplayiniciada) {
        destruir_personagem(personagens[0][0]);
        if(stage_sequence==2){
            destruir_personagem(personagens[4][0]);
        }
    }
    UnloadTexture(background);
    UnloadImage(bala_image);

    UnloadTexture(menubackground);
    UnloadTexture(imagemcreditos);
    UnloadTexture(mortebackground);
    UnloadTexture(vitoriabackground);

    UnloadMusicStream(menumusic);
    UnloadMusicStream(salainimigo);
    UnloadMusicStream(bossmusic);

    free(personagens[0]);
    free(personagens);
    
    CloseAudioDevice(); 
    CloseWindow();

    return 0;
}
>>>>>>> Stashed changes
