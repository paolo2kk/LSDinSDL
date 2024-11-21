#pragma once

#include "Module.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_rect.h"

#define NUM_MOUSE_BUTTONS 5

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{
public:

	Input();

	virtual ~Input();

	bool Awake();

	SDL_GameController* FindController();

	bool Start();

	bool PreUpdate();

	bool CleanUp();

	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	KeyState GetControllerButton(SDL_GameControllerButton button);

	Sint16 GetControllerAxis(SDL_GameControllerAxis axis);

	bool GetWindowEvent(EventWindow ev);

	void GetMousePosition(int& x, int& y);
	void GetMouseMotion(int& x, int& y);

private:
	bool windowEvents[WE_COUNT]; 
	KeyState* keyboard;          
	KeyState mouseButtons[NUM_MOUSE_BUTTONS]; 
	int mouseMotionX;
	int mouseMotionY;
	int mouseX;      
	int mouseY;      

	SDL_GameController* controller;                
	KeyState controllerButtons[SDL_CONTROLLER_BUTTON_MAX];
	Sint16 controllerAxes[SDL_CONTROLLER_AXIS_MAX];    
};
