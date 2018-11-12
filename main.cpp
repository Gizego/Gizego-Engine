#include "include/GizegoEngine.h"

GizegoEngine::Window* window;
GizegoEngine::Renderer* renderer;
GizegoEngine::Font* font;

void ExceptionHandler(std::wstring str, GizegoEngine::ExceptionGravity gravity)
{
#ifdef _WINDOWS_
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (gravity == GizegoEngine::ExceptionGravity::Error)
		SetConsoleTextAttribute(hConsole, 12);
	else
		SetConsoleTextAttribute(hConsole, 14);
#endif
	std::wcerr << str << std::endl;
#ifdef _WINDOWS_
	SetConsoleTextAttribute(hConsole, 15);
#endif
}

int main()
{
	GizegoEngine::InitializeEngine(ExceptionHandler);
	window = new GizegoEngine::Window(GizegoEngine::Size(500, 500), L"GizegoEngine Demo");
	
	renderer = window->Create(GizegoEngine::GLVersion(3, 0));
	if (renderer == NULL)
	{		
		std::wcin.get();
		return 0;
	}
	
	font = new GizegoEngine::Font(L"C:\\Windows\\Fonts\\arial.ttf", 32, 256);	
	
	unsigned int fps = 0, lfps = 0;
	unsigned long long time = GizegoEngine::Time::TimeInMilliseconds();
	while (window->IsOpen())
	{		
		window->Update();	
		
		renderer->Render(L"fps: " + std::to_wstring(lfps), font, GizegoEngine::vec2(0, 0));
		
		fps++;
		if (GizegoEngine::Time::TimeInMilliseconds() > time + 1000)
		{
			time = GizegoEngine::Time::TimeInMilliseconds();
			lfps = fps;
			fps = 0;
		}
		window->SwapBuffers();
	}

	delete window;
	delete font;
	GizegoEngine::CloseEngine();
}
