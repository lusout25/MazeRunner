#include "InputManager.h"

namespace GameEngine3D
{

	InputManager::InputManager()
	{
	}


	InputManager::~InputManager()
	{
	}

	/***********************************************************
		Map pressed keyboard key
	***********************************************************/
	void InputManager::pressKey(unsigned int keyID)
	{
		_keyMap[keyID] = true;
	}
	
	/***********************************************************
		Map unpressed keyboard key
	***********************************************************/
	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;
	}
	
	/***********************************************************
		Check if key is pressed
	***********************************************************/
	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);

		if (it != _keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}
	}

	/***********************************************************
		Update mouse coordinate for lookAtDirection
	***********************************************************/
	void InputManager::updateMouseCoordinates(int x, int y)
	{
		mouseX = x;
		mouseY = y;
	}

	/***********************************************************
		Get mouse coordinates for x-direction
	***********************************************************/
	int InputManager::getMouseXCoordinates(void)
	{
		return mouseX;
	}

	/***********************************************************
		Get mouse coordinates for y-direction
	***********************************************************/
	int InputManager::getMouseYCoordinates(void)
	{
		return mouseY;
	}

}