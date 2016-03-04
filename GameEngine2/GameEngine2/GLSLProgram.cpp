#include "GLSLProgram.h"
#include <iostream>
#include <fstream>
#include <vector>

GLSLProgram::GLSLProgram() : numAttributes(0), programID(0), vertexShaderID(0), fragmentShaderID(0)
{
}


GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string vertexShaderFilePath, const std::string fragmentShaderFilePath) {

	//Now time to link them together into a program.
	//Get a program object.
	programID = glCreateProgram();

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderID == 0) {
		std::cout << "Vertex shader failed to be created!";
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShaderID == 0) {
		std::cout << "Fragment shader failed to be created!";
	}

	compileShader(vertexShaderFilePath, vertexShaderID);
	compileShader(fragmentShaderFilePath, fragmentShaderID);
}

void GLSLProgram::linkShaders() {

	//Attach our shaders to our program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	//Link our program
	glLinkProgram(programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(programID);
		//Don't leak shaders either.
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		std::cout << "Shader failed to link!";
	}

	//Always detach shaders after a successful link.
	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void GLSLProgram::addAttribute(const std::string& attributeName) {
	glBindAttribLocation(programID, numAttributes++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		std::cout << "Uniform" + uniformName + "not found in shader!";
	}
	return location;
}

void GLSLProgram::use() {
	glUseProgram(programID);
	for (int i = 0; i < numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse() {
	glUseProgram(0);
	for (int i = 0; i < numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::compileShader(const std::string& filePath, GLuint id) {
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		perror(filePath.c_str());
		std::cout << "Failed to open " + filePath;
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(vertexFile, line)) {
		fileContents += line + "\n";
	}

	vertexFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		//We don't need the shader anymore.
		glDeleteShader(id);


		std::printf("%s\n", &(errorLog[0]));
		std::cout << "Shader" + filePath + "failed to compile!";
	}
}
