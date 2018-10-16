# Engine86

  This game framework was built in C++ and provides an easy way to render images, 2D geometry and text. It also provides an audio engine for tilemapping games, the main focus of this engine. I tried to give you control over the game loop, so you can make your application without any constraints and get the performance you wouldn't get from an ECS system.
  
  The engine uses OpenGL for graphics (Opengl 3.0 if you want multiple Engine86::Renderer*) but support for a custom CPU pixel engine is predicted. OpenAL is used for audio and GLFW for context creation. TrueType support is being added for the GCC compiler, due to problems with compiling the Freetype library.
  
  List of utilities
  
   - Support for loading text files handling BOM headers;
   - Random number Generator;
   - Timers and Stopwatches;
   - Hardware information;
   - Image loading;
   - Font loading;
    
  For more information and a usage guide please visit my website at: 
  https://gizego.github.io/Engine86/
 
