set path= "C:\Program Files (x86)\mingw-w64\x86_64\mingw64\bin
%path%\g++.exe" "./main.cpp" "./include/glad/glad.c" -std=c++17 -O3 -static "./dependencies/x64/libglfw3.a" -static "./dependencies/x64/OpenAL32.lib" -lopengl32 -lgdi32 -o "./game64.exe"
pause