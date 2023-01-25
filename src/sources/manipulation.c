#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#include "../headers/structures.h"
#include "../headers/manipulation.h"
#include "../headers/manipulation_statics.h"


static void move_cell_left(Main_obj* self, const int cell_id) {
    int move_by = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
    self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x -= move_by;

    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x > self->globals.windowses.game_window.field.cells[cell_id]->move_half.x){
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed++;
    } else if(self->globals.windowses.game_window.field.cells[cell_id]->move_speed > 1) {
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed--;
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging && self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x > self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
        int move_merge_by = self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed;
        self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x -= move_merge_by;
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x < self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
            self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x;
        }
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x > self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.x && self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed < self->globals.windowses.game_window.field.cells[cell_id]->move_speed){
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed++;
        } else if(self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed > 1) {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed--;
        } else {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
        }
    }

    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x <= self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
        self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.x = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.y = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.x = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed = 1;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = 1;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.merging = false;
    }
}

static void move_cell_right(Main_obj* self, const int cell_id) {
    int move_by = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
    self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x += move_by;

    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x < self->globals.windowses.game_window.field.cells[cell_id]->move_half.x){
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed++;
    } else if(self->globals.windowses.game_window.field.cells[cell_id]->move_speed > 1) {
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed--;
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging && self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x < self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
        int move_merge_by = self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed;
        self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x += move_merge_by;
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x > self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
            self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x;
        }
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x < self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.x && self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed < self->globals.windowses.game_window.field.cells[cell_id]->move_speed){
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed++;
        } else if(self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed > 1) {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed--;
        } else {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
        }
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x >= self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
        self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.merging = false;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.x = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.y = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.x = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed = 1;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = 1;
    }
}

static void move_cell_up(Main_obj* self, const int cell_id) {
    int move_by = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
    self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y -= move_by;
    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y > self->globals.windowses.game_window.field.cells[cell_id]->move_half.y){
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed++;
    } else if(self->globals.windowses.game_window.field.cells[cell_id]->move_speed > 1) {
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed--;
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging && self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y > self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
        int move_merge_by = self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed;
        self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y -= move_merge_by;
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y < self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
            self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y;
        }
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y > self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.y && self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed < self->globals.windowses.game_window.field.cells[cell_id]->move_speed){
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed++;
        } else if(self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed > 1) {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed--;
        } else {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
        }
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y <= self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
        self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.merging = false;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.x = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.y = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.y = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed = 1;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = 1;
    }
}

static void move_cell_down(Main_obj* self, const int cell_id) {
    int move_by = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
    self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y += move_by;

    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y < self->globals.windowses.game_window.field.cells[cell_id]->move_half.y){
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed++;
    } else if(self->globals.windowses.game_window.field.cells[cell_id]->move_speed > 1) {
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed--;
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->merging.merging && self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y < self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
        int move_merge_by = self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed;
        self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y += move_merge_by;
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y > self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
            self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y;
        }
        if(self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y < self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.y && self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed < self->globals.windowses.game_window.field.cells[cell_id]->move_speed){
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed++;
        } else if(self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed > 1) {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed--;
        } else {
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = self->globals.windowses.game_window.field.cells[cell_id]->move_speed;
        }
    }
    if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y >= self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
        self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.merging = false;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.x = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_half.y = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.y = 0;
        self->globals.windowses.game_window.field.cells[cell_id]->move_speed = 1;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.move_speed = 1;
    }
}

static void set_half_distance_left(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;

            self->globals.windowses.game_window.field.cells[cell_id]->move_half.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x + (self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x - self->globals.windowses.game_window.field.cells[cell_id]->coords.x) / 2;
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x + (self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x - self->globals.windowses.game_window.field.cells[cell_id]->coords.x) / 2;
        }
    }
}

static void set_half_distance_right(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;

            self->globals.windowses.game_window.field.cells[cell_id]->move_half.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x - (self->globals.windowses.game_window.field.cells[cell_id]->coords.x - self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x) / 2;
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.x = self->globals.windowses.game_window.field.cells[cell_id]->coords.x - (self->globals.windowses.game_window.field.cells[cell_id]->coords.x - self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.x) / 2;
        }
    }
}

static void set_half_distance_up(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;

            self->globals.windowses.game_window.field.cells[cell_id]->move_half.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y + (self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y - self->globals.windowses.game_window.field.cells[cell_id]->coords.y) / 2;
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y + (self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y - self->globals.windowses.game_window.field.cells[cell_id]->coords.y) / 2;
        }
    }
}

static void set_half_distance_down(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;

            self->globals.windowses.game_window.field.cells[cell_id]->move_half.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y - (self->globals.windowses.game_window.field.cells[cell_id]->coords.y - self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y) / 2;
            self->globals.windowses.game_window.field.cells[cell_id]->merging.move_half.y = self->globals.windowses.game_window.field.cells[cell_id]->coords.y - (self->globals.windowses.game_window.field.cells[cell_id]->coords.y - self->globals.windowses.game_window.field.cells[cell_id]->merge_tile.y) / 2;
        }
    }
}

static void merge_left(Main_obj *self){
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            if(j > 0){
                if(self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->power == self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power && self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power > -1) {
                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->power++;
                    if(!self->setting.is_endless) {
                        self->globals.windowses.game_window.field.points.score += self->globals.number_props[self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->power]->power_value;
                        if(self->globals.windowses.game_window.field.points.score > self->globals.windowses.game_window.field.points.highscore) {
                            self->globals.windowses.game_window.field.points.highscore = self->globals.windowses.game_window.field.points.score;
                        }
                    } else {
                        if(self->globals.windowses.game_window.field.points.current_highest_merge < self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->power) {
                            self->globals.windowses.game_window.field.points.current_highest_merge = self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->power;
                        }
                        if(self->globals.windowses.game_window.field.points.current_highest_merge > self->globals.windowses.game_window.field.points.total_highest_merge) {
                            self->globals.windowses.game_window.field.points.total_highest_merge = self->globals.windowses.game_window.field.points.current_highest_merge;
                        }
                    }
                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->merging.merging = true;
                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->merging.merge_animated = false;
                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.y;

                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[j - 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y;
                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power = -1;

                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x;
                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y;
                }
            }
        }
    }
}

static void merge_right(Main_obj *self){
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = self->globals.windowses.game_window.field.cols; j >= 0 ; j--) {
            if(j < self->globals.windowses.game_window.field.cols - 1){
                if(self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->power == self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power &&self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power > -1) {
                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->power++;
                    if(!self->setting.is_endless) {
                        self->globals.windowses.game_window.field.points.score += self->globals.number_props[self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->power]->power_value;
                        if(self->globals.windowses.game_window.field.points.score > self->globals.windowses.game_window.field.points.highscore) {
                            self->globals.windowses.game_window.field.points.highscore = self->globals.windowses.game_window.field.points.score;
                        }
                    } else {
                        if(self->globals.windowses.game_window.field.points.current_highest_merge < self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->power) {
                            self->globals.windowses.game_window.field.points.current_highest_merge = self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->power;
                        }
                        if(self->globals.windowses.game_window.field.points.current_highest_merge > self->globals.windowses.game_window.field.points.total_highest_merge) {
                            self->globals.windowses.game_window.field.points.total_highest_merge = self->globals.windowses.game_window.field.points.current_highest_merge;
                        }
                    }
                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->merging.merging = true;
                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->merging.merge_animated = false;
                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.y;

                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[j + 1 + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y;
                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power = -1;

                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.x;
                    self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords.y;
                }
            }
        }
    }
}

static void merge_up(Main_obj *self){
    for(int i = 0; i < self->globals.windowses.game_window.field.cols; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.rows ; j++) {
            if(j > 0){
                if(self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->power == self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power && self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power > -1) {
                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->power++;
                    if(!self->setting.is_endless) {
                        self->globals.windowses.game_window.field.points.score += self->globals.number_props[self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->power]->power_value;
                        if(self->globals.windowses.game_window.field.points.score > self->globals.windowses.game_window.field.points.highscore) {
                            self->globals.windowses.game_window.field.points.highscore = self->globals.windowses.game_window.field.points.score;
                        }
                    } else {
                        if(self->globals.windowses.game_window.field.points.current_highest_merge < self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->power) {
                            self->globals.windowses.game_window.field.points.current_highest_merge = self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->power;
                        }
                        if(self->globals.windowses.game_window.field.points.current_highest_merge > self->globals.windowses.game_window.field.points.total_highest_merge) {
                            self->globals.windowses.game_window.field.points.total_highest_merge = self->globals.windowses.game_window.field.points.current_highest_merge;
                        }
                    }
                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->merging.merging = true;
                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->merging.merge_animated = false;
                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->coords_from.y;

                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[i + (j - 1) * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.y;
                    self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power = -1;


                    self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords.x;
                    self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords.y;
                }
            }
        }
    }
}

static void merge_down(Main_obj *self){
    for(int i = 0; i < self->globals.windowses.game_window.field.cols; i++) {
        for(int j = self->globals.windowses.game_window.field.rows; j >= 0 ; j--) {
            if(j < self->globals.windowses.game_window.field.rows - 1){
                if(self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->power == self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power && self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power > -1) {
                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->power++;
                    if(!self->setting.is_endless) {
                        self->globals.windowses.game_window.field.points.score += self->globals.number_props[self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->power]->power_value;
                        if(self->globals.windowses.game_window.field.points.score > self->globals.windowses.game_window.field.points.highscore) {
                            self->globals.windowses.game_window.field.points.highscore = self->globals.windowses.game_window.field.points.score;
                        }
                    } else {
                        if(self->globals.windowses.game_window.field.points.current_highest_merge < self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->power) {
                            self->globals.windowses.game_window.field.points.current_highest_merge = self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->power;
                        }
                        if(self->globals.windowses.game_window.field.points.current_highest_merge > self->globals.windowses.game_window.field.points.total_highest_merge) {
                            self->globals.windowses.game_window.field.points.total_highest_merge = self->globals.windowses.game_window.field.points.current_highest_merge;
                        }
                    }
                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->merging.merging = true;
                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->merging.merge_animated = false;
                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->coords_from.y;

                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.x;
                    self->globals.windowses.game_window.field.cells[i + (j + 1) * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.y;
                    self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power = -1;


                    self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords.x;
                    self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords.y;
                }
            }
        }
    }
}

bool move_enabled(Main_obj* self) {
    bool buff = true;
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            if(buff) {
                int cell_id = j + i * self->globals.windowses.game_window.field.cols;
                buff = !self->globals.windowses.game_window.field.cells[cell_id]->merging.merging && self->globals.windowses.game_window.field.cells[cell_id]->coords.x == self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x && self->globals.windowses.game_window.field.cells[cell_id]->coords.y == self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y;
            }
        }
    }
    return buff;
}

void is_playable(Main_obj* self) {
    if(self->globals.is_playable) {
        bool buff = false;
        for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
            for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
                if(!buff) {
                    if(i == 0) {
                        if(j == 0) {
                            buff = check_box_element(self, 1, j, i) || check_box_element(self, self->globals.windowses.game_window.field.cols, j, i);
                        } else if(j > 0 && j < self->globals.windowses.game_window.field.cols - 1) {
                            buff = check_box_element(self, 1, j, i) || check_box_element(self, -1, j, i) || check_box_element(self, self->globals.windowses.game_window.field.cols, j, i);
                        } else {
                            buff = check_box_element(self, -1, j, i) || check_box_element(self, self->globals.windowses.game_window.field.cols, j, i);
                        }
                    } else if(i > 0 && i < self->globals.windowses.game_window.field.rows - 1) {
                        if(j == 0) {
                            buff = check_box_element(self, 1, j, i) || check_box_element(self, self->globals.windowses.game_window.field.cols, j, i) || check_box_element(self, -self->globals.windowses.game_window.field.cols, j, i);
                        } else if(j > 0 && j < self->globals.windowses.game_window.field.cols - 1) {
                            buff = check_box_element(self, 1, j, i) || check_box_element(self, -1, j, i) || check_box_element(self, self->globals.windowses.game_window.field.cols, j, i) || check_box_element(self, -self->globals.windowses.game_window.field.cols, j, i);
                        } else {
                            buff = check_box_element(self, -1, j, i) || check_box_element(self, self->globals.windowses.game_window.field.cols, j, i) || check_box_element(self, -self->globals.windowses.game_window.field.cols, j, i);
                        }
                    } else {
                        if(j == 0) {
                            buff = check_box_element(self, 1, j, i) || check_box_element(self, -self->globals.windowses.game_window.field.cols, j, i);
                        } else if(j > 0 && j < self->globals.windowses.game_window.field.cols - 1) {
                            buff = check_box_element(self, 1, j, i) || check_box_element(self, -1, j, i) || check_box_element(self, -self->globals.windowses.game_window.field.cols, j, i);
                        } else {
                            buff = check_box_element(self, -1, j, i) || check_box_element(self, -self->globals.windowses.game_window.field.cols, j, i);
                        }
                    }
                }
            }
        }
        self->globals.is_playable = buff;
    }
}

static bool check_box_element(Main_obj* self, const int adder, const int x, const int y) {
    bool buff = false;
    if(self->globals.windowses.game_window.field.cols > 1 || self->globals.windowses.game_window.field.rows > 1) {
        buff = self->globals.windowses.game_window.field.cells[y * self->globals.windowses.game_window.field.cols + x + adder]->power == self->globals.windowses.game_window.field.cells[y * self->globals.windowses.game_window.field.cols + x]->power || self->globals.windowses.game_window.field.cells[y * self->globals.windowses.game_window.field.cols + x + adder]->power == -1;
    }
    return buff;
}

bool is_win(Main_obj* self) {
    bool buff = false;
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            if(!buff) {
                int cell_id = j + i * self->globals.windowses.game_window.field.cols;
                buff = self->globals.windowses.game_window.field.cells[cell_id]->power >= 10;
            }
        }
    }
    return buff;
}

void add_cell(Main_obj* self) {
    int cell_id = rand() % (self->globals.windowses.game_window.field.rows * self->globals.windowses.game_window.field.cols);
    int rounds = 0;
    while(self->globals.windowses.game_window.field.cells[cell_id]->power > -1 && rounds < 2) {
        if(cell_id < self->globals.windowses.game_window.field.rows * self->globals.windowses.game_window.field.cols - 1) {
            cell_id++;
        } else {
            cell_id = 0;
            rounds++;
        }
    }
    if(rounds < 2) {
        self->globals.windowses.game_window.field.cells[cell_id]->power = rand() % 4 > 0 ? 0 : 1;
        self->globals.windowses.game_window.field.cells[cell_id]->merging.merge_animated = false;
    }
    self->globals.windowses.game_window.field.cell_added = true;
}

void move_cells(Main_obj* self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            int cell_id = j + i * self->globals.windowses.game_window.field.cols;
            if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x > self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
                move_cell_left(self, cell_id);
            } else if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.x < self->globals.windowses.game_window.field.cells[cell_id]->coords.x) {
                move_cell_right(self, cell_id);
            } else if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y > self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
                move_cell_up(self, cell_id);
            } else if(self->globals.windowses.game_window.field.cells[cell_id]->coords_from.y < self->globals.windowses.game_window.field.cells[cell_id]->coords.y) {
                move_cell_down(self, cell_id);
            }
        }
    }
}

void switch_direction(Main_obj* self, SDL_Event e){
    switch (e.key.keysym.sym)
    {
    case SDLK_LEFT:
        move_left(self);
        merge_left(self);
        move_left(self);
        set_half_distance_left(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_UP:
        move_up(self);
        merge_up(self);
        move_up(self);
        set_half_distance_up(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_RIGHT:
        move_right(self);
        merge_right(self);
        move_right(self);
        set_half_distance_right(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_DOWN:
        move_down(self);
        merge_down(self);
        move_down(self);
        set_half_distance_down(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_a:
        move_left(self);
        merge_left(self);
        move_left(self);
        set_half_distance_left(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_w:
        move_up(self);
        merge_up(self);
        move_up(self);
        set_half_distance_up(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_d:
        move_right(self);
        merge_right(self);
        move_right(self);
        set_half_distance_right(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    case SDLK_s:
        move_down(self);
        merge_down(self);
        move_down(self);
        set_half_distance_down(self);
        self->globals.windowses.game_window.field.cell_added = false;
    break;
    default:
    break;
    }
}

static void move_left(Main_obj *self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.cols; j++) {
            bool can = true;
            if(j > 0) {
                if(self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power > -1){
                    int id_k = j;
                    for(int k = j; k > 0; k--) {
                        if(self->globals.windowses.game_window.field.cells[k - 1 + i * self->globals.windowses.game_window.field.cols]->power < 0 && can) {
                            id_k--;
                        } else if(can) {
                            can = false;
                        }
                    }
                    if(id_k != j){
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->power = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->merging.merging = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merging;
                        self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merging = false;
                        self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power = -1;

                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merge_tile.x;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merge_tile.y;
                    }
                }
            }
        }
    }

}

static void move_right(Main_obj *self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.rows; i++) {
        for(int j = self->globals.windowses.game_window.field.cols - 1; j >= 0; j--) {
            bool can = true;
            if(j < self->globals.windowses.game_window.field.cols - 1) {
                if(self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power > -1){
                    int id_k = j;
                    for(int k = j; k < self->globals.windowses.game_window.field.cols - 1; k++) {
                        if(self->globals.windowses.game_window.field.cells[k + 1 + i * self->globals.windowses.game_window.field.cols]->power < 0 && can) {
                            id_k++;
                        } else if(can) {
                            can = false;
                        }
                    }
                    if(id_k != j){
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->power = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power;
                        self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->power = -1;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->merging.merging = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merging;
                        self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merging.merging = false;

                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.x;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->coords_from.y;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merge_tile.x;
                        self->globals.windowses.game_window.field.cells[id_k + i * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[j + i * self->globals.windowses.game_window.field.cols]->merge_tile.y;
                    }
                }
            }
        }
    }
}

static void move_up(Main_obj *self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.cols; i++) {
        for(int j = 0; j < self->globals.windowses.game_window.field.rows; j++) {
            bool can = true;
            if(j > 0) {
                if(self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power > -1){
                    int id_k = j;
                    for(int k = j; k > 0; k--) {
                        if(self->globals.windowses.game_window.field.cells[i + (k - 1) * self->globals.windowses.game_window.field.cols]->power < 0 && can) {
                            id_k--;
                        } else if(can) {
                            can = false;
                        }
                    }
                    if(id_k != j){
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->power = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power;
                        self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power = -1;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->merging.merging = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merging.merging;
                        self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merging.merging = false;

                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.x;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.y;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merge_tile.x;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merge_tile.y;
                    }
                }
            }
        }
    }
}

static void move_down(Main_obj *self) {
    for(int i = 0; i < self->globals.windowses.game_window.field.cols; i++) {
        for(int j = self->globals.windowses.game_window.field.rows - 1; j >= 0; j--) {
            bool can = true;
            if(j < self->globals.windowses.game_window.field.rows - 1) {
                if(self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power > -1){
                    int id_k = j;
                    for(int k = j; k < self->globals.windowses.game_window.field.rows - 1; k++) {
                        if(self->globals.windowses.game_window.field.cells[i + (k + 1) * self->globals.windowses.game_window.field.cols]->power < 0 && can) {
                            id_k++;
                        } else if(can) {
                            can = false;
                        }
                    }
                    if(id_k != j){
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->power = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power;
                        self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->power = -1;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->merging.merging = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merging.merging;
                        self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merging.merging = false;

                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->coords_from.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.x;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->coords_from.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->coords_from.y;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->merge_tile.x = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merge_tile.x;
                        self->globals.windowses.game_window.field.cells[i + id_k * self->globals.windowses.game_window.field.cols]->merge_tile.y = self->globals.windowses.game_window.field.cells[i + j * self->globals.windowses.game_window.field.cols]->merge_tile.y;
                    }
                }
            }
        }
    }
}
