#pragma once
#include <unordered_map>

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
		void updateMouseCoordinates(int x);
		int getMouseCoordinates(void);

	private:
		std::unordered_map<unsigned int, bool> _keyMap;
		int mouseX;

	};

}