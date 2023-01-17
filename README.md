# IN204-ProjetTetris

Pour compiler, utiliser C++20 et link gtk :

    - (deprecated) clang++ -std=c++20 TetrisElements.cpp main.cpp `pkg-config --cflags --libs gtk+-3.0` && ./a.out
    - cmake . && cmake --build . && ./Tetris


Crédit : 

    - Tetris game logic : https://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/
    - CMake files with GTK : https://gist.github.com/fracek/3323924#file-cmakelists-txt-L21

    - Creation de la fentre GTK (Muchi): https://stackoverflow.com/questions/1706550/gtk-modifying-background-color-of-gtkbutton
    
