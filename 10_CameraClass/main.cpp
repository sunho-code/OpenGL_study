#include <glad/glad.h>
#include <GLFW/glfw3.h>			// glad가 위에 있어야 함
#include <iostream>
#include "Shader.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);		// 마우스 콜백함수
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);		// 스크롤콜백함수
void processInput(GLFWwindow* window);

// 카메라 선언
Camera ourCamera;

float deltaTime = 0.0;
float lastFrame = 0.0;
float lastX = 400, lastY = 300;			// 픽셀좌표

int main(void)
{
	glfwInit();			// GLFW 초기화
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// 3.3 버전
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// core-profile mode
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		// macOS

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);		// 창 생성
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);			// context setup

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {		// glad 초기화
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);		// opengl에게 rendering window의 크기를 알려줌(왼쪽 아래 픽셀 좌표, 너비, 높이), 창크기보다 작게 해도 됨
	// -1 to 1 을 0 to 800, 0 to 600으로 mapping
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);		// callback함수 설정: 사용자가 창 크기를 바꾸면 그에 맞게 창 크기 설정
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		// 커서 숨기고 capture함(실행창에 포커스가 있으면 마우스 커서가 창 중앙에 위치)
	// 전역 opengl state 설정
	glEnable(GL_DEPTH_TEST);		// depth test 기본은 비활성화

	// 쉐이더 선언
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
	//view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));		// 카메라를 뒤로 3만큼 물리는 것은 전체를 앞으로 3만큼 옮기는 것과 같다. 카메라 처음 위치는 0, 0, 0
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);		// 첫번째는 시야각, 두번째는 aspect-ratio, 세네번째는 near, far plane 위치


	unsigned int EBO, VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//(1, &EBO);
	// 먼저 VAO를 바인딩하고, 다음에 VBO를 바인딩, 세팅하고, attributes를 설정
	glBindVertexArray(VAO);			// 바인딩 : 시작
	glBindBuffer(GL_ARRAY_BUFFER, VBO);			// 서로 다른 버퍼 바인딩, VBO를 GL_ARRAY_BUFFER에
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);		// 첫번째는 어떤 vertex 속성을 정할지(location = 0이었던 것 기억), 두번째는 벡터성분 몇 개인지, 세번째는 데이터타입, 네번째는 GL_TRUE로 설정할 시 정수형 들어오면 정규화(어떻게인지는 잘 모르겠고), 다섯번째는 stride : 한 vertex 크기? opengl에게 크기 결정하게 하고 싶으면 0으로. 마지막은 offset: buffer data의 어디부터 vertex data인지
	glEnableVertexAttribArray(0);		// location 받아 vertex 속성을 활성화함
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
	//stbi_set_flip_vertically_on_load(true);   // 회전
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

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);			// EBO unbinding - VAO가 active 되기 전에는 binding 유지
	glBindVertexArray(0);					// VAO unbinding : 끝

	float ratio = 0.2;
	// render loop - loop 하나를 frame이라 부름
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && ratio < 1.0)
			ratio += 0.0001;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && ratio > 0.0)
			ratio -= 0.0001;
		processInput(window);		// escape 키 누르면 다음 루프에서  종료

		// rendering commands here
		glClearColor(0.2f, 0.3f, 9.0f, 1.0f);			// state_setting
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// state-using, 이거 안하면 이전 프레임 색깔로 계속 나옴. GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT 써도 됨

		glm::mat4 view = ourCamera.GetViewMatrix();

		glm::mat4 projection = glm::perspective(glm::radians(ourCamera.Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		// 삼각형 그리기
		ourShader.use();
		ourShader.setInt("texture1", 0);
		ourShader.setInt("texture2", 1);
		ourShader.setFloat("ratio", ratio);

		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);					// 지금은 VAO가 하나밖에 없어서 필요는 없음. 여러 개를 그리고 싶다면 여러 VAO를 만들어 VBO와 attribute pointers를 저장한다. 이제 한 물체를 그리고 싶을 때, 해당하는 VAO를 바인딩하여 그리고, 다시 unbind한다.
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
		glfwPollEvents();					// 어떤 이벤트가 발생되었는지 체크하고 그에 따라 state 바꾸기
	}

	// optional
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();		// GLFW 종료
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
		ourCamera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);		// nomalize하는 이유는 앞뒤 움직임과 속도를 맞추기 위해
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		ourCamera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	return;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	if (firstMouse) {	// 처음에만 실행
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	static float xoffset, yoffset;				// 픽셀좌표에서는 y값 위->아래로 커짐
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