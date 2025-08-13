#ifndef HUD_E_BACKGROUND_H
#define HUD_E_BACKGROUND_H

#include "structs_e_enums_utilizadas.h"
#include "menu.h"

//Cria o background da tela
Texture2D criar_background(const char* caminho_imagem);

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



//função para desenhar o score
void desenhar_score(int score);
#endif //FUNCOES_UTILIZADAS_H