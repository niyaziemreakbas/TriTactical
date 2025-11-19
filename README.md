**A 2D Turn-Based Strategy Game**

📜 **Overview**

Shape Conquest is a 2D turn-based strategy game that blends classic board game mechanics with the simple logic of "Rock, Paper, Scissors." Players command an army of geometric shapes (Triangles, Circles, and Squares) on a grid-based map. The objective is not to destroy the enemy, but to convert them to your side, leading to dynamic shifts in power and a strategic battle for board control.

🎯 **Game Objective**

The primary goal is to achieve board supremacy by strategically outmaneuvering your opponent. You win by converting enemy units into your own, growing your army and diminishing your opponent's until you dominate the map.

🎮 **Core Gameplay Mechanics**

**Unit Types & The Advantage Triangle**
   
The game is built around three core unit types, each with a distinct advantage over another, creating a balanced and strategic combat system.

->🔺 Triangle defeats ⚫ Circle

->⚫ Circle defeats 🟥 Square

->🟥 Square defeats 🔺 Triangle

**Extendable Features**

PathFinding for soldiers (currently attempts a direct linear path to the target, if cant move just staying.)

More Intelligent AI (currently have couple states.)

Refactor GameManager (I know it contains too many things for now. It can be separated to FightManager or StateManager)
