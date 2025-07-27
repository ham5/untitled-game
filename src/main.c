#include "funcoe_utilizadas.h" // A única inclusão necessária para suas funções
#include "resource_dir.h"

int main (void)
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SearchAndSetResourceDir("resources");

    Personagens** personagens = inicializar();
    Texture2D background = criar_background("background/Arena.png");
    /*
    EXPLICANDO O VETOR DE VETORES personagens
    personagens[0] vai conter somente o vetor do player
    personagens[1] vai conter o vetor de todos os inimigos corredores
    personagens[2] vai conter o vetor de todos os inimigos atiradores horizontais
    personagens[3] vai conter o vetor de todos os inimigos atiradores verticais
    personagens[4] vai conter somente o vetor do boss 
    */

    personagens[0][0] = criar_personagem('E', (GetScreenWidth() / 2) - 25, (GetScreenHeight() / 2) - 50, 
                                        LoadImage("characters/Robo_Gladiador_NORTH.png"),
                                        LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                                        LoadImage("characters/Robo_Gladiador_WEST.png"),
                                        LoadImage("characters/Robo_Gladiador_EAST.png"), 
                                        3, 3, 50, 50);

    int stage_sequence = 0; // 0 = Primeiro estágio; 1 = Transição_Primeiro_e_Boss; 2 = Boss;
    double time = 0;
    Vector2 middle_circle;
    Color blue = {0, 100, 255, 255};
    Texture2D textura_atual_player; // Variável para guardar a textura a ser desenhada do player
    while (WindowShouldClose() == false)
    {
        //TEMPO
        if (time < 120 && stage_sequence == 0)
        {
            time = GetTime();
        }

        //EVENT HANDLING
        mover_player(personagens[0]);
        if (time >= 120 && stage_sequence == 0)
        {
            stage_sequence = 1;
            middle_circle.x =(GetScreenWidth() / 2.0f);
            middle_circle.y = (GetScreenHeight() / 2.0f) - 5;
        }
        if (stage_sequence == 1 && CheckCollisionCircleRec(middle_circle, 30, personagens[0][0].hitbox))
        {
            //Lembrar de limpar todos os inimigos da tela
            stage_sequence = 2;
            UnloadTexture(background); // Descarrega o background antigo
            background = criar_background("background/Sala_Boss.png");
            time = 0;
        }

        // DRAWING
        // Escolhe qual textura desenhar com base no sentido atual do player
        switch (personagens[0][0].sentido)
        {
            case 'N': 
                textura_atual_player = personagens[0][0].sprite_N; 
                break;
            case 'S': 
                textura_atual_player = personagens[0][0].sprite_S; 
                break;
            case 'W': 
                textura_atual_player = personagens[0][0].sprite_W; 
                break;
            case 'E': 
                textura_atual_player = personagens[0][0].sprite_E; 
                break;
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(background, 0, 0, WHITE);
        
        switch (stage_sequence)
        {
            case 0:
                DrawText(TextFormat("%d : %02d", ((int)time) / 60, ((int)time) % 60), 10, 10, 50, WHITE);
                DrawText("MISSÃO: SOBREVIVER POR 2 MINUTOS!", 10, GetScreenHeight() - 50, 30, WHITE);
                break;
            case 1:
                DrawCircleV(middle_circle, 30, blue);
                DrawText("MISSÃO: ENTRE NO PORTAL PARA ENFRENTAR O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                break;
            case 2:
                DrawText("MISSÃO: DERROTE O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
                break;
        }
        
        DrawTextureV(textura_atual_player, (Vector2){personagens[0][0].hitbox.x, personagens[0][0].hitbox.y}, WHITE);
        
        EndDrawing();
    }
    
    // LIMPEZA FINAL (é bom fazer uma função disso no futuro)
    destruir_personagem(personagens[0][0]);
    UnloadTexture(background);
    free(personagens[0]);
    free(personagens);
    CloseWindow();

    return 0;
}