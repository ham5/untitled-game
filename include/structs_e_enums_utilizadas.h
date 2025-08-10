#ifndef STRUCTS_E_ENUMS_UTILIZADAS_H
#define STRUCTS_E_ENUMS_UTILIZADAS_H

#include <raylib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Bullet
{
    Vector2   direcao;
    Rectangle hitbox_bala;   // hitbox da bala
    Texture2D sprite_bala;
} Bullet;

// Estrutura base de um Personagem (inimigo, player, boss, etc.)
typedef struct Personagens
{
    char      sentido;     // North, South, East, West
    Texture2D sprite_N;    // Textura para o Norte
    Texture2D sprite_S;    // Textura para o Sul
    Texture2D sprite_E;    // Textura para o Leste
    Texture2D sprite_W;    // Textura para o Oeste
    int       HP;          // Quantidade de vida do personagem
    float     speed;       // Velocidade com que o personagem vai se mover (float)
    Rectangle hitbox;      // Hitbox associada ao personagem

    Bullet* balas;         // vetor dinâmico para armazenar as balas
    int     qtd_balas;
} Personagens;

// Tipos de personagem (pode ser útil)
enum Tipo_Personagem
{
    Player,
    Corredor,
    Atirador_Horizontal,
    Atirador_Vertical,
    Boss
};

// Enum pro switch-case para deixar o codigo mais legível
typedef enum GameScreen
{
    MENU,
    OPTIONS,
    CREDITOS,
    GAMEPLAY,
    MORTE,
    VITORIA
} GameScreen;

#endif // STRUCTS_E_ENUMS_UTILIZADAS_H
