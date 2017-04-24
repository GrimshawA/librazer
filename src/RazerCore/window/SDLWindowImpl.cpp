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
    : Window()
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
            renderer = SDL_CreateRenderer(sd->window, -1, SDL_RENDERER_ACCELERATED);

			//Get window surface
            //sd->screenSurface = SDL_GetWindowSurface(sd->window);

			//Fill the surface white
            //SDL_FillRect(sd->screenSurface, NULL, SDL_MapRGB(sd->screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
            //SDL_UpdateWindowSurface(sd->window);

			//Wait two seconds
			//SDL_Delay(2000);
		}
	}
#endif
}

void SDLWindowImpl::poll() {
    SDL_Event e;
    //e is an SDL_Event variable we've declared before entering the main loop
    while (SDL_PollEvent(&e)){
        //If user closes the window
        if (e.type == SDL_QUIT){
            m_running = false;
        }
        //If user presses any key
        if (e.type == SDL_KEYDOWN){
            m_keys[e.key.keysym.sym] = true;
            RZLOG("key %d\n", e.key.keysym.sym);
        }
        if (e.type == SDL_KEYUP){
            m_keys[e.key.keysym.sym] = false;
            RZLOG("key up %d\n", e.key.keysym.sym);
        }
        //If user clicks the mouse
        if (e.type == SDL_MOUSEBUTTONDOWN){
            //quit = true;
        }
    }
}

void SDLWindowImpl::setSize(int width, int height) {
    SDLUserData* sd = (SDLUserData*)data;
    SDL_SetWindowSize(sd->window, width, height);

    sd->screenSurface = SDL_GetWindowSurface(sd->window);

    SDL_FillRect(sd->screenSurface, NULL, SDL_MapRGB(sd->screenSurface->format, 0xFF, 0xFF, 0xFF));
}

void SDLWindowImpl::drawRect(int x, int y, int w, int h) {
    SDLUserData* sd = (SDLUserData*)data;

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void SDLWindowImpl::drawString(const std::string& str, int x, int y) {

}

void SDLWindowImpl::open() {
	RZLOG("WINDOW OPENING %x\n", this);
	m_running = true;
}

bool SDLWindowImpl::running() {
    return m_running;
}

void SDLWindowImpl::display() {
#ifdef RAZER_SDL
	SDLUserData* sd = (SDLUserData*)data;

	//SDL_FillRect(sd->screenSurface, NULL, SDL_MapRGB(sd->screenSurface->format, rand(), 0xFF, 0xFF));



	//Update the surface
    //SDL_UpdateWindowSurface(sd->window);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
#endif
}

int32_t SDLWindowImpl::width() {
    SDLUserData* sd = (SDLUserData*)data;
    int w, h;
    SDL_GetWindowSize(sd->window, &w, &h);
    return w;
}

int32_t SDLWindowImpl::height() {
    SDLUserData* sd = (SDLUserData*)data;
    int w, h;
    SDL_GetWindowSize(sd->window, &w, &h);
    return h;
}


