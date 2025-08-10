#include "funcoe_utilizadas.h" // A única inclusão necessária para suas funções
#include "resource_dir.h"

int main (void)
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    Personagens** personagens = inicializar();
    SearchAndSetResourceDir("resources");

    // variaveis de estados e assets
    GameScreen tela_atual = MENU;
    float volume = 1.0f;
    SetMasterVolume(volume);

    Texture2D menubackground = LoadTexture("background/Menu.png");
    Texture2D imagemcreditos = LoadTexture("background/creditos.png");
    Texture2D mortebackground = LoadTexture("background/morte.png");
    Texture2D vitoriabackground = LoadTexture("background/vitoria.png");

    // ===== HUD HP (PLAYER): CARREGAR UMA VEZ =====
    // >>> COLOQUE AQUI O CAMINHO DA PNG DO PLAYER (fill) <<<
    // Ex.: "hud/fullHPBAR.png"
    Texture2D hp_fill  = LoadTexture("hud/fullHPBAR.png");
    Texture2D hp_frame = (Texture2D){0};   // se tiver moldura, carregue e troque aqui

    SetTextureFilter(hp_fill, TEXTURE_FILTER_BILINEAR);
    if (hp_frame.id != 0) SetTextureFilter(hp_frame, TEXTURE_FILTER_BILINEAR);

    // ===== HUD HP (BOSS): CARREGAR UMA VEZ =====
    // >>> COLOQUE AQUI O CAMINHO DA PNG DO BOSS (fill) <<<
    Texture2D boss_hp_fill = LoadTexture("hud/fullBOSSHPBAR.png");
    Texture2D boss_hp_frame = (Texture2D){0}; // opcional (moldura)

    SetTextureFilter(boss_hp_fill, TEXTURE_FILTER_BILINEAR);
    if (boss_hp_frame.id != 0) SetTextureFilter(boss_hp_frame, TEXTURE_FILTER_BILINEAR);

    Rectangle hp_src_area   = detectar_area_hp(hp_fill);
    Rectangle boss_src_area = detectar_area_hp(boss_hp_fill);

    Texture2D background;          // carrega quando gameplay inicia
    bool backgroundCarregado = false;

    int qtd_entidades[5] = {0};    // Quantidade de entidades de cada tipo
    int stage_sequence = 0;        // 0 = Primeiro estágio; 1 = Transição; 2 = Boss;
    double time = 0;
    Vector2 middle_circle;
    Color blue = {0, 100, 255, 255};
    Texture2D textura_atual_player;
    Image bala_image;              // imagem-base para a bala
    bool balaImageCarregada = false;

    // HUD de HP (posicionamento e escala - player)
    Vector2 hp_pos;                // posição na tela
    float   hp_scale = 0.3f;       // escala da textura (player)
    int     player_max_hp = 0;     // define depois (com HP inicial do player)

    // HUD do Boss (parâmetros)
    float boss_hp_height_scale = 0.30f;  // altura relativa à área útil original
    float boss_hp_margin_x     = 40.0f;  // margens laterais
    float boss_hp_margin_bottom= 24.0f;  // margem inferior (distância do rodapé)
    int   boss_max_hp          = 0;      // define quando o boss nasce

    int cooldown = 60;             // cooldown do tiro
    int timer = 0;                 // timer dos padrões do boss
    bool gameplayiniciada = false;

    // loop principal
    while (WindowShouldClose() == false)
    {
        switch (tela_atual)
        {
            case MENU:
            {
                if (gameplayiniciada == true)
                {
                    destruir_personagem(personagens[0][0]);
                    if (stage_sequence == 2) destruir_personagem(personagens[4][0]);
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

                    for (int i = 0; i < 5; i++) qtd_entidades[i] = 0;

                    background = criar_background("background/Arena.png");
                    backgroundCarregado = true;

                    /*
                    personagens[0] -> vetor do player
                    personagens[1] -> corredores
                    personagens[2] -> atiradores horizontais
                    personagens[3] -> atiradores verticais
                    personagens[4] -> boss
                    */
                    personagens[0][0] = criar_personagem('E',
                        (GetScreenWidth() / 2) - 25,
                        (GetScreenHeight() / 2) - 50,
                        LoadImage("characters/Robo_Gladiador_NORTH.png"),
                        LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                        LoadImage("characters/Robo_Gladiador_WEST.png"),
                        LoadImage("characters/Robo_Gladiador_EAST.png"),
                        3, 3.0f, 50, 50);

                    qtd_entidades[0] = 1;

                    bala_image = LoadImage("characters/wabbit_alpha.png");
                    balaImageCarregada = true;

                    // HUD Player: auto-centraliza (x = -1) e posiciona no topo
                    hp_pos.x = -1.0f;   // auto-centraliza dentro da função
                    hp_pos.y = 30.0f;   // altura desejada

                    // HP máximo do player = HP inicial
                    player_max_hp = personagens[0][0].HP;

                    // zera max HP do boss; vai ser setado quando nascer
                    boss_max_hp = 0;

                    gameplayiniciada = true;
                }

                cooldown++;

                // TEMPO
                if (stage_sequence == 0)
                {
                    time += GetFrameTime();
                    if (time > 120.0) time = 120.0;
                }

                // EVENTOS
                mover_player(personagens[0]);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cooldown >= 60)
                {
                    cooldown = 0;
                    atirar(&personagens[0][0], bala_image);
                }

                if (time >= 10 && stage_sequence == 0)
                {
                    stage_sequence = 1;
                    middle_circle.x = (GetScreenWidth() / 2.0f);
                    middle_circle.y = (GetScreenHeight() / 2.0f) - 5;
                }
                if (stage_sequence == 1 && CheckCollisionCircleRec(middle_circle, 30, personagens[0][0].hitbox))
                {
                    stage_sequence = 2;
                    if (backgroundCarregado) UnloadTexture(background);
                    background = criar_background("background/Sala_Boss.png");
                    backgroundCarregado = true;
                    time = 0;
                }
                if (stage_sequence == 2)
                {
                    if (timer == 0) // cria o boss
                    {
                        personagens[4][0] = criar_boss('S',
                            (GetScreenWidth() / 2),
                            (GetScreenHeight() / 2) + 70,
                            LoadImage("characters/Robo_Gladiador_NORTH.png"),
                            LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                            LoadImage("characters/Robo_Gladiador_WEST.png"),
                            LoadImage("characters/Robo_Gladiador_EAST.png"));

                        qtd_entidades[4] = 1;
                        boss_max_hp = personagens[4][0].HP;   // define HP máximo do boss agora
                    }
                    movimentacao_boss(&personagens[4][0], &personagens[0][0], bala_image, &timer);
                    timer++;
                }

                // DRAWING
                BeginDrawing();

                switch (personagens[0][0].sentido)
                {
                    case 'N': textura_atual_player = personagens[0][0].sprite_N; break;
                    case 'S': textura_atual_player = personagens[0][0].sprite_S; break;
                    case 'W': textura_atual_player = personagens[0][0].sprite_W; break;
                    case 'E': textura_atual_player = personagens[0][0].sprite_E; break;
                }

                ClearBackground(WHITE);
                if (backgroundCarregado) DrawTexture(background, 0, 0, WHITE);

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
                        if (personagens[0][0].HP == 0)
                        {
                            DrawText("MORREU ZÉ", 10, GetScreenHeight() - 50, 30, WHITE);
                            if (backgroundCarregado) UnloadTexture(background);
                            background = criar_background("background/foxe.png");
                            backgroundCarregado = true;
                        }
                        else
                        {
                            DrawText("MISSÃO: DERROTE O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                        }
                        desenhar_boss(&personagens[4][0]);

                        // ===== HUD: BARRA DE HP DO BOSS (rodapé, esticada) =====
                        if (qtd_entidades[4] > 0)
                        {
                            desenhar_barra_boss(&personagens[4][0], boss_max_hp,
                            boss_hp_fill, boss_hp_frame,
                            boss_hp_margin_x, boss_hp_margin_bottom,
                            boss_hp_height_scale,
                            boss_src_area);
                        }

                        if (personagens[4][0].HP <= 0) tela_atual = VITORIA;
                        break;
                }

                // Player
                DrawTextureV(textura_atual_player,
                             (Vector2){ personagens[0][0].hitbox.x, personagens[0][0].hitbox.y },
                             WHITE);

                // Balas
                mover_balas(personagens, &qtd_entidades);

                // ===== HUD: BARRA DE HP (PLAYER) =====
                desenhar_barra_hp(&personagens[0][0], player_max_hp,
                  hp_fill, hp_frame, hp_pos, hp_scale,
                  hp_src_area);

                EndDrawing();

                if (personagens[0][0].HP <= 0) tela_atual = MORTE;
            } break;

            case MORTE:
            {
                tela_morte(&tela_atual, mortebackground);
            } break;

            case VITORIA:
            {
                tela_vitoria(&tela_atual, vitoriabackground);
            } break;

            default: break;
        }
    }

    // LIMPEZA FINAL
    if (gameplayiniciada)
    {
        destruir_personagem(personagens[0][0]);
        if (stage_sequence == 2) destruir_personagem(personagens[4][0]);

        if (backgroundCarregado) UnloadTexture(background);
        if (balaImageCarregada)  UnloadImage(bala_image);
    }

    // HUDs
    UnloadTexture(hp_fill);
    if (hp_frame.id != 0) UnloadTexture(hp_frame);

    UnloadTexture(boss_hp_fill);
    if (boss_hp_frame.id != 0) UnloadTexture(boss_hp_frame);

    UnloadTexture(menubackground);
    UnloadTexture(imagemcreditos);
    UnloadTexture(mortebackground);
    UnloadTexture(vitoriabackground);

    // libera vetores de personagens
    for (int i = 0; i < 5; i++)
    {
        if (personagens[i] != NULL) free(personagens[i]);
    }
    free(personagens);

    CloseWindow();
    return 0;
}
