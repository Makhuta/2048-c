#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#include "../headers/structures.h"
#include "../headers/click_handler.h"
#include "../headers/click_handler_statics.h"
#include "../headers/file_handler.h"
#include "../headers/init.h"
#include "../headers/manipulation.h"

static void update_rows(Main_obj* self) {
    char rows_text[32];
    sprintf(rows_text, "%d", self->setting.rows);
    strcpy(self->globals.windowses.options.rows_text.text, rows_text);
    self->globals.windowses.options.rows_text.dimensions.w = strlen(self->globals.windowses.options.rows_text.text) * (self->globals.windowses.options.rows_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.rows_text.coords.x = self->globals.window_dimensions.w / 4 - self->globals.windowses.options.rows_text.dimensions.w / 2;
}

static void update_cols(Main_obj* self) {
    char cols_text[32];
    sprintf(cols_text, "%d", self->setting.cols);
    strcpy(self->globals.windowses.options.cols_text.text, cols_text);
    self->globals.windowses.options.cols_text.dimensions.w = strlen(self->globals.windowses.options.cols_text.text) * (self->globals.windowses.options.cols_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.cols_text.coords.x = self->globals.window_dimensions.w / 4 * 3 - self->globals.windowses.options.cols_text.dimensions.w / 2;
}

static void update_theme(Main_obj* self) {
    char theme_text[32];
    sprintf(theme_text, "%s", self->globals.themes[self->setting.selected_theme]->name);
    strcpy(self->globals.windowses.options.theme_text.text, theme_text);
    self->globals.windowses.options.theme_text.dimensions.w = strlen(self->globals.windowses.options.theme_text.text) * (self->globals.windowses.options.theme_text.dimensions.h / 5 * 2);
    self->globals.windowses.options.theme_text.coords.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.options.theme_text.dimensions.w / 2;
}

static void update_is_endless(Main_obj* self) {
    self->globals.windowses.options.is_endless_btn.text = self->setting.is_endless ? "Is endless" : "Is not endless";
    self->globals.windowses.options.is_endless_btn.position.rect.w = strlen(self->globals.windowses.options.is_endless_btn.text) * (self->globals.windowses.options.is_endless_btn.position.rect.h / 5 * 2);
    self->globals.windowses.options.is_endless_btn.position.rect.x = self->globals.window_dimensions.w / 2 - self->globals.windowses.options.is_endless_btn.position.rect.w / 2;
}

static void clean_field(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;
            self->globals.windowses.game_window.field.points.current_highest_merge = 0;
            self->globals.windowses.game_window.field.points.current_highest_merge_old = 0;
            self->globals.windowses.game_window.field.points.score = 0;
            self->globals.windowses.game_window.field.points.score_old = 0;
            self->globals.windowses.game_window.time_spend = 0;
            self->globals.windowses.game_window.field.cells[cell_id]->power = -1;
        }
    }
    for(int i = 0; i < 2; i++) {
        add_cell(self);
    }
    self->globals.is_playable = true;
}

void handle_click(Main_obj* self, SDL_Event* e) {
    if(self->globals.selected_window == 0) {
        //Main menu
        if(click_match_button(&self->globals.windowses.main_menu.start_btn.position.rect, e->button.x, e->button.y)) {
            //Start
            start_game_init(self);
            self->globals.selected_window = 1;
        } else if(click_match_button(&self->globals.windowses.main_menu.continue_btn.position.rect, e->button.x, e->button.y)) {
            //Continue
            self->globals.selected_window = 1;
        } else if(click_match_button(&self->globals.windowses.main_menu.options_btn.position.rect, e->button.x, e->button.y)) {
            //Options
            self->globals.selected_window = 2;
        } else if(click_match_button(&self->globals.windowses.main_menu.exit_btn.position.rect, e->button.x, e->button.y)) {
            //Exit
            self->globals.quit = true;
        }
    } else if(self->globals.selected_window == 1) {
        //Game
        if(self->globals.is_playable) {
            if(click_match_button(&self->globals.windowses.game_window.back_to_main_menu_btn.position.rect, e->button.x, e->button.y)) {
                //Start
                self->globals.selected_window = 0;
            }
        } else {
            if(click_match_button(&self->globals.windowses.game_over.try_again_btn.position.rect, e->button.x, e->button.y)) {
                //Try again
                clean_field(self);
            } else if(click_match_button(&self->globals.windowses.game_over.main_menu_btn.position.rect, e->button.x, e->button.y)) {
                //Main Menu Button
                self->globals.selected_window = 0;
                self->globals.is_playable = true;
            }
        }
    } else if(self->globals.selected_window == 2) {
        //Options
        if(click_match_button(&self->globals.windowses.options.rows_up_btn.position.rect, e->button.x, e->button.y)) {
            //Rows up
            if(self->setting.rows < 50) {
                self->setting.rows++;
                update_rows(self);
            }
        } else if(click_match_button(&self->globals.windowses.options.rows_down_btn.position.rect, e->button.x, e->button.y)) {
            //Rows down
            if(self->setting.rows > 4) {
                self->setting.rows--;
                update_rows(self);
            }
        } else if(click_match_button(&self->globals.windowses.options.cols_up_btn.position.rect, e->button.x, e->button.y)) {
            //Cols up
            if(self->setting.cols < 100) {
                self->setting.cols++;
                update_cols(self);
            }
        } else if(click_match_button(&self->globals.windowses.options.cols_down_btn.position.rect, e->button.x, e->button.y)) {
            //Cols down
            if(self->setting.cols > 4) {
                self->setting.cols--;
                update_cols(self);
            }
        } else if(click_match_button(&self->globals.windowses.options.theme_left_btn.position.rect, e->button.x, e->button.y)) {
            //Next theme
            if(self->setting.selected_theme <= 0) {
                self->setting.selected_theme = self->globals.number_of_themes - 1;
            } else {
                self->setting.selected_theme--;
            }
            update_theme(self);
        } else if(click_match_button(&self->globals.windowses.options.theme_right_btn.position.rect, e->button.x, e->button.y)) {
            //Previous theme
            if(self->setting.selected_theme >= self->globals.number_of_themes - 1) {
                self->setting.selected_theme = 0;
            } else {
                self->setting.selected_theme++;
            }
            update_theme(self);
        } else if(click_match_button(&self->globals.windowses.options.is_endless_btn.position.rect, e->button.x, e->button.y)) {
            //Is endless
            self->setting.is_endless = !self->setting.is_endless;
            update_is_endless(self);
        } else if(click_match_button(&self->globals.windowses.options.back_btn.position.rect, e->button.x, e->button.y)) {
            //Back
            save_settings(self);
            self->globals.selected_window = 0;
        }
    }
}

static bool click_match_button(SDL_Rect* rect, const int x, const int y) {
    return (x >= rect->x && y >= rect->y && x <= rect->x + rect->w && y <= rect->y + rect->h);
}
