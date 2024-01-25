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

void Board::GenerateGarbage(uint8 rows)
{
	for (uint32 y = 0; y < rows; y++)
	{
		for (uint32 x = 0; x < Size.x; x++)
		{
			m_coins[x][Size.y - (1 + y)] = (CoinType)((rand() % 6) + 1);
		}
	}
}

void Board::Render(Shader* shader, const uint32& vao, const uint32& ebo)
{
	glm::mat4 baseWorldMatrix = glm::identity<glm::mat4>();
	baseWorldMatrix = glm::translate(baseWorldMatrix, glm::vec3(Position, 0.0f));

	for (uint32 y = 0; y < Size.y; y++)
	{
		for (uint32 x = 0; x < Size.x; x++)
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
}
