#pragma once
#include <fstream>

bool GLSL_LoadShaderString(char* filename, const char** shader);


void GLSL_DestroyShaderString(const char** shader);
GLint GLSL_LoadShader(char *filename, GLenum shaderType, GLuint &pShader);
void GLSL_PrintShaderLog(GLuint shader);