#include "funcoe_utilizadas.h" // Inclui o cabeçalho com as declarações
#include <math.h>

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

Personagens criar_boss(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E)
{
    Personagens BOSS = criar_personagem(sentido, posicao_x, posicao_y, imagem_N, imagem_S, imagem_W, imagem_E, 3, 1.5, 64, 64);

    return BOSS;
}

void desenhar_boss(Personagens *boss)
{
    Texture2D boss_texture;
    switch (boss->sentido) {
        case 'N': boss_texture = boss->sprite_N; break;
        case 'S': boss_texture = boss->sprite_S; break;
        case 'W': boss_texture = boss->sprite_W; break;
        case 'E': boss_texture = boss->sprite_E; break;
        default:  boss_texture = boss->sprite_S; break;
    }
    DrawTextureV(boss_texture, (Vector2){boss->hitbox.x, boss->hitbox.y}, WHITE);
}

void movimentacao_boss(Personagens* boss, Personagens* player, Image bala_imagem, int *timer)
{
    if (player->hitbox.x > boss->hitbox.x + boss->hitbox.width / 2) {
        boss->hitbox.x += boss->speed;
        boss->sentido = 'E';
    } else if (player->hitbox.x < boss->hitbox.x + boss->hitbox.width / 2) {
        boss->hitbox.x -= boss->speed;
        boss->sentido = 'W';
    }

    if (player->hitbox.y > boss->hitbox.y + boss->hitbox.height / 2) {
        boss->hitbox.y += boss->speed;
        boss->sentido = 'S';
    } else if (player->hitbox.y < boss->hitbox.y + boss->hitbox.height / 2) {
        boss->hitbox.y -= boss->speed;
        boss->sentido = 'N';
    }

    if (boss->HP == 3) //fase 1
    {
        // movimentação do boss
        if (*timer % 60 == 0) { //1 tiro por segundo
            atirar_dir_player(boss, player, bala_imagem);
        }
    }

    if (boss->HP <= 2) { //fase 2
        if (*timer % 20 == 0) { //3 tiros por segundo
            atirar_dir_player(boss, player, bala_imagem);
        }
        if (*timer % 180 == 0) {
            for (int i = 0; i < 360; i += 30) { //tiro pra todos os lados
                float dir_x = cosf(i * (PI / 180.0f));
                float dir_y = sinf(i * (PI / 180.0f));
                atirar_boss(boss, bala_imagem, dir_x, dir_y);
            }
        }

        if (boss->HP == 1) { //fase 3 (mantém os ataques da fase 2)
            if (*timer % 720 == 0) {
                ; //colocar função de criar inimigo
            }
        }
    }
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

void atirar_boss(Personagens *boss, Image sprite, float dir_x, float dir_y) {
    Bullet* temp = (Bullet*) realloc(boss->balas, (boss->qtd_balas + 1) * sizeof(Bullet));

    if (temp == NULL) {
        //erro de alocação
        // ADCIONAR FUNÇÃO DE DESALOCAR TODAS AS MEMÓRIAS!!!
        CloseWindow();
    }

    int index = boss->qtd_balas;

    boss->balas = temp;
    boss->balas[index].hitbox_bala.x = boss->hitbox.x + boss->hitbox.width / 2;
    boss->balas[index].hitbox_bala.y = boss->hitbox.y + boss->hitbox.height / 2;

    boss->balas[index].direcao.x = dir_x;
    boss->balas[index].direcao.y = dir_y;

    boss->balas[index].sprite_bala = LoadTextureFromImage(sprite);

    boss->qtd_balas++;
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

void atirar_dir_player(Personagens *entidade, Personagens *player, Image sprite) {
    Bullet* temp = (Bullet*) realloc(entidade->balas, (entidade->qtd_balas + 1) * sizeof(Bullet));

    if (temp == NULL) {
        //erro de alocação
        // ADCIONAR FUNÇÃO DE DESALOCAR TODAS AS MEMÓRIAS!!!
        CloseWindow();
    }

    int index = entidade->qtd_balas;
    entidade->balas = temp;

    entidade->balas[index].hitbox_bala.x = entidade->hitbox.x + entidade->hitbox.width / 2;
    entidade->balas[index].hitbox_bala.y = entidade->hitbox.y + entidade->hitbox.height / 2;

    int j = player->hitbox.x - entidade->hitbox.x;
    int k = player->hitbox.y - entidade->hitbox.y;

    if (j == 0 && k == 0) { // só para evitar divisão por zero
        j = 1;
        k = 1;
    }

    entidade->balas[index].sprite_bala = LoadTextureFromImage(sprite);
    entidade->balas[index].direcao.x = (j / sqrt(pow(j, 2) + pow(k, 2)));
    entidade->balas[index].direcao.y = (k / sqrt(pow(j, 2) + pow(k, 2)));

    entidade->qtd_balas++;
}
void mover_balas(Personagens **entidades, int (*qtd_entidades)[5]) {
    //atualiza a posição das balas (por enquanto só do player)
    if ((*qtd_entidades)[0] > 0) { //PLAYER
        for (int i = 0; i < entidades[0][0].qtd_balas; i++) {
            entidades[0][0].balas[i].hitbox_bala.x += (entidades[0][0].balas[i].direcao.x) * 10;
            entidades[0][0].balas[i].hitbox_bala.y += (entidades[0][0].balas[i].direcao.y) * 10;
    
            DrawTextureV(entidades[0][0].balas[i].sprite_bala,
                         (Vector2){entidades[0][0].balas[i].hitbox_bala.x, entidades[0][0].balas[i].hitbox_bala.y},
                         WHITE);
    
            if (entidades[0][0].balas[i].hitbox_bala.x < 0 || 
                entidades[0][0].balas[i].hitbox_bala.x > GetScreenWidth() ||
                entidades[0][0].balas[i].hitbox_bala.y < 0 || 
                entidades[0][0].balas[i].hitbox_bala.y > GetScreenHeight()) {
                    UnloadTexture(entidades[0][0].balas[i].sprite_bala);
                    for (int j = i; j < entidades[0][0].qtd_balas - 1; j++) {
                        entidades[0][0].balas[j] = entidades[0][0].balas[j + 1];
                    }
                    entidades[0][0].qtd_balas--;
                    i--; 
                }
        }

        //checador de colisões de bala inimigo -> player
        if ((*qtd_entidades)[4] > 0 ) { //balas do boss
            for (int i = 0; i < entidades[4][0].qtd_balas; i++) {
                if (CheckCollisionRecs(entidades[0][0].hitbox, entidades[4][0].balas[i].hitbox_bala)) {
                    entidades[0][0].HP--;
                    UnloadTexture(entidades[4][0].balas[i].sprite_bala);
                    for (int j = i; j < entidades[4][0].qtd_balas - 1; j++) {
                        entidades[4][0].balas[j] = entidades[4][0].balas[j + 1];
                    }
                    entidades[4][0].qtd_balas--;
                    i--;
                }
            }
        }

    } 
    //daí depois faz um loop para cada tipo de npc q atira
    if ((*qtd_entidades)[4] > 0) { //BOSS 
        for (int i = 0; i < entidades[4][0].qtd_balas; i++) {
            entidades[4][0].balas[i].hitbox_bala.x += (entidades[4][0].balas[i].direcao.x) * 3;
            entidades[4][0].balas[i].hitbox_bala.y += (entidades[4][0].balas[i].direcao.y) * 3;
    
            DrawTextureV(entidades[4][0].balas[i].sprite_bala,
                         (Vector2){entidades[4][0].balas[i].hitbox_bala.x, entidades[4][0].balas[i].hitbox_bala.y},
                         WHITE);
            
            if (entidades[4][0].balas[i].hitbox_bala.x < 0 || 
                entidades[4][0].balas[i].hitbox_bala.x > GetScreenWidth() ||
                entidades[4][0].balas[i].hitbox_bala.y < 0 ||
                entidades[4][0].balas[i].hitbox_bala.y > GetScreenHeight()) {
                UnloadTexture(entidades[4][0].balas[i].sprite_bala);
                    for (int j = i; j < entidades[4][0].qtd_balas - 1; j++) {
                        entidades[4][0].balas[j] = entidades[4][0].balas[j + 1];
                    }
                    entidades[4][0].qtd_balas--;
                    i--; 
                }
        }
        //checador de colisões de bala -> boss
        for (int i = 0; i < entidades[0][0].qtd_balas; i++) {
            for (int j = 0; j < (*qtd_entidades)[4]; j++) {
                if (CheckCollisionRecs(entidades[0][0].balas[i].hitbox_bala, entidades[4][0].hitbox)) {
                    entidades[4][0].HP--;
                    UnloadTexture(entidades[0][0].balas[i].sprite_bala);
                    for (int k = i; k < entidades[0][0].qtd_balas - 1; k++) {
                        entidades[0][0].balas[k] = entidades[0][0].balas[k + 1];
                    }
                    entidades[0][0].qtd_balas--;
                    i--;
                }
            }
        }

    }  
}

void mover_inimigo (Personagens* inimigo, Personagens* player){
    //definindo distancias nos eixos x e y
    float dx = player->hitbox.x - inimigo->hitbox.x;
    float dy = player->hitbox.y - inimigo->hitbox.y;
    //vetor da direcao
    float distancia = sqrt(dx*dx + dy*dy);
    if(distancia>0){
        dx = dx/distancia;
        dy = dy/distancia;
    }
    //movimentações
    inimigo->hitbox.x += dx * inimigo->speed;
    inimigo->hitbox.y += dy * inimigo->speed;

    if(fabs(dx)>fabs(dy)){//movimento predominante horizontal
        if(dx>0)inimigo->sentido = 'E';
        else inimigo->sentido = 'W';
    }
    else {//movimento predominante vertical
        if(dy>0)inimigo->sentido = 'S';
        else inimigo->sentido = 'N';
    }
}



void adicionar_inimigo(Personagens** entidades, int (*qtd_entidades)[5], int spawn_x, int spawn_y, int id, Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E)
{
    int index_entidade;
    Personagens* temp = NULL;
    temp = (Personagens*) realloc(entidades[id], ((*qtd_entidades)[id] + 1) * sizeof(Personagens));

    if (temp == NULL) 
    {
        //erro de memoria
        exit(10);
    }
    entidades[id] = temp;

    int qtd_entidade = (*qtd_entidades)[id]++;
    switch (id) //Personagens criar_personagem(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E, int HP, int speed, int tamanho, int largura);

    {
    case 1:
        entidades[id][qtd_entidade] = criar_personagem('S', spawn_x, spawn_y, IMAGEM_N, IMAGEM_S, IMAGEM_W, IMAGEM_E, 1, 2, 50, 50); //status inimigos 1
        
        break;
    case 2:
        entidades[id][qtd_entidade] = criar_personagem('S', spawn_x, spawn_y, IMAGEM_N, IMAGEM_S, IMAGEM_W, IMAGEM_E, 1, 2, 50, 50); //statos inimigos 2

        break;
    case 3:
        entidades[id][qtd_entidade] = criar_personagem('S', spawn_x, spawn_y, IMAGEM_N, IMAGEM_S, IMAGEM_W, IMAGEM_E, 1, 2, 50, 50); //statos inimigos 3
        break;
    }
    
}

void spawnador(Personagens** entidades,  int (*qtd_entidades)[5], Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E){
    int tipo = (rand() % 3)+1;
    int beirada = (rand()%4)+1; 
    int x = 10000;
    int y = 10000;
    switch (beirada)
    {
    case 1:
        x=0;
        y = rand();
        break;
    
    case 2:
        y=GetScreenHeight();
        break;
    case 3:
        x = GetScreenWidth();
        break;
    case 4:
        y=0;
        break;
    }
    for(int i = 0; i < 6;i++){
        
        adicionar_inimigo(entidades, (*qtd_entidades)[5], x, y, tipo, IMAGEM_N, IMAGEM_S, IMAGEM_W, IMAGEM_E);

    }
}


void desenhar_inimigos(Personagens** entidades, int (*qtd_entidades)[5]){
    for(int i = 0;i<qtd_entidades[1];i++){
        Texture2D textura;
        switch (entidades[1][i].sentido)
        {
       

        case 'N': 
            textura = entidades[1][i].sprite_N;
            break;
        case 'S': 
            textura = entidades[1][i].sprite_S;
            break;
        case 'W': 
            textura = entidades[1][i].sprite_W;
            break;
        case 'E': 
            textura = entidades[1][i].sprite_E;
            break;
        }
        DrawTextureV(textura, (Vector2){entidades[1][i].hitbox.x, entidades[1][i].hitbox.y}, WHITE);
    }
    for(int i = 0; i<qtd_entidades[2];i++){
        Texture2D textura;
        switch (entidades[2][i].sentido)
        {
       
        case 'N': 
            textura = entidades[2][i].sprite_N;
            break;
        case 'S': 
            textura = entidades[2][i].sprite_S;
            break;
        case 'W': 
            textura = entidades[2][i].sprite_W;
            break;
        case 'E': 
            textura = entidades[2][i].sprite_E;
            break;
        }
        DrawTextureV(textura, (Vector2){entidades[2][i].hitbox.x, entidades[2][i].hitbox.y}, WHITE);
    }
    for(int i = 0; i<qtd_entidades[3];i++){
        Texture2D textura;
        switch (entidades[3][i].sentido)
        {
       
        case 'N': 
            textura = entidades[3][i].sprite_N;
            break;
        case 'S': 
            textura = entidades[3][i].sprite_S;
            break;
        case 'W': 
            textura = entidades[3][i].sprite_W;
            break;
        case 'E': 
            textura = entidades[3][i].sprite_E;
            break;
        }
        DrawTextureV(textura, (Vector2){entidades[3][i].hitbox.x, entidades[3][i].hitbox.y}, WHITE);
    }

        
}