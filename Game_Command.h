#pragma once

#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

// Declare the Game States that can occur. Might need to move to another file at some point??
enum class Game_Command {
	NO_COMMAND = 0,
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT, 
	MOUSE_MOVE,
	MOUSE_SCROLL, 
};

#endif