#ifndef MC_E_PERSONAGENS_H
#define MC_E_PERSONAGENS_H

#include "structs_e_enums_utilizadas.h"
#include "menu.h"

//Cria um personagem e atribui suas caracteristicas
Personagens criar_personagem(char sentido, float posicao_x, float posicao_y, Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E, int HP, int speed, int tamanho, int largura);

//Inicializa a janela e as estruturas básicas
Personagens** inicializar();

// Função de mover o player (e mudar seu sentido)
void mover_player(Personagens* player);

//função de atirar 
void atirar(Personagens *player, Image sprite);

//Descarrega todas as texturas de um personagem
void destruir_personagem(Personagens personagem);

void limpeza_final(
    // matriz de personagens e contadores
    Personagens** personagens,
    int* qtd_entidades,
    int stage_sequence,

    // imagens 
    Image bala_player,
    Image bala_inimigo,

    // texturas de Background
    Texture2D background,
    Texture2D arena_jogo,
    Texture2D arena_boss,
    Texture2D lorebackground,
    Texture2D vitoriabackground,
    Texture2D derrotainimigosbackground,
    Texture2D derrotabossbackground,
    Texture2D menuImage,

    // texturas do HUD
    Texture2D hp_fill,
    Texture2D hp_frame,
    Texture2D boss_hp_fill,
    Texture2D boss_hp_frame,

    // músicas
    Music bossmusic,
    Music salainimigo,
    Music menumusic,
    Music victorymusic
);

#endif