#include "structs_e_enums_utilizadas.h"
#include "MC_e_personagens.h"


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

void mover_balas(Personagens **entidades, int (*qtd_entidades)[5], GameState *state, Image IMAGEM_BALA, Image imagem_explosao) {
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

    if ((*qtd_entidades)[1] > 0) { 
        for (int i = 0; i < entidades[0][0].qtd_balas; i++) { //checador de colisão de bala player -> inimigo corredor
            for (int j = 0; j < (*qtd_entidades)[1]; j++) {
                if (CheckCollisionRecs(entidades[0][0].balas[i].hitbox_bala, entidades[1][j].hitbox)) {
                    entidades[1][j].HP--;
                    UnloadTexture(entidades[0][0].balas[i].sprite_bala);
                    for (int k = i; k < entidades[0][0].qtd_balas - 1; k++) {
                        entidades[0][0].balas[k] = entidades[0][0].balas[k + 1];
                    }
                    entidades[0][0].qtd_balas--;
                    i--;
                    //mata o inimigo
                    state->score += 10;
                    printf("Removendo inimigo %d\n", j);

                    destruir_personagem(entidades[1][j]);
                    for (int k = j; k < (*qtd_entidades)[1] - 1; k++) {
                        entidades[1][k] = entidades[1][k + 1];
                    }
                    (*qtd_entidades)[1]--;
                    j--;
                    
                    break;
                }
            }
        }

        for (int i = 0; i < (*qtd_entidades)[1]; i++) { //checador de colisão de inimigo corredor -> player
            if (CheckCollisionRecs(entidades[0][0].hitbox, entidades[1][i].hitbox)) {
                entidades[0][0].HP--;
                //mata o inimigo
                printf("Removendo inimigo %d\n", i);
                destruir_personagem(entidades[1][i]);
                for (int k = i; k < (*qtd_entidades)[1] - 1; k++) {
                    entidades[1][k] = entidades[1][k + 1];
                }
                (*qtd_entidades)[1]--;
                
                i--;
            }
        }
    }

    if ((*qtd_entidades)[2] > 0) { 
        for (int i = 0; i < entidades[0][0].qtd_balas; i++) { //checador de colisão de bala player -> inimigo atirador
            for (int j = 0; j < (*qtd_entidades)[2]; j++) {
                if (CheckCollisionRecs(entidades[0][0].balas[i].hitbox_bala, entidades[2][j].hitbox)) {
                    entidades[2][j].HP--;
                    UnloadTexture(entidades[0][0].balas[i].sprite_bala);
                    for (int k = i; k < entidades[0][0].qtd_balas - 1; k++) {
                        entidades[0][0].balas[k] = entidades[0][0].balas[k + 1];
                    }
                    entidades[0][0].qtd_balas--;
                    i--;
                    //mata o inimigo
                    state->score += 15;
                    printf("Removendo inimigo %d\n", j);

                    destruir_personagem(entidades[2][j]);
                    for (int k = j; k < (*qtd_entidades)[2] - 1; k++) {
                        entidades[2][k] = entidades[2][k + 1];
                    }
                    (*qtd_entidades)[2]--;
                    j--;
                    
                    break;
                }
            }
        }

        for (int i = 0; i < (*qtd_entidades)[2]; i++) { //checador de colisão de bala inimigo atirador -> player
            for (int j = 0; j < entidades[2][i].qtd_balas; j++) {
                if (CheckCollisionRecs(entidades[0][0].hitbox, entidades[2][i].balas[j].hitbox_bala)) {
                    entidades[0][0].HP--;
                    UnloadTexture(entidades[2][i].balas[j].sprite_bala);
                    for (int k = j; k < entidades[2][i].qtd_balas - 1; k++) {
                        entidades[2][i].balas[k] = entidades[2][i].balas[k + 1];
                    }
                    entidades[2][i].qtd_balas--;
                    j--;
                    
                }
            }
        }

        for (int i = 0; i < (*qtd_entidades)[2]; i++) { //faz o inimigo atirar
            if (GetRandomValue(1, 100) <= 1) { //1% de chance de atirar a cada frame
                atirar_dir_player(&entidades[2][i], &entidades[0][0], IMAGEM_BALA);
            }
        }

        for (int i = 0; i < (*qtd_entidades)[2]; i++) {
            for (int j = 0; j < entidades[2][i].qtd_balas; j++) {
                entidades[2][i].balas[j].hitbox_bala.x += entidades[2][i].balas[j].direcao.x * 6;
                entidades[2][i].balas[j].hitbox_bala.y += entidades[2][i].balas[j].direcao.y * 6;
                DrawTextureV(entidades[2][i].balas[j].sprite_bala,
                                (Vector2){entidades[2][i].balas[j].hitbox_bala.x, entidades[2][i].balas[j].hitbox_bala.y},
                                WHITE);
                // Remover bala se sair da tela...
            }
        }


    }
}