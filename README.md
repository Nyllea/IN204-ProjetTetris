# IN204-ProjetTimeTetris

This game works like classic Tetris, with one exception: time travel is possible !

## Game Rules

Of course, everything is not allowed with time travel, and one has to be careful of Game Over due to time paradoxes.

The rules to avoid them are as follows

- It's possible to place one or more pieces in the futur
- The pieces can be placed one or two steps into the futur
- When in the futur, time flows differently; The pieces are placed but time doesn't pass (for example, if a piece is placed one step into the futur, the pieces placed two steps into the futur won't appear)
- Be very careful after having placed pieces in the futur and coming back to the present: in the present, each time a piece is placed, time moves one step forward and all the pieces placed one step in the futur will appear at once...
- If a piece in the present is placed where a piece from the futur appears, it's Game Over. It is therefore important to be careful and remember exactly where each piece placed in the futur is
- However, using the futur allows for completing more than 4 lines at once and multiplies the score accordingly: time travel involves risks, but also rewards !

## Controls

- Left and right arrows to move the current piece in the desired direction
- Down arrow to lower the piece faster
- A/Z to rotate the piece left/right
- X to instantly place the piece
- Q/S to move backward/forward in time

## How to compile

To compile, use C++20 and link GTK with the following commands

    cmake . && cmake --build . && ./Time_Tetris

If there is an error of type symbol lookup, use

    cmake . && cmake --build . && sudo ./Time_Tetris

_Note: The terminal has to be in the main folder_

## Credits

This game was made by Paul Poirmeur and Walid Outtaleb

## Ressources used

Here are all the ressources used to help us realize this project

- [Tetris game logic](https://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/)
- [Tetris rotation rules](https://strategywiki.org/wiki/Tetris/Rotation_systems)
- [Bitset manipulation (TemplateRex)](https://stackoverflow.com/questions/17857596/how-to-convert-a-range-subset-of-bits-in-a-c-bitset-to-a-number)

- [CMake files with GTK](https://gist.github.com/fracek/3323924#file-cmakelists-txt-L21)

- [Creation of the GTK window (Michi)](https://stackoverflow.com/questions/1706550/gtk-modifying-background-color-of-gtkbutton)
- [Random choice for the color and type of pieces (Bart Louwers)](https://stackoverflow.com/questions/2999012/generating-random-enums)
- [Keyboard support with GTK (PintoDoido)](https://stackoverflow.com/questions/44098084/how-do-i-handle-keyboard-events-in-gtk3)

- [Use of Gtkmm](http://sdz.tdct.org/sdz/creer-des-interfaces-graphiques-en-c-avec-gtkmm.html)

- [Create buttons with CSS](https://www.w3schools.com/css/css3_buttons.asp)
