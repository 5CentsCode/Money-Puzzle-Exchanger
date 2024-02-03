#pragma once
#include <queue>
#include <glm/glm.hpp>
#include "KeyCodes.h"

class Input
{
public:

	static bool IsKeyPressed(KeyCode key);
	static bool IsKeyPressRepeat(KeyCode key);
	static bool IsKeyDown(KeyCode key);
	static bool IsKeyReleased(KeyCode key);
	static bool IsKeyUp(KeyCode key);
	static KeyCode GetKeyPressed(void);
	static KeyCode GetCharPressed(void);

public:

	static void PollInputEvents(void);

	static void KeyCallback(struct GLFWwindow* glfwWindow, int32 key, int32 scancode, int32 action, int32 mods);
	static void CharCallback(struct GLFWwindow* glfwWindow, uint32 key);
	static void MouseButtonCallback(struct GLFWwindow* glfwWindow, int32 button, int32 action, int32 mods);
	static void MouseCursorPosCallback(struct GLFWwindow* glfwWindow, double x, double y);
	static void MouseScrollCallback(struct GLFWwindow* glfwWindow, double xOffset, double yOffset);
	static void CursorEnterCallback(struct GLFWwindow* glfwWindow, int32 enter);

private:

	struct KeyboardState
	{
		int8 CurrentKeyState[512];
		int8 PreviousKeyState[512];

		std::queue<KeyCode> KeyPressQueue;
		std::queue<KeyCode> CharPressQueue;
	};

	struct MouseState
	{
		glm::vec2 Offset;
		glm::vec2 Scale;
		glm::vec2 CurrentPosition;
		glm::vec2 PreviousPosition;

		int8 Cursor;
		bool Hidden;
		bool OnScreen;

		int8 CurrentButtonState[8];
		int8 PreviousButtonState[8];

		glm::vec2 CurrentWheelMove;
		glm::vec2 PreviousWheelMove;
	};

	struct TouchState
	{
		int8 PointCount;
		int8 PointId[8];
		glm::vec2 Position;
		uint8 CurrentTouchState[8];
		uint8 PreviousTouchState[8];
	};

	struct GamepadState
	{
		int8 LastButtonPressed;
		int8 AxisCount;
		bool Ready[4];
		char Name[4][64];
		int8 CurrentButtonState[16];
		int8 PreviousButtonState[16];
		float AxisState[4][8];
	};

	static KeyboardState Keyboard;
	static MouseState Mouse;
	static TouchState Touch;
	static GamepadState Gamepad;
};