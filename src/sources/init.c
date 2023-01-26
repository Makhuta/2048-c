#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../headers/structures.h"
#include "../headers/init.h"
#include "../headers/init_statics.h"
#include "../headers/file_handler.h"
#include "../headers/manipulation.h"


void main_init(Main_obj* self){
    globals_init(self);
    settings_init(self);
    number_props_init(self);
    //load_themes(self);
    //themes_init(self);
}

void update_main_dimensions(Main_obj* self, SDL_Window* window) {
    SDL_Rect win;
    SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(window), &win);
    self->globals.window_dimensions.h = win.h;
    self->globals.window_dimensions.w = win.w;
    int percentual_width = ceil(self->globals.window_dimensions.w / (float)(1920.0 / 100.0)); 
    int percentual_height = ceil(self->globals.window_dimensions.h / (float)(1080.0 / 100.0)); 
    self->globals.window_percentual_ratio = percentual_width < percentual_height ? percentual_width : percentual_height;

    init_windowses(self);
    game_init(self);
    game_over_init(self);
}

void update_time(Main_obj* self){
    //Time spend
    char time_spend_text[256];

    int elapsed_time = self->globals.windowses.game_window.time_spend;
    int days = elapsed_time / (60 * 60 * 24);
    int hours = (elapsed_time % (60 * 60 * 24)) / (60 * 60);
    int minutes = (elapsed_time % (60 * 60)) / 60;
    int seconds = (elapsed_time % (60 * 60)) % 60;

    sprintf(time_spend_text, "Play time: %d:%s%d:%s%d:%s%d", days, hours < 10 ? "0" : "", hours, minutes < 10 ? "0" : "", minutes, seconds < 10 ? "0" : "", seconds);
    //self->globals.windowses.game_window.time_spend_text.text = malloc(sizeof(time_spend_text) * strlen(time_spend_text));
    strcpy(self->globals.windowses.game_window.time_spend_text.text, time_spend_text);
    self->globals.windowses.game_window.time_spend_text.dimensions.h = 30 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.time_spend_text.dimensions.w = strlen(self->globals.windowses.game_window.time_spend_text.text) * (self->globals.windowses.game_window.time_spend_text.dimensions.h / 5 * 2);

}

void update_score(Main_obj* self){
    if(self->globals.windowses.game_window.field.points.score != self->globals.windowses.game_window.field.points.score_old || self->globals.windowses.game_window.field.points.score == 0 || self->globals.windowses.game_window.field.points.current_highest_merge != self->globals.windowses.game_window.field.points.current_highest_merge_old || self->globals.windowses.game_window.field.points.current_highest_merge == 0) {
    if(self->globals.windowses.game_window.field.points.score != self->globals.windowses.game_window.field.points.score_old || self->globals.windowses.game_window.field.points.current_highest_merge != self->globals.windowses.game_window.field.points.current_highest_merge_old) {
        self->globals.windowses.game_window.field.points.score_power_increase.draw = true;
        self->globals.windowses.game_window.field.points.score_power_increase.move_for = 0;
        self->globals.windowses.game_window.field.points.score_power_increase.value = self->setting.is_endless ? self->globals.windowses.game_window.field.points.current_highest_merge - self->globals.windowses.game_window.field.points.current_highest_merge_old : self->globals.windowses.game_window.field.points.score - self->globals.windowses.game_window.field.points.score_old;
    }
    if(self->globals.windowses.game_window.field.points.highscore != self->globals.windowses.game_window.field.points.highscore_old || self->globals.windowses.game_window.field.points.total_highest_merge != self->globals.windowses.game_window.field.points.total_highest_merge_old) {
        self->globals.windowses.game_window.field.points.highscore_toppower_increase.draw = true;
        self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for = 0;
        self->globals.windowses.game_window.field.points.highscore_toppower_increase.value = self->setting.is_endless ? self->globals.windowses.game_window.field.points.total_highest_merge - self->globals.windowses.game_window.field.points.total_highest_merge_old : self->globals.windowses.game_window.field.points.score - self->globals.windowses.game_window.field.points.score_old;
    }
    char score_text[256];

    if(self->setting.is_endless) {
        sprintf(score_text, "Highest power: %d Total highest power: %d", self->globals.windowses.game_window.field.points.current_highest_merge, self->globals.windowses.game_window.field.points.total_highest_merge);
    } else {
        sprintf(score_text, "Score: %d Highscore: %d", self->globals.windowses.game_window.field.points.score, self->globals.windowses.game_window.field.points.highscore);
    }
    strcpy(self->globals.windowses.game_window.score_text.text, score_text);
    self->globals.windowses.game_window.score_text.dimensions.h = 30 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.score_text.dimensions.w = strlen(self->globals.windowses.game_window.score_text.text) * (self->globals.windowses.game_window.score_text.dimensions.h / 5 * 2);
    self->globals.windowses.game_window.field.points.score_old = self->globals.windowses.game_window.field.points.score;
    self->globals.windowses.game_window.field.points.highscore_old = self->globals.windowses.game_window.field.points.highscore;
    self->globals.windowses.game_window.field.points.current_highest_merge_old = self->globals.windowses.game_window.field.points.current_highest_merge;
    self->globals.windowses.game_window.field.points.total_highest_merge_old = self->globals.windowses.game_window.field.points.total_highest_merge;
    }
}

void start_game_init(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            free(self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]);
        }
    }
    free(self->globals.windowses.game_window.field.cells);
    self->globals.windowses.game_window.time_spend = 0;
    self->globals.windowses.game_window.field.points.score = 0;
    self->globals.windowses.game_window.field.points.score_old = 0;
    self->globals.windowses.game_window.field.points.current_highest_merge = 0;
    self->globals.windowses.game_window.field.points.current_highest_merge_old = 0;
    self->globals.windowses.game_window.field.rows = self->setting.rows;
    self->globals.windowses.game_window.field.cols = self->setting.cols;
    self->globals.windowses.game_window.field.cells = malloc(sizeof(Cell*) * self->globals.windowses.game_window.field.rows * self->globals.windowses.game_window.field.cols);
    int max_field_height = self->globals.window_dimensions.h - self->globals.windowses.game_window.padding_top * self->globals.window_percentual_ratio / 100;
    int percent_dimensions = 100;
    int rows = self->globals.windowses.game_window.field.rows;
    int cols = self->globals.windowses.game_window.field.cols;
    int max_cell_size = 100;

    while(max_field_height < self->globals.number_props[0]->padding * percent_dimensions / 100 + (max_cell_size + self->globals.number_props[0]->padding) * rows * percent_dimensions / 100 || self->globals.window_dimensions.w < self->globals.number_props[0]->padding * percent_dimensions / 100 + (max_cell_size + self->globals.number_props[0]->padding) * cols * percent_dimensions / 100) {
        percent_dimensions--;
    }
    self->globals.windowses.game_window.field.percent_size = percent_dimensions;
    int field_width = self->globals.number_props[0]->padding * percent_dimensions / 100 + cols * (max_cell_size * percent_dimensions / 100 + self->globals.number_props[0]->padding * percent_dimensions / 100);
    int field_height = self->globals.number_props[0]->padding * percent_dimensions / 100 + rows * (max_cell_size * percent_dimensions / 100 + self->globals.number_props[0]->padding * percent_dimensions / 100);
    int moved_x = self->globals.window_dimensions.w / 2 - field_width / 2 + (self->globals.number_props[0]->padding * percent_dimensions / 100);
    int moved_y = self->globals.windowses.game_window.padding_top * self->globals.window_percentual_ratio / 100 + max_field_height / 2 - field_height / 2;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            self->globals.windowses.game_window.field.cells[j + i * cols] = malloc(sizeof(Cell));
            int percent_padding = (self->globals.number_props[0]->padding * percent_dimensions / 100);
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power = -1;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_animated = true;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_expansion_keep_frames = 2;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_expansion_pixels = 0;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merging = false;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.move_half.x = 0;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.move_half.y = 0;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.move_speed = 1;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->move_speed = 1;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->roundness = 50;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x = moved_x + (j * max_cell_size * percent_dimensions / 100 + j * percent_padding);
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y = moved_y + (i * max_cell_size * percent_dimensions / 100 + i * percent_padding);
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->dimensions.w = max_cell_size * percent_dimensions / 100;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->dimensions.h = max_cell_size * percent_dimensions / 100;


            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y;
        }
    }
    for(int i = 0; i < 2; i++) {
        add_cell(self);
    }
    self->globals.is_playable = true;
}

static void game_over_init(Main_obj* self) {
    int padding_top = 100 * self->globals.window_percentual_ratio / 100;
    int spacing = 40 * self->globals.window_percentual_ratio / 100;
    int element_id = 0;
    int element_h = 60 * self->globals.window_percentual_ratio / 100;
    int game_over_h = element_h * 4;


    //Name text
    self->globals.windowses.game_over.game_over_text.text = "GAME OVER";
    self->globals.windowses.game_over.game_over_text.dimensions.h = game_over_h;
    self->globals.windowses.game_over.game_over_text.dimensions.w = strlen(self->globals.windowses.game_over.game_over_text.text) * (self->globals.windowses.game_over.game_over_text.dimensions.h / 5 * 2);
    self->globals.windowses.game_over.game_over_text.coords.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.game_over.game_over_text.dimensions.w / 2;
    self->globals.windowses.game_over.game_over_text.coords.y = padding_top + (element_id * (spacing + element_h));
    element_id++;

    //Try again button
    self->globals.windowses.game_over.try_again_btn.text = "Try again";
    self->globals.windowses.game_over.try_again_btn.position.rect.h = element_h;
    self->globals.windowses.game_over.try_again_btn.position.roundness = 50;
    self->globals.windowses.game_over.try_again_btn.position.thickness = 5;
    self->globals.windowses.game_over.try_again_btn.position.rect.w = strlen(self->globals.windowses.game_over.try_again_btn.text) * (self->globals.windowses.game_over.try_again_btn.position.rect.h / 5 * 2);
    self->globals.windowses.game_over.try_again_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.game_over.try_again_btn.position.rect.w / 2;
    self->globals.windowses.game_over.try_again_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + game_over_h;
    element_id++;

    //Try again button
    self->globals.windowses.game_over.main_menu_btn.text = "Main menu";
    self->globals.windowses.game_over.main_menu_btn.position.rect.h = element_h;
    self->globals.windowses.game_over.main_menu_btn.position.roundness = 50;
    self->globals.windowses.game_over.main_menu_btn.position.thickness = 5;
    self->globals.windowses.game_over.main_menu_btn.position.rect.w = strlen(self->globals.windowses.game_over.main_menu_btn.text) * (self->globals.windowses.game_over.main_menu_btn.position.rect.h / 5 * 2);
    self->globals.windowses.game_over.main_menu_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.game_over.main_menu_btn.position.rect.w / 2;
    self->globals.windowses.game_over.main_menu_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + game_over_h;
    element_id++;
}

static void game_init(Main_obj* self) {
    load_game(self);
    load_score(self);
    self->globals.windowses.game_window.field.points.score_power_increase.draw = false;
    self->globals.windowses.game_window.field.points.highscore_toppower_increase.draw = false;
    self->globals.windowses.game_window.field.points.score_power_increase.dimensions.h = 25 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.field.points.highscore_toppower_increase.dimensions.h = 25 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.field.points.score_power_increase.coords.y = 20 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.field.points.highscore_toppower_increase.coords.y = 20 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.field.points.score_power_increase.move_for = 0;
    self->globals.windowses.game_window.field.points.highscore_toppower_increase.move_for = 0;
    self->globals.windowses.game_window.field.points.score_power_increase.color.a = 255;
    self->globals.windowses.game_window.field.points.highscore_toppower_increase.color.a = 255;
    self->globals.windowses.game_window.field.cell_added = true;
    self->globals.windowses.game_window.padding_top = 100 * self->globals.window_percentual_ratio / 100;

    //Time spend
    char time_spend_text[256];

    int elapsed_time = self->globals.windowses.game_window.time_spend;
    int days = elapsed_time / (60 * 60 * 24);
    int hours = (elapsed_time % (60 * 60 * 24)) / (60 * 60);
    int minutes = (elapsed_time % (60 * 60)) / 60;
    int seconds = (elapsed_time % (60 * 60)) % 60;

    sprintf(time_spend_text, "Play time: %d:%s%d:%s%d:%s%d", days, hours < 10 ? "0" : "", hours, minutes < 10 ? "0" : "", minutes, seconds < 10 ? "0" : "", seconds);
    self->globals.windowses.game_window.time_spend_text.text = malloc(sizeof(time_spend_text) * strlen(time_spend_text));
    strcpy(self->globals.windowses.game_window.time_spend_text.text, time_spend_text);
    self->globals.windowses.game_window.time_spend_text.dimensions.h = 30 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.time_spend_text.dimensions.w = strlen(self->globals.windowses.game_window.time_spend_text.text) * (self->globals.windowses.game_window.time_spend_text.dimensions.h / 5 * 2);
    self->globals.windowses.game_window.time_spend_text.coords.x = 10 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.time_spend_text.coords.y = 10 * self->globals.window_percentual_ratio / 100;

    //Score
    char score_text[256];
    if(self->setting.is_endless) {
        sprintf(score_text, "Highest power: %d Total highest power: %d", self->globals.windowses.game_window.field.points.current_highest_merge, self->globals.windowses.game_window.field.points.total_highest_merge);
    } else {
        sprintf(score_text, "Score: %d Highscore: %d", self->globals.windowses.game_window.field.points.score, self->globals.windowses.game_window.field.points.highscore);
    }
    self->globals.windowses.game_window.score_text.text = malloc(sizeof(score_text) * strlen(score_text));
    strcpy(self->globals.windowses.game_window.score_text.text, score_text);
    self->globals.windowses.game_window.score_text.dimensions.h = 30 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.score_text.dimensions.w = strlen(self->globals.windowses.game_window.score_text.text) * (self->globals.windowses.game_window.score_text.dimensions.h / 5 * 2);
    self->globals.windowses.game_window.score_text.coords.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.game_window.score_text.dimensions.w / 2;
    self->globals.windowses.game_window.score_text.coords.y = 10 * self->globals.window_percentual_ratio / 100;

    //Back to main menu button
    self->globals.windowses.game_window.back_to_main_menu_btn.text = "Menu";
    self->globals.windowses.game_window.back_to_main_menu_btn.position.rect.h = self->globals.windowses.game_window.padding_top * self->globals.window_percentual_ratio / 100 * self->globals.window_percentual_ratio / 100 - 50 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.roundness = 50;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.thickness = 5;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.rect.w = strlen(self->globals.windowses.game_window.back_to_main_menu_btn.text) * (self->globals.windowses.game_window.back_to_main_menu_btn.position.rect.h / 5 * 2);
    self->globals.windowses.game_window.back_to_main_menu_btn.position.rect.x = self->globals.window_dimensions.w - self->globals.windowses.game_window.back_to_main_menu_btn.position.rect.w - 20 * self->globals.window_percentual_ratio / 100;
    self->globals.windowses.game_window.back_to_main_menu_btn.position.rect.y = 10 * self->globals.window_percentual_ratio / 100;

    //Cells
    int max_field_height = self->globals.window_dimensions.h - self->globals.windowses.game_window.padding_top;
    int percent_dimensions = 100;
    int rows = self->globals.windowses.game_window.field.rows;
    int cols = self->globals.windowses.game_window.field.cols;
    int max_cell_size = 100;

    while(max_field_height < self->globals.number_props[0]->padding * percent_dimensions / 100 + (max_cell_size + self->globals.number_props[0]->padding) * rows * percent_dimensions / 100 || self->globals.window_dimensions.w < self->globals.number_props[0]->padding * percent_dimensions / 100 + (max_cell_size + self->globals.number_props[0]->padding) * cols * percent_dimensions / 100) {
        percent_dimensions--;
    }
    self->globals.windowses.game_window.field.percent_size = percent_dimensions;
    int field_width = self->globals.number_props[0]->padding * percent_dimensions / 100 + cols * (max_cell_size * percent_dimensions / 100 + self->globals.number_props[0]->padding * percent_dimensions / 100);
    int field_height = self->globals.number_props[0]->padding * percent_dimensions / 100 + rows * (max_cell_size * percent_dimensions / 100 + self->globals.number_props[0]->padding * percent_dimensions / 100);
    int moved_x = self->globals.window_dimensions.w / 2 - field_width / 2 + (self->globals.number_props[0]->padding * percent_dimensions / 100);
    int moved_y = self->globals.windowses.game_window.padding_top * self->globals.window_percentual_ratio / 100 + max_field_height / 2 - field_height / 2;
    for(int i = 0; i < self->setting.rows; i++) {
        for(int j = 0; j < self->setting.cols; j++) {
            int percent_padding = (self->globals.number_props[0]->padding * percent_dimensions / 100);
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_expansion_keep_frames = 2;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_expansion_pixels = 0;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merging = false;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.move_half.x = 0;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.move_half.y = 0;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.move_speed = 1;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->move_speed = 1;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->roundness = 50;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x = moved_x + (j * max_cell_size * percent_dimensions / 100 + j * percent_padding);
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y = moved_y + (i * max_cell_size * percent_dimensions / 100 + i * percent_padding);
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->dimensions.w = max_cell_size * percent_dimensions / 100;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->dimensions.h = max_cell_size * percent_dimensions / 100;


            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x;
            self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y;
        }
    }
}

static void globals_init(Main_obj* self) {
    self->globals.fps = 60;
    self->globals.name = "2048";
    self->globals.quit = false;
    self->globals.selected_window = 0;
}

static void settings_init(Main_obj* self) {
    load_settings(self);
    self->setting.is_endless = false;
}

static void init_windowses(Main_obj* self) {
    init_main_menu(self);
    init_options(self);
}

static void init_main_menu(Main_obj* self) {
    int padding_top = 100 * self->globals.window_percentual_ratio / 100;
    int spacing = 40 * self->globals.window_percentual_ratio / 100;
    int element_id = 0;
    int element_h = 60 * self->globals.window_percentual_ratio / 100;
    int name_h = element_h * 4;

    //Name text
    self->globals.windowses.main_menu.name_text.text = self->globals.name;
    self->globals.windowses.main_menu.name_text.dimensions.h = name_h;
    self->globals.windowses.main_menu.name_text.dimensions.w = strlen(self->globals.windowses.main_menu.name_text.text) * (self->globals.windowses.main_menu.name_text.dimensions.h / 5 * 2);
    self->globals.windowses.main_menu.name_text.coords.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.main_menu.name_text.dimensions.w / 2;
    self->globals.windowses.main_menu.name_text.coords.y = padding_top + (element_id * (spacing + element_h));
    element_id++;

    //Start button
    self->globals.windowses.main_menu.start_btn.text = "Start";
    self->globals.windowses.main_menu.start_btn.position.rect.h = element_h;
    self->globals.windowses.main_menu.start_btn.position.roundness = 50;
    self->globals.windowses.main_menu.start_btn.position.thickness = 5;
    self->globals.windowses.main_menu.start_btn.position.rect.w = strlen(self->globals.windowses.main_menu.start_btn.text) * (self->globals.windowses.main_menu.start_btn.position.rect.h / 5 * 2);
    self->globals.windowses.main_menu.start_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.main_menu.start_btn.position.rect.w / 2;
    self->globals.windowses.main_menu.start_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Continue button
    self->globals.windowses.main_menu.continue_btn.text = "Continue";
    self->globals.windowses.main_menu.continue_btn.position.rect.h = element_h;
    self->globals.windowses.main_menu.continue_btn.position.roundness = 50;
    self->globals.windowses.main_menu.continue_btn.position.thickness = 5;
    self->globals.windowses.main_menu.continue_btn.position.rect.w = strlen(self->globals.windowses.main_menu.continue_btn.text) * (self->globals.windowses.main_menu.continue_btn.position.rect.h / 5 * 2);
    self->globals.windowses.main_menu.continue_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.main_menu.continue_btn.position.rect.w / 2;
    self->globals.windowses.main_menu.continue_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Options button
    self->globals.windowses.main_menu.options_btn.text = "Options";
    self->globals.windowses.main_menu.options_btn.position.rect.h = element_h;
    self->globals.windowses.main_menu.options_btn.position.roundness = 50;
    self->globals.windowses.main_menu.options_btn.position.thickness = 5;
    self->globals.windowses.main_menu.options_btn.position.rect.w = strlen(self->globals.windowses.main_menu.options_btn.text) * (self->globals.windowses.main_menu.options_btn.position.rect.h / 5 * 2);
    self->globals.windowses.main_menu.options_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.main_menu.options_btn.position.rect.w / 2;
    self->globals.windowses.main_menu.options_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Exit button
    self->globals.windowses.main_menu.exit_btn.text = "Exit";
    self->globals.windowses.main_menu.exit_btn.position.rect.h = element_h;
    self->globals.windowses.main_menu.exit_btn.position.roundness = 50;
    self->globals.windowses.main_menu.exit_btn.position.thickness = 5;
    self->globals.windowses.main_menu.exit_btn.position.rect.w = strlen(self->globals.windowses.main_menu.exit_btn.text) * (self->globals.windowses.main_menu.exit_btn.position.rect.h / 5 * 2);
    self->globals.windowses.main_menu.exit_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.main_menu.exit_btn.position.rect.w / 2;
    self->globals.windowses.main_menu.exit_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;
}

static void init_options(Main_obj* self) {
    int padding_top = 100 * self->globals.window_percentual_ratio / 100;
    int spacing = 40 * self->globals.window_percentual_ratio / 100;
    int element_id = 0;
    int element_h = 60 * self->globals.window_percentual_ratio / 100;
    int name_height = element_h * 4 * self->globals.window_percentual_ratio / 100;
    int name_h = name_height / 4 * 3;

    //Name text
    self->globals.windowses.options.name_text.text = self->globals.name;
    self->globals.windowses.options.name_text.dimensions.h = name_height;
    self->globals.windowses.options.name_text.dimensions.w = strlen(self->globals.windowses.options.name_text.text) * (self->globals.windowses.options.name_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.name_text.coords.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.options.name_text.dimensions.w / 2;
    self->globals.windowses.options.name_text.coords.y = padding_top + (element_id * (spacing + element_h));
    element_id++;

    //Rows up button
    self->globals.windowses.options.rows_up_btn.text = " ^ ";
    self->globals.windowses.options.rows_up_btn.position.rect.h = element_h;
    self->globals.windowses.options.rows_up_btn.position.roundness = 25;
    self->globals.windowses.options.rows_up_btn.position.thickness = 2;
    self->globals.windowses.options.rows_up_btn.position.rect.w = strlen(self->globals.windowses.options.rows_up_btn.text) * (self->globals.windowses.options.rows_up_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.rows_up_btn.position.rect.x = self->globals.window_dimensions.w / 4 - self->globals.windowses.options.rows_up_btn.position.rect.w / 2;
    self->globals.windowses.options.rows_up_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;

    //Cols up button
    self->globals.windowses.options.cols_up_btn.text = " ^ ";
    self->globals.windowses.options.cols_up_btn.position.rect.h = element_h;
    self->globals.windowses.options.cols_up_btn.position.roundness = 25;
    self->globals.windowses.options.cols_up_btn.position.thickness = 2;
    self->globals.windowses.options.cols_up_btn.position.rect.w = strlen(self->globals.windowses.options.cols_up_btn.text) * (self->globals.windowses.options.cols_up_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.cols_up_btn.position.rect.x = self->globals.window_dimensions.w / 4 * 3 - self->globals.windowses.options.cols_up_btn.position.rect.w / 2;
    self->globals.windowses.options.cols_up_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Rows text
    char rows_text[32];
    sprintf(rows_text, "%d", self->setting.rows);
    self->globals.windowses.options.rows_text.text = malloc(sizeof(strlen(rows_text)));
    strcpy(self->globals.windowses.options.rows_text.text, rows_text);
    self->globals.windowses.options.rows_text.dimensions.h = element_h;
    self->globals.windowses.options.rows_text.dimensions.w = strlen(self->globals.windowses.options.rows_text.text) * (self->globals.windowses.options.rows_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.rows_text.coords.x = self->globals.window_dimensions.w / 4 - self->globals.windowses.options.rows_text.dimensions.w / 2;
    self->globals.windowses.options.rows_text.coords.y = padding_top + (element_id * (spacing + element_h)) + name_h;

    //Cols text
    char cols_text[32];
    sprintf(cols_text, "%d", self->setting.cols);
    self->globals.windowses.options.cols_text.text = malloc(sizeof(strlen(cols_text)));
    strcpy(self->globals.windowses.options.cols_text.text, cols_text);
    self->globals.windowses.options.cols_text.dimensions.h = element_h;
    self->globals.windowses.options.cols_text.dimensions.w = strlen(self->globals.windowses.options.cols_text.text) * (self->globals.windowses.options.cols_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.cols_text.coords.x = self->globals.window_dimensions.w / 4 * 3 - self->globals.windowses.options.cols_text.dimensions.w / 2;
    self->globals.windowses.options.cols_text.coords.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Rows down button
    self->globals.windowses.options.rows_down_btn.text = " v ";
    self->globals.windowses.options.rows_down_btn.position.rect.h = element_h;
    self->globals.windowses.options.rows_down_btn.position.roundness = 25;
    self->globals.windowses.options.rows_down_btn.position.thickness = 2;
    self->globals.windowses.options.rows_down_btn.position.rect.w = strlen(self->globals.windowses.options.rows_down_btn.text) * (self->globals.windowses.options.rows_down_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.rows_down_btn.position.rect.x = self->globals.window_dimensions.w / 4 - self->globals.windowses.options.rows_down_btn.position.rect.w / 2;
    self->globals.windowses.options.rows_down_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;

    //Cols down button
    self->globals.windowses.options.cols_down_btn.text = " v ";
    self->globals.windowses.options.cols_down_btn.position.rect.h = element_h;
    self->globals.windowses.options.cols_down_btn.position.roundness = 25;
    self->globals.windowses.options.cols_down_btn.position.thickness = 2;
    self->globals.windowses.options.cols_down_btn.position.rect.w = strlen(self->globals.windowses.options.cols_down_btn.text) * (self->globals.windowses.options.cols_down_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.cols_down_btn.position.rect.x = self->globals.window_dimensions.w / 4 * 3 - self->globals.windowses.options.cols_down_btn.position.rect.w / 2;
    self->globals.windowses.options.cols_down_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Theme left button
    self->globals.windowses.options.theme_left_btn.text = "<";
    self->globals.windowses.options.theme_left_btn.position.rect.h = element_h;
    self->globals.windowses.options.theme_left_btn.position.roundness = 25;
    self->globals.windowses.options.theme_left_btn.position.thickness = 2;
    self->globals.windowses.options.theme_left_btn.position.rect.w = strlen(self->globals.windowses.options.theme_left_btn.text) * (self->globals.windowses.options.theme_left_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.theme_left_btn.position.rect.x = self->globals.window_dimensions.w / 4 - self->globals.windowses.options.theme_left_btn.position.rect.w / 2;
    self->globals.windowses.options.theme_left_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;

    //Theme text
    char theme_text[32];
    sprintf(theme_text, "%s", self->globals.themes[self->setting.selected_theme]->name);
    self->globals.windowses.options.theme_text.text = malloc(sizeof(strlen(theme_text)));
    strcpy(self->globals.windowses.options.theme_text.text, theme_text);
    self->globals.windowses.options.theme_text.dimensions.h = element_h;
    self->globals.windowses.options.theme_text.dimensions.w = strlen(self->globals.windowses.options.theme_text.text) * (self->globals.windowses.options.theme_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.theme_text.coords.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.options.theme_text.dimensions.w / 2;
    self->globals.windowses.options.theme_text.coords.y = padding_top + (element_id * (spacing + element_h)) + name_h;

    //Theme right button
    self->globals.windowses.options.theme_right_btn.text = ">";
    self->globals.windowses.options.theme_right_btn.position.rect.h = element_h;
    self->globals.windowses.options.theme_right_btn.position.roundness = 25;
    self->globals.windowses.options.theme_right_btn.position.thickness = 2;
    self->globals.windowses.options.theme_right_btn.position.rect.w = strlen(self->globals.windowses.options.theme_right_btn.text) * (self->globals.windowses.options.theme_right_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.theme_right_btn.position.rect.x = self->globals.window_dimensions.w / 4 * 3 - self->globals.windowses.options.theme_right_btn.position.rect.w / 2;
    self->globals.windowses.options.theme_right_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;

    element_id++;

    //Is endless button
    self->globals.windowses.options.is_endless_btn.text = self->setting.is_endless ? "Is endless" : "Is not endless";
    self->globals.windowses.options.is_endless_btn.position.rect.h = element_h;
    self->globals.windowses.options.is_endless_btn.position.roundness = 50;
    self->globals.windowses.options.is_endless_btn.position.thickness = 5;
    self->globals.windowses.options.is_endless_btn.position.rect.w = strlen(self->globals.windowses.options.is_endless_btn.text) * (self->globals.windowses.options.is_endless_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.is_endless_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.options.is_endless_btn.position.rect.w / 2;
    self->globals.windowses.options.is_endless_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;

    //Back button
    self->globals.windowses.options.back_btn.text = "Back";
    self->globals.windowses.options.back_btn.position.rect.h = element_h;
    self->globals.windowses.options.back_btn.position.roundness = 50;
    self->globals.windowses.options.back_btn.position.thickness = 5;
    self->globals.windowses.options.back_btn.position.rect.w = strlen(self->globals.windowses.options.back_btn.text) * (self->globals.windowses.options.back_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.back_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.options.back_btn.position.rect.w / 2;
    self->globals.windowses.options.back_btn.position.rect.y = padding_top + (element_id * (spacing + element_h)) + name_h;
    element_id++;
}

static void themes_init(Main_obj* self) {
    int selected_theme = 0;
    self->globals.number_of_themes = 4;
    self->globals.themes = malloc(sizeof(Theme*) * self->globals.number_of_themes);
    for(int i = 0; i < self->globals.number_of_themes; i++) {
        self->globals.themes[i] = malloc(sizeof(Theme));
    }

/*
    self->globals.themes[selected_theme]->name = "Default";

    self->globals.themes[selected_theme]->background.r = 255;
    self->globals.themes[selected_theme]->background.g = 255;
    self->globals.themes[selected_theme]->background.b = 204;
    self->globals.themes[selected_theme]->background.a = 255;

    self->globals.themes[selected_theme]->foreground.r = 235;
    self->globals.themes[selected_theme]->foreground.g = 235;
    self->globals.themes[selected_theme]->foreground.b = 184;
    self->globals.themes[selected_theme]->foreground.a = 255;

    self->globals.themes[selected_theme]->text_color.r = 235;
    self->globals.themes[selected_theme]->text_color.g = 235;
    self->globals.themes[selected_theme]->text_color.b = 184;
    self->globals.themes[selected_theme]->text_color.a = 255;
    selected_theme++;


    self->globals.themes[selected_theme]->name = "Ocean";

    self->globals.themes[selected_theme]->background.r = 85;
    self->globals.themes[selected_theme]->background.g = 206;
    self->globals.themes[selected_theme]->background.b = 255;
    self->globals.themes[selected_theme]->background.a = 255;

    self->globals.themes[selected_theme]->foreground.r = 45;
    self->globals.themes[selected_theme]->foreground.g = 166;
    self->globals.themes[selected_theme]->foreground.b = 215;
    self->globals.themes[selected_theme]->foreground.a = 255;

    self->globals.themes[selected_theme]->text_color.r = 45;
    self->globals.themes[selected_theme]->text_color.g = 166;
    self->globals.themes[selected_theme]->text_color.b = 215;
    self->globals.themes[selected_theme]->text_color.a = 255;
    selected_theme++;

    self->globals.themes[selected_theme]->name = "Turnip";

    self->globals.themes[selected_theme]->background.r = 235;
    self->globals.themes[selected_theme]->background.g = 150;
    self->globals.themes[selected_theme]->background.b = 150;
    self->globals.themes[selected_theme]->background.a = 255;

    self->globals.themes[selected_theme]->foreground.r = 255;
    self->globals.themes[selected_theme]->foreground.g = 170;
    self->globals.themes[selected_theme]->foreground.b = 170;
    self->globals.themes[selected_theme]->foreground.a = 255;

    self->globals.themes[selected_theme]->text_color.r = 255;
    self->globals.themes[selected_theme]->text_color.g = 170;
    self->globals.themes[selected_theme]->text_color.b = 170;
    self->globals.themes[selected_theme]->text_color.a = 255;
    selected_theme++;

    self->globals.themes[selected_theme]->name = "Losos";

    self->globals.themes[selected_theme]->background.r = 100;
    self->globals.themes[selected_theme]->background.g = 252;
    self->globals.themes[selected_theme]->background.b = 252;
    self->globals.themes[selected_theme]->background.a = 255;

    self->globals.themes[selected_theme]->foreground.r = 70;
    self->globals.themes[selected_theme]->foreground.g = 222;
    self->globals.themes[selected_theme]->foreground.b = 222;
    self->globals.themes[selected_theme]->foreground.a = 255;

    self->globals.themes[selected_theme]->text_color.r = 70;
    self->globals.themes[selected_theme]->text_color.g = 222;
    self->globals.themes[selected_theme]->text_color.b = 222;
    self->globals.themes[selected_theme]->text_color.a = 255;
    selected_theme++;*/
}

static void number_props_init(Main_obj* self) {
    self->globals.number_props = malloc(sizeof(Number_prop*) * 12);
    for(int i = 0; i < 12; i++) {
        self->globals.number_props[i] = malloc(sizeof(Number_prop));
    }

    //Color of numbers

    //Number 2
    self->globals.number_props[0]->text_color.r = 119;
    self->globals.number_props[0]->text_color.g = 119;
    self->globals.number_props[0]->text_color.b = 119;
    self->globals.number_props[0]->text_color.a = 255;

    //Number 4
    self->globals.number_props[1]->text_color.r = 233;
    self->globals.number_props[1]->text_color.g = 184;
    self->globals.number_props[1]->text_color.b = 86;
    self->globals.number_props[1]->text_color.a = 255;

    //Number 8
    self->globals.number_props[2]->text_color.r = 249;
    self->globals.number_props[2]->text_color.g = 78;
    self->globals.number_props[2]->text_color.b = 47;
    self->globals.number_props[2]->text_color.a = 255;

    //Number 16
    self->globals.number_props[3]->text_color.r = 255;
    self->globals.number_props[3]->text_color.g = 255;
    self->globals.number_props[3]->text_color.b = 255;
    self->globals.number_props[3]->text_color.a = 255;

    //Number 32
    self->globals.number_props[4]->text_color.r = 253;
    self->globals.number_props[4]->text_color.g = 253;
    self->globals.number_props[4]->text_color.b = 253;
    self->globals.number_props[4]->text_color.a = 255;

    //Number 64
    self->globals.number_props[5]->text_color.r = 255;
    self->globals.number_props[5]->text_color.g = 255;
    self->globals.number_props[5]->text_color.b = 255;
    self->globals.number_props[5]->text_color.a = 255;

    //Number 128
    self->globals.number_props[6]->text_color.r = 249;
    self->globals.number_props[6]->text_color.g = 78;
    self->globals.number_props[6]->text_color.b = 47;
    self->globals.number_props[6]->text_color.a = 255;

    //Number 256
    self->globals.number_props[7]->text_color.r = 255;
    self->globals.number_props[7]->text_color.g = 204;
    self->globals.number_props[7]->text_color.b = 102;
    self->globals.number_props[7]->text_color.a = 255;

    //Number 512
    self->globals.number_props[8]->text_color.r = 251;
    self->globals.number_props[8]->text_color.g = 78;
    self->globals.number_props[8]->text_color.b = 78;
    self->globals.number_props[8]->text_color.a = 255;

    //Number 1024
    self->globals.number_props[9]->text_color.r = 246;
    self->globals.number_props[9]->text_color.g = 247;
    self->globals.number_props[9]->text_color.b = 255;
    self->globals.number_props[9]->text_color.a = 255;

    //Number 2048
    self->globals.number_props[10]->text_color.r = 255;
    self->globals.number_props[10]->text_color.g = 243;
    self->globals.number_props[10]->text_color.b = 0;
    self->globals.number_props[10]->text_color.a = 255;

    //Number bigger than 2048
    self->globals.number_props[11]->text_color.r = 255;
    self->globals.number_props[11]->text_color.g = 255;
    self->globals.number_props[11]->text_color.b = 255;
    self->globals.number_props[11]->text_color.a = 255;

    //Color of cells

    //Number 2
    self->globals.number_props[0]->cell_color.r = 255;
    self->globals.number_props[0]->cell_color.g = 255;
    self->globals.number_props[0]->cell_color.b = 255;
    self->globals.number_props[0]->cell_color.a = 255;

    //Number 4
    self->globals.number_props[1]->cell_color.r = 250;
    self->globals.number_props[1]->cell_color.g = 250;
    self->globals.number_props[1]->cell_color.b = 250;
    self->globals.number_props[1]->cell_color.a = 255;

    //Number 8
    self->globals.number_props[2]->cell_color.r = 243;
    self->globals.number_props[2]->cell_color.g = 243;
    self->globals.number_props[2]->cell_color.b = 243;
    self->globals.number_props[2]->cell_color.a = 255;

    //Number 16
    self->globals.number_props[3]->cell_color.r = 245;
    self->globals.number_props[3]->cell_color.g = 236;
    self->globals.number_props[3]->cell_color.b = 0;
    self->globals.number_props[3]->cell_color.a = 255;

    //Number 32
    self->globals.number_props[4]->cell_color.r = 252;
    self->globals.number_props[4]->cell_color.g = 194;
    self->globals.number_props[4]->cell_color.b = 14;
    self->globals.number_props[4]->cell_color.a = 255;

    //Number 64
    self->globals.number_props[5]->cell_color.r = 249;
    self->globals.number_props[5]->cell_color.g = 74;
    self->globals.number_props[5]->cell_color.b = 59;
    self->globals.number_props[5]->cell_color.a = 255;

    //Number 128
    self->globals.number_props[6]->cell_color.r = 34;
    self->globals.number_props[6]->cell_color.g = 34;
    self->globals.number_props[6]->cell_color.b = 34;
    self->globals.number_props[6]->cell_color.a = 255;

    //Number 256
    self->globals.number_props[7]->cell_color.r = 209;
    self->globals.number_props[7]->cell_color.g = 49;
    self->globals.number_props[7]->cell_color.b = 0;
    self->globals.number_props[7]->cell_color.a = 255;

    //Number 512
    self->globals.number_props[8]->cell_color.r = 255;
    self->globals.number_props[8]->cell_color.g = 243;
    self->globals.number_props[8]->cell_color.b = 0;
    self->globals.number_props[8]->cell_color.a = 255;

    //Number 1024
    self->globals.number_props[9]->cell_color.r = 100;
    self->globals.number_props[9]->cell_color.g = 131;
    self->globals.number_props[9]->cell_color.b = 181;
    self->globals.number_props[9]->cell_color.a = 255;

    //Number 2048
    self->globals.number_props[10]->cell_color.r = 0;
    self->globals.number_props[10]->cell_color.g = 0;
    self->globals.number_props[10]->cell_color.b = 0;
    self->globals.number_props[10]->cell_color.a = 255;

    //Number bigger than 2048
    self->globals.number_props[11]->cell_color.r = 255;
    self->globals.number_props[11]->cell_color.g = 0;
    self->globals.number_props[11]->cell_color.b = 153;
    self->globals.number_props[11]->cell_color.a = 255;

    //Coords of powers

    //Number 2
    self->globals.number_props[0]->power_coords.x = 0;
    self->globals.number_props[0]->power_coords.y = 0;

    //Number 4
    self->globals.number_props[1]->power_coords.x = 0;
    self->globals.number_props[1]->power_coords.y = 0;

    //Number 8
    self->globals.number_props[2]->power_coords.x = 0;
    self->globals.number_props[2]->power_coords.y = 0;

    //Number 16
    self->globals.number_props[3]->power_coords.x = 0;
    self->globals.number_props[3]->power_coords.y = 0;

    //Number 32
    self->globals.number_props[4]->power_coords.x = 0;
    self->globals.number_props[4]->power_coords.y = 0;

    //Number 64
    self->globals.number_props[5]->power_coords.x = 0;
    self->globals.number_props[5]->power_coords.y = 0;

    //Number 128
    self->globals.number_props[6]->power_coords.x = 0;
    self->globals.number_props[6]->power_coords.y = 0;

    //Number 256
    self->globals.number_props[7]->power_coords.x = 0;
    self->globals.number_props[7]->power_coords.y = 0;

    //Number 512
    self->globals.number_props[8]->power_coords.x = 0;
    self->globals.number_props[8]->power_coords.y = 0;

    //Number 1024
    self->globals.number_props[9]->power_coords.x = 0;
    self->globals.number_props[9]->power_coords.y = 0;

    //Number 2048
    self->globals.number_props[10]->power_coords.x = 0;
    self->globals.number_props[10]->power_coords.y = 0;

    //Number bigger than 2048
    self->globals.number_props[11]->power_coords.x = 55;
    self->globals.number_props[11]->power_coords.y = -5;

    //Coords of numbers

    //Number 2
    self->globals.number_props[0]->coords.x = 25;
    self->globals.number_props[0]->coords.y = -5;

    //Number 4
    self->globals.number_props[1]->coords.x = 25;
    self->globals.number_props[1]->coords.y = -5;

    //Number 8
    self->globals.number_props[2]->coords.x = 25;
    self->globals.number_props[2]->coords.y = -5;

    //Number 16
    self->globals.number_props[3]->coords.x = -2;
    self->globals.number_props[3]->coords.y = -5;

    //Number 32
    self->globals.number_props[4]->coords.x = 0;
    self->globals.number_props[4]->coords.y = -5;

    //Number 64
    self->globals.number_props[5]->coords.x = 0;
    self->globals.number_props[5]->coords.y = -5;

    //Number 128
    self->globals.number_props[6]->coords.x = -2;
    self->globals.number_props[6]->coords.y = 10;

    //Number 256
    self->globals.number_props[7]->coords.x = 0;
    self->globals.number_props[7]->coords.y = 10;

    //Number 512
    self->globals.number_props[8]->coords.x = 0;
    self->globals.number_props[8]->coords.y = 10;

    //Number 1024
    self->globals.number_props[9]->coords.x = 0;
    self->globals.number_props[9]->coords.y = 20;

    //Number 2048
    self->globals.number_props[10]->coords.x = 0;
    self->globals.number_props[10]->coords.y = 20;

    //Number bigger than 2048
    self->globals.number_props[11]->coords.x = 10;
    self->globals.number_props[11]->coords.y = -5;

    //Dimensions of numbers

    //Number 2
    self->globals.number_props[0]->dimensions.w = 50;
    self->globals.number_props[0]->dimensions.h = 100;

    //Number 4
    self->globals.number_props[1]->dimensions.w = 50;
    self->globals.number_props[1]->dimensions.h = 100;

    //Number 8
    self->globals.number_props[2]->dimensions.w = 50;
    self->globals.number_props[2]->dimensions.h = 100;

    //Number 16
    self->globals.number_props[3]->dimensions.w = 100;
    self->globals.number_props[3]->dimensions.h = 100;

    //Number 32
    self->globals.number_props[4]->dimensions.w = 100;
    self->globals.number_props[4]->dimensions.h = 100;

    //Number 64
    self->globals.number_props[5]->dimensions.w = 100;
    self->globals.number_props[5]->dimensions.h = 100;

    //Number 128
    self->globals.number_props[6]->dimensions.w = 100;
    self->globals.number_props[6]->dimensions.h = 75;

    //Number 256
    self->globals.number_props[7]->dimensions.w = 100;
    self->globals.number_props[7]->dimensions.h = 75;

    //Number 512
    self->globals.number_props[8]->dimensions.w = 100;
    self->globals.number_props[8]->dimensions.h = 75;

    //Number 1024
    self->globals.number_props[9]->dimensions.w = 100;
    self->globals.number_props[9]->dimensions.h = 50;

    //Number 2048
    self->globals.number_props[10]->dimensions.w = 100;
    self->globals.number_props[10]->dimensions.h = 50;

    //Number bigger than 2048
    self->globals.number_props[11]->dimensions.w = 50;
    self->globals.number_props[11]->dimensions.h = 100;

    //Power values

    self->globals.number_props[0]->power_value = 2;
    self->globals.number_props[1]->power_value = 4;
    self->globals.number_props[2]->power_value = 8;
    self->globals.number_props[3]->power_value = 16;
    self->globals.number_props[4]->power_value = 32;
    self->globals.number_props[5]->power_value = 64;
    self->globals.number_props[6]->power_value = 128;
    self->globals.number_props[7]->power_value = 256;
    self->globals.number_props[8]->power_value = 512;
    self->globals.number_props[9]->power_value = 1024;
    self->globals.number_props[10]->power_value = 2048;
    self->globals.number_props[11]->power_value = 2;

    //Paddings
    int padding = 10;
    self->globals.number_props[0]->padding = padding;
    self->globals.number_props[1]->padding = padding;
    self->globals.number_props[2]->padding = padding;
    self->globals.number_props[3]->padding = padding;
    self->globals.number_props[4]->padding = padding;
    self->globals.number_props[5]->padding = padding;
    self->globals.number_props[6]->padding = padding;
    self->globals.number_props[7]->padding = padding;
    self->globals.number_props[8]->padding = padding;
    self->globals.number_props[9]->padding = padding;
    self->globals.number_props[10]->padding = padding;
    self->globals.number_props[11]->padding = padding;
}
