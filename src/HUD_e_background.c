#include "structs_e_enums_utilizadas.h"
#include "menu.h"
#include <math.h>

//Cria o background da tela
Texture2D criar_background(const char* caminho_imagem)
{
    Image imagem = LoadImage(caminho_imagem);
    Texture2D background = LoadTextureFromImage(imagem);
    UnloadImage(imagem);
    return background;
}

void desenhar_score(int score)
{
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    int fontSize = 30;
    int textWidth = MeasureText(scoreText, fontSize);
    DrawText(scoreText, GetScreenWidth() - textWidth - 20, 20, fontSize, WHITE);
}


/* ===========================
   HUD: BARRA DE HP (auto-detecção da área útil)
   =========================== */

Rectangle detectar_area_hp(Texture2D tex)
{
    // fallback: se falhar, usa a textura inteira
    Rectangle area = { 0, 0, (float)tex.width, (float)tex.height };

    // Converte a Texture para Image para poder ler pixels
    Image img = LoadImageFromTexture(tex);
    if (img.data == NULL) return area;

    Color *px = LoadImageColors(img);
    if (px == NULL) { UnloadImage(img); return area; }

    int minx = img.width, miny = img.height, maxx = -1, maxy = -1;

    // Heurística: “vermelho dominante” (ajuste os thresholds se precisar)
    const int TH = 25;   // quanto o R precisa ser maior que G e B
    for (int y = 0; y < img.height; y++)
    {
        for (int x = 0; x < img.width; x++)
        {
            Color c = px[y*img.width + x];
            if (c.a < 10) continue;               // ignora totalmente transparente
            if ((int)c.r - (int)c.g > TH && (int)c.r - (int)c.b > TH)
            {
                if (x < minx) minx = x;
                if (y < miny) miny = y;
                if (x > maxx) maxx = x;
                if (y > maxy) maxy = y;
            }
        }
    }

    UnloadImageColors(px);
    UnloadImage(img);

    if (maxx >= minx && maxy >= miny)
    {
        area.x = (float)minx;
        area.y = (float)miny;
        area.width  = (float)(maxx - minx + 1);
        area.height = (float)(maxy - miny + 1);
    }
    // senão, permanece fallback (textura toda)

    return area;
}

/* ===========================
   HUD: BARRA DE HP (player)
   =========================== */
void desenhar_barra_hp(const Personagens* player, int max_hp,
                       Texture2D barra_fill, Texture2D barra_frame,
                       Vector2 pos, float scale, Rectangle src_area)
{
    if (barra_fill.id == 0) return;

    int safe_max = (max_hp > 0) ? max_hp : 1;
    float ratio = (float)player->HP / (float)safe_max;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;

    // usa a área útil recebida
    Rectangle srcArea = src_area;

    int drawW = (int)(srcArea.width  * scale);
    int drawH = (int)(srcArea.height * scale);

    // Auto-centraliza se pos.x < 0
    float x = (pos.x < 0.0f) ? (GetScreenWidth() - drawW) * 0.5f : pos.x;
    float y = pos.y;

     // === FUNDO "VIDRO FOSCO" (substitui o bloco preto antigo) ===
    const Color GLASS = (Color){ 200, 205, 215, 255 }; // tom de vidro
    Rectangle bg = (Rectangle){ x, y, (float)drawW, (float)drawH };

    DrawRectangleRec(bg, Fade(GLASS, 0.40f)); // painel principal translúcido
    DrawRectangleRec((Rectangle){ bg.x - 1, bg.y - 1, bg.width + 2, bg.height + 2 }, Fade(GLASS, 0.22f));
    DrawRectangleRec((Rectangle){ bg.x - 3, bg.y - 3, bg.width + 6, bg.height + 6 }, Fade(GLASS, 0.10f));

    // brilho topo + sombra base (opcional)
    DrawRectangle((int)bg.x, (int)bg.y, (int)bg.width, 1, Fade(WHITE, 0.35f));
    DrawRectangle((int)bg.x, (int)(bg.y + bg.height - 1), (int)bg.width, 1, Fade(BLACK, 0.20f));
    // === FIM DO FUNDO ===

    // parte cheia proporcional
    Rectangle srcClip = srcArea; srcClip.width = srcArea.width * ratio;
    Rectangle dst = (Rectangle){ x, y, srcClip.width * scale, (float)drawH };
    DrawTexturePro(barra_fill, srcClip, dst, (Vector2){0,0}, 0.0f, WHITE);

    // moldura, se houver
    if (barra_frame.id != 0)
    {
        Rectangle dstFrame = (Rectangle){ x, y, srcArea.width * scale, (float)drawH };
        DrawTexturePro(barra_frame, srcArea, dstFrame, (Vector2){0,0}, 0.0f, WHITE);
    }
}


void desenhar_barra_boss(const Personagens* boss, int max_hp,
                         Texture2D barra_fill, Texture2D barra_frame,
                         float margem_lateral, float margem_inferior,
                         float height_scale, Rectangle src_area)
{
    if (barra_fill.id == 0) return;

    int safe_max = (max_hp > 0) ? max_hp : 1;
    float ratio = (float)boss->HP / (float)safe_max;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;

    Rectangle srcArea = src_area;

    int drawW = (int)(GetScreenWidth() - 2.0f * margem_lateral);
    int drawH = (int)(srcArea.height * height_scale);

    float x = margem_lateral;
    float y = GetScreenHeight() - margem_inferior - drawH;

    // === FUNDO "VIDRO FOSCO" (substitui o bloco preto antigo) ===
    const Color GLASS = (Color){ 200, 205, 215, 255 };
    Rectangle bg = (Rectangle){ x, y, (float)drawW, (float)drawH };

    DrawRectangleRec(bg, Fade(GLASS, 0.40f));
    DrawRectangleRec((Rectangle){ bg.x - 1, bg.y - 1, bg.width + 2, bg.height + 2 }, Fade(GLASS, 0.22f));
    DrawRectangleRec((Rectangle){ bg.x - 3, bg.y - 3, bg.width + 6, bg.height + 6 }, Fade(GLASS, 0.10f));

    // brilho topo + sombra base (opcional)
    DrawRectangle((int)bg.x, (int)bg.y, (int)bg.width, 1, Fade(WHITE, 0.35f));
    DrawRectangle((int)bg.x, (int)(bg.y + bg.height - 1), (int)bg.width, 1, Fade(BLACK, 0.20f));
    // === FIM DO FUNDO ===

    // parte cheia proporcional (esticada)
    Rectangle dst = (Rectangle){ x, y, drawW * ratio, (float)drawH };
    DrawTexturePro(barra_fill, srcArea, dst, (Vector2){0,0}, 0.0f, WHITE);

    // moldura, se houver
    if (barra_frame.id != 0)
    {
        Rectangle dstFrame = (Rectangle){ x, y, (float)drawW, (float)drawH };
        DrawTexturePro(barra_frame, srcArea, dstFrame, (Vector2){0,0}, 0.0f, WHITE);
    }
}