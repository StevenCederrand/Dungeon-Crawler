#include "Shader.h"
#include "System/Log.h"
#include "Globals/Paths.h"

Shader::Shader(std::string vertex, std::string fragment)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint compileResult;
	char buffer[1024];
	m_shaderSetUp(vertex, vertexShader);
	m_shaderSetUp(fragment, fragmentShader);

	m_shaderProg = glCreateProgram();
	glAttachShader(m_shaderProg, vertexShader);
	glAttachShader(m_shaderProg, fragmentShader);
	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		memset(buffer, 0, 1024);
		glGetProgramInfoLog(m_shaderProg, 1024, nullptr, buffer);
		LOG_WARNING("ERROR WITH SHADER");
		LOG_INFO(buffer);
	}

	glDetachShader(m_shaderProg, vertexShader);
	glDetachShader(m_shaderProg, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

Shader::Shader(std::string vertex, std::string geometry, std::string fragment)
{

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLint compileResult;
	char buffer[1024];
	//m_shaderSetUp does Vertex, geometry and fragment shader setup
	m_shaderSetUp(vertex, vertexShader);
	m_shaderSetUp(geometry, geometryShader);
	m_shaderSetUp(fragment, fragmentShader);

	m_shaderProg = glCreateProgram();
	glAttachShader(m_shaderProg, vertexShader);
	glAttachShader(m_shaderProg, geometryShader);
	glAttachShader(m_shaderProg, fragmentShader);
	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		memset(buffer, 0, 1024);
		glGetProgramInfoLog(m_shaderProg, 1024, nullptr, buffer);
		LOG_WARNING("ERROR WITH SHADER");
		LOG_INFO(buffer);
	}

	glDetachShader(m_shaderProg, vertexShader);
	glDetachShader(m_shaderProg, geometryShader);
	glDetachShader(m_shaderProg, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
}

void Shader::use()
{
	glUseProgram(m_shaderProg);
}

void Shader::unuse()
{
	glUseProgram(NULL);
}

//uniform mat3
void Shader::setMat3(std::string name, glm::mat3 mat)
{
	GLint uniformLoc = glGetUniformLocation(this->getShaderID(), name.c_str());
	if (uniformLoc != -1)
	{
		glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, &mat[0][0]);
	}
	else 
	{
		LOG_WARNING("ERROR MAT3: ");
		LOG_WARNING(name);
	}
}
//uniform mat4
void Shader::setMat4(std::string name, glm::mat4 mat)
{
	GLint uniformLoc = glGetUniformLocation(this->getShaderID(), name.c_str());
	if (uniformLoc != -1)
	{
		glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &mat[0][0]);
	}
	else
	{
		LOG_WARNING("ERROR MAT4: ");
		LOG_WARNING(name);
	}
}
//uniform vec3
void Shader::setVec3(std::string name, glm::vec3 vec)
{
	GLint uniformLoc = glGetUniformLocation(this->getShaderID(), name.c_str());
	if (uniformLoc != -1)
	{
		glUniform3fv(uniformLoc, 1, &vec[0]);
	}
	else
	{
		LOG_WARNING("ERROR VEC3: ");
		LOG_WARNING(name);
	}
}
//uniform vec4
void Shader::setVec4(std::string name, glm::vec4 vec)
{
	GLint uniformLoc = glGetUniformLocation(this->getShaderID(), name.c_str());
	if (uniformLoc != -1)
	{
		glUniform4fv(uniformLoc, 1, &vec[0]);
	}
	else
	{
		LOG_WARNING("ERROR VEC4: ");
		LOG_WARNING(name);
	}
}
//uniform float
void Shader::setFloat(std::string name, float num)
{
	GLint uniformLoc = glGetUniformLocation(this->getShaderID(), name.c_str());
	if (uniformLoc != -1)
	{
		glUniform1f(uniformLoc, num);
	}
	else
	{
		LOG_WARNING("ERROR FLOAT: ");
		LOG_WARNING(name);
	}
}
//uniform int
void Shader::setInt(std::string name, int num)
{
	GLint uniformLoc = glGetUniformLocation(this->getShaderID(), name.c_str());
	if (uniformLoc != -1)
	{
		glUniform1i(uniformLoc, num);
	}
	else
	{
		LOG_WARNING("ERROR INT: ");
		LOG_WARNING(name);
	}
}

int Shader::getShaderID() const
{
	return m_shaderProg;
}

void Shader::setName(std::string name)
{
	m_name = name;
}

std::string Shader::getName() const
{
	return m_name;
}

//one function for all the shaders instead of one for each
void Shader::m_shaderSetUp(std::string shaderName, unsigned int &shader)
{


	std::string Code;
	std::ifstream Shader;
	Shader.open(ShaderPath + shaderName);

	if (!Shader.is_open())
	{
		LOG_ERROR("Failed to find shader file " + shaderName);
	}

	std::stringstream Stream;
	Stream << Shader.rdbuf();
	Shader.close();

	Code = Stream.str();

	GLint compileResult;
	char buffer[1024];

	const char* shaderPtr = Code.c_str();
	glShaderSource(shader, 1, &shaderPtr, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == GL_FALSE)
	{
		glGetShaderInfoLog(shader, 1024, nullptr, buffer);
		LOG_WARNING("ERROR COMPILING: ");
		LOG_WARNING(shaderName);
		LOG_INFO(buffer);
	}
}
