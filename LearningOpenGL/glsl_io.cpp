#include "stdafx.h"
#include "glsl_io.h"



bool GLSL_LoadShaderString(char* filename, const char **  shader)
{
	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::ate);
	if (!file) return false;

	std::ifstream::pos_type pos = file.tellg();
	*shader = new char[static_cast<unsigned int>(pos) + 1];
	
	file.seekg(0, std::ios::beg);
	file.read(const_cast<char*>(*shader), pos);
	const_cast<char*>(*shader)[pos] = '\0';

	return true;
}


void GLSL_DestroyShaderString(const char** shader)
{
	if (*shader != 0)
		delete[] * shader;
	*shader = nullptr;
}

GLint GLSL_LoadShader(char *filename, GLenum shaderType, GLuint &pShader)
{
	const char *source;
	if (!GLSL_LoadShaderString(filename, &source))
		return 0;
	pShader = glCreateShader(shaderType);
	glShaderSource(pShader, 1, &source, NULL);
	glCompileShader(pShader);
	GLSL_DestroyShaderString(&source);
	GLint status;
	glGetShaderiv(pShader, GL_COMPILE_STATUS, &status);
	return status;
}

void GLSL_PrintShaderLog(GLuint shader)
{
	char buffer[512];
	glGetShaderInfoLog(shader, 512, NULL, buffer);
	printf("%s", buffer);
}