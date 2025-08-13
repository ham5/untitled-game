#ifndef BOSS_H
#define BOSS_H

#include "structs_e_enums_utilizadas.h"
#include "menu.h"

//cria o boss
Personagens criar_boss(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E);

//função para desenhar o boss
void desenhar_boss(Personagens *boss);

//função para o na direção x e y (usado pelo boss)
void atirar_boss(Personagens *boss, Image sprite, float dir_x, float dir_y);

//movimentação geral do boss
void movimentacao_boss(Personagens* boss, Personagens* player, Image bala_imagem, int *timer, Personagens** personagens, int (*qtd_entidades)[5], Image img_corredor, Image img_atirador);

//função para fim de jogo
void destruir_inimigos_e_boss(Personagens** entidades, int (*qtd_entidades)[5]);
#endif 