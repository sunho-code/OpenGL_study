#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::ifstream vShaderFile;			// vertex shader�� ���� ��Ʈ��
	std::ifstream fShaderFile;			// fragment shader�� ���� ��Ʈ��
	std::ostringstream voutstr;
	std::ostringstream foutstr;

	vShaderFile.open(vertexPath);
	if (!vShaderFile.is_open())
		std::cerr << vertexPath << "����� ������ �� �� �����ϴ�.\n";
	fShaderFile.open(fragmentPath);
	if (!fShaderFile.is_open())
		std::cerr << fragmentPath << "����� ������ �� �� �����ϴ�.\n";

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

	// ���̴� ������
	unsigned int vertexShader, fragmentShader;		// id
	int success;
	char infoLog[512];
	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);			// ���̴� ���� �˷���
	glShaderSource(vertexShader, 1, &vertexCode, NULL);			// ���̴� �ҽ��ڵ� ����
	glCompileShader(vertexShader);				// ������
	//������ ���� üũ
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);			// ���̴� ���� �˷���
	glShaderSource(fragmentShader, 1, &fragmentCode, NULL);			// ���̴� �ҽ��ڵ� ����
	glCompileShader(fragmentShader);				// ������
	//������ ���� üũ
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// �������� ���̴��� shader program�� link
	ID = glCreateProgram();			// id ��ȯ�ϴ� �Լ�
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);			// ��ũ
	// linking error üũ
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);			// ��ũ�� ���� �Ŀ��� shader objects ������ ��.

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