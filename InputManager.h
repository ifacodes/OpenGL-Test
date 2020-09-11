#pragma once

#pragma warning (push, 0)
#include <SDL.h>
#pragma warning (pop)
#include <map>
#include <memory>
#include <functional>
#include "Game_Command.h"


enum class Game_State {
	INGAME,
	MENU,
	CAMERA_MOVEMENT,
};

// Command Definitions consist of the bound input, name of the command type, and the 

struct Command_Definition
{
	Command_Definition(Game_Command cmnd, const char* n, SDL_Scancode sc, bool keyCheck, bool pr) : gamecommand(cmnd), name(n), scancode(sc), isKey(keyCheck), pressed(pr) {}

	Game_Command gamecommand;	// Game Command to Execute

	SDL_Scancode scancode;		// bound key
	const char* name;			// for display purposes
	bool pressed;
	bool isKey;
};


class InputManager
{
public:
	InputManager() : keyboardState(SDL_GetKeyboardState(NULL)) { }
	~InputManager() {}

	void configureInput(const char* configPath);
	void registerCommand(Game_Command new_command, std::function<void()> function);
	void bind(Game_State state, Command_Definition* definition);
	void handleInput(SDL_Event* e);
	void mouseInput(int posX, int posY, int scroll = 0);
	float xoffset = 0;
	float yoffset = 0;
	float scrolloffset = 0;
private:
	
	int mouseX = 0;
	int mouseY = 0;
	float lastX = 1920.0f / 2.0f;
	float lastY = 1080.0f / 2.0f;
	
	bool firstMouse = true;

	const Uint8* keyboardState = nullptr;
	std::multimap<std::pair<Game_State, bool>, std::unique_ptr<Command_Definition>> bindings;
	std::map<Game_Command, std::function<void()>> commands;

	// private methods

	bool matchDefinitions(Command_Definition def1, Command_Definition def2);


};