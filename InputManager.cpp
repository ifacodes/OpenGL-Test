#include "InputManager.h"

void InputManager::configureInput(const char* configPath)
{
	// load the config and assign the commands
	
}

void InputManager::registerCommand(Game_Command new_command, std::function<void()> function)
{
	commands.insert({ new_command, function });
}

void InputManager::bind(Game_State state, Command_Definition* definition)
{
	if (definition)
		bindings.insert({ std::make_pair(state, definition->isKey), std::unique_ptr<Command_Definition>(definition) });
	else
		printf("command definition not created!!!!!\n");
}

void InputManager::handleInput(SDL_Event* e)
{
	// check commands under current gamestate
	auto state = std::make_pair(Game_State::INGAME, true);
	auto range = bindings.equal_range(state);
	std::multimap<std::pair<Game_State, bool>, std::unique_ptr<Command_Definition>>::iterator it;
	// iterate over each currently valid command
	for (it = range.first; it != range.second; ++it)
	{
		if (keyboardState[(*it).second->scancode])
		{
			commands[(*it).second->gamecommand]();
		}
	}
}

void InputManager::mouseInput(int posX, int posY, int scroll)
{
	if (firstMouse)
	{
		lastX = (float)posX;
		lastY = (float)posY;
		firstMouse = false;
	}

	xoffset = (float)posX - lastX;
	yoffset = lastY - (float)posY;

	lastX = (float)posX;
	lastY = (float)posY;

	auto state = std::make_pair(Game_State::INGAME, false);
	auto range = bindings.equal_range(state);
	std::multimap<std::pair<Game_State, bool>, std::unique_ptr<Command_Definition>>::iterator it;
	for (it = range.first; it != range.second; ++it)
	{
		if ((*it).second->gamecommand == Game_Command::MOUSE_MOVE)
		{
			commands[(*it).second->gamecommand]();
		}
		if ( (scroll < 0 || scroll > 0) && (*it).second->gamecommand == Game_Command::MOUSE_SCROLL )
		{
			scrolloffset = scroll;
			commands[(*it).second->gamecommand]();
		}
	}


}

bool InputManager::matchDefinitions(Command_Definition def1, Command_Definition def2)
{
	if (def1.scancode != def2.scancode)
	{
		return false;
	}
	return true;
}