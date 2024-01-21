#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

// TODO Move elseware.
// That way we can utilize this during compile time
// to make sure all of our shaders are valid.
#define MAX_UNIFORM_NAME_LENGTH 64
class Shader
{
public:
	Shader(std::string vertexFile, std::string fragmentFile);
	~Shader();

public:
	virtual void Bind() const;
	virtual void Unbind() const;
	virtual bool IsBound() const;

	virtual void SetUniform(const char* name, const int32& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const uint32& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const float& value, const int32& count = 1);

	virtual void SetUniform(const char* name, const glm::ivec2& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const glm::ivec3& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const glm::ivec4& value, const int32& count = 1);

	virtual void SetUniform(const char* name, const glm::uvec2& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const glm::uvec3& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const glm::uvec4& value, const int32& count = 1);

	virtual void SetUniform(const char* name, const glm::vec2& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const glm::vec3& value, const int32& count = 1);
	virtual void SetUniform(const char* name, const glm::vec4& value, const int32& count = 1);

	virtual void SetUniform(const char* name, const glm::mat2& value, const int32& count = 1, const bool transpose = false);
	virtual void SetUniform(const char* name, const glm::mat2x3& value, const int32& count = 1, const bool transpose = false);
	virtual void SetUniform(const char* name, const glm::mat2x4& value, const int32& count = 1, const bool transpose = false);

	virtual void SetUniform(const char* name, const glm::mat3& value, const int32& count = 1, const bool transpose = false);
	virtual void SetUniform(const char* name, const glm::mat3x2& value, const int32& count = 1, const bool transpose = false);
	virtual void SetUniform(const char* name, const glm::mat3x4& value, const int32& count = 1, const bool transpose = false);

	virtual void SetUniform(const char* name, const glm::mat4& value, const int32& count = 1, const bool transpose = false);
	virtual void SetUniform(const char* name, const glm::mat4x2& value, const int32& count = 1, const bool transpose = false);
	virtual void SetUniform(const char* name, const glm::mat4x3& value, const int32& count = 1, const bool transpose = false);

private:

	void PopulateUniformLocationMap();
	//void PopulateVertexAttributes();

	inline bool TryToFindUniform(const char* name, int32& uniformLocation) const;

private:
	uint32 m_id;

	std::unordered_map<uint64, int32> m_uniformLocationMap;
	//std::vector<VertexAttribute> m_vertexAttributes;
};