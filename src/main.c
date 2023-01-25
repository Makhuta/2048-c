#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#include "./headers/structures.h"
#include "./headers/init.h"
#include "./headers/free.h"
#include "./headers/windows_drawer.h"
#include "./headers/click_handler.h"
#include "./headers/file_handler.h"
#include "./headers/manipulation.h"

int main(int argc, char **argv) {
    Main_obj main_obj;


    if (SDL_Init(SDL_INIT_TIMER)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    main_init(&main_obj);

    TTF_Init();
    TTF_Font * sans = TTF_OpenFont("./src/fonts/ClearSans.ttf", 256);

        if(!sans) {
        fprintf(stderr, "TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_Quit();
        TTF_Quit();
        return 1;
    }

    SDL_Window * window = SDL_CreateWindow(main_obj.globals.name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* icon = IMG_Load("./src/images/icon.png");
    SDL_SetWindowIcon(window, icon);

    update_main_dimensions(&main_obj, window);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Event e;
    uint32_t timer_start = time(NULL);

    while (!main_obj.globals.quit) {
        Uint64 start_time = SDL_GetPerformanceCounter();
        SDL_RenderClear(renderer);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                main_obj.globals.quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_ESCAPE && main_obj.globals.selected_window == 0) {
                    main_obj.globals.quit = true;
                } else if(e.key.keysym.sym == SDLK_ESCAPE) {
                    if(main_obj.globals.selected_window == 1) {

                    }
                    main_obj.globals.selected_window = 0;
                }
                if(move_enabled(&main_obj)) {
                    switch_direction(&main_obj, e);
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                handle_click(&main_obj, &e);
            }
        }

        is_playable(&main_obj);
        draw_window(&main_obj, renderer, sans);
        move_cells(&main_obj);
        if(is_win(&main_obj) && !main_obj.setting.is_endless && main_obj.globals.selected_window == 1) {
            main_obj.globals.is_playable = false;
        }
        if(!main_obj.globals.windowses.game_window.field.cell_added && move_enabled(&main_obj)) {
            add_cell(&main_obj);
        }

        SDL_SetRenderDrawColor(renderer, main_obj.globals.themes[main_obj.setting.selected_theme]->background.r, main_obj.globals.themes[main_obj.setting.selected_theme]->background.g, main_obj.globals.themes[main_obj.setting.selected_theme]->background.b, main_obj.globals.themes[main_obj.setting.selected_theme]->background.a); // Nastaven� barvy
        SDL_RenderPresent(renderer);  // Prezentace kresl�tka


        Uint64 end_time = SDL_GetPerformanceCounter();
        float elapsed_time = (end_time - start_time) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

        if(floor(1.0f / main_obj.globals.fps * 1000 - elapsed_time) > 0.0f){
            SDL_Delay(floor(1.0 / main_obj.globals.fps * 1000 - elapsed_time));
        }

        uint32_t timer_end = time(NULL);
        if(main_obj.globals.selected_window == 1 && main_obj.globals.is_playable) {
            if(difftime(timer_end, timer_start) == 1.0) {
                main_obj.globals.windowses.game_window.time_spend += difftime(timer_end, timer_start);
                update_time(&main_obj);
                timer_start = time(NULL);
            }
        } else {
            timer_start = time(NULL);
        }
    }
    save_game(&main_obj);
    save_score(&main_obj);


    main_free(&main_obj);

    TTF_CloseFont(sans);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
