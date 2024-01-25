#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include "CoinType.h"

class Board
{
private:
	static const int8 BoardWidth = 7;
	static const int8 BoardHeight = 12;

public:

	const glm::uvec2 Size = glm::uvec2(BoardWidth, BoardHeight);
	glm::ivec2 Position;

	std::unordered_map<CoinType, uint32> m_coinTypeToTextureId;
	
public:
	Board();
	~Board();

	void GenerateGarbage(uint8 rows);

	void Render(class Shader* shader, const uint32& vao, const uint32& ebo);

private:

	int m_currentColumn = 4;
	CoinType m_coins[BoardWidth][BoardHeight];
};

