#include "include/GizegoEngine.h"

GizegoEngine::Window* window;
GizegoEngine::Renderer* renderer;
GizegoEngine::ParticleInstanciator* instantiator;
GizegoEngine::ParticleCore* core;
GizegoEngine::Sprite* sprite;

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

void UpdateParticles(unsigned long long ellapsed, GizegoEngine::vec2* pos, GizegoEngine::vec2 initpos, GizegoEngine::vecf* scale, GizegoEngine::Color* color)
{
	pos->x = 10 * sin(ellapsed / 50.0f) + initpos.x;
	pos->y = initpos.y + (ellapsed / 8);
	*color = GizegoEngine::Color(255, 255, 255);
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

	instantiator = new GizegoEngine::ParticleInstanciator();
	core = new GizegoEngine::ParticleCore(10, 10);
	instantiator->InstantiateParticle(core, UpdateParticles, 5000, GizegoEngine::vec2(100, 200));
	instantiator->InstantiateParticle(core, UpdateParticles, 5000, GizegoEngine::vec2(150, 200));
	instantiator->InstantiateParticle(core, UpdateParticles, 5000, GizegoEngine::vec2(200, 200));
	unsigned int fps = 0, lfps = 0;
	unsigned long long time = GizegoEngine::Time::TimeInMilliseconds();
	while (window->IsOpen())
	{		
		window->Update();	
		instantiator->Update();
		instantiator->Render();

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
	delete instantiator;
	delete core;
	GizegoEngine::CloseEngine();
}