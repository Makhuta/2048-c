#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    int x;
    int y;
} Coords;

typedef struct {
    int w;
    int h;
} Dimensions;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} RGBA;

typedef struct {
    SDL_Rect rect;
    RGBA color;
    int roundness;
} Rounded_Rectangle_filled;

typedef struct {
    SDL_Rect rect;
    RGBA outline_color;
    RGBA inside_color;
    int roundness;
    int thickness;
} Rounded_Rectangle_outline;

typedef struct {
    RGBA text_color;
    RGBA cell_color;
    Coords coords;
    Coords power_coords;
    Dimensions dimensions;
    int padding;
    int power_value;
} Number_prop;

typedef struct {
    char* name;
    RGBA background;
    RGBA foreground;
    RGBA text_color;
} Theme;

typedef struct {
    char* text;
    Coords coords;
    Dimensions dimensions;
} Text_element;

typedef struct {
    char* text;
    Rounded_Rectangle_outline position;
} Button_element;

typedef struct {
    bool merging;
    bool merge_animated;
    int merge_expansion_pixels;
    int merge_expansion_keep_frames;
    int move_speed;
    Coords move_half;
} Merging;

typedef struct {
    Coords coords;
    Coords coords_from;
    Coords merge_tile;
    Dimensions dimensions;
    Merging merging;
    int move_speed;
    Coords move_half;
    int power;
    int roundness;
} Cell;

typedef struct {
    int value;
    int move_for;
    bool draw;
    Coords coords;
    Dimensions dimensions;
    RGBA color;
} Point_increase;

typedef struct {
    int score;
    int highscore;
    int current_highest_merge;
    int total_highest_merge;
    int score_old;
    int highscore_old;
    int current_highest_merge_old;
    int total_highest_merge_old;
    Point_increase score_power_increase;
    Point_increase highscore_toppower_increase;
} Points;

typedef struct {
    bool cell_added;
    int rows;
    int cols;
    int percent_size;
    Cell** cells;
    Points points;
} Field;

typedef struct {
    Text_element name_text;
    Button_element start_btn;
    Button_element continue_btn;
    Button_element options_btn;
    Button_element exit_btn;
} Main_menu;

typedef struct {
    int padding_top;
    int time_spend;
    Text_element time_spend_text;
    Text_element score_text;
    Button_element back_to_main_menu_btn;
    Field field;
} Game_window;

typedef struct {
    Text_element game_over_text;
    Button_element try_again_btn;
    Button_element main_menu_btn;
} Game_over;

typedef struct {
    Text_element name_text;
    Button_element rows_up_btn;
    Text_element rows_text;
    Button_element rows_down_btn;
    Text_element x_text;
    Button_element cols_up_btn;
    Text_element cols_text;
    Button_element cols_down_btn;
    Button_element is_endless_btn;
    Button_element theme_left_btn;
    Text_element theme_text;
    Button_element theme_right_btn;
    Button_element back_btn;
} Options;

typedef struct {
    Main_menu main_menu;
    Game_window game_window;
    Game_over game_over;
    Options options;
} Windowses;

typedef struct {
    bool is_playable;
    bool quit;
    int fps;
    char* name;
    int selected_window;
    int number_of_themes;
    int window_percentual_ratio;
    Dimensions window_dimensions;
    Number_prop** number_props;
    Theme** themes;
    Windowses windowses;
} Global_variables;

typedef struct {
    int selected_theme;
    int rows;
    int cols;
    bool is_endless;
} Settings;

typedef struct {
    Global_variables globals;
    Settings setting;
} Main_obj;
