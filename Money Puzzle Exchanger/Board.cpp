#include "PCH.h"
#include <glm/gtx/matrix_decompose.hpp>
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
