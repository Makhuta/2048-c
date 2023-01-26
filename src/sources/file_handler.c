#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/structures.h"
#include "../headers/file_handler.h"
#include "../headers/file_handler_statics.h"

static void load_dimensions(Main_obj* self) {
    FILE* game_file = fopen("./src/saves/game.csv", "rb");
    if(!game_file){
        fprintf(stderr, "Unable to open game file.\n");
        self->setting.rows = 4;
        self->setting.cols = 4;
    } else {
        fscanf(game_file, "%d", &self->setting.rows);
        fscanf(game_file, "%d", &self->setting.cols);
        fclose(game_file);
    }
}

void load_game(Main_obj* self) {
    FILE* game_file = fopen("./src/saves/game.csv", "rb");
    if(!game_file){
        fprintf(stderr, "Unable to open game file.\n");
        self->globals.windowses.game_window.field.rows = 4;
        self->globals.windowses.game_window.field.cols = 4;
        self->globals.windowses.game_window.time_spend = 0;
        self->globals.windowses.game_window.field.cells = malloc(sizeof(Cell*) * self->globals.windowses.game_window.field.rows * self->globals.windowses.game_window.field.cols);
        for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
            for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
                self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols] = malloc(sizeof(Cell));
                self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power = -1;
            }
        }
        for(int i = 0; i < 2; i++) {
            self->globals.windowses.game_window.field.cells[rand() % (self->globals.windowses.game_window.field.cols * self->globals.windowses.game_window.field.rows - 1)]->power = rand() % 2;
        }
    } else {
        fscanf(game_file, "%d", &self->globals.windowses.game_window.field.rows);
        self->setting.rows = self->globals.windowses.game_window.field.rows;
        fscanf(game_file, "%d", &self->globals.windowses.game_window.field.cols);
        self->setting.cols = self->globals.windowses.game_window.field.cols;
        fscanf(game_file, "%d", &self->globals.windowses.game_window.time_spend);
        self->globals.windowses.game_window.field.cells = malloc(sizeof(Cell*) * self->globals.windowses.game_window.field.rows * self->globals.windowses.game_window.field.cols);
        for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
            for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
                self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols] = malloc(sizeof(Cell));
                fscanf(game_file, "%d", &self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power);
                if(self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power > -1){
                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_animated = false;
                } else {
                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merge_animated = true;
                }
            }
        }
        fclose(game_file);
    }
}

static bool name_is_same(const char *name1, const char *name2){
    int buff = true;
    if(strlen(name1) != strlen(name2)) {
        buff = false;
    } else {
        if(buff) {
            for(int i = 0; i < strlen(name1); i++) {
                if(name1[i] != name2[i]) {
                    buff = false;
                }
            }
        }
    }

    return buff;
}

static int count_themes() {
    int number_of_themes = 0;
    FILE* themes_file = fopen("./src/saves/themes.csv", "rb");
    if(!themes_file){
        number_of_themes++;
    } else {
        int buff;
        char last_name[64] = "0";
        char name[64] = "1";
        while(!name_is_same(name, last_name)) {
            strcpy(last_name, name);

            fscanf(themes_file, "%s %d %d %d %d %d %d %d %d %d %d %d %d", &name, &buff, &buff, &buff, &buff, &buff, &buff, &buff, &buff, &buff, &buff, &buff, &buff);
            if(!name_is_same(name, last_name)) {
                number_of_themes++;
            }
        }
        fclose(themes_file);
    }
    return number_of_themes;
}

void load_themes(Main_obj* self){
    self->globals.number_of_themes = count_themes();
    self->globals.themes = malloc(sizeof(Theme*) * (self->globals.number_of_themes));
    for(int i = 0; i < self->globals.number_of_themes; i++) {
        self->globals.themes[i] = malloc(sizeof(Theme));
    }

    FILE* themes_file = fopen("./src/saves/themes.csv", "rb");
    if(!themes_file){
        fprintf(stderr, "Unable to open theme file.\n");
        strcpy(self->globals.themes[self->globals.number_of_themes]->name, "Default");

        self->globals.themes[self->globals.number_of_themes]->background.r = 255;
        self->globals.themes[self->globals.number_of_themes]->background.g = 255;
        self->globals.themes[self->globals.number_of_themes]->background.b = 204;
        self->globals.themes[self->globals.number_of_themes]->background.a = 255;

        self->globals.themes[self->globals.number_of_themes]->foreground.r = 235;
        self->globals.themes[self->globals.number_of_themes]->foreground.g = 235;
        self->globals.themes[self->globals.number_of_themes]->foreground.b = 184;
        self->globals.themes[self->globals.number_of_themes]->foreground.a = 255;

        self->globals.themes[self->globals.number_of_themes]->text_color.r = 235;
        self->globals.themes[self->globals.number_of_themes]->text_color.g = 235;
        self->globals.themes[self->globals.number_of_themes]->text_color.b = 184;
        self->globals.themes[self->globals.number_of_themes]->text_color.a = 255;
        self->globals.number_of_themes++;
    } else {
        int id = 0;
        char last_name[64] = "0";
        char name[64] = "1";
        while(!name_is_same(name, last_name)) {
            RGBA b;
            RGBA f;
            RGBA t;
            strcpy(last_name, name);
            fscanf(themes_file, "%s %d %d %d %d %d %d %d %d %d %d %d %d", &name, &b.r, &b.g, &b.b, &b.a, &f.r, &f.g, &f.b, &f.a, &t.r, &t.g, &t.b, &t.a);
            if(!name_is_same(name, last_name)) {
                strcpy(self->globals.themes[id]->name, name);

                self->globals.themes[id]->background.r = b.r;
                self->globals.themes[id]->background.g = b.g;
                self->globals.themes[id]->background.b = b.b;
                self->globals.themes[id]->background.a = b.a;

                self->globals.themes[id]->foreground.r = f.r;
                self->globals.themes[id]->foreground.g = f.g;
                self->globals.themes[id]->foreground.b = f.b;
                self->globals.themes[id]->foreground.a = f.a;

                self->globals.themes[id]->text_color.r = t.r;
                self->globals.themes[id]->text_color.g = t.g;
                self->globals.themes[id]->text_color.b = t.b;
                self->globals.themes[id]->text_color.a = t.a;
                id++;
            }
        }
        fclose(themes_file);
    }
}


static void load_theme(Main_obj* self) {
    load_themes(self);
    FILE* theme_file = fopen("./src/saves/theme.csv", "rb");
    if(!theme_file){
        fprintf(stderr, "Unable to open theme file.\n");
        self->setting.selected_theme = 0;
    } else {
        fscanf(theme_file, "%d", &self->setting.selected_theme);
        fclose(theme_file);
    }
}

static void save_theme(Main_obj* self) {
    FILE* theme_file = fopen("./src/saves/theme.csv", "wb");
    if(theme_file){
        fprintf(theme_file, "%d\n", self->setting.selected_theme);
        fclose(theme_file);
    }
}

void save_game(Main_obj* self) {
    FILE* game_file = fopen("./src/saves/game.csv", "wb");
    if(game_file){
        fprintf(game_file, "%d\n%d\n%d\n", self->globals.windowses.game_window.field.rows, self->globals.windowses.game_window.field.cols, self->globals.windowses.game_window.time_spend);
        for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
            for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
                fprintf(game_file, "%d ", self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power); //priprava na hru
                free(self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]);
            }
            fprintf(game_file, "\n");
        }
        free(self->globals.windowses.game_window.field.cells);
        fclose(game_file);
    }
}

void load_settings(Main_obj* self) {
    load_dimensions(self);
    load_theme(self);
}

void save_settings(Main_obj* self) {
    save_theme(self);
}


void load_score(Main_obj* self){
    FILE* score_file = fopen("./src/saves/score.csv", "rb");
    if(!score_file){
        fprintf(stderr, "Unable to open score file.\n");
        self->globals.windowses.game_window.field.points.score = 0;
        self->globals.windowses.game_window.field.points.highscore = 0;
        self->globals.windowses.game_window.field.points.score_old = 0;
        self->globals.windowses.game_window.field.points.highscore_old = 0;

        self->globals.windowses.game_window.field.points.current_highest_merge = 0;
        self->globals.windowses.game_window.field.points.total_highest_merge = 0;
        self->globals.windowses.game_window.field.points.current_highest_merge_old = 0;
        self->globals.windowses.game_window.field.points.total_highest_merge_old = 0;
    } else {
        fscanf(score_file, "%d", &self->globals.windowses.game_window.field.points.score);
        fscanf(score_file, "%d", &self->globals.windowses.game_window.field.points.highscore);
        self->globals.windowses.game_window.field.points.score_old = self->globals.windowses.game_window.field.points.score;
        self->globals.windowses.game_window.field.points.highscore_old = self->globals.windowses.game_window.field.points.highscore;

        fscanf(score_file, "%d", &self->globals.windowses.game_window.field.points.current_highest_merge);
        fscanf(score_file, "%d", &self->globals.windowses.game_window.field.points.total_highest_merge);
        self->globals.windowses.game_window.field.points.current_highest_merge_old = self->globals.windowses.game_window.field.points.current_highest_merge;
        self->globals.windowses.game_window.field.points.total_highest_merge_old = self->globals.windowses.game_window.field.points.total_highest_merge;
        fclose(score_file);
    }
}

void save_score(Main_obj* self){
    FILE* score_file = fopen("./src/saves/score.csv", "wb");
    if(score_file){
        fprintf(score_file, "%d\n%d\n%d\n%d\n", self->globals.windowses.game_window.field.points.score, self->globals.windowses.game_window.field.points.highscore, self->globals.windowses.game_window.field.points.current_highest_merge, self->globals.windowses.game_window.field.points.total_highest_merge);
        fclose(score_file);
    }
}
