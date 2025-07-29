#ifndef CHEAT_ENGINE_H
#define CHEAT_ENGINE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t matches[0x10000]; // list of matching addresses
    int match_count;

    bool freeze_enabled;
    uint16_t freeze_address;
    uint8_t freeze_value;
} cheat_context_t;

void cheat_init();
void cheat_search_init(uint8_t value);
void cheat_search_refine(uint8_t value);
void cheat_print_matches();
void cheat_write(uint16_t address, uint8_t value);
void cheat_freeze_enable(uint16_t address, uint8_t value);
void cheat_freeze_disable();
void cheat_tick(); // call this every frame to freeze if enabled
int cheat_get_match_count();
int cheat_get_match(int i);
uint16_t cheat_get_match_address(int index);

cheat_context_t *cheat_get_context();

#endif // CHEAT_ENGINE_H
