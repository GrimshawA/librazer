#include <RazerCore/window/SDLWindowImpl.h>
#include <Logger.h>

#ifdef RAZER_SDL
#include <SDL.h>

struct SDLUserData
{
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
};

#endif

SDLWindowImpl::SDLWindowImpl()
{
#ifdef RAZER_SDL
	RZLOG("WINDOW INSTANCED\n");

	m_running = false;

	SDLUserData* sd = new SDLUserData;
	data = sd;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		RZLOG("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		sd->window = SDL_CreateWindow("Razer Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 300, SDL_WINDOW_SHOWN);
		if (sd->window == NULL)
		{
			RZLOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			sd->screenSurface = SDL_GetWindowSurface(sd->window);

			//Fill the surface white
			SDL_FillRect(sd->screenSurface, NULL, SDL_MapRGB(sd->screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(sd->window);

			//Wait two seconds
			//SDL_Delay(2000);
		}
	}
#endif
}


void Window::open()
{
	RZLOG("WINDOW OPENING %x\n", this);
	m_running = true;
}

bool Window::running()
{
	RZLOG("WINDOW RUNNING %x\n", this);
	return m_running;
}

void Window::display()
{
#ifdef RAZER_SDL
	SDLUserData* sd = (SDLUserData*)data;

	SDL_Event e;
	//e is an SDL_Event variable we've declared before entering the main loop
	while (SDL_PollEvent(&e)){
		//If user closes the window
		if (e.type == SDL_QUIT){
			m_running = false;
		}
		//If user presses any key
		if (e.type == SDL_KEYDOWN){
			//quit = true;
		}
		//If user clicks the mouse
		if (e.type == SDL_MOUSEBUTTONDOWN){
			//quit = true;
		}
	}

	//SDL_FillRect(sd->screenSurface, NULL, SDL_MapRGB(sd->screenSurface->format, rand(), 0xFF, 0xFF));

	//Update the surface
	SDL_UpdateWindowSurface(sd->window);
#endif
}

