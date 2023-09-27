#include <glad/glad.h>
#include <GLFW/glfw3.h>			// glad�� ���� �־�� ��
#include <iostream>
#include "Shader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);		// ���콺 �ݹ��Լ�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);		// ��ũ���ݹ��Լ�
void processInput(GLFWwindow* window);

// ī�޶� ����
Camera ourCamera;

float deltaTime = 0.0;
float lastFrame = 0.0;
float lastX = 400, lastY = 300;			// �ȼ���ǥ

int main(void)
{
	glfwInit();			// GLFW �ʱ�ȭ
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// 3.3 ����
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// core-profile mode
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		// macOS

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);		// â ����
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);			// context setup

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {		// glad �ʱ�ȭ
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);		// opengl���� rendering window�� ũ�⸦ �˷���(���� �Ʒ� �ȼ� ��ǥ, �ʺ�, ����), âũ�⺸�� �۰� �ص� ��
	// -1 to 1 �� 0 to 800, 0 to 600���� mapping
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		// callback�Լ� ����: ����ڰ� â ũ�⸦ �ٲٸ� �׿� �°� â ũ�� ����
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		// Ŀ�� ����� capture��(����â�� ��Ŀ���� ������ ���콺 Ŀ���� â �߾ӿ� ��ġ)
	// ���� opengl state ����
	glEnable(GL_DEPTH_TEST);		// depth test �⺻�� ��Ȱ��ȭ

	// ���̴� ����
	Shader ourShader("vertexShader.txt", "fragmentShader.txt");
	
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
	   glm::vec3(0.0f,  0.0f,  0.0f),
	   glm::vec3(2.0f,  5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(1.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//glm::mat4 model = glm::mat4(1.0);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	//glm::mat4 view = glm::mat4(1.0);
	//view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));		// ī�޶� �ڷ� 3��ŭ ������ ���� ��ü�� ������ 3��ŭ �ű�� �Ͱ� ����. ī�޶� ó�� ��ġ�� 0, 0, 0
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);		// ù��°�� �þ߰�, �ι�°�� aspect-ratio, ���׹�°�� near, far plane ��ġ


	unsigned int EBO, VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//(1, &EBO);
	// ���� VAO�� ���ε��ϰ�, ������ VBO�� ���ε�, �����ϰ�, attributes�� ����
	glBindVertexArray(VAO);			// ���ε� : ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			// ���� �ٸ� ���� ���ε�, VBO�� GL_ARRAY_BUFFER��
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);		// ù��°�� � vertex �Ӽ��� ������(location = 0�̾��� �� ���), �ι�°�� ���ͼ��� �� ������, ����°�� ������Ÿ��, �׹�°�� GL_TRUE�� ������ �� ������ ������ ����ȭ(��������� �� �𸣰ڰ�), �ټ���°�� stride : �� vertex ũ��? opengl���� ũ�� �����ϰ� �ϰ� ������ 0����. �������� offset: buffer data�� ������ vertex data����
	glEnableVertexAttribArray(0);		// location �޾� vertex �Ӽ��� Ȱ��ȭ��
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);		// VBO unbinding

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//stbi_set_flip_vertically_on_load(true);   // ȸ��
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);			// EBO unbinding - VAO�� active �Ǳ� ������ binding ����
	glBindVertexArray(0);					// VAO unbinding : ��

	float ratio = 0.2;
	// render loop - loop �ϳ��� frame�̶� �θ�
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && ratio < 1.0)
			ratio += 0.0001;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && ratio > 0.0)
			ratio -= 0.0001;
		processInput(window);		// escape Ű ������ ���� ��������  ����

		// rendering commands here
		glClearColor(0.2f, 0.3f, 9.0f, 1.0f);			// state_setting
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// state-using, �̰� ���ϸ� ���� ������ ����� ��� ����. GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT �ᵵ ��

		glm::mat4 view = ourCamera.GetViewMatrix();

		glm::mat4 projection = glm::perspective(glm::radians(ourCamera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		// �ﰢ�� �׸���
		ourShader.use();
		ourShader.setInt("texture1", 0);
		ourShader.setInt("texture2", 1);
		ourShader.setFloat("ratio", ratio);

		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);					// ������ VAO�� �ϳ��ۿ� ��� �ʿ�� ����. ���� ���� �׸��� �ʹٸ� ���� VAO�� ����� VBO�� attribute pointers�� �����Ѵ�. ���� �� ��ü�� �׸��� ���� ��, �ش��ϴ� VAO�� ���ε��Ͽ� �׸���, �ٽ� unbind�Ѵ�.
		for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0 * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0, 0.3, 0.5));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glBindVertexArray(0);		//VAO unbinding
		glfwSwapBuffers(window);			// double buffer(front / back)
		glfwPollEvents();					// � �̺�Ʈ�� �߻��Ǿ����� üũ�ϰ� �׿� ���� state �ٲٱ�
	}

	// optional
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();		// GLFW ����
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	return;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);		// nomalize�ϴ� ������ �յ� �����Ӱ� �ӵ��� ���߱� ����
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	return;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse) {	// ó������ ����
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	static float xoffset, yoffset;				// �ȼ���ǥ������ y�� ��->�Ʒ��� Ŀ��
	xoffset = xpos - lastX;
	yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	ourCamera.ProcessMouseMovement(xoffset, yoffset);

	return;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ourCamera.ProcessMouseScroll(yoffset);
	return;
}