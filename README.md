
#Maze Runner
######by Luke Southard and Nathan Southard
--------------------------
###Summary:


####Purpose: 
The goal of this project is to gain experience in graphics development and learn the intricacies of OpenGL.  Game engine architecture has been an interest of ours for quite some time.  We decided to put the concepts we've learned and read about to code.

####Controls:
W - move forward  
S - move backward  
A - strafe left  
D - strafe right  
SPACE - jump  
MOUSE - look around  
Q,ESC - exit game  

####Technology:
C++11  
OpenGL v4.4 - graphic rendering  APIs  
GLSL v4.0 (OpenGL Shading Language) - Used for writing shader programs in the modern graphics pipeline.  
SDL v2.0.3 (Simple DirectMEdia Layer) - hardware abstraction library used for input devices, audiot, etc.  
GLM (OpenGL Mathematics) - Math library  

####Features + Technical Details:
- Movevement 
  - Input handling
  - 3rd person camera  
- Maze generation  
  - Dijkstra's algorithm  
- Collision  
  - Silo'd AABB detection
  - Quadtree data structure for consideration
  - Preventative  
- Object models

####Future Enhancements:
- Trail to goal
- Success animation
- Textured floor
- New levels
- Use object for player
- Lighting
- Camera collision with walls

####Resources:
*OpenGL Programming Guide 8th edition* by Dave Shreiner, Graham Sellers, Josh Kessenich, and Bill Licea-Kane  
*Game Engine Architecture* by Jason Gregory
