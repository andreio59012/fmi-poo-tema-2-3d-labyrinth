# Labyrinth

"Labyrinth" is a simple terminal-based game written in C++ where the player must navigate a maze, collect 10 keys, and escape through the locked exit door.

The renderer is a basic "raycaster", inspired by classic games such as Wolfenstein 3D.
For each column of the terminal screen, a ray is cast from the camera position into the maze (which is represented as a two-dimensional array of booleans).
The distance between the camera and the wall hit by the ray determines how tall that column should be.
Edges between surfaces are detected by comparing adjacent ray directions and are drawn with the corresponding ASCII characters.
Collectibles and the exit door are rendered as billboards (sprites that always face the camera), also using ASCII characters.

The project is built around an entity-component system, inspired by the way the Unity Game Engine works.
A Scene owns a list of Entity objects, and each Entity owns a list of Component objects.
The Component base class defines the interface for all game logic and rendering, using a virtual interface pattern.
Every component also implements clone() for correct deep copying of entities.

The component types are:
- PositionComponent and RotationComponent for transform data;
- LevelComponent for the tile map and raycasting;
- BillboardComponent for sprite rendering;
- PlayerComponent for input and camera control;
- CollectibleComponent for pickup logic;
- CollectibleCounterComponent for tracking collected keys;
- ExitDoorComoponent for win detection.

Error handling is based on a hierarchy with the base class "LabyrinthException" and specific subclasses for various types of errors (InvalidLevelException, InvalidSpriteException, OutOfBoundsExceptions).
These are thrown in constructors and caught in main.cpp.

Rendering state (RenderInfo) stores the depth buffer, the ASCII character buffer, and camera parameters. The depth buffer is used to cut billboards hidden behind walls.

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resurse

- https://www.w3schools.com/cpp/
- https://www.geeksforgeeks.org/cpp/
- https://github.com/mcmarius/oop-template
- https://medium.com/@togunchan/building-a-simple-3d-graphics-engine-in-c-on-macos-a07b81387d9f