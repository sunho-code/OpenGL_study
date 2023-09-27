#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vShaderFile;			// vertex shader를 읽을 스트림
	std::ifstream fShaderFile;			// fragment shader를 읽을 스트림
	std::ostringstream voutstr;
	std::ostringstream foutstr;

	vShaderFile.open(vertexPath);
	if (!vShaderFile.is_open())
		std::cerr << vertexPath << "경로의 파일을 열 수 없습니다.\n";
	fShaderFile.open(fragmentPath);
	if (!fShaderFile.is_open())
		std::cerr << fragmentPath << "경로의 파일을 열 수 없습니다.\n";

	char ch;
	while (vShaderFile.get(ch))
		voutstr << ch;
	while (fShaderFile.get(ch))
		foutstr << ch;
	std::string vstring = voutstr.str();
	std::string fstring = foutstr.str();
	const char* vertexCode = vstring.c_str();
	const char* fragmentCode = fstring.c_str();
	vShaderFile.close();
	fShaderFile.close();

	// 쉐이더 컴파일
	unsigned int vertexShader, fragmentShader;		// id
	int success;
	char infoLog[512];
	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);			// 쉐이더 종류 알려줌
	glShaderSource(vertexShader, 1, &vertexCode, NULL);			// 쉐이더 소스코드 붙임
	glCompileShader(vertexShader);				// 컴파일
	//컴파일 에러 체크
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);			// 쉐이더 종류 알려줌
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);			// 쉐이더 소스코드 붙임
	glCompileShader(fragmentShader);				// 컴파일
	//컴파일 에러 체크
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 컴파일한 쉐이더를 shader program에 link
	ID = glCreateProgram();			// id 반환하는 함수
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);			// 링크
	// linking error 체크
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);			// 링크가 끝난 후에는 shader objects 지워도 됨.

	return;
}
void Shader::use()
{
	glUseProgram(ID);
	return;
}
void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	return;
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	return;
}
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	return;
}