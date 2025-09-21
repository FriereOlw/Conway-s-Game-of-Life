# Game of Life

An implementation of **Conway's Game of Life**, a zero-player cellular automaton devised by British mathematician John Horton Conway in 1970.  
It's a grid of cells that evolve through .

---

##  Rules
1. Any live cell with **two or three live neighbors** survives.
2. Any live cell with **fewer than two live neighbors** dies. -> Underpopulation
3. Any live cell with **more than three live neighbors** dies. -> Overpopulation
4. Any dead cell with **exactly three live neighbors** becomes a live cell. -> Reproduction

---

## 🚀 Features
- Supports an arbitrary grid size.
- Customizable starting patterns.
- Step through generations manually or run continuously.
- Terminal/console rendering using ASCII (█ for alive, space for dead).  
- Adjustable simulation speed.

---

## Installation & Usage
### Requirements
- A C++17 compatible compiler  
  *(g++ / clang++)*  

### Build & Run
```bash
# Compile
g++ -std=c++17 -O2 -o game_of_life main.cpp

# Run
./game_of_life
