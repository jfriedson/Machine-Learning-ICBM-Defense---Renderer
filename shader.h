#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	//Member variables
	GLuint id;

	//Private functions
	std::string loadShaderSource(const char* fileName)
	{
		std::string line = "";
		std::string src = "";

		std::ifstream in_file;

		//Vertex
		in_file.open(fileName);

		if (in_file.is_open())
		{
			while (std::getline(in_file, line))
				src += line + "\n";
		}
		else
		{
			std::cerr << "Shader file not found: " << fileName << "\n";
		}

		in_file.close();

		return src;
	}

	GLuint loadShader(GLenum type, const char* fileName)
	{
		char infoLog[512];
		GLint success;

		GLuint shader = glCreateShader(type);
		std::string str_src = this->loadShaderSource(fileName);
		const GLchar* src = str_src.c_str();
		glShaderSource(shader, 1, &src, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
			std::cout << infoLog << "\n";
		}

		return shader;
	}

	void linkProgram(GLuint vertexShader, GLuint fragmentShader)
	{
		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);
		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

		glUseProgram(0);
	}

public:

	//Constructors/Destructors
	Shader(const char* fileprefix)
	{
		GLuint vertexShader = 0;
		GLuint tessCtrlShader = 0;
		GLuint tessEvalShader = 0;
		GLuint geometryShader = 0;
		GLuint fragmentShader = 0;

		vertexShader = loadShader(GL_VERTEX_SHADER, (std::string(fileprefix) + ".vs").c_str());
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, (std::string(fileprefix) + ".fs").c_str());

		struct stat buffer;
		if (stat((std::string(fileprefix) + ".tc").c_str(), &buffer) == 0)
			tessCtrlShader = loadShader(GL_TESS_CONTROL_SHADER, (std::string(fileprefix) + ".tc").c_str());

		if (stat((std::string(fileprefix) + ".te").c_str(), &buffer) == 0)
			tessEvalShader = loadShader(GL_TESS_EVALUATION_SHADER, (std::string(fileprefix) + ".te").c_str());

		if (stat((std::string(fileprefix) + ".gs").c_str(), &buffer) == 0)
			geometryShader = loadShader(GL_GEOMETRY_SHADER, (std::string(fileprefix) + ".gs").c_str());


		char infoLog[512];
		GLint success;

		this->id = glCreateProgram();

		glAttachShader(this->id, vertexShader);
		if(tessCtrlShader > 0)
			glAttachShader(this->id, tessCtrlShader);
		if (tessEvalShader > 0)
			glAttachShader(this->id, tessEvalShader);
		if (geometryShader > 0)
			glAttachShader(this->id, geometryShader);
		glAttachShader(this->id, fragmentShader);

		glLinkProgram(this->id);

		glGetProgramiv(this->id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(this->id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
			std::cout << infoLog << "\n";
		}

		glUseProgram(0);


		//End
		glDeleteShader(vertexShader);
		glDeleteShader(tessCtrlShader);
		glDeleteShader(tessEvalShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
	}

	~Shader()
	{
		glDeleteProgram(this->id);
	}

	//Set uniform functions
	void use()
	{
		glUseProgram(this->id);
	}

	void unuse()
	{
		glUseProgram(0);
	}

	void set1i(GLint value, const GLchar* name)
	{
		this->use();

		glUniform1i(glGetUniformLocation(this->id, name), value);

		this->unuse();
	}

	void set1f(GLfloat value, const GLchar* name)
	{
		this->use();

		glUniform1f(glGetUniformLocation(this->id, name), value);

		this->unuse();
	}

	void setVec2f(glm::fvec2 value, const GLchar* name)
	{
		this->use();

		glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void setVec3f(glm::fvec3 value, const GLchar* name)
	{
		this->use();

		glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void setVec4f(glm::fvec4 value, const GLchar* name)
	{
		this->use();

		glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

		this->unuse();
	}

	void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE)
	{
		this->use();

		glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}

	void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE)
	{
		this->use();

		glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

		this->unuse();
	}
};
