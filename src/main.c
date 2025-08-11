/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/
*/

#include "raylib.h"
#include "resource_dir.h"   // biblioteca auxiliar criada para para garantir que arquivos como imagens, sons e fontes possam ser carregados corretamente, mesmo que o executável esteja em outro diretório
#include "funcoe_utilizadas.h" // A única inclusão necessária para suas funções

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

    Texture menuImage = LoadTexture("background/menu.png"); //ultilizada a função criar_background na linha 62!!

    Music menuMusic = LoadMusicStream("music/menumusic.wav");

    GameState state = {.currentScreen = INIT,
                        .audio = true,
                        .music = menuMusic,
                        .score = 0,
                        .exit = false };
    int fontSize = 30;

    PlayMusicStream(state.music);

    //inicialização de variáveis do jogo
    Personagens** personagens = inicializar();
    personagens[0][0] = criar_personagem('E', (GetScreenWidth() / 2) - 25, (GetScreenHeight() / 2) - 50, 
                                        LoadImage("characters/Robo_Gladiador_NORTH.png"),
                                        LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                                        LoadImage("characters/Robo_Gladiador_WEST.png"),
                                        LoadImage("characters/Robo_Gladiador_EAST.png"), 
                                        10, 3, 50, 50);
    int qtd_entidades[5] = {1, 0, 0, 0, 0}; //vetor que armazena a quantidade de cada tipo de personagem

    int stage_sequence = 0; // 0 = Primeiro estágio; 1 = Transição_Primeiro_e_Boss; 2 = Boss;
    int cooldown = 60; // Variável para controlar o cooldown do tiro
    int timer = 0; // Timer para controlar os movimentos do boss
    int dano_colldown = 0;
    double time = 0;
    double gameStartTime = 0;


     // ===== HUD HP (PLAYER): CARREGAR UMA VEZ =====  Parte 1
    // >>> COLOQUE AQUI O CAMINHO DA PNG DO PLAYER (fill) <<<
    // Ex.: "hud/fullHPBAR.png"
    Texture2D hp_fill  = LoadTexture("hud/fullHPBAR.png");
    Texture2D hp_frame = (Texture2D){0};   // se tiver moldura, carregue e troque aqui

    SetTextureFilter(hp_fill, TEXTURE_FILTER_BILINEAR);
    if (hp_frame.id != 0) SetTextureFilter(hp_frame, TEXTURE_FILTER_BILINEAR);

    // ===== HUD HP (BOSS): CARREGAR UMA VEZ =====
    // >>> COLOQUE AQUI O CAMINHO DA PNG DO BOSS <<<
    Texture2D boss_hp_fill = LoadTexture("hud/fullBOSSHPBAR.png");
    Texture2D boss_hp_frame = (Texture2D){0}; // opcional (moldura)

    SetTextureFilter(boss_hp_fill, TEXTURE_FILTER_BILINEAR);
    if (boss_hp_frame.id != 0) SetTextureFilter(boss_hp_frame, TEXTURE_FILTER_BILINEAR);

    Rectangle hp_src_area   = detectar_area_hp(hp_fill);
    Rectangle boss_src_area = detectar_area_hp(boss_hp_fill);

    

    //============================================================






     //===== HUD HP  =====  Parte 2
    //============================================================
    // HUD de HP (posicionamento e escala - player)
    Vector2 hp_pos;                // posição na tela
    float   hp_scale = 0.3f;       // escala da textura (player)
    int     player_max_hp = 0;     // define depois (com HP inicial do player)
    player_max_hp = personagens[0][0].HP;   // define 1x antes do loop

    // HUD do Boss (parâmetros)
    float boss_hp_height_scale = 0.30f;  // altura relativa à área útil original
    float boss_hp_margin_x     = 40.0f;  // margens laterais
    float boss_hp_margin_bottom= 24.0f;  // margem inferior (distância do rodapé)
    int   boss_max_hp          = 0;      // define quando o boss nasce
    //============================================================








    Vector2 middle_circle;
    Color blue = {0, 100, 255, 255};
    Texture2D textura_atual_player; // Variável para guardar a textura a ser desenhada do player
    Texture2D background = criar_background("background/menu.png");
    Texture2D arena_jogo = criar_background("background/Arena.png");
    Texture2D arena_boss = criar_background("background/Sala_Boss.png");
    Texture2D alternador = background; // Variável para alternar entre os backgrounds
        
    Image bala_player = LoadImage("characters/bullet.png");
    Image bala_inimigo = LoadImage("characters/enemy_bullet.png");
    Image img_N = LoadImage("characters/corredor_EAST.png");
    Image img_A = LoadImage("characters/atirador_east.png");
    Image explosao = LoadImage("characters/boom.png");




    //Gameplay loop
    while (!WindowShouldClose() && !state.exit){

        if (state.audio == true) {
            UpdateMusicStream(state.music);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE); // limpa o background
        DrawTexture(alternador, 0, 0, WHITE); // carrega imagem do menu

        switch(state.currentScreen){
            case INIT:
                alternador = background;
                showInitScreen(fontSize);
                gameStartTime = 0; 
                state.score = 0;
                break;   
            case PLAY: // rodar o jogo aqui (chamar a função para rodar o jogo)

                //============================================================ HUD
                // HUD Player: auto-centraliza (x = -1) e posiciona no topo
                hp_pos.x = -1.0f;   // auto-centraliza dentro da função
                hp_pos.y = 30.0f;   // altura desejada

                // HP máximo do player = HP inicial
                

                // zera max HP do boss; vai ser setado quando nascer
                //   boss_max_hp = 0;
                //============================================================







                //TEMPO
                if (gameStartTime == 0)
                {
                    gameStartTime = GetTime(); // Captura o tempo inicial uma única vez
                }

                time = GetTime() - gameStartTime; // Calcula o tempo decorrido no jogo

                cooldown++;
                if (time < 120 && stage_sequence == 0)
                {
                    alternador = arena_jogo;
                    int time_spawn = 2;
                    if(((int)time % time_spawn == 0) && (time > 0) && (((int)time != ((int)(time - GetFrameTime()))))) // Evita spawns múltiplos no mesmo segundo
                    {
                        if (qtd_entidades[1] + qtd_entidades[2] < 8)
                        {
                            spawnador(personagens, &qtd_entidades, img_N, img_N, img_N, img_N, 1);
                        }
                        if (qtd_entidades[1] + qtd_entidades[2] < 8)
                        {
                            spawnador(personagens, &qtd_entidades, img_A, img_A, img_A, img_A, 2);
                        }
                    }
                    
                }

                for(int i = 0;i<qtd_entidades[1];i++){ //move o inimigo corredor
                    mover_inimigo(&personagens[1][i], &personagens[0][0]);
                }
                for(int i = 0;i<qtd_entidades[2];i++){ //move o inimigo atirador
                    mover_atirador(&personagens[2][i]);
                }
                if (dano_colldown > 0) dano_colldown--;

                for (int i = 0; i < qtd_entidades[1]; i++) {
                    if (CheckCollisionRecs(personagens[0][0].hitbox, personagens[1][i].hitbox) && dano_colldown == 0) {
                        personagens[0][0].HP--;
                        dano_colldown = 60;
                        if(personagens[0][0].HP < 0){
                            personagens[0][0].HP = 0;
                            destruir_personagem(personagens[0][0]);
                        } 
                    }
                }

            
                
                mover_player(personagens[0]);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cooldown >= 60)
                {
                    cooldown = 0; // Reseta o cooldown
                    atirar(&personagens[0][0], bala_player);
                }
            
                if (time >= 12 && stage_sequence == 0) //!!!!!!!!!!!!!!!! BOSS_TIME
                {
                    destruir_inimigos(personagens, &qtd_entidades); //destrói os inimigos
                    stage_sequence = 1;
                    middle_circle.x =(GetScreenWidth() / 2.0f);
                    middle_circle.y = (GetScreenHeight() / 2.0f) - 5;
                }

                if (stage_sequence == 1 && CheckCollisionCircleRec(middle_circle, 30, personagens[0][0].hitbox))
                {
                    //Lembrar de limpar todos os inimigos da tela
                    stage_sequence = 2;
                    alternador = arena_boss;
                    time = 0;
                }
                if (stage_sequence == 2)
                {
                    if (timer == 0) { //cria o boss e destrói os inimigos
                        personagens[4][0] = criar_boss('S', (GetScreenWidth() / 2), (GetScreenHeight() / 2) + 70,
                                                LoadImage("characters/BOSS.png"),
                                                LoadImage("characters/BOSS.png"),
                                                LoadImage("characters/BOSS.png"),
                                                LoadImage("characters/BOSS.png"));

                        qtd_entidades[4] = 1; // Atualiza a quantidade de entidades do tipo boss
                        boss_max_hp = personagens[4][0].HP;   // ***********

                        if (boss_max_hp <= 0) boss_max_hp = 1;


                        /*
                        //============================================================ HUD
                        // ===== HUD: BARRA DE HP DO BOSS (rodapé, esticada) =====
                        
                        desenhar_barra_boss(&personagens[4][0], boss_max_hp,
                        boss_hp_fill, boss_hp_frame,
                        boss_hp_margin_x, boss_hp_margin_bottom,
                        boss_hp_height_scale,
                        boss_src_area);
                        
                        //============================================================
                        */
                       

                    }
                    
                    movimentacao_boss(&personagens[4][0], &personagens[0][0], bala_inimigo, &timer, personagens, &qtd_entidades, img_N, img_A);
                    timer++;

                    if (personagens[4][0].HP <= 1) { //trigger para começar a spawnar bixo
                        if (timer % 180 == 0 && (qtd_entidades[1] + qtd_entidades[2]) < 3) { // A cada 3 segundos
                            spawnador(personagens, &qtd_entidades, img_A, img_A, img_A, img_A, 2);
                        }
                    }
                }

                // DRAWING
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
                
                switch (stage_sequence)
                {
                    case 0:
                    DrawText(TextFormat("%d : %02d", ((int)time) / 60, ((int)time) % 60), 10, 10, 50, WHITE);
                    DrawText("MISSÃO: SOBREVIVER POR 2 MINUTOS!", 10, GetScreenHeight() - 50, 30, WHITE);
                    //desenhando inimigos
                    desenhar_inimigos(personagens, &qtd_entidades);
                    break;
                    case 1:
                    DrawCircleV(middle_circle, 30, blue);
                    DrawText("MISSÃO: ENTRE NO PORTAL PARA ENFRENTAR O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                    break;
                    case 2:
                    if (personagens[0][0].HP == 0) {
                        DrawText("MORREU ZÉ", 10, GetScreenHeight() - 50, 30, WHITE); //só p testar
                    }else {
                        DrawText("MISSÃO: DERROTE O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                    }
                    desenhar_boss(&personagens[4][0]);
                    desenhar_inimigos(personagens, &qtd_entidades);
                    //*******************
                               
                    if (qtd_entidades[4] > 0 && boss_max_hp > 0)
                    {       
                        desenhar_barra_boss(&personagens[4][0], boss_max_hp,
                        boss_hp_fill, boss_hp_frame,
                        boss_hp_margin_x, boss_hp_margin_bottom,
                        boss_hp_height_scale,
                        boss_src_area);
                    }



                    break;
                }
                


                //============================================================ HUD
                // ===== HUD: BARRA DE HP (PLAYER) =====
                desenhar_barra_hp(&personagens[0][0], player_max_hp,
                hp_fill, hp_frame, hp_pos, hp_scale,
                hp_src_area);
                //============================================================






                DrawTextureV(textura_atual_player, (Vector2){personagens[0][0].hitbox.x, personagens[0][0].hitbox.y}, WHITE);
                desenhar_score(state.score);
                mover_balas(personagens, &qtd_entidades, &state ,bala_inimigo, explosao);
                break;
                
            case CONFIGURATIONS:
                alternador = background;
                showConfigScreen(fontSize, state.audio);
                gameStartTime = 0;
                state.score = 0;
                break;
                
            case DEVELOPERS:
                alternador = background;
                showDevelopScreen(fontSize);
                gameStartTime = 0;
                state.score = 0;
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

    //============================================================
    // HUDs 
    UnloadTexture(hp_fill);
    if (hp_frame.id != 0) UnloadTexture(hp_frame);

    UnloadTexture(boss_hp_fill);
    if (boss_hp_frame.id != 0) UnloadTexture(boss_hp_frame);
    //============================================================


	UnloadTexture(menuImage);

	CloseAudioDevice(); // fecha o sistema de áudio
	CloseWindow();	// destrói a janela e limpa o contexto OpenGL
	
	return 0;
}
