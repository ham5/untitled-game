#ifndef STRUCTS_E_ENUMS_UTILIZADAS_H
#define STRUCTS_E_ENUMS_UTILIZADAS_H

#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura base de um Personagem (inimigo, player, boss, etc.)
typedef struct Personagens 
{
    char sentido; // North, South, East, West
    Texture2D sprite_N; // Textura para o Norte
    Texture2D sprite_S; // Textura para o Sul
    Texture2D sprite_E; // Textura para o Leste
    Texture2D sprite_W; // Textura para o Oeste
    int HP; // Quantidade de vida do personagem
    int speed; // Velocidade com o que o personagem vai se mover
    Rectangle hitbox; // Hitbox (Retangulo) associada ao personagem
} Personagens;

//Achei legal fazer um enum dos tipos de Personagens que vai ter, vai que pode ser útil.
enum Tipo_Personagem
{
    Player,
    Corredor,
    Atirador_Horizontal,
    Atirador_Vertical,
    Boss
};

#endif //STRUCTS_E_ENUMS_UTILIZADAS_H