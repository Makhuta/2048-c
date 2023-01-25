static void draw_main_menu(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans);

static void draw_options(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans);

static void draw_game(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans);

static void draw_cells_placeholders(Main_obj* self, SDL_Renderer* renderer);

static void draw_cells(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans);

static void draw_cell_number(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans, const int cell_id, const bool is_merge_cell);

static void draw_game_over(Main_obj* self, SDL_Renderer* renderer, TTF_Font* sans);