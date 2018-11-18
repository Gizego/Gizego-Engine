set path= "C:\Program Files\mingw-w64\i686\mingw32\bin
%path%\g++.exe" "./main.cpp" "./include/glad/glad.c" -std=c++17 -O3 -static "./dependencies/x86/libglfw3.a" -static "./dependencies/x86/OpenAL32.lib" -lopengl32 -lgdi32 -o "./game32.exe"
pause