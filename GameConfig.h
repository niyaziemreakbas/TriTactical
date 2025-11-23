#pragma once

enum class AppState {
    MainMenu,
    GameSetup,
    Gameplay,
    GameOver
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
    BackToMenu,
    IncHuman, DecHuman,
    IncAI, DecAI,
    ReturnToMain,
    RestartGame
};