#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "structs_e_enums_utilizadas.h"
#include "menu.h"

//função para mover os inimigos
void mover_inimigo (Personagens* inimigo, Personagens* player);

//função para adcionar inimigos
void adicionar_inimigo(Personagens** entidades, int (*qtd_entidades)[5], int spawn_x, int spawn_y, int id, Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E);

//função para adicionar inimigos
void desenhar_inimigos(Personagens** entidades, int (*qtd_entidades)[5]);

//função para destruir os inimigos
void destruir_inimigos(Personagens** entidades, int (*qtd_entidades)[5]);

//função para mover o atirador
void mover_atirador (Personagens* imimigo);

//função pra spawnar
void spawnador(Personagens** entidades,  int (*qtd_entidades)[5], Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E, int tipo);

#endif 