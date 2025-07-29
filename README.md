# LLD Game Boy Emulator with Cheat Engine

A lightweight, low-level Game Boy emulator written in C with SDL2 for rendering and input, featuring an integrated cheat engine and graphical cheat UI.

---

## Features

- Accurate emulation of the Game Boy CPU (LR35902), memory, and PPU  
- Real-time graphical output using SDL2  
- Gamepad input support  
- Built-in cheat engine to search, refine, write, and freeze memory values  
- Interactive cheat UI with keyboard-driven controls  
- Cheat search supports initial value scans and refined searches  
- Ability to freeze memory addresses to specific values (e.g., infinite lives or scores)  
- Displays matching memory addresses and values in the UI  

---

## Getting Started

### Prerequisites

- C compiler (gcc/clang)  
- SDL2 and SDL2_ttf development libraries installed  
- [NotoSansMono](https://www.google.com/get/noto/#mono) or other monospace font for cheat UI  



##Controls

-Z: Button B

- X: Button A

- Enter: Start

- Tab: Select

-  Arrow keys: Directional pad

-  P: Pause/Unpause emulator

-  F1: Toggle Cheat UI

- F2: Start New Cheat Search

- F3: Refine Cheat Search

- F4: View Matches (prints to terminal)

-  F5: Write Value to Address

-  F6: Freeze Value at Address

  Enter (within Cheat UI): Confirm current action/input

## Cheat Engine Usage

- Press F1 to open the cheat UI window.

-  Use F2 to start a new search by entering the initial value to find.

-  Use F3 to refine the search with updated values after the game state changes.

-  Use F4 to view all current matches (shown in terminal).

-  Use F5 to write a value directly to a memory address (format: address value).

-  Use F6 to freeze a memory address to a value (format: address value).

-  Press Enter to confirm input or execute commands.

-  Press F1 again to close the cheat UI.
