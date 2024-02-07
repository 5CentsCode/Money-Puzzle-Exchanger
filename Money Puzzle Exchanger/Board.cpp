#include "PCH.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "glm/gtx/hash.hpp"
#include "Board.h"
#include "glad/glad.h"
#include "Shader.h"

Board::Board()
{
	memset(&m_coins, CoinType::Coin_None, sizeof(CoinType) * Size.x * Size.y);
}

Board::~Board()
{
	m_coinTypeToTextureId.clear();
}

void Board::MoveCharacterLeft(void)
{
	m_currentColumn--;
	if (m_currentColumn < 1)
	{
		m_currentColumn = 1;
	}
}

void Board::MoveCharacterRight(void)
{
	m_currentColumn++;
	if (m_currentColumn > BoardWidth)
	{
		m_currentColumn = BoardWidth;
	}
}

void Board::ThrowCoinsInColumn(void)
{
	if (m_holdingCoin == Coin_None)
	{
		return;
	}

	for (int32 y = 0; y <= Size.y; y++)
	{
		CoinType coin = CoinType::Coin_None;
		if (y != Size.y)
		{
			coin = m_coins[m_currentColumn - 1][y];
		}

		if (coin != Coin_None ||
			y == Size.y)
		{
			for (int32 c = m_holdingCoinCount; c > 0; c--)
			{
				if (y - c < 0)
				{
					ClearBoard();
					break;
				}

				m_coins[m_currentColumn - 1][y - c] = m_holdingCoin;
			}

			std::unordered_set<glm::ivec2> coinsToCombine;
			if (CombineCoins(glm::ivec2(m_currentColumn - 1, y - m_holdingCoinCount), coinsToCombine) == true)
			{
				CoinType combineToType = CoinType::Coin_None;
				switch (m_holdingCoin)
				{
					case Coin_1:
						combineToType = CoinType::Coin_5;
						break;
					case Coin_5:
						combineToType = CoinType::Coin_10;
						break;
					case Coin_10:
						combineToType = CoinType::Coin_50;
						break;
					case Coin_50:
						combineToType = CoinType::Coin_100;
						break;
					case Coin_100:
						combineToType = CoinType::Coin_500;
						break;
					case Coin_500:
						combineToType = CoinType::Coin_None;
						break;
				}

				glm::ivec2 combineToPosition = *coinsToCombine.begin();
				for (auto coin : coinsToCombine)
				{
					if (coin.x * coin.y < combineToPosition.x * combineToPosition.y)
					{
						combineToPosition = coin;
					}
					m_coins[coin.x][coin.y] = CoinType::Coin_None;
				}

				m_coins[combineToPosition.x][combineToPosition.y] = combineToType;

				// Replace the coins that are being combined
			}

			m_holdingCoin = Coin_None;
			m_holdingCoinCount = 0;

			return;
		}
	}

	m_holdingCoin = Coin_None;
	m_holdingCoinCount = 0;
}

void Board::GrabCoinsInColumn(void)
{
	for (int32 y = 0; y < Size.y; y++)
	{
		CoinType coin = m_coins[m_currentColumn - 1][y];

		if (coin != Coin_None)
		{
			if (m_holdingCoin != Coin_None &&
				m_holdingCoin != coin)
			{
				return;
			}

			m_holdingCoin = coin;
			m_coins[m_currentColumn - 1][y] = Coin_None;
			m_holdingCoinCount++;
		}
	}
}

void Board::GenerateGarbage(uint8 rows)
{
	for (int32 y = 0; y < rows; y++)
	{
		MoveCoinsDown();

		for (int32 x = 0; x < Size.x; x++)
		{
			m_coins[x][Size.y - 1] = (CoinType)((rand() % 6) + 1);
		}
	}
}

void Board::ClearBoard()
{
	for (int32 y = 0; y < Size.y; y++)
	{
		for (int32 x = 0; x < Size.x; x++)
		{
			m_coins[x][y] = CoinType::Coin_None;
		}
	}

	GenerateGarbage(rand() % Size.y);
}

void Board::MoveCoinsDown()
{
	for (int32 y = 1; y < Size.y; y++)
	{
		for (int32 x = 0; x < Size.x; x++)
		{
			if (y == 1 &&
				m_coins[x][y - 1] != CoinType::Coin_None)
			{
				ClearBoard();
				return;
			}

			m_coins[x][y - 1] = m_coins[x][y];
			m_coins[x][y] = CoinType::Coin_None;
		}
	}
}

bool Board::CombineCoins(glm::ivec2 position, std::unordered_set<glm::ivec2>& coinsToCombine)
{
	int32 coinsRequiredToCombine = 0;

	CoinType coinToCombine = m_coins[position.x][position.y];

	switch (coinToCombine)
	{
		case Coin_1:
		case Coin_10:
		case Coin_100:
			coinsRequiredToCombine = 5;
			break;

		case Coin_5:
		case Coin_50:
		case Coin_500:
			coinsRequiredToCombine = 2;
			break;

		case Coin_None:
		case Coin_RU:
		case Coin_ER:
		default:
			return false;
	}

	CheckCoinRecursive(position, coinToCombine, coinsToCombine);
	return coinsToCombine.size() >= coinsRequiredToCombine;
}

void Board::CheckCoinRecursive(glm::ivec2 position, CoinType interestedType, std::unordered_set<glm::ivec2>& coinsToCombine)
{
	if (position.x < 0 ||
		position.y < 0 ||
		position.x >= Size.x ||
		position.y >= Size.y)
	{
		return;
	}

	CoinType coinToCombine = m_coins[position.x][position.y];
	auto visitedCoin = coinsToCombine.find(glm::ivec2(position.x, position.y));

	if (coinToCombine != interestedType ||
		visitedCoin != coinsToCombine.end())
	{
		return;
	}

	coinsToCombine.emplace(position);
	CheckCoinRecursive(position + glm::ivec2(0, 1), coinToCombine, coinsToCombine);
	CheckCoinRecursive(position + glm::ivec2(1, 0), coinToCombine, coinsToCombine);
	CheckCoinRecursive(position + glm::ivec2(-1, 0), coinToCombine, coinsToCombine);
	CheckCoinRecursive(position + glm::ivec2(0, -1), coinToCombine, coinsToCombine);
}

void Board::Render(Shader* shader, const uint32& vao, const uint32& ebo)
{
	glm::mat4 baseWorldMatrix = glm::identity<glm::mat4>();
	baseWorldMatrix = glm::translate(baseWorldMatrix, glm::vec3(Position, 0.0f));

	for (int32 y = 0; y < Size.y; y++)
	{
		for (int32 x = 0; x < Size.x; x++)
		{
			if (m_coins[x][y] == CoinType::Coin_None)
			{
				continue;
			}

			glm::mat4 coinWorldMatrix = glm::translate(baseWorldMatrix, glm::vec3(x * 16.0f, y * 16.0f, 0.0f));
			coinWorldMatrix = glm::scale(coinWorldMatrix, glm::vec3(16.0f, 16.0f, 0.0f));

			glBindTexture(GL_TEXTURE_2D, m_coinTypeToTextureId[m_coins[x][y]]);
			shader->SetUniform("model", coinWorldMatrix);
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	glm::mat4 characterWorldMatrix = glm::translate(baseWorldMatrix, glm::vec3((m_currentColumn - 1) * 16.0f - 8.0f, -8.0f, 0.0f));
	characterWorldMatrix = glm::scale(characterWorldMatrix, glm::vec3(32.0f, 32.0f, 0.0f));

	glBindTexture(GL_TEXTURE_2D, m_characterTextureId);
	shader->SetUniform("model", characterWorldMatrix);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
