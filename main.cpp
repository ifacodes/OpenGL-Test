// SDL_Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Engine.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	Engine* engine = new Engine("SDL & OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	while (engine->running())
	{

		engine->event();	
		engine->update();
		engine->render();
	}

	engine->close();

	return 0;
}

