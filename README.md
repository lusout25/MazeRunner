
#Maze Runner
######by Luke Southard and Nathan Southard
--------------------------
###Summary:
Standard maze game. Can you reach the end?

####Purpose: 
The goal of this project is to gain experience in graphics development and learn the intricacies of OpenGL.  Game engine architecture has been an interest of ours for quite some time.  We implemented a game engine from scratch to learn about the foundation of games.  We also sought out to implement a couple advanced algorithms and data structures. 

####Controls:
W - move forward  
S - move backward  
A - strafe left  
D - strafe right
V - show solution trail
SPACE - jump  
MOUSE - look around  
Q,ESC - exit game  

####Technology:
**C++11**  
**OpenGL v4.4** - graphic rendering  APIs  
**GLSL v4.0** (OpenGL Shading Language) - Used for writing shader programs in the modern graphics pipeline.  
**SDL v2.0.3** (Simple DirectMedia Layer) - hardware abstraction library used for input devices, audiot, etc.  
**GLM** (OpenGL Mathematics) - Math library  

####Features + Technical Details:
- **Movement**  
  Keyboard and mouse events are recieved with SDL and converted into player movements.   
- **Camera**  
  The game is played through a 3rd person follow camera. Model-View-Projection matrices are used to render the world in this perspective to bring the maze to life.
- **HUD**  
A 2D camera is used to show data on the HUD.  An orthographic-projection matrix is used to static graphics to the screen.  The HUD will always display in the same spot, independent of the player's location and rotation.
- **Maze generation**  
The maze is randomly generated using Prim's algorithm.  The entire maze is rendered in a single draw call.
- **Solution Trail**  
  A golden line leads the player to the goal when they are in need of help. The trail uses Dijkstra's algorithm to find the shortest path from the player to the goal.
- **Collision**  
  AABB bounding box structures are used for collision detection.  A quad-tree data structure is used to store the walls according to their location.  On movement, the player checks collision with a maximum of ten objects in that zone.
- **Shaders**  
  Shader programs our used in the graphics-pipeline to finalize data for the vertices (such as color, location, texture). Shaders are compiled and linked to be used on the GPU.
- **Texture**  
  Textures are used for flooring.  They are handled within the shader.  
- **Object models**  
  Object model files can be loaded with the game engine.

####Future Enhancements:
- Success animation
- New levels
- Use object for player
- Lighting
- Camera collision with walls
- Continuous collision
- HUD update (show more interesting information such as life bar, speed, etc).

####Resources:
*OpenGL Programming Guide 8th edition* by Dave Shreiner, Graham Sellers, Josh Kessenich, and Bill Licea-Kane  
*Game Engine Architecture* by Jason Gregory
