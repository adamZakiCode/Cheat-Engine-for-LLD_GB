// Cheat_ui.c - Handle event for the the ui
#include "cheat_ui.h"
#include "cheat_engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <bus.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool ui_visible = false;
static SDL_Rect panel_rect = {5, 5, 790, 590};
static char input_value[16] = "";
static int input_len = 0;
static TTF_Font *font = NULL;
static SDL_Renderer *ui_renderer = NULL;

static ui_state_t ui_state = UI_STATE_IDLE;
static char input_buffer[32] = {0};
static int input_pos = 0;

void cheat_ui_init(SDL_Renderer *renderer) {
    ui_renderer = renderer;
    if (TTF_WasInit() == 0) {
        TTF_Init();
    }
    font = TTF_OpenFont("../NotoSansMono-Medium.ttf", 16);
    printf("Font Loaded..\n");
    if (!font) {
        printf("[CheatUI] Failed to load font: %s\n", TTF_GetError());
    }
}

void cheat_ui_toggle() {
    ui_visible = !ui_visible;
    printf("CHEAT UI TOGGLE: %d\n",ui_visible);

    if (ui_visible) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }
}

bool cheat_ui_is_visible() {
    return ui_visible;
}

void render_text(const char *text, int x, int y) {
    if (!font) return;
    if (!text || text[0] == '\0') return;

    SDL_Color white = {255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Solid(font, text, white);
    if (!surf) {
        printf("TTF_RenderText_Solid failed: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ui_renderer, surf);
    SDL_Rect dst = {x, y, surf->w, surf->h};
    SDL_RenderCopy(ui_renderer, tex, NULL, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void handle_ui_action() {
    cheat_context_t *ctx = cheat_get_context();

    switch (ui_state) {
        case UI_STATE_SEARCH_INIT: {
            uint8_t val = (uint8_t)atoi(input_buffer);
            cheat_search_init(val);
            break;
        }
        case UI_STATE_SEARCH_REFINE: {
            uint8_t val = (uint8_t)atoi(input_buffer);
            cheat_search_refine(val);
            break;
        }
        case UI_STATE_WRITE_VALUE: {
            uint16_t addr;
            uint8_t val;
            if (sscanf(input_buffer, "%hx %hhu", &addr, &val) == 2) {
                cheat_write(addr, val);
            }
            break;
        }
        case UI_STATE_FREEZE_VALUE: {
            uint16_t addr;
            uint8_t val;
            if (sscanf(input_buffer, "%hx %hhu", &addr, &val) == 2) {
                cheat_freeze_enable(addr, val);
            }
            break;
        }
        default: break;
    }
}


void cheat_ui_handle_event(SDL_Event *e) {
    if (!ui_visible) return;

    if (e->type == SDL_KEYDOWN) {
        SDL_Keycode key = e->key.keysym.sym;

        if (key >= SDLK_F2 && key <= SDLK_F6) {
        int option = key - SDLK_F1;  // F2=1, F3=2, ..., F6=5
        switch (option) {
            case 1: ui_state = UI_STATE_SEARCH_INIT; break;
            case 2: ui_state = UI_STATE_SEARCH_REFINE; break;
            case 3: ui_state = UI_STATE_VIEW_MATCHES; cheat_print_matches(); break;
            case 4: ui_state = UI_STATE_WRITE_VALUE; break;
            case 5: ui_state = UI_STATE_FREEZE_VALUE; break;
            default: break;
        }
        input_buffer[0] = '\0';
        input_pos = 0;
        } else if (key == SDLK_RETURN) {
            handle_ui_action();
            input_buffer[0] = '\0';
            input_pos = 0;
            if(ui_state == UI_STATE_SEARCH_INIT || ui_state == UI_STATE_SEARCH_REFINE ){
                ui_state = UI_STATE_VIEW_MATCHES;
            }
            else{
                ui_state = UI_STATE_IDLE;
            }
            
        } else if (key == SDLK_BACKSPACE && input_pos > 0) {
            input_buffer[--input_pos] = '\0';
        }
    } else if (e->type == SDL_TEXTINPUT) {
        if (input_pos < (int)(sizeof(input_buffer) - 1)) {
            input_buffer[input_pos++] = e->text.text[0];
            input_buffer[input_pos] = '\0';
        }
    }
}


void cheat_ui_render() {
    if (!ui_renderer) {
    printf("[CheatUI] Renderer is NULL!\n");
    return;
    }
    if (!ui_visible) {
        SDL_SetRenderDrawColor(ui_renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(ui_renderer, &panel_rect);

        SDL_SetRenderDrawColor(ui_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(ui_renderer, &panel_rect);

        render_text("Cheat Engine", panel_rect.x + 10, panel_rect.y + 10);
        return;
    }
    
    SDL_RenderClear(ui_renderer);

    SDL_SetRenderDrawColor(ui_renderer, 0, 0, 0, 10);
    SDL_RenderFillRect(ui_renderer, &panel_rect);

    SDL_SetRenderDrawColor(ui_renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(ui_renderer, &panel_rect);

    render_text("Cheat Engine", panel_rect.x + 200, panel_rect.y + 10);

    SDL_SetRenderDrawColor(ui_renderer, 0, 0, 0, 255);
    // SDL_RenderClear(ui_renderer);

    render_text("F1: Close UI", 10, 10);

    if(ui_state == UI_STATE_IDLE){
        render_text("1. New Search", 30, 40);
        render_text("2. Refine Search", 30, 70);
        render_text("3. View Matches", 30, 100);
        render_text("4. Write Value", 30, 130);
        render_text("5. Freeze Value", 30, 160);
        render_text("6. Unfreeze", 30, 190);
        render_text("Input:", 30, 230);
        render_text(input_buffer, 70, 230);
        
    }
    else{
        render_text("Mode: ", 30, 40);
        switch (ui_state) {
            case UI_STATE_SEARCH_INIT: {
                render_text("Search", 100, 40);
                break;
            }
            case UI_STATE_SEARCH_REFINE: {
                render_text("Refine ", 100, 40);
                break;
            }
            case UI_STATE_WRITE_VALUE: {
                render_text("Write ", 100, 40);
                break;
            }
            case UI_STATE_FREEZE_VALUE: {
                render_text("Freeze ", 100, 40);
                break;
            }
            case UI_STATE_VIEW_MATCHES: {
                render_text("Matches:", 100, 40);
                cheat_context_t *ctx = cheat_get_context();
                int max_display = 30; // show only first 10 matches
                char buffer[64];
                for (int i = 0; i < ctx->match_count && i < max_display; ++i) {
                    uint16_t addr = ctx->matches[i];
                    uint8_t val = bus_read(addr);
                    snprintf(buffer, sizeof(buffer), "0x%04X -> %d", addr, val);
                    render_text(buffer, 30, 100 + i * 20);
                }
                break;
            }
            default: break;
        }

        render_text("Input:", 30, 70);
        render_text(input_buffer, 100, 70);
    }
    
}


