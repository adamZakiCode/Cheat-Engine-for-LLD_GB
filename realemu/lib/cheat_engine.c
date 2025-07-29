// Cheat.c - Handle the code for cheat engine
#include <cheat_engine.h>
#include <stdio.h>
#include <bus.h>  // for bus_read and bus_write

static cheat_context_t cheat;


cheat_context_t *cheat_get_context() {
    return &cheat;
}

void cheat_init() {
    cheat.match_count = 0;
    cheat.freeze_enabled = false;
    cheat.freeze_address = 0;
    cheat.freeze_value = 0;
}

void cheat_search_init(uint8_t value) {
    cheat.match_count = 0;

    for (uint32_t addr = 0; addr < 0x10000; addr++) {
        if (bus_read((uint16_t)addr) == value) {
            cheat.matches[cheat.match_count++] = (uint16_t)addr;
        }
    }

    printf("[Cheat] Initial search found %d matches for value %d\n", cheat.match_count, value);
}

void cheat_search_refine(uint8_t value) {
    int new_count = 0;

    for (int i = 0; i < cheat.match_count; i++) {
        uint16_t addr = cheat.matches[i];
        if (bus_read(addr) == value) {
            cheat.matches[new_count++] = addr;
        }
    }

    cheat.match_count = new_count;
    printf("[Cheat] Refined to %d matches for value %d\n", cheat.match_count, value);
}

void cheat_print_matches() {
    printf("[Cheat] Matches (%d):\n", cheat.match_count);
    for (int i = 0; i < cheat.match_count; i++) {
        uint16_t addr = cheat.matches[i];
        printf("  0x%04X -> %d\n", addr, bus_read(addr));
    }
}

void cheat_write(uint16_t address, uint8_t value) {
    printf("[Cheat] Writing value %d to 0x%04X\n", value, address);
    bus_write(address, value);
}

void cheat_freeze_enable(uint16_t address, uint8_t value) {
    printf("[Cheat] Freezing address 0x%04X to value %d\n", address, value);
    cheat.freeze_enabled = true;
    cheat.freeze_address = address;
    cheat.freeze_value = value;
}

void cheat_freeze_disable() {
    printf("[Cheat] Freeze disabled\n");
    cheat.freeze_enabled = false;
}

void cheat_tick() {
    if (cheat.freeze_enabled) {
        bus_write(cheat.freeze_address, cheat.freeze_value);
    }
}

int cheat_get_match(int i) {
    return cheat.matches[i];
}

int cheat_get_match_count() {
    return cheat.match_count;
}

uint16_t cheat_get_match_address(int index) {
    if (index < 0 || index >= cheat.match_count) return 0xFFFF;
    return cheat.matches[index];
}