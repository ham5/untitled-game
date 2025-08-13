#include "structs_e_enums_utilizadas.h"

//Cria um personagem e atribui suas caracteristicas 
Personagens criar_personagem(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E, int HP, int speed, int tamanho, int largura)
{
    Personagens novo_personagem;
    novo_personagem.sentido = sentido;
    novo_personagem.hitbox.x = posicao_x;
    novo_personagem.hitbox.y = posicao_y;
    novo_personagem.speed = speed;
    novo_personagem.HP = HP;
    novo_personagem.hitbox.height = tamanho;
    novo_personagem.hitbox.width = largura;


    novo_personagem.sprite_N = LoadTextureFromImage(imagem_N);
    novo_personagem.sprite_S = LoadTextureFromImage(imagem_S);
    novo_personagem.sprite_W = LoadTextureFromImage(imagem_W);
    novo_personagem.sprite_E = LoadTextureFromImage(imagem_E);


    UnloadImage(imagem_N);
    UnloadImage(imagem_S);
    UnloadImage(imagem_W);
    UnloadImage(imagem_E);

    novo_personagem.balas = NULL;
    novo_personagem.qtd_balas = 0;

    return novo_personagem;
}

void destruir_personagem(Personagens personagem)
{
    UnloadTexture(personagem.sprite_N);
    UnloadTexture(personagem.sprite_S);
    UnloadTexture(personagem.sprite_W);
    UnloadTexture(personagem.sprite_E);
}

Personagens** inicializar()
{
    int tamanho = 900;
    InitWindow(tamanho, tamanho, "COLISEU_GAME");
    SetTargetFPS(60);

    // Aloca espaço para 5 tipos de personagens
    Personagens** personagens = (Personagens**) malloc(5 * sizeof(Personagens*));
    if (personagens == NULL)
    {
        CloseWindow();
    }
    for (int i = 0; i < 5; i++) {
        personagens[i] = NULL;
        // Aloca espaço para 1 personagem em cada tipo 
        personagens[i] = (Personagens*) malloc(sizeof(Personagens));
        if (personagens[i] == NULL)
        {
            for (int j = 0; j < i; j++) free(personagens[j]);
            free(personagens);
            CloseWindow();
        }
    }
    return personagens;
}

// Função de mover o player (e mudar seu sentido) (intuitivo)
void mover_player(Personagens* player)
{
    if (IsKeyDown(KEY_UP))
    {
        player->hitbox.y -= player->speed;
        if (player->hitbox.y <= 134)
        {
            player->hitbox.y = 134;
        }
        player->sentido = 'N';
    }
    if (IsKeyDown(KEY_DOWN))
    {
        player->hitbox.y += player->speed;
        if (player->hitbox.y >= GetScreenHeight() - 134 - player->hitbox.height)
        {
            player->hitbox.y = GetScreenHeight() - 134 - player->hitbox.height;
        }
        player->sentido = 'S';
    }
    if (IsKeyDown(KEY_LEFT))
    {
        player->hitbox.x -= player->speed;
        if (player->hitbox.x <= 83)
        {
            player->hitbox.x = 83;
        }
        player->sentido = 'W';
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        player->hitbox.x += player->speed;
        if (player->hitbox.x >= GetScreenWidth() - 83 - player->hitbox.width)
        {
            player->hitbox.x = GetScreenWidth() - 83 - player->hitbox.width;
        }
        player->sentido = 'E';
    }
}

void atirar(Personagens *player, Image sprite) {
    
    Bullet* temp = (Bullet*) realloc(player->balas, (player->qtd_balas + 1) * sizeof(Bullet));

    if (temp == NULL) {
        //erro de alocação
        // ADCIONAR FUNÇÃO DE DESALOCAR TODAS AS MEMÓRIAS!!!
        CloseWindow();
    }

    player->balas = temp;
    int index = player->qtd_balas;

    player->balas[index].hitbox_bala.x = player->hitbox.x;
    player->balas[index].hitbox_bala.y = player->hitbox.y;

    int x_mouse = GetMouseX();
    int y_mouse = GetMouseY();

    if (x_mouse == player->hitbox.x && y_mouse == player->hitbox.x) { // só para evitar divisão por zero
        x_mouse = 0;
        y_mouse = 0;
    }

    //cálculo da direção da bala
    int j = x_mouse - player->hitbox.x;
    int k = y_mouse - player->hitbox.y;

    player->balas[index].direcao.x = (j / sqrt(pow(j, 2) + pow(k, 2)));
    player->balas[index].direcao.y = (k / sqrt(pow(j, 2) + pow(k, 2)));

    player->balas[index].sprite_bala = LoadTextureFromImage(sprite);

    (player->qtd_balas)++;
}

void limpeza_final(
    // matriz de personagens e contadores
    Personagens** personagens,
    int* qtd_entidades,
    int stage_sequence,

    // imagens 
    Image bala_player,
    Image bala_inimigo,

    // texturas de Background
    Texture2D background,
    Texture2D arena_jogo,
    Texture2D arena_boss,
    Texture2D lorebackground,
    Texture2D vitoriabackground,
    Texture2D derrotainimigosbackground,
    Texture2D derrotabossbackground,
    Texture2D menuImage,

    // texturas do HUD
    Texture2D hp_fill,
    Texture2D hp_frame,
    Texture2D boss_hp_fill,
    Texture2D boss_hp_frame,

    // músicas
    Music bossmusic,
    Music salainimigo,
    Music menumusic,
    Music victorymusic
) {

    // garante que o jogador seja destruído (se existir)
    if (qtd_entidades[0] > 0 && personagens[0] != NULL) 
    {
        destruir_personagem(personagens[0][0]);
    }

    // libera o boss (se foi criado)
    if (stage_sequence == 2 && qtd_entidades[4] > 0 && personagens[4] != NULL) 
    {
        destruir_personagem(personagens[4][0]);
    }

    // libera os inimigos restantes
    for (int i = 1; i < 4; i++) 
    {
        if (personagens[i] != NULL) 
        {
            for (int j = 0; j < qtd_entidades[i]; j++) 
            {
                destruir_personagem(personagens[i][j]);
            }
            free(personagens[i]); // Libera o array do tipo de inimigo
        }
    }

    // libera a matriz principal de personagens
    free(personagens);

    // ---- Libera as Imagens ----
    UnloadImage(bala_player);
    UnloadImage(bala_inimigo);

    // ---- Libera as Músicas ----
    UnloadMusicStream(bossmusic);
    UnloadMusicStream(salainimigo);
    UnloadMusicStream(menumusic);
    UnloadMusicStream(victorymusic);

    // ---- Libera as Texturas ----
    UnloadTexture(background);
    UnloadTexture(arena_jogo);
    UnloadTexture(arena_boss);
    UnloadTexture(vitoriabackground);
    UnloadTexture(derrotainimigosbackground);
    UnloadTexture(derrotabossbackground);
    UnloadTexture(lorebackground);
    UnloadTexture(menuImage);

    // ---- Libera Texturas do HUD ----
    UnloadTexture(hp_fill);
    if (hp_frame.id != 0) UnloadTexture(hp_frame);

    UnloadTexture(boss_hp_fill);
    if (boss_hp_frame.id != 0) UnloadTexture(boss_hp_frame);

    // ---- Fecha dispositivos ----
    CloseAudioDevice(); // Fecha o sistema de áudio
    CloseWindow();      // Fecha a janela e o contexto OpenGL
}