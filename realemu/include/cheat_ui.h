// cheat_ui.h
#ifndef CHEAT_UI_H
#define CHEAT_UI_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef enum {
    UI_STATE_IDLE,
    UI_STATE_SEARCH_INIT,
    UI_STATE_SEARCH_REFINE,
    UI_STATE_VIEW_MATCHES,
    UI_STATE_WRITE_VALUE,
    UI_STATE_FREEZE_VALUE
} ui_state_t;




// Initialize cheat UI with renderer
void cheat_ui_init(SDL_Renderer *renderer);

// Toggle visibility of cheat UI
void cheat_ui_toggle();

// Check if UI is currently visible
bool cheat_ui_is_visible();

// Handle SDL input events for cheat UI
void cheat_ui_handle_event(SDL_Event *e);

// Render the cheat UI elements
void cheat_ui_render();

#endif // CHEAT_UI_H
