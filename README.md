# IN204-ProjetTetris

Pour compiler, utiliser C++20 et link gtk :
    - (deprecated) clang++ -std=c++20 TetrisElements.cpp main.cpp `pkg-config --cflags --libs gtk+-3.0` && ./a.out
    - cmake . && cmake --build . && ./Tetris

Crédit : https://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/