#include "../headers/structures.h"
#include "../headers/free.h"
#include "../headers/free_statics.h"

void main_free(Main_obj* self) {
    number_props_free(self);
    themes_free(self);
    free_options(self);
    free_game(self);
    free_afk(self);
}

static void free_afk(Main_obj* self){
    for(int i = 0; i < self->globals.windowses.afk_window.number_of_start; i++){
        free(self->globals.windowses.afk_window.stars[i]);
    }
    free(self->globals.windowses.afk_window.stars);
}

static void number_props_free(Main_obj* self) {
    for(int i = 0; i < 12; i++) {
        free(self->globals.number_props[i]);
    }
    free(self->globals.number_props);
}

static void themes_free(Main_obj* self) {
    for(int i = 0; i < self->globals.number_of_themes; i++) {
        free(self->globals.themes[i]);
    }
    free(self->globals.themes);
}

static void free_options(Main_obj* self) {
    free(self->globals.windowses.options.rows_text.text);
    free(self->globals.windowses.options.cols_text.text);
    free(self->globals.windowses.options.theme_text.text);
}

static void free_game(Main_obj* self) {
    free(self->globals.windowses.game_window.time_spend_text.text);
    free(self->globals.windowses.game_window.score_text.text);
}
