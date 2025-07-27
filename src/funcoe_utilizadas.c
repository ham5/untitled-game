#include "funcoe_utilizadas.h" // Inclui o cabeçalho com as declarações

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

    return novo_personagem;
}

//Cria o background da tela
Texture2D criar_background(const char* caminho_imagem)
{
    Image imagem = LoadImage(caminho_imagem);
    Texture2D background = LoadTextureFromImage(imagem);
    UnloadImage(imagem);
    return background;
}

//Tira um personagem da tela (Provavelmente vai precisar de mais coisa no futuro)
void destruir_personagem(Personagens personagem)
{
    UnloadTexture(personagem.sprite_N);
    UnloadTexture(personagem.sprite_S);
    UnloadTexture(personagem.sprite_W);
    UnloadTexture(personagem.sprite_E);
}

//Inicializa o jogo, setando o tamanho da tela e o FPS, e retornando o ponteiro duplo que vai conter cada tipo de personagem
Personagens** inicializar()
{
    int tamanho = 900;
    InitWindow(tamanho, tamanho, "COLISEU_GAME");
    SetTargetFPS(60);
    Personagens** personagens = NULL;
    personagens = (Personagens**) malloc(sizeof(Personagens*));
    if (personagens == NULL)
    {
        CloseWindow();
    }
    personagens[0] = NULL;
    personagens[0] = (Personagens*) malloc(sizeof(Personagens));
    if (personagens[0] == NULL)
    {
        free(personagens);
        CloseWindow();
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