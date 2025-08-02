#ifndef FUNCOES_UTILIZADAS_H
#define FUNCOES_UTILIZADAS_H

#include "structs_e_enums_utilizadas.h"

//Cria um personagem e atribui suas caracteristicas
Personagens criar_personagem(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E, int HP, int speed, int tamanho, int largura);

//Cria o background da tela
Texture2D criar_background(const char* caminho_imagem);

//Descarrega todas as texturas de um personagem
void destruir_personagem(Personagens personagem);

//Inicializa a janela e as estruturas básicas
Personagens** inicializar();

// Função de mover o player (e mudar seu sentido)
void mover_player(Personagens* player);

#endif //FUNCOES_UTILIZADAS_H