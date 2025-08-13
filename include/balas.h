#ifndef BALAS_H
#define BALAS_H

#include "structs_e_enums_utilizadas.h"
#include "menu.h"

//função de mover balas
void mover_balas(Personagens **entidades, int (*qtd_entidades)[5], GameState *state, Image IMAGEM_BALA, Image imagem_explosao);

//função para atirar na direção do player
void atirar_dir_player(Personagens *entidade, Personagens *player, Image sprite);

#endif 