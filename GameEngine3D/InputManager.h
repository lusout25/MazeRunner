/***********************************************************
	Input Manager.  Handles inputs from keyboard and mouse events.
***********************************************************/

#pragma once
#include <unordered_map>

using namespace std;

namespace GameEngine3D
{

	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		//Keyboard Inputs
		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		//Mouse Inputs
		void updateMouseCoordinates(int x, int y);
		int getMouseXCoordinates(void);
		int getMouseYCoordinates(void);

	private:
		unordered_map<unsigned int, bool> _keyMap;
		int mouseX;
		int mouseY;

	};

}