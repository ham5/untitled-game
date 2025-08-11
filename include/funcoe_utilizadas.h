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

//função de atirar 
void atirar(Personagens *player, Image sprite);

//função de mover balas
void mover_balas(Personagens **entidades, int (*qtd_entidades)[5], Image IMAGEM_BALA, Image imagem_explosao);

//movimentação geral do boss
void movimentacao_boss(Personagens* boss, Personagens* player, Image bala_imagem, int *timer);

//cria o boss
Personagens criar_boss(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E);

//função para o na direção x e y (usado pelo boss)
void atirar_boss(Personagens *boss, Image sprite, float dir_x, float dir_y);

//função para atirar na direção do player
void atirar_dir_player(Personagens *entidade, Personagens *player, Image sprite);

//função para desenhar o boss
void desenhar_boss(Personagens *boss);

//função para adicionar inimigos
void desenhar_inimigos(Personagens** entidades, int (*qtd_entidades)[5]);

//função pra peidar <- foi gustavo que fez
void spawnador(Personagens** entidades,  int (*qtd_entidades)[5], Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E, int tipo);

//função para adcionar inimigos
void adicionar_inimigo(Personagens** entidades, int (*qtd_entidades)[5], int spawn_x, int spawn_y, int id, Image IMAGEM_N, Image IMAGEM_S, Image IMAGEM_W, Image IMAGEM_E);

//função para mover os inimigos
void mover_inimigo (Personagens* inimigo, Personagens* player);

//função para mover o atirador
void mover_atirador (Personagens* imimigo);

//função de explosão
void explodir_corredor(Personagens personagem, Image explosao_img);

//função para destruir os inimigos
void destruir_inimigos(Personagens** entidades, int (*qtd_entidades)[5]);

//função para fim de jogo
void destruir_inimigos_e_boss(Personagens** entidades, int (*qtd_entidades)[5]);
#endif //FUNCOES_UTILIZADAS_H