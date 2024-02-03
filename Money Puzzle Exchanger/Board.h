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

	const glm::ivec2 Size = glm::ivec2(BoardWidth, BoardHeight);
	glm::ivec2 Position;

	std::unordered_map<CoinType, uint32> m_coinTypeToTextureId;
	uint32 m_characterTextureId;
	
public:
	Board();
	~Board();

	void MoveCharacterLeft(void);
	void MoveCharacterRight(void);
	void ThrowCoinsInColumn(void);
	void GrabCoinsInColumn(void);

	void GenerateGarbage(uint8 rows);
	void ClearBoard();
	void MoveCoinsDown();

	void Render(class Shader* shader, const uint32& vao, const uint32& ebo);

private:

	int m_currentColumn = 4;
	CoinType m_coins[BoardWidth][BoardHeight];

	CoinType m_holdingCoin = CoinType::Coin_None;
	int m_holdingCoinCount = 0;
};

