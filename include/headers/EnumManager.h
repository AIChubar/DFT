#pragma once

#include "cstddef"

using Level = std::size_t;
// Game.h headers
enum class GameState {PLAY, EXIT};

enum GroupLabels : std::size_t // Specifying underlying enum type
{
    groupMAP,
    groupPLAYERCHARS,
    groupENEMYCHARS,
    groupCHARACTERTILES,
    groupSHOP,
    groupPLAYERBOARD,
    groupENEMYBOARD,
    groupLABELS,
    groupBUTTONS
};

enum Labels : std::size_t // Specifying underlying enum type
{
    CHARACTERLABEL,
    MONEYLABEL
};

enum UserEvents : std::size_t // Specifying underlying enum type
{
    MODEBUTTON,
    STARTFIGHT,
    POWERUP,
    ADDMONEY
};

enum class GameMode
{
    EVIL,
    GOOD
};

enum class GameStage
{
    MODECHOOSING,
    BOARDMANAGEMENT,
    FIGHTING,
    GAMEOVER
};


// Map.h headers

enum TileStatus : std::size_t
{
    PLAYERBOARD,
    SHOP,
    ENEMYBOARD,
    BACKGROUND
};

// LevelManager.h headers

enum PowerLevels : std::size_t // Used only to reflect power
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    KING
};

enum Characters : std::size_t 
{
    goodPEASANT,
    goodARCHER,
    goodKNIGHT,
    goodMONK,
    goodKING,
    evilIMP,
    evilGOG,
    evilSUCCUB,
    evilDEMON,
    evilDEVIL
};

enum class Side 
{
    PLAYER,
    ENEMY
};
