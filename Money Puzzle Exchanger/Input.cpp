#include "PCH.h"
#include "Input.h"
#include <GLFW/glfw3.h>

Input::KeyboardState Input::Keyboard;
Input::MouseState Input::Mouse;
Input::TouchState Input::Touch;
Input::GamepadState Input::Gamepad;

bool Input::IsKeyPressed(KeyCode key)
{
	return Keyboard.PreviousKeyState[key] == InputState::Release &&
		Keyboard.CurrentKeyState[key] > InputState::Release;
}

bool Input::IsKeyPressRepeat(KeyCode key)
{
	return Keyboard.CurrentKeyState[key] == InputState::Repeat;
}

bool Input::IsKeyDown(KeyCode key)
{
	return Keyboard.CurrentKeyState[key] >= InputState::Press;
}

bool Input::IsKeyReleased(KeyCode key)
{
	return Keyboard.CurrentKeyState[key] > InputState::Release &&
		Keyboard.CurrentKeyState[key] == InputState::Release;
}

bool Input::IsKeyUp(KeyCode key)
{
	return Keyboard.CurrentKeyState[key] >= InputState::Release;
}

KeyCode Input::GetKeyPressed(void)
{
	KeyCode key = Keyboard.KeyPressQueue.front();
	Keyboard.KeyPressQueue.pop();
	return key;
}

KeyCode Input::GetCharPressed(void)
{
	KeyCode key = Keyboard.CharPressQueue.front();
	Keyboard.CharPressQueue.pop();
	return key;
}

void Input::PollInputEvents(void)
{
	Keyboard.KeyPressQueue = {};
	Keyboard.CharPressQueue = {};

	Gamepad.LastButtonPressed = 0;
	Gamepad.AxisCount = 0;

	memcpy(&Keyboard.PreviousKeyState, &Keyboard.CurrentKeyState, sizeof(Keyboard.CurrentKeyState));
	memcpy(&Mouse.PreviousButtonState, &Mouse.CurrentButtonState, sizeof(Mouse.CurrentButtonState));

	Mouse.PreviousWheelMove = Mouse.CurrentWheelMove;
	Mouse.CurrentWheelMove = glm::vec2(0.0f);

	Mouse.PreviousPosition = Mouse.CurrentPosition;

	glfwPollEvents();
}

void Input::KeyCallback(GLFWwindow* glfwWindow, int32 key, int32 scancode, int32 action, int32 mods)
{
	KeyCode keyCode =(KeyCode)key;
	if (keyCode == KeyCode::Key_None)
	{
		return;
	}

	Keyboard.CurrentKeyState[key] = action;

	if (keyCode == KeyCode::Key_CapsLock && mods & ModifierCode::Mod_CapsLock ||
		keyCode == KeyCode::Key_NumLock && mods & ModifierCode::Mod_NumLock)
	{
		Keyboard.CurrentKeyState[key] = 1;
	}

	Keyboard.KeyPressQueue.emplace(keyCode);
}

void Input::CharCallback(GLFWwindow* glfwWindow, uint32 key)
{
	KeyCode keyCode = (KeyCode)key;
	Keyboard.CharPressQueue.emplace(keyCode);
}

void Input::MouseButtonCallback(GLFWwindow* glfwWindow, int32 button, int32 action, int32 mods)
{
	Mouse.CurrentButtonState[button] = action;
}

void Input::MouseCursorPosCallback(GLFWwindow* glfwWindow, double x, double y)
{
	Mouse.CurrentPosition = glm::vec2((float)x, (float)y);
	Touch.Position = Mouse.CurrentPosition;
}

void Input::MouseScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset)
{
	Mouse.CurrentWheelMove = glm::vec2((float)xOffset, (float)yOffset);
}

void Input::CursorEnterCallback(GLFWwindow* glfwWindow, int32 enter)
{
	Mouse.OnScreen = enter;
}