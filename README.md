# IN204-ProjetTimeTetris

Pour compiler, utiliser C++20 et link gtk avec l'une des commandes suivantes :

    - cmake . && cmake --build . && ./Time_Tetris
    - Peut résoudre erreurs de type symbol lookup error : cmake . && cmake --build . && sudo ./Time_Tetris


Cela fonctionne comme un Tetris classique, à une exception près : on peut voyager dans le temps !
    Bien sûr tout n'est pas possible avec le voyage dans le temps, il faut faire gaffe aux GameOver liés aux paradoxes temporels, les règles pour les évitées sont les suivantes:
    -On peut placer une ou plusieurs pièces dans le futur, on peut les placer un cran dans le futur ou deux crans dans le futur
    -Lorsqu'on est dans le futur le temps s'écoule différemment, les pièces s'empilent mais on n'avance plus dans le temps (par exemple si je suis un cran dans le futur et que je place une pièce, les pièces qui sont deux crans dans le futur ne vont pas s'afficher).
    -Lorsqu'on revient dans le présent après avoir placé des pièces dans le futur c'est là qu'il faut faire attention: dans le présent, à chaque fois qu'on place une pièce on avance d'un cran dans le temps...
    -Il faut donc faire attention et se rappeler ou l'on a placé les pièces dans le futur, si on place une pièce dans le présent à un endroit où on a déjà placé une pièce dans le futur, c'est le GameOver.
    -Mais pour les plus habiles qui osent s'aventurer dans le futur, il y a des grosses récompenses à la clé en terme de points : on peut compléter bien plus de 4 ligne à la fois.


Controles :

    - Fleches droite et gauche pour deplacer la piece
    - Fleche du bas pour descendre la pièce plus rapidement
    - a/z pour rotation a droite/gauche
    - x pour poser instantanément la pièce
    - q/s pour se déplacer dans le passé/présent

Crédit :

    - Tetris game logic : https://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/
    - Regles de rotation Tetris : https://strategywiki.org/wiki/Tetris/Rotation_systems
    - Bitset manipulation (TemplateRex): https://stackoverflow.com/questions/17857596/how-to-convert-a-range-subset-of-bits-in-a-c-bitset-to-a-number

    - CMake files with GTK : https://gist.github.com/fracek/3323924#file-cmakelists-txt-L21

    - Creation de la fenetre GTK (Muchi): https://stackoverflow.com/questions/1706550/gtk-modifying-background-color-of-gtkbutton
    - Choix aleatoires de la couleur et du type de piece (Bart Louwers): https://stackoverflow.com/questions/2999012/generating-random-enums
    - Ajout du support du clavier (PintoDoido): https://stackoverflow.com/questions/44098084/how-do-i-handle-keyboard-events-in-gtk3

    - Passage a Gtkmm : http://sdz.tdct.org/sdz/creer-des-interfaces-graphiques-en-c-avec-gtkmm.html

    - Créer des boutons en CSS : https://www.w3schools.com/css/css3_buttons.asp
