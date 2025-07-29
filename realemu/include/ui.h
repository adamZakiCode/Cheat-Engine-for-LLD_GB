#pragma once

#include <common.h>
#include <SDL2/SDL.h> 

static const int SCREEN_WIDTH = 1024;
static const int SCREEN_HEIGHT = 768;

void ui_init();
void ui_handle_events();
void ui_update();

// Handle cheat
void ui_draw_cheat_window();
void ui_handle_cheat_events(SDL_Event *e);
void ui_toggle_cheat_window();

void refresh_paused_screen();
