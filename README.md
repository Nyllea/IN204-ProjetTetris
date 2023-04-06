# IN204-ProjetTetris

A faire :

    - Utiliser du CSS pour les menus
    - Améliorer le choix aléatoire des couleurs pour éviter d'avoir 3 fois la même
    - Resize le terrain de jeu quand on resize la fenêtre

Pour compiler, utiliser C++20 et link gtk :

    - (deprecated) clang++ -std=c++20 TetrisElements.cpp main.cpp `pkg-config --cflags --libs gtk+-3.0` && ./a.out
    - cmake . && cmake --build . && ./Tetris
    - Peut résoudre erreurs de type symbol lookup error : cmake . && cmake --build . && sudo ./Tetris

Pour pouvoir debug avec gdb apres :

    - cmake -DCMAKE_BUILD_TYPE=Debug . && cmake --build . && sudo gdb ./Tetris

Pour la librairie audio:

    - Installer API audio ASA : sudo apt-get install libasound-dev
    - Installer Boost : sudo apt-get install libboost-all-dev
    - Installer libsndfile : apt-get install libsndfile-dev

Controles :

    - Fleches droite et gauche pour deplacer la piece
    - a/z pour rotation a droite/gauche
    - q/s pour se déplacer dans le temps
    - Fleche bas pour descendre la piece plus vite
    - x pour drop instantanément

Crédit :

    - Tetris game logic : https://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/
    - Regles de rotation Tetris : https://strategywiki.org/wiki/Tetris/Rotation_systems
    - Bitset manipulation (TemplateRex): https://stackoverflow.com/questions/17857596/how-to-convert-a-range-subset-of-bits-in-a-c-bitset-to-a-number

    - CMake files with GTK : https://gist.github.com/fracek/3323924#file-cmakelists-txt-L21

    - Creation de la fenetre GTK (Muchi): https://stackoverflow.com/questions/1706550/gtk-modifying-background-color-of-gtkbutton
    - Choix aleatoires de la couleur et du type de piece (Bart Louwers): https://stackoverflow.com/questions/2999012/generating-random-enums
    - Ajout du support du clavier (PintoDoido): https://stackoverflow.com/questions/44098084/how-do-i-handle-keyboard-events-in-gtk3

    - Passage a Gtkmm : http://sdz.tdct.org/sdz/creer-des-interfaces-graphiques-en-c-avec-gtkmm.html
