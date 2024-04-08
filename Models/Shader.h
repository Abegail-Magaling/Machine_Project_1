#pragma once

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>


class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint shaderProg;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(std::string Vert_Path, std::string Frag_Path);
	
	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

public:
	GLuint getShaderProg() const { return shaderProg; }
};


