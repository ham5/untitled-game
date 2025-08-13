#include "MC_e_personagens.h"

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
        //erro de memoria(*qtd_entidades)[4] > 0 
        exit(10);
    }
    entidades[id] = temp;

    int qtd_entidade = (*qtd_entidades)[id]++;
    switch (id) //Personagens criar_personagem(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E, int HP, int speed, int tamanho, int largura);
    {
    case 1:
        entidades[id][qtd_entidade] = criar_personagem('S', spawn_x, spawn_y, IMAGEM_N, IMAGEM_S, IMAGEM_W, IMAGEM_E, 1, 2, 50, 50); //status inimigos 1
        
        break;
    case 2:{
        int dado = rand() % 2; //0 ou 1
        entidades[id][qtd_entidade] = criar_personagem(dado ? 'N': 'S', spawn_x, spawn_y, IMAGEM_N, IMAGEM_S, IMAGEM_W, IMAGEM_E, 1, 2, 50, 50); //statos inimigos 2

        break;
    }
    }
    
}

void desenhar_inimigos(Personagens** entidades, int (*qtd_entidades)[5]){
    for(int i = 0;i<(*qtd_entidades)[1];i++){
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
    for(int i = 0; i<(*qtd_entidades)[2];i++){
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
    for(int i = 0; i<(*qtd_entidades)[3];i++){
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

void destruir_inimigos(Personagens** entidades, int (*qtd_entidades)[5]) {
    for (int i = 1; i < 4; i++) {
        for (int j = 0; j < (*qtd_entidades)[i]; j++) {
            destruir_personagem(entidades[i][j]);
        }
        free(entidades[i]);
        entidades[i] = (Personagens*)malloc(sizeof(Personagens)); // realoca para evitar crash
        (*qtd_entidades)[i] = 0; // zera a quantidade
    }
}

void mover_atirador (Personagens* imimigo) {
    //movimenta o atirador verticalmente
    if (imimigo->sentido == 'N') {
        imimigo->hitbox.y -= imimigo->speed;
        if (imimigo->hitbox.y < 0) {
            imimigo->sentido = 'S';
        }
    } else if (imimigo->sentido == 'S') {
        imimigo->hitbox.y += imimigo->speed;
        if (imimigo->hitbox.y > GetScreenHeight() - imimigo->hitbox.height) {
            imimigo->sentido = 'N';
        }
    }
}

void spawnador(Personagens** entidades,  int (*qtd_entidades)[5], Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E, int tipo){
    int x, y;
    if (tipo == 1) {
        int beirada = (rand()%4)+1; 
        switch (beirada)
        {
        case 1: // esquerda
            x = 20;
            y = rand() % GetScreenHeight();
            break;
        case 2: // baixo
            x = rand() % GetScreenWidth();
            y = GetScreenHeight() - 50;
            break;
        case 3: // direita
            x = GetScreenWidth() - 50;
            y = rand() % GetScreenHeight();
            break;
        case 4: // cima
            x = rand() % GetScreenWidth();
            y = 0;
            break;
        }
    }
    else if (tipo == 2) {
        //lado esquerdo ou lado direito
        int lado = (rand() % 2) + 1; // 1 para esquerda, 2 para direita
        if (lado == 1) {
            x = 20; // lado esquerdo
            y = rand() % GetScreenHeight();
        } else {
            x = GetScreenWidth() - 50; // lado direito
            y = rand() % GetScreenHeight();
        }
    }
    
    adicionar_inimigo(entidades, qtd_entidades, x, y, tipo, 
        ImageCopy(IMAGEM_N),
        ImageCopy(IMAGEM_S),
        ImageCopy(IMAGEM_W),
        ImageCopy(IMAGEM_E));
    
}