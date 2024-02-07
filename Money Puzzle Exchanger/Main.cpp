#include "PCH.h"
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Shader.h"
#include "Camera.h"
#include "Board.h"
#include "Input.h"

float vertices[] = {
	// Positions        // texture coords
	 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
	 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // bottom left
	 0.0f, 1.0f, 0.0f, 0.0f, 1.0f    // top left
};
const unsigned int indices[] =
{
	0, 1, 3,
	1, 2, 3
};
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

const glm::uvec2 PREFERRED_SIZE = glm::uvec2(304.0f, 224.0f);
const glm::uvec2 WINDOW_SIZE = glm::uvec2(304.0f * 3, 224.0f * 3);
std::unordered_map<std::string, uint32> filePathToTexture;
Shader* spriteShader;

uint32 LoadTexture(const char* filepath)
{
	auto textureData = filePathToTexture.find(filepath);
	if (textureData == filePathToTexture.end())
	{
		int32 imageWidth, imageHeight, nrChannels;
		unsigned char* data = stbi_load(filepath, &imageWidth, &imageHeight, &nrChannels, 0);
		if (data)
		{
			uint32 texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			int32 RGBAInput = nrChannels == 3 ? GL_RGB : GL_RGBA;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, RGBAInput, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
			return texture;
		}
		else
		{
			printf("Failed to load texture\n");
			return UINT32_MAX;
		}
	}

	return textureData->second;
}

int main()
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif

	stbi_set_flip_vertically_on_load(true);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_FLOATING, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, "Money Puzzle Exchanger", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 2560 + 2560 / 2 - WINDOW_SIZE.x / 2, 1440 / 2 - WINDOW_SIZE.y / 2);

	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetCharCallback(window, Input::CharCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetCursorPosCallback(window, Input::MouseCursorPosCallback);   // Track mouse position changes
	glfwSetScrollCallback(window, Input::MouseScrollCallback);
	glfwSetCursorEnterCallback(window, Input::CursorEnterCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	Board board1, board2;

	board1.m_coinTypeToTextureId.emplace(CoinType::Coin_1, LoadTexture("data/textures/game/coin_1.png"));
	board1.m_coinTypeToTextureId.emplace(CoinType::Coin_5, LoadTexture("data/textures/game/coin_5.png"));
	board1.m_coinTypeToTextureId.emplace(CoinType::Coin_10, LoadTexture("data/textures/game/coin_10.png"));
	board1.m_coinTypeToTextureId.emplace(CoinType::Coin_50, LoadTexture("data/textures/game/coin_50.png"));
	board1.m_coinTypeToTextureId.emplace(CoinType::Coin_100, LoadTexture("data/textures/game/coin_100.png"));
	board1.m_coinTypeToTextureId.emplace(CoinType::Coin_500, LoadTexture("data/textures/game/coin_500.png"));
	board1.m_characterTextureId = LoadTexture("data/textures/game/character.png");

	board2.m_coinTypeToTextureId.emplace(CoinType::Coin_1, LoadTexture("data/textures/game/coin_1.png"));
	board2.m_coinTypeToTextureId.emplace(CoinType::Coin_5, LoadTexture("data/textures/game/coin_5.png"));
	board2.m_coinTypeToTextureId.emplace(CoinType::Coin_10, LoadTexture("data/textures/game/coin_10.png"));
	board2.m_coinTypeToTextureId.emplace(CoinType::Coin_50, LoadTexture("data/textures/game/coin_50.png"));
	board2.m_coinTypeToTextureId.emplace(CoinType::Coin_100, LoadTexture("data/textures/game/coin_100.png"));
	board2.m_coinTypeToTextureId.emplace(CoinType::Coin_500, LoadTexture("data/textures/game/coin_500.png"));
	board2.m_characterTextureId = LoadTexture("data/textures/game/character.png");

	board1.Position = glm::ivec2(8, 16);
	board2.Position = glm::ivec2(PREFERRED_SIZE.x - (8 + board2.Size.x * 16), 16);
	
	board1.GenerateGarbage(3);
	board2.GenerateGarbage(5);

	uint32 backgroundTexture = LoadTexture("data/textures/game/background.png");
	glm::mat4 backgroundWorldMatrix = glm::identity<glm::mat4>();
	backgroundWorldMatrix = glm::translate(backgroundWorldMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	backgroundWorldMatrix = glm::scale(backgroundWorldMatrix, glm::vec3(PREFERRED_SIZE, 0.0f));

	Camera camera;
	camera.SetAspectRatio(PREFERRED_SIZE.x / (float)PREFERRED_SIZE.y);
	camera.SetOrthographicSize((float)PREFERRED_SIZE.y);
	camera.SetProjectionMode(Camera::ProjectionMode::Orthographic);

	spriteShader = NEW Shader("data/shaders/sprite.vert", "data/shaders/sprite.frag");
	spriteShader->Bind();
	spriteShader->SetUniform("view", glm::lookAt(glm::vec3(PREFERRED_SIZE, -1.0f) * 0.5f,  glm::vec3(PREFERRED_SIZE, 0.0f) * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f)));
	spriteShader->SetUniform("projection", camera.GetProjectionMatrix());

	glm::vec4 clearColor = glm::vec4(179, 69, 156, 255) / 255.0f;
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

	float deltaTime;
	float totalTime = (float)glfwGetTime();
	int frameNum = 0;
	float garbageTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		if (Input::IsKeyPressed(Key_A))
		{
			board1.MoveCharacterLeft();
		}
		else if (Input::IsKeyPressed(Key_D))
		{
			board1.MoveCharacterRight();
		}
		if (Input::IsKeyPressed(Key_S))
		{
			board1.GrabCoinsInColumn();
		}
		if (Input::IsKeyPressed(Key_W))
		{
			board1.ThrowCoinsInColumn();
		}

		if (Input::IsKeyPressed(Key_H))
		{
			board2.MoveCharacterLeft();
		}
		else if (Input::IsKeyPressed(Key_K))
		{
			board2.MoveCharacterRight();
		}
		if (Input::IsKeyPressed(Key_J))
		{
			board2.GrabCoinsInColumn();
		}
		if (Input::IsKeyPressed(Key_U))
		{
			board2.ThrowCoinsInColumn();
		}

		deltaTime = (float)glfwGetTime() - totalTime;
		totalTime += deltaTime;

		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		spriteShader->SetUniform("model", backgroundWorldMatrix);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		garbageTime += deltaTime;
		if (garbageTime >= 5.0f)
		{
			board1.GenerateGarbage(1);
			board2.GenerateGarbage(1);
			garbageTime = 0;
		}

		board1.Render(spriteShader, VAO, EBO);
		board2.Render(spriteShader, VAO, EBO);

		glfwSwapBuffers(window);
		Input::PollInputEvents();
	}

	delete spriteShader;

	for (auto textureMap : filePathToTexture)
	{
		glDeleteTextures(1, &textureMap.second);
	}
	glfwTerminate();

	return 0;
}