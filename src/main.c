#include "raylib.h"
#include "resource_dir.h"  // biblioteca auxiliar criada para para garantir que arquivos como imagens, sons e fontes possam ser carregados corretamente, mesmo que o executável esteja em outro diretório
#include "HUD_e_background.h" 
#include "boss.h"
#include "MC_e_personagens.h"
#include "menu.h"
#include "balas.h"
#include "inimigos.h"

int main (){

    // ===== Configurações iniciais de janela e renderização =====
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // define largura e altura da tela
    const int screenWidth = 900;
    const int screenHeight = 900;

    // inicia a janela e o contexto OpenGL
    InitWindow(screenWidth, screenHeight, "Untitled Game");
    InitAudioDevice();

    // função utilitária de resource_dir.h para encontrar a pasta de recursos e defini-la como diretório de trabalho atual,
    // assim podemos carregar as imagens a partir dela (evita problemas caso o executável esteja em outro diretório)
    SearchAndSetResourceDir("resources");

    // volume mestre do jogo (0.0f a 1.0f)
    float volume = 1.0;
    SetMasterVolume(volume);

    // textura do menu (carregada aqui, usada pela função criar_background também)
    Texture menuImage = LoadTexture("background/menu.png"); // ultlizada a função criar_background na linha 62!!

    // ===== carregamento das músicas do jogo =====
    Music bossmusic = LoadMusicStream("music/bossmusic.wav");
    Music salainimigo = LoadMusicStream("music/salainimigo.wav");
    Music menumusic = LoadMusicStream("music/menumusic.wav");
    Music victorymusic = LoadMusicStream("music/victory.wav");

    // estado geral do jogo
    GameState state = {.currentScreen = INIT,
                        .score = 0,
                        .exit = false };

    int fontSize = 30; // usado para textos (não aparece explicitamente em todo lugar)

    // Toca música de menu no começo
    PlayMusicStream(menumusic);

    // ===== inicialização de variáveis de jogo =====
    Personagens** personagens = inicializar(); // matriz dinâmica de listas de personagens
    int qtd_entidades[5] = {1, 0, 0, 0, 0}; // vetor que armazena a quantidade de cada tipo de personagem

    int stage_sequence = 0; // 0 = Primeiro estágio; 1 = Transição_Primeiro_e_Boss; 2 = Boss;
    int cooldown = 60; // Variável para controlar o cooldown do tiro do jogador
    int timer = 0; // Timer para controlar os movimentos do boss (incrementado por frame)
    int dano_colldown = 0; // cooldown entre danos por colisão (hits do inimigo no jogador)
    double time = 0; // tempo decorrido desde o início da rodada (em segundos)
    double gameStartTime = 0; // instante (GetTime) em que a rodada começou
    bool gameplayiniciada = false; // flag para inicializar a jogabilidade apenas uma vez

    // ===== HUD HP (PLAYER): CARREGAR UMA VEZ =====  Parte 1
    // textura do preenchimento da barra de HP do player
    Texture2D hp_fill  = LoadTexture("hud/fullHPBAR.png");
    Texture2D hp_frame = (Texture2D){0};   // se tiver moldura, carregue e troque aqui

    SetTextureFilter(hp_fill, TEXTURE_FILTER_BILINEAR);
    if (hp_frame.id != 0) SetTextureFilter(hp_frame, TEXTURE_FILTER_BILINEAR);

    // ===== HUD HP (BOSS): CARREGAR UMA VEZ =====
    Texture2D boss_hp_fill = LoadTexture("hud/fullBOSSHPBAR.png");
    Texture2D boss_hp_frame = (Texture2D){0}; // opcional (moldura)

    SetTextureFilter(boss_hp_fill, TEXTURE_FILTER_BILINEAR);
    if (boss_hp_frame.id != 0) SetTextureFilter(boss_hp_frame, TEXTURE_FILTER_BILINEAR);

    // detectar áreas úteis das texturas de HP (função utilitária)
    Rectangle hp_src_area   = detectar_area_hp(hp_fill);
    Rectangle boss_src_area = detectar_area_hp(boss_hp_fill);

    //============================================================

    // ===== HUD HP  =====  Parte 2 (posicionamento e escala)
    Vector2 hp_pos;                // posição na tela da barra do player
    float   hp_scale = 0.3f;       // escala da textura (player)
    int     player_max_hp = 0;     // será definido com o HP inicial do player
    player_max_hp = personagens[0][0].HP;   // define 1x antes do loop

    // Parâmetros da barra de HP do boss
    float boss_hp_height_scale = 0.30f;  // altura relativa à área útil original
    float boss_hp_margin_x = 40.0f;  // margens laterais
    float boss_hp_margin_bottom = 24.0f;  // margem inferior (distância do rodapé)
    int   boss_max_hp = 0;      // será definido quando o boss nascer
    //============================================================

    Vector2 middle_circle; // usado na transição para o portal do boss
    Color blue = {0, 100, 255, 255};
    Texture2D textura_atual_player; // variável para guardar a textura a ser desenhada do player (dependendo da direção)

    // carregamento/criação das texturas de background usando a função utilitária criar_background
    Texture2D background = criar_background("background/menu.png");
    Texture2D arena_jogo = criar_background("background/Arena.png");
    Texture2D arena_boss = criar_background("background/Sala_Boss.png");
    Texture2D lorebackground = criar_background("background/sobra_ou_nao_sobra.png");
    Texture2D vitoriabackground = criar_background("background/vitoria.png");
    Texture2D derrotainimigosbackground = criar_background("background/sobrou_nada.png");
    Texture2D derrotabossbackground = criar_background("background/soubrou_absolutamente_nada.png");
    Texture2D alternador = background; // Variável para alternar entre os backgrounds (desenhado toda frame)

    // imagens usadas para tiros e sprites (carregadas como Image para possíveis manipulações)
    Image bala_player = LoadImage("characters/bullet.png");
    Image bala_inimigo = LoadImage("characters/enemy_bullet.png");
    Image img_N = LoadImage("characters/corredor_EAST.png");
    Image img_A = LoadImage("characters/atirador_east.png");
    Image explosao = LoadImage("characters/boom.png");


    // ===================== LOOP PRINCIPAL DO JOGO =====================
    while (!WindowShouldClose() && !state.exit){

        // atualiza o stream de música correto dependendo da tela/estado atual
        if(state.currentScreen == PLAY){
            if(stage_sequence < 1){
                UpdateMusicStream(salainimigo);
            }
            else { // stage_sequence == 2
                UpdateMusicStream(bossmusic);
            }
        } else if(state.currentScreen  == VICTORY){
            UpdateMusicStream(victorymusic);
        } else { // se não for gameplay, é menu/config/etc
            UpdateMusicStream(menumusic);
        } 

        BeginDrawing();
        ClearBackground(RAYWHITE); // limpa o background
        DrawTexture(alternador, 0, 0, WHITE); // desenha o background atual

        // switch principal para gerenciar telas/estados do jogo
        switch(state.currentScreen){
            case INIT:
                // caso inicial (menu)

                // se a música de vitória estiver tocando, parar (garantia)
                if(IsMusicStreamPlaying(victorymusic)){
                    StopMusicStream(victorymusic);
                }

                // se uma jogada anterior estava ativa, destruir o personagem e resetar
                if (gameplayiniciada == true)
                {
                    destruir_personagem(personagens[0][0]);
                    if(stage_sequence == 2){
                        destruir_personagem(personagens[4][0]);
                    }
                    PlayMusicStream(menumusic);
                    gameplayiniciada = false;
                }

                alternador = background; // mostra background de menu
                showInitScreen(&state); // função que desenha e processa o menu inicial

                // reseta alguns estados relacionados ao tempo e score para a tela inicial
                gameStartTime = 0; 
                state.score = 0;
                break;   

            case PLAY: // rodar o jogo aqui (chamar a função para rodar o jogo)

                //============================================================ HUD
                // posicionamento automático da barra de HP do player (auto-centraliza se x = -1)
                hp_pos.x = -1.0f;   // auto-centraliza dentro da função
                hp_pos.y = 30.0f;   // altura desejada

                // HP máximo do player = HP inicial (já definido antes)

                // zera max HP do boss; vai ser setado quando nascer
                // boss_max_hp = 0;
                //============================================================

                // inicialização da jogabilidade (executada apenas uma vez por rodada)
                if (gameplayiniciada == false) // condição inicial que reinicia os timers e troca a musica 
                {
                    stage_sequence = 0;
                    time = 0;
                    timer = 0;
                    StopMusicStream(menumusic);
                    PlayMusicStream(salainimigo);

                    // cria o personagem jogador na posição central
                    personagens[0][0] = criar_personagem('E', (GetScreenWidth() / 2) - 25, (GetScreenHeight() / 2) - 50, 
                                        LoadImage("characters/Robo_Gladiador_NORTH.png"),
                                        LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                                        LoadImage("characters/Robo_Gladiador_WEST.png"),
                                        LoadImage("characters/Robo_Gladiador_EAST.png"), 
                                        10, 3, 50, 50);
                    
                    // zera a contagem de entidades dos tipos 1..4 (exceto o player)
                    for (int i = 1; i < 5; i++)
                    {
                        qtd_entidades[i] = 0; // zera a contagem de todos os personagens
                    }
                    qtd_entidades[0] = 1; // conta do player
                    gameplayiniciada = true;

                    // recarrega as texturas de HUD (padrão)
                    hp_fill  = LoadTexture("hud/fullHPBAR.png");
                    hp_frame = (Texture2D){0};   // se tiver moldura, carregue e troque aqui
                    SetTextureFilter(hp_fill, TEXTURE_FILTER_BILINEAR);
                    if (hp_frame.id != 0) SetTextureFilter(hp_frame, TEXTURE_FILTER_BILINEAR);
                    boss_hp_fill = LoadTexture("hud/fullBOSSHPBAR.png");
                    boss_hp_frame = (Texture2D){0}; // opcional (moldura)
                    SetTextureFilter(boss_hp_fill, TEXTURE_FILTER_BILINEAR);
                    if (boss_hp_frame.id != 0) SetTextureFilter(boss_hp_frame, TEXTURE_FILTER_BILINEAR);
                    hp_src_area   = detectar_area_hp(hp_fill);
                    boss_src_area = detectar_area_hp(boss_hp_fill);

                    // reinicia parâmetros visuais e de HP
                    hp_scale = 0.3f;   
                    player_max_hp = personagens[0][0].HP; 

                    boss_hp_height_scale = 0.30f;
                    boss_hp_margin_x = 40.0f; 
                    boss_hp_margin_bottom = 24.0f;  
                    boss_max_hp = 0;     
                }

                // ===== tempo de jogo =====
                if (gameStartTime == 0)
                {
                    gameStartTime = GetTime(); // captura o tempo inicial uma única vez
                }

                time = GetTime() - gameStartTime; // calcula o tempo decorrido no jogo

                // incrementa cooldown do tiro do player
                cooldown++;

                // durante os primeiros 120 segundos (2 minutos) e stage_sequence == 0, spawna inimigos periodicamente
                if (time < 120 && stage_sequence == 0)
                {
                    alternador = arena_jogo; // background da arena normal
                    int time_spawn = 2; // intervalo de spawn em segundos

                    // lógica para spawnar apenas uma vez por segundo (evita múltiplos spawns no mesmo segundo)
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

                // move inimigos do tipo corredor
                for(int i = 0;i<qtd_entidades[1];i++){ // move o inimigo corredor
                    mover_inimigo(&personagens[1][i], &personagens[0][0]);
                }
                // move inimigos do tipo atirador
                for(int i = 0;i<qtd_entidades[2];i++){ // move o inimigo atirador
                    mover_atirador(&personagens[2][i]);
                }

                // decrementa cooldown de dano por colisão
                if (dano_colldown > 0) dano_colldown--;

                // verifica colisões entre player e corredores e aplica dano com cooldown
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

                // processamento do jogador: movimentação e tiro
                mover_player(personagens[0]);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cooldown >= 30)
                {
                    cooldown = 0; // Reseta o cooldown
                    atirar(&personagens[0][0], bala_player);
                }

                // transição para o evento do boss (após 120s)
                if (time >= 120 && stage_sequence == 0) // !!!!!!!!!! BOSS_TIME
                {
                    destruir_inimigos(personagens, &qtd_entidades); // destrói os inimigos
                    stage_sequence = 1; // entra na sequência de transição
                    middle_circle.x =(GetScreenWidth() / 2.0f);
                    middle_circle.y = (GetScreenHeight() / 2.0f) - 5;
                    StopMusicStream(salainimigo);
                }

                // se o jogador entrar no círculo do portal, inicia o boss
                if (stage_sequence == 1 && CheckCollisionCircleRec(middle_circle, 30, personagens[0][0].hitbox))
                {
                    // lembrar de limpar todos os inimigos da tela (já feito anteriormente)
                    PlayMusicStream(bossmusic);
                    stage_sequence = 2; // entra no estágio do boss
                    alternador = arena_boss;
                    time = 0; // reinicia contador para a fase do boss
                }

                // lógica do boss quando em stage_sequence == 2
                if (stage_sequence == 2)
                {
                    if (timer == 0) { // cria o boss e destrói os inimigos (apenas na primeira iteração)
                        personagens[4][0] = criar_boss('S', (GetScreenWidth() / 2), (GetScreenHeight() / 2) + 70,
                                                LoadImage("characters/BOSS.png"),
                                                LoadImage("characters/BOSS.png"),
                                                LoadImage("characters/BOSS.png"),
                                                LoadImage("characters/BOSS.png"));

                        qtd_entidades[4] = 1; // Atualiza a quantidade de entidades do tipo boss
                        boss_max_hp = personagens[4][0].HP;   // guarda HP máximo do boss para HUD

                        if (boss_max_hp <= 0) boss_max_hp = 1;

                    }
                    
                    // movimentação e ataques do boss (função controla spawns auxiliares e tiros)
                    movimentacao_boss(&personagens[4][0], &personagens[0][0], bala_inimigo, &timer, personagens, &qtd_entidades, img_N, img_A);
                    timer++;

                    // se o boss estiver com HP baixo, começa a spawnar inimigos de suporte periodicamente
                    if (personagens[4][0].HP <= 20) { // trigger para começar a spawnar bixo
                        if (timer % 180 == 0 && (qtd_entidades[1] + qtd_entidades[2]) < 3) { // A cada 3 segundos
                            spawnador(personagens, &qtd_entidades, img_A, img_A, img_A, img_A, 2);
                        }
                    }
                }

                // ================== DESENHOS ==================
                // escolhe qual textura desenhar com base no sentido atual do player
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
                
                // desenhos específicos por fase da sequência
                switch (stage_sequence)
                {
                    case 0:
                        DrawText(TextFormat("%d : %02d", ((int)time) / 60, ((int)time) % 60), 10, 10, 50, WHITE);
                        DrawText("MISSÃO: SOBREVIVER POR 2 MINUTOS!", 10, GetScreenHeight() - 50, 30, WHITE);
                        // desenhando inimigos
                        desenhar_inimigos(personagens, &qtd_entidades);
                        break;

                    case 1:
                        DrawCircleV(middle_circle, 30, blue);
                        DrawText("MISSÃO: ENTRE NO PORTAL PARA ENFRENTAR O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                        break;

                    case 2:
                        if (personagens[0][0].HP == 0) {
                            DrawText("MORREU ZÉ", 10, GetScreenHeight() - 50, 30, WHITE); // só p testar
                        }else {
                            DrawText("MISSÃO: DERROTE O BOSS!", 10, GetScreenHeight() - 100, 30, WHITE);
                        }
                        desenhar_boss(&personagens[4][0]);
                        desenhar_inimigos(personagens, &qtd_entidades);
                                   
                        if (qtd_entidades[4] > 0 && boss_max_hp > 0)
                        {       
                            // Desenha a barra de HP do boss usando a função auxiliar
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

                // verifica condição de derrota (player morreu)
                if(personagens[0][0].HP == 0){ // condição para o jogador perder e ir para a tela de derrota 
                    state.currentScreen = DEFEAT;
                }

                // verifica condição de vitória (boss derrotado)
                if(stage_sequence == 2 && personagens[4][0].HP == 0){ // condição para o jogador ganhar e ir para a tela de vitoria 
                    state.currentScreen = VICTORY;
                }

                // desenha o player na tela (posição baseada na hitbox)
                DrawTextureV(textura_atual_player, (Vector2){personagens[0][0].hitbox.x, personagens[0][0].hitbox.y}, WHITE);
                desenhar_score(state.score);

                // move e desenha as balas (tanto do player quanto dos inimigos)
                mover_balas(personagens, &qtd_entidades, &state ,bala_inimigo, explosao);
                break;
                
            case CONFIGURATIONS:
                alternador = background;
                showConfigScreen(&state, &volume);
                gameStartTime = 0;
                state.score = 0;
                break;
                
            case DEVELOPERS:
                alternador = background;
                showDevelopScreen(&state);
                gameStartTime = 0;
                state.score = 0;
                break;
            case LORE:
                alternador = lorebackground;
                showLoreScreen(&state);
                gameStartTime = 0;
                state.score = 0;
                break;
            case VICTORY:
                // ao entrar na tela de vitória, garantir que a música do boss pare e tocar a música de vitória
                if(IsMusicStreamPlaying(bossmusic)){
                    StopMusicStream(bossmusic);
                    PlayMusicStream(victorymusic);
                } 
                showVictoryScreen(&state);
                gameStartTime = 0;
                
                	break;
            case DEFEAT:
                // ao entrar na tela de derrota, parar músicas ativas relacionadas ao gameplay
                if(IsMusicStreamPlaying(bossmusic)) StopMusicStream(bossmusic);
                if(IsMusicStreamPlaying(salainimigo)) StopMusicStream(salainimigo);

                if(stage_sequence == 2){
                    alternador = derrotabossbackground;
                } else {
                    alternador = derrotainimigosbackground;
                }

                showGameoverScreen(&state);
                gameStartTime = 0;
                
                	break;
            default:
                break;
			}
		EndDrawing();

	}

	// ===================== LIMPEZA E LIBERAÇÃO DE RECURSOS ====================
    limpeza_final(
        personagens,
        qtd_entidades,
        stage_sequence,
        bala_player,
        bala_inimigo,
        background,
        arena_jogo,
        arena_boss,
        lorebackground,
        vitoriabackground,
        derrotainimigosbackground,
        derrotabossbackground,
        menuImage,
        hp_fill,
        hp_frame,
        boss_hp_fill,
        boss_hp_frame,
        bossmusic,
        salainimigo,
        menumusic,
        victorymusic
    );

	return 0;
}
