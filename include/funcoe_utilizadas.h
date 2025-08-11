#ifndef FUNCOES_UTILIZADAS_H
#define FUNCOES_UTILIZADAS_H

#include "structs_e_enums_utilizadas.h"

// Cria um personagem e atribui suas caracteristicas
Personagens criar_personagem(char sentido, float posicao_x, float posicao_y,
                             Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E,
                             int HP, float speed, int tamanho, int largura);

// Cria o background da tela
Texture2D criar_background(const char* caminho_imagem);

// Descarrega texturas do personagem e limpa balas
void destruir_personagem(Personagens personagem);

// Inicializa a janela e as estruturas básicas
Personagens** inicializar();

// Função de mover o player (e mudar seu sentido)
void mover_player(Personagens* player);

// função de atirar (player)
void atirar(Personagens *player, Image sprite);

// função de mover balas (desenho, OOB, colisões básicas)
void mover_balas(Personagens **entidades, int (*qtd_entidades)[5]);

// movimentação geral do boss
void movimentacao_boss(Personagens* boss, Personagens* player, Image bala_imagem, int *timer);

// cria o boss
Personagens criar_boss(char sentido, float posicao_x, float posicao_y,
                       Image imagem_N, Image imagem_S, Image imagem_W, Image imagem_E);

// função para o boss atirar em dir x/y
void atirar_boss(Personagens *boss, Image sprite, float dir_x, float dir_y);

// função para entidade atirar na direção do player
void atirar_dir_player(Personagens *entidade, Personagens *player, Image sprite);

// desenhar boss
void desenhar_boss(Personagens *boss);

// ================= HUD =================
// Detecta, em uma textura de barra, a área útil (região vermelha) a partir do conteúdo dos pixels.
Rectangle detectar_area_hp(Texture2D tex);

// Barra de HP comum (player). Se pos.x < 0, centraliza automaticamente pela área útil.
void desenhar_barra_hp(const Personagens* player, int max_hp,
                       Texture2D barra_fill, Texture2D barra_frame,
                       Vector2 pos, float scale, Rectangle src_area);

// Barra de HP do boss, esticada na horizontal até (screenWidth - 2*margem_lateral),
// ancorada no rodapé com margem_inferior. Altura controlada por height_scale.
void desenhar_barra_boss(const Personagens* boss, int max_hp,
                         Texture2D barra_fill, Texture2D barra_frame,
                         float margem_lateral, float margem_inferior,
                         float height_scale, Rectangle src_area);
// telas
void tela_menu(GameScreen *tela_atual, Texture2D background);
void tela_opcoes(GameScreen *tela_atual, Texture2D background, float *masterVolume);
void tela_creditos(GameScreen *tela_atual, Texture2D imagemcreditos);
void tela_morte(GameScreen *tela_atual, Texture2D mortebackground);
void tela_vitoria(GameScreen *tela_atual, Texture2D vitoriabackground);

#endif // FUNCOES_UTILIZADAS_H
