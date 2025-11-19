#pragma once

enum class AppState {
    MainMenu,
    GameSetup,
    Gameplay
};

enum class GameMode {
    PvP,
    PvAI,
    AIvAI
};

// Buton týklamalarýnýn sonucunu döndürmek için
enum class MenuAction {
    None,
    SelectPvP,
    SelectPvAI,
    SelectAIvAI,
    StartGame,
    BackToMenu
};