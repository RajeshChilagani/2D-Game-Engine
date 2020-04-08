2D Game Engine
The GameEngine contains following systems
# Memory Manager
 - This Game Engine uses a custom memory manager which includes Fixed Size Allocators and other Allocation and Core Memory Functions
# Containers
 - Custom BitArray class
 - SmartPointer and WeakPtrs for managing creation and deletion of Objects
#Job System
 - Job System which helps in creating jobs that can be executed in parallel for better performance
# Input System
 - Input System manages the input from standard input(currently only keyboard is supported)
# Rendering System
 - Importing dds images and rendering using the graphics library(Glib) Only Direct3d currently
# Physics System
- A Global physics system that handles the physics in game engine
# Collision System
- Collision System handles the collision between all collidibles in the game using SAT(Seperating Axis Thereom)
# ECS
 - ECS for creating and managing gameobjects 

