#include "structs_e_enums_utilizadas.h"
#include "MC_e_personagens.h"
#include "balas.h"

Personagens criar_boss(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E)
{
    Personagens BOSS = criar_personagem(sentido, posicao_x, posicao_y, imagem_N, imagem_S, imagem_W, imagem_E, 60, 1, 64, 64);

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

void movimentacao_boss(Personagens* boss, Personagens* player, Image bala_imagem, int *timer, Personagens** personagens, int (*qtd_entidades)[5], Image img_corredor, Image img_atirador)
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

    if (boss->HP <= 60 && boss->HP > 40) //fase 1
    {
        // movimentação do boss
        if (*timer % 60 == 0) { //1 tiro por segundo
            atirar_dir_player(boss, player, bala_imagem);
        }
    }

    if (boss->HP <= 40) //fase 2
    {
        boss->speed = 1;
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

    }
}

void destruir_inimigos_e_boss(Personagens** entidades, int (*qtd_entidades)[5]) 
{
    for (int i = 1; i < 5; i++) 
    {
        for (int j = 0; j < (*qtd_entidades)[i]; j++) 
        {
            destruir_personagem(entidades[i][j]);
        }
        free(entidades[i]);
        entidades[i] = (Personagens*)malloc(sizeof(Personagens)); // realoca para evitar crash
        (*qtd_entidades)[i] = 0; // zera a quantidade
    }
}
