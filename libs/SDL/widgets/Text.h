#include <SDL2/SDL_ttf.h>
#include <strings.h>

typedef struct
{
    char *text;
    TTF_Font *font;
    SDL_Color color;
    SDL_Texture *texture;
    SDL_Rect *rect;
} Text;

Text *createText(SDL_Renderer *renderer, TTF_Font *font, char *fontPath, int fontSize, char *text, SDL_Color fgClr, int x, int y)
{
    if (font == NULL)
    {
        font = TTF_OpenFont(fontPath, fontSize);
    }

    char *text2 = text;
    if (text2[0] == *"\0")
    {
        text2 = " ";
    }

    Text *widget = (Text *)malloc(sizeof(Text));
    widget->text = text;
    widget->color = fgClr;
    widget->font = font;
    widget->rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));

    SDL_Surface *surface = TTF_RenderText_Solid(font, text2, fgClr);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    widget->rect->x = x;
    widget->rect->y = y;
    widget->rect->w = surface->w;
    widget->rect->h = surface->h;
    widget->texture = texture;

    return widget;
}

void drawText(SDL_Renderer *renderer, Text *text)
{
    SDL_RenderCopy(renderer, text->texture, NULL, text->rect);
    // SDL_DestroyTexture(text->texture);
}

void updateText(SDL_Renderer *renderer, Text *widget, char *text)
{
    char *text2 = text;
    if (text2[0] == *"\0")
    {
        text2 = " ";
    }

    if (strcmp(text2, widget->text))
    {
        
        
        //free(widget->text);

        widget->text = text2;

        SDL_Surface *surface = TTF_RenderText_Solid(widget->font, text2, widget->color);
        SDL_DestroyTexture(widget->texture);
        widget->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        widget->rect->w = surface->w;
        widget->rect->h = surface->h;
    }
}

void eraseText(Text *txt)
{
    SDL_DestroyTexture(txt->texture);
    free(txt->rect);
    free(txt->text);
    free(txt);
}

void changeTextColor(Text *widget, SDL_Color color)
{
    widget->color = color;
}