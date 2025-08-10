#include "funcoe_utilizadas.h"
#include <math.h>

// Cria um personagem e atribui suas caracteristicas
Personagens criar_personagem(char sentido, float posicao_x, float posicao_y,
                             Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E,
                             int HP, float speed, int tamanho, int largura)
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

Personagens criar_boss(char sentido, float posicao_x, float posicao_y,
                       Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E)
{
    Personagens BOSS = criar_personagem(sentido, posicao_x, posicao_y,
                                        imagem_N, imagem_S, imagem_W, imagem_E,
                                        3, 1.5f, 64, 64);
    return BOSS;
}

void desenhar_boss(Personagens *boss)
{
    Texture2D boss_texture;
    switch (boss->sentido)
    {
        case 'N': boss_texture = boss->sprite_N; break;
        case 'S': boss_texture = boss->sprite_S; break;
        case 'W': boss_texture = boss->sprite_W; break;
        case 'E': boss_texture = boss->sprite_E; break;
        default : boss_texture = boss->sprite_S; break;
    }
    DrawTextureV(boss_texture, (Vector2){ boss->hitbox.x, boss->hitbox.y }, WHITE);
}

void movimentacao_boss(Personagens* boss, Personagens* player, Image bala_imagem, int *timer)
{
    if (player->hitbox.x > boss->hitbox.x + boss->hitbox.width / 2)
    {
        boss->hitbox.x += boss->speed;
        boss->sentido = 'E';
    }
    else if (player->hitbox.x < boss->hitbox.x + boss->hitbox.width / 2)
    {
        boss->hitbox.x -= boss->speed;
        boss->sentido = 'W';
    }

    if (player->hitbox.y > boss->hitbox.y + boss->hitbox.height / 2)
    {
        boss->hitbox.y += boss->speed;
        boss->sentido = 'S';
    }
    else if (player->hitbox.y < boss->hitbox.y + boss->hitbox.height / 2)
    {
        boss->hitbox.y -= boss->speed;
        boss->sentido = 'N';
    }

    if (boss->HP == 3) // fase 1
    {
        if (*timer % 60 == 0) // 1 tiro por segundo
        {
            atirar_dir_player(boss, player, bala_imagem);
        }
    }

    if (boss->HP <= 2) // fase 2
    {
        if (*timer % 20 == 0) // 3 tiros por segundo
        {
            atirar_dir_player(boss, player, bala_imagem);
        }

        if (*timer % 180 == 0)
        {
            for (int i = 0; i < 360; i += 30) // tiro pra todos os lados
            {
                float dir_x = cosf(i * (PI / 180.0f));
                float dir_y = sinf(i * (PI / 180.0f));
                atirar_boss(boss, bala_imagem, dir_x, dir_y);
            }
        }

        if (boss->HP == 1) // fase 3 (mantém os ataques da fase 2)
        {
            if (*timer % 720 == 0)
            {
                ; // placeholder: criar inimigo
            }
        }
    }
}

// Cria o background da tela
Texture2D criar_background(const char* caminho_imagem)
{
    Image imagem = LoadImage(caminho_imagem);
    Texture2D background = LoadTextureFromImage(imagem);
    UnloadImage(imagem);
    return background;
}

// Descarrega texturas do personagem e limpa balas
void destruir_personagem(Personagens personagem)
{
    // limpa balas restantes
    for (int b = 0; b < personagem.qtd_balas; b++)
    {
        UnloadTexture(personagem.balas[b].sprite_bala);
    }
    if (personagem.balas != NULL)
    {
        free(personagem.balas);
    }

    // descarrega sprites do personagem
    UnloadTexture(personagem.sprite_N);
    UnloadTexture(personagem.sprite_S);
    UnloadTexture(personagem.sprite_W);
    UnloadTexture(personagem.sprite_E);
}

// Inicializa o jogo
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

    for (int i = 0; i < 5; i++)
    {
        personagens[i] = NULL;
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

// Função de mover o player (WASD + setas)
void mover_player(Personagens* player)
{
    // UP / W
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    {
        player->hitbox.y -= player->speed;
        if (player->hitbox.y <= 134) player->hitbox.y = 134;
        player->sentido = 'N';
    }
    // DOWN / S
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    {
        player->hitbox.y += player->speed;
        if (player->hitbox.y >= GetScreenHeight() - 134 - player->hitbox.height)
            player->hitbox.y = GetScreenHeight() - 134 - player->hitbox.height;
        player->sentido = 'S';
    }
    // LEFT / A
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        player->hitbox.x -= player->speed;
        if (player->hitbox.x <= 83) player->hitbox.x = 83;
        player->sentido = 'W';
    }
    // RIGHT / D
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        player->hitbox.x += player->speed;
        if (player->hitbox.x >= GetScreenWidth() - 83 - player->hitbox.width)
            player->hitbox.x = GetScreenWidth() - 83 - player->hitbox.width;
        player->sentido = 'E';
    }
}

void atirar_boss(Personagens *boss, Image sprite, float dir_x, float dir_y)
{
    Bullet* temp = (Bullet*) realloc(boss->balas, (boss->qtd_balas + 1) * sizeof(Bullet));
    if (temp == NULL) { CloseWindow(); }

    int index = boss->qtd_balas;
    boss->balas = temp;

    boss->balas[index].hitbox_bala.x = boss->hitbox.x + boss->hitbox.width / 2;
    boss->balas[index].hitbox_bala.y = boss->hitbox.y + boss->hitbox.height / 2;

    boss->balas[index].direcao.x = dir_x;
    boss->balas[index].direcao.y = dir_y;

    boss->balas[index].sprite_bala = LoadTextureFromImage(sprite);
    boss->balas[index].hitbox_bala.width  = (float)boss->balas[index].sprite_bala.width;
    boss->balas[index].hitbox_bala.height = (float)boss->balas[index].sprite_bala.height;

    boss->qtd_balas++;
}

void atirar(Personagens *player, Image sprite)
{
    Bullet* temp = (Bullet*) realloc(player->balas, (player->qtd_balas + 1) * sizeof(Bullet));
    if (temp == NULL) { CloseWindow(); }

    player->balas = temp;
    int index = player->qtd_balas;

    player->balas[index].hitbox_bala.x = player->hitbox.x;
    player->balas[index].hitbox_bala.y = player->hitbox.y;

    float x_mouse = (float)GetMouseX();
    float y_mouse = (float)GetMouseY();

    float dx = x_mouse - player->hitbox.x;
    float dy = y_mouse - player->hitbox.y;

    float mag = sqrtf(dx*dx + dy*dy);
    if (mag < 0.0001f) { dx = 1.0f; dy = 0.0f; mag = 1.0f; }

    player->balas[index].direcao.x = dx / mag;
    player->balas[index].direcao.y = dy / mag;

    player->balas[index].sprite_bala = LoadTextureFromImage(sprite);
    player->balas[index].hitbox_bala.width  = (float)player->balas[index].sprite_bala.width;
    player->balas[index].hitbox_bala.height = (float)player->balas[index].sprite_bala.height;

    (player->qtd_balas)++;
}

void atirar_dir_player(Personagens *entidade, Personagens *player, Image sprite)
{
    Bullet* temp = (Bullet*) realloc(entidade->balas, (entidade->qtd_balas + 1) * sizeof(Bullet));
    if (temp == NULL) { CloseWindow(); }

    int index = entidade->qtd_balas;
    entidade->balas = temp;

    entidade->balas[index].hitbox_bala.x = entidade->hitbox.x + entidade->hitbox.width / 2;
    entidade->balas[index].hitbox_bala.y = entidade->hitbox.y + entidade->hitbox.height / 2;

    float dx = player->hitbox.x - entidade->hitbox.x;
    float dy = player->hitbox.y - entidade->hitbox.y;

    float mag = sqrtf(dx*dx + dy*dy);
    if (mag < 0.0001f)
    { 
        dx = 1.0f; dy = 0.0f; mag = 1.0f;
    }

    entidade->balas[index].sprite_bala = LoadTextureFromImage(sprite);
    entidade->balas[index].direcao.x = dx / mag;
    entidade->balas[index].direcao.y = dy / mag;
 
    entidade->balas[index].hitbox_bala.width  = (float)entidade->balas[index].sprite_bala.width;
    entidade->balas[index].hitbox_bala.height = (float)entidade->balas[index].sprite_bala.height;

    entidade->qtd_balas++;
}

void mover_balas(Personagens **entidades, int (*qtd_entidades)[5])
{
    // PLAYER
    if ((*qtd_entidades)[0] > 0)
    {
        for (int i = 0; i < entidades[0][0].qtd_balas; i++)
        {
            entidades[0][0].balas[i].hitbox_bala.x += (entidades[0][0].balas[i].direcao.x) * 10.0f;
            entidades[0][0].balas[i].hitbox_bala.y += (entidades[0][0].balas[i].direcao.y) * 10.0f;

            DrawTextureV(entidades[0][0].balas[i].sprite_bala,
                         (Vector2){ entidades[0][0].balas[i].hitbox_bala.x, entidades[0][0].balas[i].hitbox_bala.y },
                         WHITE);

            if (entidades[0][0].balas[i].hitbox_bala.x + entidades[0][0].balas[i].hitbox_bala.width  < 0 ||
                entidades[0][0].balas[i].hitbox_bala.x > GetScreenWidth() ||
                entidades[0][0].balas[i].hitbox_bala.y + entidades[0][0].balas[i].hitbox_bala.height < 0 ||
                entidades[0][0].balas[i].hitbox_bala.y > GetScreenHeight())
            {
                UnloadTexture(entidades[0][0].balas[i].sprite_bala);
                for (int j = i; j < entidades[0][0].qtd_balas - 1; j++)
                {
                    entidades[0][0].balas[j] = entidades[0][0].balas[j + 1];
                }
                entidades[0][0].qtd_balas--;
                i--;
            }
        }

        // colisão: bala do boss -> player
        if ((*qtd_entidades)[4] > 0)
        {
            for (int i = 0; i < entidades[4][0].qtd_balas; i++)
            {
                if (CheckCollisionRecs(entidades[0][0].hitbox, entidades[4][0].balas[i].hitbox_bala))
                {
                    entidades[0][0].HP--;
                    UnloadTexture(entidades[4][0].balas[i].sprite_bala);
                    for (int j = i; j < entidades[4][0].qtd_balas - 1; j++)
                    {
                        entidades[4][0].balas[j] = entidades[4][0].balas[j + 1];
                    }
                    entidades[4][0].qtd_balas--;
                    i--;
                }
            }
        }
    }

    // BOSS
    if ((*qtd_entidades)[4] > 0)
    {
        for (int i = 0; i < entidades[4][0].qtd_balas; i++)
        {
            entidades[4][0].balas[i].hitbox_bala.x += (entidades[4][0].balas[i].direcao.x) * 3.0f;
            entidades[4][0].balas[i].hitbox_bala.y += (entidades[4][0].balas[i].direcao.y) * 3.0f;

            DrawTextureV(entidades[4][0].balas[i].sprite_bala,
                         (Vector2){ entidades[4][0].balas[i].hitbox_bala.x, entidades[4][0].balas[i].hitbox_bala.y },
                         WHITE);

            if (entidades[4][0].balas[i].hitbox_bala.x + entidades[4][0].balas[i].hitbox_bala.width  < 0 ||
                entidades[4][0].balas[i].hitbox_bala.x > GetScreenWidth() ||
                entidades[4][0].balas[i].hitbox_bala.y + entidades[4][0].balas[i].hitbox_bala.height < 0 ||
                entidades[4][0].balas[i].hitbox_bala.y > GetScreenHeight())
            {
                UnloadTexture(entidades[4][0].balas[i].sprite_bala);
                for (int j = i; j < entidades[4][0].qtd_balas - 1; j++)
                {
                    entidades[4][0].balas[j] = entidades[4][0].balas[j + 1];
                }
                entidades[4][0].qtd_balas--;
                i--;
            }
        }

        // colisão: bala do player -> boss
        for (int i = 0; i < entidades[0][0].qtd_balas; i++)
        {
            if (CheckCollisionRecs(entidades[0][0].balas[i].hitbox_bala, entidades[4][0].hitbox))
            {
                entidades[4][0].HP--;
                UnloadTexture(entidades[0][0].balas[i].sprite_bala);
                for (int k = i; k < entidades[0][0].qtd_balas - 1; k++)
                {
                    entidades[0][0].balas[k] = entidades[0][0].balas[k + 1];
                }
                entidades[0][0].qtd_balas--;
                i--;
            }
        }
    }
}

/* ===========================
   HUD: BARRA DE HP (auto-detecção da área útil)
   =========================== */

Rectangle detectar_area_hp(Texture2D tex)
{
    // fallback: se falhar, usa a textura inteira
    Rectangle area = { 0, 0, (float)tex.width, (float)tex.height };

    // Converte a Texture para Image para poder ler pixels
    Image img = LoadImageFromTexture(tex);
    if (img.data == NULL) return area;

    Color *px = LoadImageColors(img);
    if (px == NULL) { UnloadImage(img); return area; }

    int minx = img.width, miny = img.height, maxx = -1, maxy = -1;

    // Heurística: “vermelho dominante” (ajuste os thresholds se precisar)
    const int TH = 25;   // quanto o R precisa ser maior que G e B
    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            Color c = px[y*img.width + x];
            if (c.a < 10) continue;               // ignora totalmente transparente
            if ((int)c.r - (int)c.g > TH && (int)c.r - (int)c.b > TH)
            {
                if (x < minx) minx = x;
                if (y < miny) miny = y;
                if (x > maxx) maxx = x;
                if (y > maxy) maxy = y;
            }
        }
    }

    UnloadImageColors(px);
    UnloadImage(img);

    if (maxx >= minx && maxy >= miny)
    {
        area.x = (float)minx;
        area.y = (float)miny;
        area.width  = (float)(maxx - minx + 1);
        area.height = (float)(maxy - miny + 1);
    }
    // senão, permanece fallback (textura toda)

    return area;
}

/* ===========================
   HUD: BARRA DE HP (player)
   =========================== */
void desenhar_barra_hp(const Personagens* player, int max_hp,
                       Texture2D barra_fill, Texture2D barra_frame,
                       Vector2 pos, float scale, Rectangle src_area)
{
    if (barra_fill.id == 0) return;

    int safe_max = (max_hp > 0) ? max_hp : 1;
    float ratio = (float)player->HP / (float)safe_max;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;

    // usa a área útil recebida (sem cache global)
    Rectangle srcArea = src_area;

    int drawW = (int)(srcArea.width  * scale);
    int drawH = (int)(srcArea.height * scale);

    // Auto-centraliza se pos.x < 0
    float x = (pos.x < 0.0f) ? (GetScreenWidth() - drawW) * 0.5f : pos.x;
    float y = pos.y;

    // fundo preto opaco + “soft edges” só na área útil
    Rectangle bg = { x, y, (float)drawW, (float)drawH };
    DrawRectangleRec(bg, Fade(BLACK, 0.85f));
    DrawRectangleRec((Rectangle){ bg.x - 1, bg.y - 1, bg.width + 2, bg.height + 2 }, Fade(BLACK, 0.35f));
    DrawRectangleRec((Rectangle){ bg.x - 2, bg.y - 2, bg.width + 4, bg.height + 4 }, Fade(BLACK, 0.15f));

    // parte cheia proporcional
    Rectangle srcClip = srcArea;
    srcClip.width = srcArea.width * ratio;

    Rectangle dst = { x, y, srcClip.width * scale, drawH };
    DrawTexturePro(barra_fill, srcClip, dst, (Vector2){0,0}, 0.0f, WHITE);

    // moldura por cima (se houver)
    if (barra_frame.id != 0)
    {
        Rectangle dstFrame = { x, y, srcArea.width * scale, drawH };
        DrawTexturePro(barra_frame, srcArea, dstFrame, (Vector2){0,0}, 0.0f, WHITE);
    }
}


void desenhar_barra_boss(const Personagens* boss, int max_hp,
                         Texture2D barra_fill, Texture2D barra_frame,
                         float margem_lateral, float margem_inferior,
                         float height_scale, Rectangle src_area)
{
    if (barra_fill.id == 0) return;

    int safe_max = (max_hp > 0) ? max_hp : 1;
    float ratio = (float)boss->HP / (float)safe_max;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;

    // usa a área útil recebida (sem cache global)
    Rectangle srcArea = src_area;

    int drawW = (int)(GetScreenWidth() - 2.0f * margem_lateral);
    int drawH = (int)(srcArea.height * height_scale);

    float x = margem_lateral;
    float y = GetScreenHeight() - margem_inferior - drawH;

    // fundo preto opaco + “soft edges”
    Rectangle bg = { x, y, (float)drawW, (float)drawH };
    DrawRectangleRec(bg, Fade(BLACK, 0.85f));
    DrawRectangleRec((Rectangle){ bg.x - 1, bg.y - 1, bg.width + 2, bg.height + 2 }, Fade(BLACK, 0.35f));
    DrawRectangleRec((Rectangle){ bg.x - 2, bg.y - 2, bg.width + 4, bg.height + 4 }, Fade(BLACK, 0.15f));

    // parte cheia proporcional (esticada)
    Rectangle dst = { x, y, drawW * ratio, (float)drawH };
    DrawTexturePro(barra_fill, srcArea, dst, (Vector2){0,0}, 0.0f, WHITE);

    // moldura (se houver)
    if (barra_frame.id != 0)
    {
        Rectangle dstFrame = { x, y, (float)drawW, (float)drawH };
        DrawTexturePro(barra_frame, srcArea, dstFrame, (Vector2){0,0}, 0.0f, WHITE);
    }
}


/* ===========================
   TELAS
   =========================== */

void tela_menu(GameScreen *tela_atual, Texture2D background)
{
    Rectangle playButton    = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 - 50, 250, 50 };
    Rectangle optionsButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 20, 250, 50 };
    Rectangle creditsButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 90, 250, 50 };
    Rectangle exitButton    = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 160, 250, 50 };

    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, playButton)    && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = GAMEPLAY;
    if (CheckCollisionPointRec(mousePoint, optionsButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = OPTIONS;
    if (CheckCollisionPointRec(mousePoint, creditsButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = CREDITOS;
    if (CheckCollisionPointRec(mousePoint, exitButton)    && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) CloseWindow();

    BeginDrawing();
    DrawTexture(background, 0, 0, WHITE);

    DrawText("Coliseu", GetScreenWidth()/2 - MeasureText("Coliseu", 80)/2, 100, 80, WHITE);
    DrawRectangleRec(playButton, LIGHTGRAY);
    DrawText("Jogar", playButton.x + 75, playButton.y + 10, 30, DARKGRAY);
    DrawRectangleRec(optionsButton, LIGHTGRAY);
    DrawText("Opções", optionsButton.x + 65, optionsButton.y + 10, 30, DARKGRAY);
    DrawRectangleRec(creditsButton, LIGHTGRAY);
    DrawText("Créditos", creditsButton.x + 55, creditsButton.y + 10, 30, DARKGRAY);
    DrawRectangleRec(exitButton, LIGHTGRAY);
    DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, DARKGRAY);
    EndDrawing();
}

void tela_opcoes(GameScreen *tela_atual, Texture2D background, float *volume)
{
    Rectangle volumeSliderBar    = { GetScreenWidth()/2 - 150, GetScreenHeight()/2 - 10, 300, 20 };
    Rectangle volumeSliderHandle = { volumeSliderBar.x + (*volume * volumeSliderBar.width) - 10, GetScreenHeight()/2.0f - 20, 20, 40 };
    Rectangle backButton         = { GetScreenWidth()/2 - 100, GetScreenHeight()/2 + 100, 200, 50 };

    Vector2 mousePoint = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePoint, volumeSliderBar))
    {
        *volume = (mousePoint.x - volumeSliderBar.x) / volumeSliderBar.width;
        if (*volume < 0.0f) *volume = 0.0f;
        if (*volume > 1.0f) *volume = 1.0f;
        SetMasterVolume(*volume);
    }
    if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = MENU;

    BeginDrawing();
    DrawTexture(background, 0, 0, WHITE);

    DrawText("Opções", GetScreenWidth()/2 - MeasureText("Opções", 40)/2, 150, 40, WHITE);
    DrawText("Volume", GetScreenWidth()/2 - MeasureText("Volume", 30)/2, GetScreenHeight()/2 - 60, 30, WHITE);

    DrawRectangleRec(volumeSliderBar, LIGHTGRAY);
    DrawRectangleRec(volumeSliderHandle, MAROON);

    DrawRectangleRec(backButton, LIGHTGRAY);
    DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, DARKGRAY);
    EndDrawing();
}

void tela_creditos(GameScreen *tela_atual, Texture2D imagemcreditos)
{
    Rectangle backButton = { GetScreenWidth()/2 - 100, GetScreenHeight() - 100, 200, 50 };
    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = MENU;

    BeginDrawing();
    DrawTexture(imagemcreditos, 0, 0, WHITE);
    DrawRectangleRec(backButton, LIGHTGRAY);
    DrawText("Voltar", backButton.x + 55, backButton.y + 10, 30, DARKGRAY);
    EndDrawing();
}

void tela_morte(GameScreen *tela_atual, Texture2D mortebackground)
{
    Rectangle menuButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 90, 250, 50 };
    Rectangle exitButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 160, 250, 50 };
    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, menuButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = MENU;
    if (CheckCollisionPointRec(mousePoint, exitButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) CloseWindow();

    BeginDrawing();
    DrawTexture(mortebackground, 0, 0, WHITE);
    DrawText("VOCÊ MORREU", GetScreenWidth()/2 - MeasureText("VOCÊ MORREU", 100)/2, 350, 100, RED);
    DrawRectangleRec(menuButton, DARKGRAY);
    DrawText("Voltar ao Menu", menuButton.x + 7, menuButton.y + 10, 30, RED);
    DrawRectangleRec(exitButton, DARKGRAY);
    DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, RED);
    EndDrawing();
}

void tela_vitoria(GameScreen *tela_atual, Texture2D vitoriabackground)
{
    Rectangle menuButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 90, 250, 50 };
    Rectangle exitButton = { GetScreenWidth()/2 - 125, GetScreenHeight()/2 + 160, 250, 50 };
    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, menuButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) *tela_atual = MENU;
    if (CheckCollisionPointRec(mousePoint, exitButton) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) CloseWindow();

    BeginDrawing();
    DrawTexture(vitoriabackground, 0, 0, WHITE);
    DrawText("VOCÊ VENCEU", GetScreenWidth()/2 - MeasureText("VOCÊ VENCEU", 100)/2, 350, 100, GREEN);
    DrawRectangleRec(menuButton, LIGHTGRAY);
    DrawText("Voltar ao Menu", menuButton.x + 7, menuButton.y + 10, 30, DARKGRAY);
    DrawRectangleRec(exitButton, LIGHTGRAY);
    DrawText("Sair do Jogo", exitButton.x + 30, exitButton.y + 10, 30, DARKGRAY);
    EndDrawing();
}
