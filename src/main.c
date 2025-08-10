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

    int qtd_entidades[5] = {1, 0, 0, 0, 0} ; // Quantidade de entidades de cada tipo

    int stage_sequence = 0; // 0 = Primeiro estágio; 1 = Transição_Primeiro_e_Boss; 2 = Boss;
    double time = 0;
    Vector2 middle_circle;
    Color blue = {0, 100, 255, 255};
    Texture2D textura_atual_player; // Variável para guardar a textura a ser desenhada do player
    Image bala_player = LoadImage("characters/bullet.png");
    Image bala_inimigo = LoadImage("characters/enemy_bullet.png");
    int cooldown = 60; // Variável para controlar o cooldown do tiro
    int timer = 0; // Timer para controlar os movimentos do boss
    int dano_colldown = 0;

    Image img_N = LoadImage("characters/corredor_EAST.png");
    Image img_A = LoadImage("characters/atirador_east.png");
    
    while (WindowShouldClose() == false)
    {
        cooldown++;
        //TEMPO
        if (time < 120 && stage_sequence == 0)
        {
            time = GetTime();
            int time_spawn = 2;
            if(((int)time % time_spawn == 0) && (time > 0) && (((int)time != ((int)(time - GetFrameTime()))))) // Evita spawns múltiplos no mesmo segundo
            {
                
                spawnador(personagens, &qtd_entidades, img_N, img_N, img_N, img_N, 1);
                spawnador(personagens, &qtd_entidades, img_A, img_A, img_A, img_A, 2);
                
            }
            
        }

        //EVENT HANDLING
        
        //movendo o inimigo corredor
        for(int i = 0;i<qtd_entidades[1];i++){
            mover_inimigo(&personagens[1][i], &personagens[0][0]);
        }
        for(int i = 0;i<qtd_entidades[2];i++){ //move o inimigo atirador
            mover_atirador(&personagens[2][i]);
        }
        if (dano_colldown > 0) dano_colldown--;

        for (int i = 0; i < qtd_entidades[1]; i++) {
            if (CheckCollisionRecs(personagens[0][0].hitbox, personagens[1][i].hitbox) && dano_colldown == 0) {
                personagens[0][0].HP--;
                dano_colldown = 60;
                if(personagens[0][0].HP < 0){
                    personagens[0][0].HP = 0;
                    destruir_personagem(personagens[0][0]);
                } 
            }
        }

      
        
        mover_player(personagens[0]);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && cooldown >= 60)
        {
            cooldown = 0; // Reseta o cooldown
            atirar(&personagens[0][0], bala_player);
        }
    
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
        if (stage_sequence == 2)
        {
            if (timer == 0) { //cria o boss
                personagens[4][0] = criar_boss('S', (GetScreenWidth() / 2), (GetScreenHeight() / 2) + 70,
                                        LoadImage("characters/Robo_Gladiador_NORTH.png"),
                                        LoadImage("characters/Robo_Gladiador_SOUTH.png"),
                                        LoadImage("characters/Robo_Gladiador_WEST.png"),
                                        LoadImage("characters/Robo_Gladiador_EAST.png"));

                qtd_entidades[4] = 1; // Atualiza a quantidade de entidades do tipo boss
            }
            movimentacao_boss(&personagens[4][0], &personagens[0][0], bala_inimigo, &timer);
            timer++;
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

        // Dentro do BeginDrawing(), por exemplo, logo após desenhar o background:
        
        switch (stage_sequence)
        {
            case 0:
            DrawText(TextFormat("%d : %02d", ((int)time) / 60, ((int)time) % 60), 10, 10, 50, WHITE);
            DrawText("MISSÃO: SOBREVIVER POR 2 MINUTOS!", 10, GetScreenHeight() - 50, 30, WHITE);
            //desenhando inimigos
            desenhar_inimigos(personagens, &qtd_entidades);
            break;
            case 1:
            DrawCircleV(middle_circle, 30, blue);
            DrawText("MISSÃO: ENTRE NO PORTAL PARA ENFRENTAR O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
            break;
            case 2:
            if (personagens[0][0].HP == 0) {
                DrawText("MORREU ZÉ", 10, GetScreenHeight() - 50, 30, WHITE); //só p testar
                background = criar_background("background/foxe.png");
            }else {
                DrawText("MISSÃO: DERROTE O BOSS!", 10, GetScreenHeight() - 50, 30, WHITE);
            }
            desenhar_boss(&personagens[4][0]);

            break;
        }
        
        DrawTextureV(textura_atual_player, (Vector2){personagens[0][0].hitbox.x, personagens[0][0].hitbox.y}, WHITE);
        mover_balas(personagens, &qtd_entidades, bala_inimigo);
        EndDrawing();
    }
    
    // LIMPEZA FINAL (é bom fazer uma função disso no futuro)
    destruir_personagem(personagens[0][0]);
    if(stage_sequence==2){
        destruir_personagem(personagens[4][0]);
    }
    UnloadTexture(background);
    UnloadImage(bala_player);   
   
    for (int i = 0; i < qtd_entidades[1]; i++) {
    destruir_personagem(personagens[1][i]);
    }
    for (int i = 0; i < qtd_entidades[2]; i++) {
        destruir_personagem(personagens[2][i]);
    }
    for (int i = 0; i < qtd_entidades[3]; i++) {
        destruir_personagem(personagens[3][i]);
    }


    for (int i = 1; i < 4; i++) {
        if (personagens[i]) free(personagens[i]);
    }

    free(personagens[0]);
    free(personagens);
    CloseWindow();

    return 0;
}