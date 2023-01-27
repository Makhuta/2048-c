#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../headers/structures.h"
#include "../headers/windows_drawer.h"
#include "../headers/windows_drawer_statics.h"
#include "../headers/functions.h"
#include "../headers/init.h"
#include "../headers/manipulation.h"

void draw_window(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans) {
    if(self->globals.windowses.afk_window.idle_threshold > self->globals.windowses.afk_window.idle_time) {
        if(self->globals.selected_window == 0) {
            draw_main_menu(self, renderer, sans);
        } else if(self->globals.selected_window == 1) {
            draw_game(self, renderer, sans);
            update_score(self);
            if(!self->globals.is_playable) {
                draw_game_over(self, renderer, sans);
            }
        } else if(self->globals.selected_window == 2) {
            draw_options(self, renderer, sans);
        }
        SDL_SetRenderDrawColor(renderer, self->globals.themes[self->setting.selected_theme]->background.r, self->globals.themes[self->setting.selected_theme]->background.g, self->globals.themes[self->setting.selected_theme]->background.b, self->globals.themes[self->setting.selected_theme]->background.a); // Nastaven� barvy
    } else {
        draw_afk(self, renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastaven� barvy
    }
}

static void draw_afk(Main_obj* self, SDL_Renderer* renderer) {
    SDL_Rect star_rect;
    for(int i = 0; i < self->globals.windowses.afk_window.number_of_start; i++) {
        star_rect.x = self->globals.windowses.afk_window.stars[i]->circle.rect.x;
        star_rect.y = self->globals.windowses.afk_window.stars[i]->circle.rect.y;
        star_rect.w = self->globals.windowses.afk_window.stars[i]->circle.rect.w;
        star_rect.h = self->globals.windowses.afk_window.stars[i]->circle.rect.h;
        
        draw_filled_rounded_rectangle(&self->globals.windowses.afk_window.stars[i]->circle, renderer);
        int moved_x = self->globals.windowses.afk_window.stars[i]->circle.rect.x + self->globals.windowses.afk_window.stars[i]->circle.rect.w / 2;
        int moved_y = self->globals.windowses.afk_window.stars[i]->circle.rect.y + self->globals.windowses.afk_window.stars[i]->circle.rect.h / 2;
        int velocity_x = self->globals.windowses.afk_window.stars[i]->velocity.x * self->globals.windowses.afk_window.direction.x;
        int velocity_y = self->globals.windowses.afk_window.stars[i]->velocity.y * self->globals.windowses.afk_window.direction.y;
        int trail_length_multiplier = 7;
        SDL_SetRenderDrawColor(renderer, self->globals.windowses.afk_window.stars[i]->circle.color.r, self->globals.windowses.afk_window.stars[i]->circle.color.g, self->globals.windowses.afk_window.stars[i]->circle.color.b, self->globals.windowses.afk_window.stars[i]->circle.color.a);
        SDL_RenderDrawLine(renderer, moved_x, moved_y, moved_x - velocity_x * trail_length_multiplier, moved_y - velocity_y * trail_length_multiplier);



        if(self->globals.windowses.afk_window.stars[i]->circle.rect.x + velocity_x > self->globals.window_dimensions.w) {
            self->globals.windowses.afk_window.stars[i]->circle.rect.x = 0;
        } else if (self->globals.windowses.afk_window.stars[i]->circle.rect.x + velocity_x < 0) {
            self->globals.windowses.afk_window.stars[i]->circle.rect.x = self->globals.window_dimensions.w;
        }else {
            self->globals.windowses.afk_window.stars[i]->circle.rect.x += velocity_x;
        }

        if(self->globals.windowses.afk_window.stars[i]->circle.rect.y + velocity_y > self->globals.window_dimensions.h) {
            self->globals.windowses.afk_window.stars[i]->circle.rect.y = 0;
        } else if (self->globals.windowses.afk_window.stars[i]->circle.rect.y + velocity_y < 0) {
            self->globals.windowses.afk_window.stars[i]->circle.rect.y = self->globals.window_dimensions.h;
        }else {
            self->globals.windowses.afk_window.stars[i]->circle.rect.y += velocity_y;
        }
    }
}

static void draw_main_menu(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans){
    int selected_theme = self->setting.selected_theme;
    SDL_Color text_color = {.r = self->globals.themes[selected_theme]->text_color.r, .g = self->globals.themes[selected_theme]->text_color.g, .b = self->globals.themes[selected_theme]->text_color.b};

    //Game name text
    SDL_Rect name_rect = {.h = self->globals.windowses.main_menu.name_text.dimensions.h, .w = self->globals.windowses.main_menu.name_text.dimensions.w, .x = self->globals.windowses.main_menu.name_text.coords.x, .y = self->globals.windowses.main_menu.name_text.coords.y};
    SDL_Surface* name_surface = TTF_RenderText_Solid(sans, self->globals.windowses.main_menu.name_text.text, text_color);
    SDL_Texture* name_texture = SDL_CreateTextureFromSurface(renderer, name_surface);
    SDL_RenderCopy(renderer, name_texture, NULL, &name_rect);
    SDL_FreeSurface(name_surface);
    SDL_DestroyTexture(name_texture);

    //Start button
    self->globals.windowses.main_menu.start_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.main_menu.start_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.main_menu.start_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.main_menu.start_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.main_menu.start_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.main_menu.start_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.main_menu.start_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.main_menu.start_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.main_menu.start_btn.position, renderer);
    SDL_Surface* start_surface = TTF_RenderText_Solid(sans, self->globals.windowses.main_menu.start_btn.text, text_color);
    SDL_Texture* start_texture = SDL_CreateTextureFromSurface(renderer, start_surface);
    SDL_RenderCopy(renderer, start_texture, NULL, &self->globals.windowses.main_menu.start_btn.position.rect);
    SDL_FreeSurface(start_surface);
    SDL_DestroyTexture(start_texture);

    //Continue button
    self->globals.windowses.main_menu.continue_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.main_menu.continue_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.main_menu.continue_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.main_menu.continue_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.main_menu.continue_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.main_menu.continue_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.main_menu.continue_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.main_menu.continue_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.main_menu.continue_btn.position, renderer);
    SDL_Surface* continue_surface = TTF_RenderText_Solid(sans, self->globals.windowses.main_menu.continue_btn.text, text_color);
    SDL_Texture* continue_texture = SDL_CreateTextureFromSurface(renderer, continue_surface);
    SDL_RenderCopy(renderer, continue_texture, NULL, &self->globals.windowses.main_menu.continue_btn.position.rect);
    SDL_FreeSurface(continue_surface);
    SDL_DestroyTexture(continue_texture);

    //Options button
    self->globals.windowses.main_menu.options_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.main_menu.options_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.main_menu.options_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.main_menu.options_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.main_menu.options_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.main_menu.options_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.main_menu.options_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.main_menu.options_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.main_menu.options_btn.position, renderer);
    SDL_Surface* options_surface = TTF_RenderText_Solid(sans, self->globals.windowses.main_menu.options_btn.text, text_color);
    SDL_Texture* options_texture = SDL_CreateTextureFromSurface(renderer, options_surface);
    SDL_RenderCopy(renderer, options_texture, NULL, &self->globals.windowses.main_menu.options_btn.position.rect);
    SDL_FreeSurface(options_surface);
    SDL_DestroyTexture(options_texture);

    //Exit button
    self->globals.windowses.main_menu.exit_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.main_menu.exit_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.main_menu.exit_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.main_menu.exit_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.main_menu.exit_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.main_menu.exit_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.main_menu.exit_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.main_menu.exit_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.main_menu.exit_btn.position, renderer);
    SDL_Surface* exit_surface = TTF_RenderText_Solid(sans, self->globals.windowses.main_menu.exit_btn.text, text_color);
    SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surface);
    SDL_RenderCopy(renderer, exit_texture, NULL, &self->globals.windowses.main_menu.exit_btn.position.rect);
    SDL_FreeSurface(exit_surface);
    SDL_DestroyTexture(exit_texture);

}

static void draw_options(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans){
    int selected_theme = self->setting.selected_theme;
    SDL_Color text_color = {.r = self->globals.themes[selected_theme]->text_color.r, .g = self->globals.themes[selected_theme]->text_color.g, .b = self->globals.themes[selected_theme]->text_color.b};

    //Game name text
    SDL_Rect name_rect = {.h = self->globals.windowses.options.name_text.dimensions.h, .w = self->globals.windowses.options.name_text.dimensions.w, .x = self->globals.windowses.options.name_text.coords.x, .y = self->globals.windowses.options.name_text.coords.y};
    SDL_Surface* name_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.name_text.text, text_color);
    SDL_Texture* name_texture = SDL_CreateTextureFromSurface(renderer, name_surface);
    SDL_RenderCopy(renderer, name_texture, NULL, &name_rect);
    SDL_FreeSurface(name_surface);
    SDL_DestroyTexture(name_texture);


    //Rows up button
    self->globals.windowses.options.rows_up_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.rows_up_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.rows_up_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.rows_up_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.rows_up_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.rows_up_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.rows_up_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.rows_up_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.rows_up_btn.position, renderer);
    SDL_Surface* rows_up_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.rows_up_btn.text, text_color);
    SDL_Texture* rows_up_texture = SDL_CreateTextureFromSurface(renderer, rows_up_surface);
    SDL_RenderCopy(renderer, rows_up_texture, NULL, &self->globals.windowses.options.rows_up_btn.position.rect);
    SDL_FreeSurface(rows_up_surface);
    SDL_DestroyTexture(rows_up_texture);

    //Cols up button
    self->globals.windowses.options.cols_up_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.cols_up_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.cols_up_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.cols_up_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.cols_up_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.cols_up_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.cols_up_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.cols_up_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.cols_up_btn.position, renderer);
    SDL_Surface* cols_up_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.cols_up_btn.text, text_color);
    SDL_Texture* cols_up_texture = SDL_CreateTextureFromSurface(renderer, cols_up_surface);
    SDL_RenderCopy(renderer, cols_up_texture, NULL, &self->globals.windowses.options.cols_up_btn.position.rect);
    SDL_FreeSurface(cols_up_surface);
    SDL_DestroyTexture(cols_up_texture);

    //Rows text
    SDL_Rect rows_text_rect = {.h = self->globals.windowses.options.rows_text.dimensions.h, .w = self->globals.windowses.options.rows_text.dimensions.w, .x = self->globals.windowses.options.rows_text.coords.x, .y = self->globals.windowses.options.rows_text.coords.y};
    SDL_Surface* rows_text_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.rows_text.text, text_color);
    SDL_Texture* rows_text_texture = SDL_CreateTextureFromSurface(renderer, rows_text_surface);
    SDL_RenderCopy(renderer, rows_text_texture, NULL, &rows_text_rect);
    SDL_FreeSurface(rows_text_surface);
    SDL_DestroyTexture(rows_text_texture);

    //Cols text
    SDL_Rect cols_text_rect = {.h = self->globals.windowses.options.cols_text.dimensions.h, .w = self->globals.windowses.options.cols_text.dimensions.w, .x = self->globals.windowses.options.cols_text.coords.x, .y = self->globals.windowses.options.cols_text.coords.y};
    SDL_Surface* cols_text_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.cols_text.text, text_color);
    SDL_Texture* cols_text_texture = SDL_CreateTextureFromSurface(renderer, cols_text_surface);
    SDL_RenderCopy(renderer, cols_text_texture, NULL, &cols_text_rect);
    SDL_FreeSurface(cols_text_surface);
    SDL_DestroyTexture(cols_text_texture);

    //Rows down button
    self->globals.windowses.options.rows_down_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.rows_down_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.rows_down_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.rows_down_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.rows_down_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.rows_down_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.rows_down_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.rows_down_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.rows_down_btn.position, renderer);
    SDL_Surface* rows_down_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.rows_down_btn.text, text_color);
    SDL_Texture* rows_down_texture = SDL_CreateTextureFromSurface(renderer, rows_down_surface);
    SDL_RenderCopy(renderer, rows_down_texture, NULL, &self->globals.windowses.options.rows_down_btn.position.rect);
    SDL_FreeSurface(rows_down_surface);
    SDL_DestroyTexture(rows_down_texture);

    //Cols down button
    self->globals.windowses.options.cols_down_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.cols_down_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.cols_down_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.cols_down_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.cols_down_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.cols_down_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.cols_down_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.cols_down_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.cols_down_btn.position, renderer);
    SDL_Surface* cols_down_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.cols_down_btn.text, text_color);
    SDL_Texture* cols_down_texture = SDL_CreateTextureFromSurface(renderer, cols_down_surface);
    SDL_RenderCopy(renderer, cols_down_texture, NULL, &self->globals.windowses.options.cols_down_btn.position.rect);
    SDL_FreeSurface(cols_down_surface);
    SDL_DestroyTexture(cols_down_texture);

    //Theme left button
    self->globals.windowses.options.theme_left_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.theme_left_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.theme_left_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.theme_left_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.theme_left_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.theme_left_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.theme_left_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.theme_left_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.theme_left_btn.position, renderer);
    SDL_Surface* theme_left_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.theme_left_btn.text, text_color);
    SDL_Texture* theme_left_texture = SDL_CreateTextureFromSurface(renderer, theme_left_surface);
    SDL_RenderCopy(renderer, theme_left_texture, NULL, &self->globals.windowses.options.theme_left_btn.position.rect);
    SDL_FreeSurface(theme_left_surface);
    SDL_DestroyTexture(theme_left_texture);

    //Theme text
    SDL_Rect theme_text_rect = {.h = self->globals.windowses.options.theme_text.dimensions.h, .w = self->globals.windowses.options.theme_text.dimensions.w, .x = self->globals.windowses.options.theme_text.coords.x, .y = self->globals.windowses.options.theme_text.coords.y};
    SDL_Surface* theme_text_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.theme_text.text, text_color);
    SDL_Texture* theme_text_texture = SDL_CreateTextureFromSurface(renderer, theme_text_surface);
    SDL_RenderCopy(renderer, theme_text_texture, NULL, &theme_text_rect);
    SDL_FreeSurface(theme_text_surface);
    SDL_DestroyTexture(theme_text_texture);

    //Theme right button
    self->globals.windowses.options.theme_right_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.theme_right_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.theme_right_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.theme_right_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.theme_right_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.theme_right_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.theme_right_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.theme_right_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.theme_right_btn.position, renderer);
    SDL_Surface* theme_right_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.theme_right_btn.text, text_color);
    SDL_Texture* theme_right_texture = SDL_CreateTextureFromSurface(renderer, theme_right_surface);
    SDL_RenderCopy(renderer, theme_right_texture, NULL, &self->globals.windowses.options.theme_right_btn.position.rect);
    SDL_FreeSurface(theme_right_surface);
    SDL_DestroyTexture(theme_right_texture);

    //Is endless button
    self->globals.windowses.options.is_endless_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.is_endless_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.is_endless_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.is_endless_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.is_endless_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.is_endless_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.is_endless_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.is_endless_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.is_endless_btn.position, renderer);
    SDL_Surface* is_endless_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.is_endless_btn.text, text_color);
    SDL_Texture* is_endless_texture = SDL_CreateTextureFromSurface(renderer, is_endless_surface);
    SDL_RenderCopy(renderer, is_endless_texture, NULL, &self->globals.windowses.options.is_endless_btn.position.rect);
    SDL_FreeSurface(is_endless_surface);
    SDL_DestroyTexture(is_endless_texture);

    //Back button
    self->globals.windowses.options.back_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.options.back_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.options.back_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.options.back_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.options.back_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.options.back_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.options.back_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.options.back_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.options.back_btn.position, renderer);
    SDL_Surface* back_surface = TTF_RenderText_Solid(sans, self->globals.windowses.options.back_btn.text, text_color);
    SDL_Texture* back_texture = SDL_CreateTextureFromSurface(renderer, back_surface);
    SDL_RenderCopy(renderer, back_texture, NULL, &self->globals.windowses.options.back_btn.position.rect);
    SDL_FreeSurface(back_surface);
    SDL_DestroyTexture(back_texture);
}

static void draw_game_over(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans) {
    int selected_theme = self->setting.selected_theme;
    SDL_Color text_color = {.r = self->globals.themes[selected_theme]->text_color.r, .g = self->globals.themes[selected_theme]->text_color.g, .b = self->globals.themes[selected_theme]->text_color.b};
    SDL_Rect overlay_rect = {.x = 0, .y = 0, .w = self->globals.window_dimensions.w, .h = self->globals.window_dimensions.h};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &overlay_rect);

    //Game over text
    SDL_Rect game_over_rect = {.h = self->globals.windowses.game_over.game_over_text.dimensions.h, .w = self->globals.windowses.game_over.game_over_text.dimensions.w, .x = self->globals.windowses.game_over.game_over_text.coords.x, .y = self->globals.windowses.game_over.game_over_text.coords.y};
    SDL_Surface* game_over_surface;
    if(!self->globals.is_playable) {
        if(self->setting.is_endless && is_win(self)) {
            game_over_surface = TTF_RenderText_Solid(sans, "YOU WIN", text_color);
        } else if(is_win(self)) {
            game_over_surface = TTF_RenderText_Solid(sans, "YOU WIN", text_color);
        } else {
            game_over_surface = TTF_RenderText_Solid(sans, "GAME OVER", text_color);
        }
    }
    SDL_Texture* game_over_texture = SDL_CreateTextureFromSurface(renderer, game_over_surface);
    SDL_RenderCopy(renderer, game_over_texture, NULL, &game_over_rect);
    SDL_FreeSurface(game_over_surface);
    SDL_DestroyTexture(game_over_texture);

    //Try again button
    self->globals.windowses.game_over.try_again_btn.position.outline_color.a = 60;
    self->globals.windowses.game_over.try_again_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.game_over.try_again_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.game_over.try_again_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.game_over.try_again_btn.position.inside_color.a = 30;
    self->globals.windowses.game_over.try_again_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.game_over.try_again_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.game_over.try_again_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.game_over.try_again_btn.position, renderer);
    SDL_Surface* try_again_surface = TTF_RenderText_Solid(sans, self->globals.windowses.game_over.try_again_btn.text, text_color);
    SDL_Texture* try_again_texture = SDL_CreateTextureFromSurface(renderer, try_again_surface);
    SDL_RenderCopy(renderer, try_again_texture, NULL, &self->globals.windowses.game_over.try_again_btn.position.rect);
    SDL_FreeSurface(try_again_surface);
    SDL_DestroyTexture(try_again_texture);

    //Main menu button
    self->globals.windowses.game_over.main_menu_btn.position.outline_color.a = 60;
    self->globals.windowses.game_over.main_menu_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.game_over.main_menu_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.game_over.main_menu_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.game_over.main_menu_btn.position.inside_color.a = 30;
    self->globals.windowses.game_over.main_menu_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.game_over.main_menu_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.game_over.main_menu_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.game_over.main_menu_btn.position, renderer);
    SDL_Surface* main_menu_surface = TTF_RenderText_Solid(sans, self->globals.windowses.game_over.main_menu_btn.text, text_color);
    SDL_Texture* main_menu_texture = SDL_CreateTextureFromSurface(renderer, main_menu_surface);
    SDL_RenderCopy(renderer, main_menu_texture, NULL, &self->globals.windowses.game_over.main_menu_btn.position.rect);
    SDL_FreeSurface(main_menu_surface);
    SDL_DestroyTexture(main_menu_texture);
}

static void draw_game(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans) {
    int selected_theme = self->setting.selected_theme;
    SDL_Color text_color = {.r = self->globals.themes[selected_theme]->text_color.r, .g = self->globals.themes[selected_theme]->text_color.g, .b = self->globals.themes[selected_theme]->text_color.b};

    //Play time text
    SDL_Rect time_spend_rect = {.h = self->globals.windowses.game_window.time_spend_text.dimensions.h, .w = self->globals.windowses.game_window.time_spend_text.dimensions.w, .x = self->globals.windowses.game_window.time_spend_text.coords.x, .y = self->globals.windowses.game_window.time_spend_text.coords.y};
    SDL_Surface* time_spend_surface = TTF_RenderText_Solid(sans, self->globals.windowses.game_window.time_spend_text.text, text_color);
    SDL_Texture* time_spend_texture = SDL_CreateTextureFromSurface(renderer, time_spend_surface);
    SDL_RenderCopy(renderer, time_spend_texture, NULL, &time_spend_rect);
    SDL_FreeSurface(time_spend_surface);
    SDL_DestroyTexture(time_spend_texture);

    //Score text
    SDL_Rect score_rect = {.h = self->globals.windowses.game_window.score_text.dimensions.h, .w = self->globals.windowses.game_window.score_text.dimensions.w, .x = self->globals.windowses.game_window.score_text.coords.x, .y = self->globals.windowses.game_window.score_text.coords.y};
    SDL_Surface* score_surface = TTF_RenderText_Solid(sans, self->globals.windowses.game_window.score_text.text, text_color);
    SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
    SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
    SDL_FreeSurface(score_surface);
    SDL_DestroyTexture(score_texture);

    if(self->globals.windowses.game_window.field.points.score_power_increase.draw) {
        //Score/power increment text
        char increment_text[256];
        sprintf(increment_text, "+%d", self->globals.windowses.game_window.field.points.score_power_increase.value);
        SDL_Rect score_power_increment_rect;
        score_power_increment_rect.h = self->globals.windowses.game_window.field.points.score_power_increase.dimensions.h;
        score_power_increment_rect.w = score_power_increment_rect.h / 5 * 2 * strlen(increment_text);
        score_power_increment_rect.x = score_rect.x + score_rect.w / 4;
        score_power_increment_rect.y = self->globals.windowses.game_window.field.points.score_power_increase.coords.y + self->globals.windowses.game_window.field.points.score_power_increase.move_for;
        text_color.a = 255 - self->globals.windowses.game_window.field.points.score_power_increase.move_for * 5;
        self->globals.windowses.game_window.field.points.score_power_increase.move_for++;

        SDL_Surface* score_power_increment_surface = TTF_RenderText_Solid(sans, increment_text, text_color);
        SDL_Texture* score_power_increment_texture = SDL_CreateTextureFromSurface(renderer, score_power_increment_surface);
        SDL_RenderCopy(renderer, score_power_increment_texture, NULL, &score_power_increment_rect);
        SDL_FreeSurface(score_power_increment_surface);
        SDL_DestroyTexture(score_power_increment_texture);
        if(255 - self->globals.windowses.game_window.field.points.score_power_increase.move_for * 5 < 5) {
            self->globals.windowses.game_window.field.points.score_power_increase.draw = false;
            self->globals.windowses.game_window.field.points.score_power_increase.move_for = 0;
        }
    }

    if(self->globals.windowses.game_window.field.points.highscore_toppower_increase.draw) {
        //Score/power increment text
        char increment_text[256];
        sprintf(increment_text, "+%d", self->globals.windowses.game_window.field.points.highscore_toppower_increase.value);
        SDL_Rect highscore_toppower_increment_rect;
        highscore_toppower_increment_rect.h = self->globals.windowses.game_window.field.points.highscore_toppower_increase.dimensions.h;
        highscore_toppower_increment_rect.w = highscore_toppower_increment_rect.h / 5 * 2 * strlen(increment_text);
        highscore_toppower_increment_rect.x = score_rect.x + score_rect.w / 4 * 3;
        highscore_toppower_increment_rect.y = self->globals.windowses.game_window.field.points.highscore_toppower_increase.coords.y + self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for;
        text_color.a = 255 - self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for * 5;
        self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for++;

        SDL_Surface* highscore_toppower_increment_surface = TTF_RenderText_Solid(sans, increment_text, text_color);
        SDL_Texture* highscore_toppower_increment_texture = SDL_CreateTextureFromSurface(renderer, highscore_toppower_increment_surface);
        SDL_RenderCopy(renderer, highscore_toppower_increment_texture, NULL, &highscore_toppower_increment_rect);
        SDL_FreeSurface(highscore_toppower_increment_surface);
        SDL_DestroyTexture(highscore_toppower_increment_texture);
        if(255 - self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for * 5 < 5) {
            self->globals.windowses.game_window.field.points.highscore_toppower_increase.draw = false;
            self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for = 0;
        }
    }

    text_color.a = 255;

    //Main menu button
    self->globals.windowses.game_window.back_to_main_menu_btn.position.outline_color.a = self->globals.themes[selected_theme]->foreground.a;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.outline_color.r = self->globals.themes[selected_theme]->foreground.r;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.outline_color.g = self->globals.themes[selected_theme]->foreground.g;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.outline_color.b = self->globals.themes[selected_theme]->foreground.b;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.inside_color.a = self->globals.themes[selected_theme]->background.a;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.inside_color.r = self->globals.themes[selected_theme]->background.r;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.inside_color.g = self->globals.themes[selected_theme]->background.g;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.inside_color.b = self->globals.themes[selected_theme]->background.b;

    draw_outline_rounded_rectangle(&self->globals.windowses.game_window.back_to_main_menu_btn.position, renderer);
    SDL_Surface* back_ro_main_menu_surface = TTF_RenderText_Solid(sans, self->globals.windowses.game_window.back_to_main_menu_btn.text, text_color);
    SDL_Texture* back_ro_main_menu_texture = SDL_CreateTextureFromSurface(renderer, back_ro_main_menu_surface);
    SDL_RenderCopy(renderer, back_ro_main_menu_texture, NULL, &self->globals.windowses.game_window.back_to_main_menu_btn.position.rect);
    SDL_FreeSurface(back_ro_main_menu_surface);
    SDL_DestroyTexture(back_ro_main_menu_texture);

    draw_cells_placeholders(self, renderer);
    draw_cells(self, renderer, sans);
}

static void draw_cells_placeholders(Main_obj* self, SDL_Renderer* renderer) {
    int selected_theme = self->setting.selected_theme;
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
                Rounded_Rectangle_filled rect;
                rect.color.a = self->globals.themes[selected_theme]->foreground.a;
                rect.color.r = self->globals.themes[selected_theme]->foreground.r;
                rect.color.g = self->globals.themes[selected_theme]->foreground.g;
                rect.color.b = self->globals.themes[selected_theme]->foreground.b;
                rect.roundness = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->roundness;
                rect.rect.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x;
                rect.rect.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y;
                rect.rect.h = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->dimensions.h;
                rect.rect.w = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->dimensions.w;
                draw_filled_rounded_rectangle(&rect, renderer);
        }
    }
}

static void draw_cells(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;
            int cell_power = self->globals.windowses.game_window.field.cells[cell_id]->power;
            cell_power = self->globals.windowses.game_window.field.cells[cell_id]->merging.merging ? cell_power - 1 : cell_power;
            cell_power = cell_power < 11 ? cell_power : 11;
            if(cell_power >= 0) {
                Rounded_Rectangle_filled rect;
                rect.roundness = self->globals.windowses.game_window.field.cells[cell_id]->roundness;
                rect.rect.h = self->globals.windowses.game_window.field.cells[cell_id]->dimensions.h + self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels * 2 * self->globals.windowses.game_window.field.percent_size / 100;
                rect.rect.w = self->globals.windowses.game_window.field.cells[cell_id]->dimensions.w + self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels * 2 * self->globals.windowses.game_window.field.percent_size / 100;

                rect.color.a = self->globals.number_props[cell_power]->cell_color.a;
                rect.color.r = self->globals.number_props[cell_power]->cell_color.r;
                rect.color.g = self->globals.number_props[cell_power]->cell_color.g;
                rect.color.b = self->globals.number_props[cell_power]->cell_color.b;

                rect.rect.x = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x - self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels * self->globals.windowses.game_window.field.percent_size / 100;
                rect.rect.y = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y - self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels * self->globals.windowses.game_window.field.percent_size / 100;
                draw_filled_rounded_rectangle(&rect, renderer);

                draw_cell_number(self, renderer, sans, cell_id, false);
                if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging) {
                    rect.rect.x = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x - self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels * self->globals.windowses.game_window.field.percent_size / 100;
                    rect.rect.y = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y - self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels * self->globals.windowses.game_window.field.percent_size / 100;
                    draw_filled_rounded_rectangle(&rect, renderer);
                    draw_cell_number(self, renderer, sans, cell_id, true);
                }
                if(!self->globals.windowses.game_window.field.cells[cell_id]->merging.merging && !self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_animated) {
                    if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_keep_frames < 2) {
                        self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_keep_frames++;
                    } else {
                        self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels++;
                        self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_keep_frames = 0;
                        if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels >= 5) {
                            self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_animated = true;
                        }
                    }
                } else {
                    if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels > 0) {
                        self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_expansion_pixels--;
                    }
                }
            }
        }
    }
}

static void draw_cell_number(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans, const int cell_id, const bool is_merge_cell) {
    int power = self->globals.windowses.game_window.field.cells[cell_id]->power - (self->globals.windowses.game_window.field.cells[cell_id]->merging.merging ? 1 : 0);
    int power_id = power < 11 ? power : 11;
    SDL_Color text_color = {.r = self->globals.number_props[power_id]->text_color.r, .g = self->globals.number_props[power_id]->text_color.g, .b = self->globals.number_props[power_id]->text_color.b};
    int relative_x = self->globals.number_props[power_id]->coords.x * self->globals.windowses.game_window.field.percent_size / 100;
    int relative_y = self->globals.number_props[power_id]->coords.y * self->globals.windowses.game_window.field.percent_size / 100;
    int relative_w = self->globals.number_props[power_id]->dimensions.w * self->globals.windowses.game_window.field.percent_size / 100;
    int relative_h = self->globals.number_props[power_id]->dimensions.h * self->globals.windowses.game_window.field.percent_size / 100;
    char number_text[32];
    sprintf(number_text, "%d", self->globals.number_props[power_id]->power_value);





    if(power < 11) {
        SDL_Rect number_rect = {.x = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x + relative_x, .y = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y + relative_y, .w = relative_w, .h = relative_h};

        SDL_Surface* number_surface = TTF_RenderText_Solid(sans, number_text, text_color);

        SDL_Texture * number_texture = SDL_CreateTextureFromSurface(renderer, number_surface);

        if(!is_merge_cell) {
            SDL_RenderCopy(renderer, number_texture, NULL, &number_rect);
        }

        if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging) {
            number_rect.x = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x + relative_x;
            number_rect.y = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y + relative_y;
            if(is_merge_cell) {
                SDL_RenderCopy(renderer, number_texture, NULL, &number_rect);
            }
        }

        SDL_DestroyTexture(number_texture);
        SDL_FreeSurface(number_surface);

    } else {
        char power_text[32];
        sprintf(power_text, "%d", power + 1);
        int relative_power_x = self->globals.number_props[power_id]->power_coords.x * self->globals.windowses.game_window.field.percent_size / 100;
        int relative_power_y = self->globals.number_props[power_id]->power_coords.y * self->globals.windowses.game_window.field.percent_size / 100;
        int relative_power_h = 50 * self->globals.windowses.game_window.field.percent_size / 100;
        int relative_power_w = relative_power_h / 5 * 2 * self->globals.windowses.game_window.field.percent_size / 100 * strlen(power_text);
        SDL_Rect number_rect = {.x = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x + relative_x, .y = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y + relative_y, .w = relative_w, .h = relative_h};
        SDL_Rect power_rect = {.x = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x + relative_power_x, .y = self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y + relative_power_y, .w = relative_power_w, .h = relative_power_h};

        SDL_Surface* number_surface = TTF_RenderText_Solid(sans, number_text, text_color);
        SDL_Surface* power_surface = TTF_RenderText_Solid(sans, power_text, text_color);

        SDL_Texture * number_texture = SDL_CreateTextureFromSurface(renderer, number_surface);
        SDL_Texture * power_texture = SDL_CreateTextureFromSurface(renderer, power_surface);

        if(!is_merge_cell) {
            SDL_RenderCopy(renderer, number_texture, NULL, &number_rect);
            SDL_RenderCopy(renderer, power_texture, NULL, &power_rect);
        }

        if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging) {
            number_rect.x = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x + relative_x;
            number_rect.y = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y + relative_y;
            power_rect.x = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x + relative_power_x;
            power_rect.y = self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y + relative_power_y;
            if(is_merge_cell) {
            SDL_RenderCopy(renderer, number_texture, NULL, &number_rect);
            SDL_RenderCopy(renderer, power_texture, NULL, &power_rect);
        }
        }

        SDL_DestroyTexture(number_texture);
        SDL_DestroyTexture(power_texture);
        SDL_FreeSurface(number_surface);
        SDL_FreeSurface(power_surface);
    }
}
