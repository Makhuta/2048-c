#include <SDL2/SDL.h>

#include "../headers/structures.h"
#include "../headers/functions.h"
#include "../headers/functions_statics.h"

#define TEXT_PADDING 5

void draw_filled_rounded_rectangle(Rounded_Rectangle_filled* self, SDL_Renderer* renderer){
    int radius = (self->rect.h < self->rect.w ? self->rect.h : self->rect.w) * self->roundness / 100 / 2;
    const int32_t diameter = (radius * 2);
    int centreX = self->rect.x - TEXT_PADDING * 0 + radius;
    int centreY = self->rect.y - TEXT_PADDING * 0 + radius;
    int w = self->rect.w - diameter;
    int h = self->rect.h - diameter;


    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    SDL_SetRenderDrawColor(renderer, self->color.r, self->color.g, self->color.b, self->color.a);

    if(self->roundness > 0) {
        while (x >= y)
        {

        SDL_Rect rect = {.x = centreX - x, .y = centreY - y, .w = w + x * 2 + TEXT_PADDING * 2 * 0, .h = h + y * 2 + TEXT_PADDING * 2 * 0};
        SDL_RenderFillRect(renderer, &rect);
        rect.x = centreX - y;
        rect.y = centreY - x;
        rect.w = w + y * 2 + TEXT_PADDING * 2 * 0;
        rect.h = h + x * 2 + TEXT_PADDING * 2 * 0;
        SDL_RenderFillRect(renderer, &rect);

          if (error <= 0)
          {
            ++y;
            error += ty;
            ty += 2;
          }

          if (error > 0)
          {
            --x;
            tx += 2;
            error += (tx - diameter);
          }
        }
    } else {
        SDL_Rect rect = {.x = centreX, .y = centreY, .w = self->rect.w + TEXT_PADDING * 2, .h = self->rect.h};
        SDL_RenderFillRect(renderer, &rect);
        rect.x = centreX;
        rect.y = centreY;
        rect.w = self->rect.w + TEXT_PADDING * 2 * 0;
        rect.h = self->rect.h;
        SDL_RenderFillRect(renderer, &rect);
    }
}

void draw_outline_rounded_rectangle(Rounded_Rectangle_outline* self, SDL_Renderer* renderer){
    Rounded_Rectangle_filled rect;

    rect.roundness = self->roundness;
    rect.rect.x = self->rect.x - TEXT_PADDING * 2;
    rect.rect.y = self->rect.y - TEXT_PADDING;
    rect.rect.h = self->rect.h + TEXT_PADDING * 2;
    rect.rect.w = self->rect.w + TEXT_PADDING * 4;
    rect.color.a = self->outline_color.a;
    rect.color.r = self->outline_color.r;
    rect.color.g = self->outline_color.g;
    rect.color.b = self->outline_color.b;


    draw_filled_rounded_rectangle(&rect, renderer);

    int t = self->thickness;

    rect.rect.x = self->rect.x + t - TEXT_PADDING * 2;
    rect.rect.y = self->rect.y + t - TEXT_PADDING;
    rect.rect.h = self->rect.h - t * 2 + TEXT_PADDING * 2;
    rect.rect.w = self->rect.w - t * 2 + TEXT_PADDING * 4;
    rect.color.a = self->inside_color.a;
    rect.color.r = self->inside_color.r;
    rect.color.g = self->inside_color.g;
    rect.color.b = self->inside_color.b;

    draw_filled_rounded_rectangle(&rect, renderer);
}
