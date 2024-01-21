#include "PCH.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

std::string ReadFile(const char* filepath)
{
	std::string buffer;
	std::ifstream fileStream;
	try
	{
		fileStream.open(filepath);
		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();
		fileStream.close();
		buffer = stringStream.str();
	}
	catch (std::ifstream::failure e)
	{
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
	}

	return buffer;
}

Shader::Shader(std::string vertexFile, std::string fragmentFile)
{
	std::string vertexCode = ReadFile(vertexFile.c_str());
	std::string fragmentCode = ReadFile(fragmentFile.c_str());
	const char* vCode = vertexCode.c_str();
	const char* fCode = fragmentCode.c_str();

	int success;
	char infoLog[512];

	uint32 vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::OpenGLShader::VERTEX::COMPILATION_FAILED\n");
		printf("\t%s\n", infoLog);
	};

	uint32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::OpenGLShader::FRAGMENT::COMPILATION_FAILED\n");
		printf("\t%s\n", infoLog);
	};

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex);
	glAttachShader(m_id, fragment);
	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_id, 512, NULL, infoLog);
		printf("ERROR::OpenGLShader::PROGRAM::LINKING_FAILED\n");
		printf("\t%s\n", infoLog);
	};

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	PopulateUniformLocationMap();
	//PopulateVertexAttributes();
}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::Bind() const
{
	glUseProgram(m_id);
}

void Shader::Unbind() const
{
#ifdef DEBUG
	if (!IsBound())
	{
		// TODO:
		// Warning, unbinding Vertex Array, when Vertex Array isn't active.
	}
#endif // DEBUG

	glUseProgram(0);
}

bool Shader::IsBound() const
{
	int32 activeProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &activeProgram);
	return activeProgram == m_id;
}

void Shader::PopulateUniformLocationMap()
{
	int attributeCount;
	glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &attributeCount);

	int uniformNameLength;
	int uniformSize;
	uint32 uniformType;
	char uniformName[MAX_UNIFORM_NAME_LENGTH];
	int uniformLocation;

	m_uniformLocationMap.reserve(attributeCount);

	for (int i = 0; i < attributeCount; ++i)
	{
		glGetActiveUniform(m_id, i, MAX_UNIFORM_NAME_LENGTH, &uniformNameLength, &uniformSize, &uniformType, uniformName);
		uniformLocation = glGetUniformLocation(m_id, uniformName);

		uint64 nameHash = std::hash<std::string>{}(uniformName);

		m_uniformLocationMap[nameHash] = uniformLocation;
	}
}

//void Shader::PopulateVertexAttributes()
//{
//	int32 attributeCount = 0;
//	glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &attributeCount);
//	const int32 maxNameLength = 32;
//
//	uint32 offset = 0;
//
//	for (int i = 0; i < attributeCount; i++)
//	{
//		char name[maxNameLength];
//		int32 nameLength = 0;
//		uint32 glType = 0;
//		bool normalized = 0;
//		int32 size = 0;
//		glGetActiveAttrib(m_id, i, maxNameLength, &nameLength, &size, &glType, name);
//		int32 location = glGetAttribLocation(m_id, name);
//
//		m_vertexAttributes.push_back({
//			location,
//			size,
//			glType,
//			normalized,
//			stride,
//			offset,
//									 });
//		
//		glEnableVertexAttribArray(location);
//		glVertexAttribPointer(location,
//			size,
//			glType,
//			normalized,
//			0,
//			(const void*)offset);
//	}
//}

inline bool Shader::TryToFindUniform(const char* name, int32& uniformLocation) const
{
	uniformLocation = -1;
	uint64 nameHash = std::hash<std::string>{}(name);
	auto uniform = m_uniformLocationMap.find(nameHash);
	bool uniformExists = uniform != m_uniformLocationMap.end();
	if (uniformExists)
	{
		uniformLocation = uniform->second;

		// TODO: Move. This should be done in another function.
		// since this function is meant to find uniforms
		if (!IsBound())
		{
			Bind();
		}
	}

	return uniformExists;
}

void Shader::SetUniform(const char* name, const int32& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform1iv(uniformLocation, count, &value);
	}
}

void Shader::SetUniform(const char* name, const uint32& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform1uiv(uniformLocation, count, &value);
	}
}

void Shader::SetUniform(const char* name, const float& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform1fv(uniformLocation, count, &value);
	}
}

void Shader::SetUniform(const char* name, const glm::ivec2& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform2iv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::ivec3& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform3iv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::ivec4& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform4iv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::uvec2& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform2uiv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::uvec3& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform3uiv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::uvec4& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform4uiv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::vec2& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform2fv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::vec3& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform3fv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::vec4& value, const int32& count)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniform4fv(uniformLocation, count, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat2& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix2fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat2x3& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix2x3fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat2x4& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix2fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat3& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix3fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat3x2& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix3x2fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat3x4& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix3x4fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat4& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix4fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat4x2& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix4x2fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}

void Shader::SetUniform(const char* name, const glm::mat4x3& value, const int32& count, bool transpose)
{
	int32 uniformLocation;
	if (TryToFindUniform(name, uniformLocation))
	{
		glUniformMatrix4x3fv(uniformLocation, count, transpose, glm::value_ptr(value));
	}
}