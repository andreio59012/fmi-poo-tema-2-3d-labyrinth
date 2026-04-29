# Labyrinth

"Labyrinth" is a simple terminal-based game written in C++ where the player must navigate a maze, collect 10 keys, and escape through the exit door.

The renderer is a "raycaster", inspired by classic games such as Wolfenstein 3D.
For each column of the terminal screen, a ray is cast from the camera into the maze (which is represented as a two-dimensional array of booleans).
The distance between the camera and the wall hit by the ray is used to figure out how tall said column should be.
Adjacent ray directions are compared to detect edges between walls, so as to draw them with the corresponding ASCII characters.
Collectibles and the exit door are rendered as billboards (sprites that always face the camera), also using ASCII characters.

The project uses entity-component system, inspired by the way the Unity Game Engine works.
A Scene has a list of Entity objects, and an Entity has a list of Component objects.
The Component base class has virtual functions that run when the game starts, every frame while the game is running. and when the game is drawing to the terminal.

The component types are:
- PositionComponent and RotationComponent: stores transform data;
- LevelComponent: stores the tile map and does the raycasting/drawing;
- BillboardComponent: stores sprites and does the billboard sprite rendering;
- PlayerComponent: player input and camera logic;
- CollectibleComponent: pickup logic;
- CollectibleCounterComponent: tracks collected keys;
- ExitDoorComoponent: win detection;

Error handling is based on a hierarchy with the base class "LabyrinthException" and specific subclasses for various types of project-specific errors (InvalidLevelException, InvalidSpriteException, OutOfBoundsExceptions).
These are thrown in constructors and caught in main.cpp.

Rendering state (RenderInfo) stores the depth buffer, the ASCII character buffer, and camera parameters. The depth buffer is used to avoid rendering billboards hidden behind walls.

## License

The project is licensed under [AGPLv3](LICENSE).

The [template repository](https://github.com/mcmarius/oop-template) itself is licensed under [Unlicense](LICENSE.template).

## Resurse

- https://www.w3schools.com/cpp/
- https://www.geeksforgeeks.org/cpp/
- https://github.com/mcmarius/oop-template
- https://medium.com/@togunchan/building-a-simple-3d-graphics-engine-in-c-on-macos-a07b81387d9f