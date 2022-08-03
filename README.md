# DFT - Demon Fight Tactics
Single-player auto-battler game.

Project for Charles University course "Programming in C++".

# Game Description
Auto-battler is a relatively new genre in which players set up their own lineup of characters which combat automatically with other competitors. 
There are many nuances and variations among representatives of this genre.
However, such games usually consists of players board, enemies boards and a source of new characters.

# Game Flow
The game starts with choosing the side: Evil or Good (of course, these terma are generally relative). Each side consists of different set of characters with different parameters.

After that gameplay consists of two phases: board management and fighting. 

During the board management phase players are able to buy or sell characters, observe current characteristics of each character and buy power up which is an increasing of damage and HP of all player characters by 10%. After the player is done with board management phase a player can start fighting phase (at least one character needs to be placed on the board). 

This phase consists of rounds during which each character of the player team alternately attacks random character from the enemy team and after that enemies character do the same. While attacking a character deal a damage to the attacked and receive a damage from the attacked corresponding to the characteristic of the character. There are two abilities that a character can have: Ranged which makes attacker not recieve damage back and Cleave (currently not in the game) which makes attacker deal damage also to neighbour character (in this case damage back is recieved from the initially attacked character). This phase ends when each character of one of the teams is destroyed. 

If all the enemy characters are dead game proceed to the next level (this happens even if all player characters are also dead). On the new Board Management phase empty Shop slots and Enemy Board are filled with a randomly chosen characters from the pool. If enemy side wins this phase game is over. 

The goal of the game is to reach the highest level possible.

# Interface
There are three main parts of the interface: Player Board, Enemy Board, and Shop. There are also labels with an information about the current Level, Gold, and a place where the information about the character is shown. The character information consist of the Name, Current and Maximum Health, Damage, if character has and ability Cleave and Ranged, and Cost. 
![Capture](https://user-images.githubusercontent.com/61264363/182048085-4cebe9c2-12b0-4051-b896-7afaa60acafa.PNG)

# Controls
All the manipulation in the game is performed with a mouse. All the buttons on the screen can be clicked with a left mouth button. The character can be bought from the shop by dragging it to the empty slot. To sell the character it should be dragged from the Player Board to the Shop area (currently yellow). The information about the character can be shown by clicking on it with a right mouse button.

# Used soft
- IDE - Visual Studio Code
- Assets - Pyxel Edit
- C++ Compiler - gcc 8.1.0
- Runtime Environment - MinGW-w64 
- OS - Windows 10

# Used libraries
- SDL2 MinGW version
- SDL2_image
- SDL2_ttf
- Nlohmann JSON for Modern C++

# How to compile
The game can be builded on the Windows machine with installed MinGW-w64 for gcc 8.1.0. The version of the game with all the libraries can be found on the FullProject branch of this repository. To build the game you need to run the MakeFile with a command `mingw32-make -f MakeFile`.

# Architecture
The project is done using an Entity Component System.

![DTF](https://user-images.githubusercontent.com/61264363/182501022-ee38f4f6-3215-4ccd-bcb7-1747ef7a470b.png)


# To be improved
- Use Button Component for Character Labels 
- Create Event Manager class to store and control Events
- Improve mouse input handling
- Make unified Label Component
- Move more logic of the game to a Component system (Map, Levels, Enemy)
- Move fighting to another class and rework it (Make it calculates the fight first and then Animate it)
- Create Entity for a player that handles all the information (Characters, Money, Fight statistics)
- Move Character Frame logic to Tile Component (create specific Tiles that has different properties). Or rework Character Component to handle it.
- Add animations and create assets.
