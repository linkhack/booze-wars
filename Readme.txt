CGUE18 - Booze Wars
Patrick Link (11728332)
Carlos Rodriguez (1635311)

“Booze Wars” is a tower defense style player vs. enemy game. The player can build defenses to save innocent teenagers.
Features
Player vs. Enemy
Cel shaded characters and environment
Particle system
Procedural Textures
Gameplay
The player enters an empty part of a city. There are two modes. In the first, called God-mode, he can place pre-defined buildings and obstacles onto a regular grid. After he placed the buildings and starts the round, enemies start to spawn on one side of the map. The goal is that no enemies reach the other side of the map. We call this mode spectator-mode.
The enemies are different alcoholic beverages, beer having high life, low speed, wine having medium life and speed and cocktails having low life, but high speed.
The buildings represent different places where alcoholic beverages are consumed, like soccer clubs, discos and bars and have also different ressource and damage statistics. Moreover the player can also place obstacles that cut of certain paths for the enemy, but these can be destroyed.
Optional: Enable adding buildings or placing debuffs in spectator-mode (during runtime).
Controls
We use a control scheme inspired and used by many first person sandbox games. The player can freely fly around on the map.

W, A, S, D
Move camera to X and Y coordinates
R, F
Move camera to Z coordinates
right and left mouse click
Set or remove objects into/from the map
Enter
Run wave
Space
Pause
Numbers
Different Buildings
F3
Debug Info (Wireframe, etc)
ESC
Quit Game
Technical Details
Types of Objects in Game
Map-Static meshes
Enemies - Static meshes
Particles - 2D Billboard
Camera
The player has a own camera and he controls the camera with the controls mentioned above.
Illumination
God mode: During this mode the illumination is provided by a sun. The light is simple parallel lines. 
Spectator mode: During this mode the illumination is for one given by the night sky, thus “dark” parallel light rays and moreover the scene will be also illuminated by the buildings that the player placed. This has to be calculated during runtime.
Collision Detection
Collision detection will be done in 2D. Some obstacles can be jumped over by certain enemies, but this shouldn’t be a difficult extension. Collision detection will also be used to define the point where a building will be placed.
Effects
In-Game Lightmap Calculation
CPU-Particle System 
Procedural textures 
Cel Shading
Contours (edge detection)
Sketches

