#include "glad.h"
#include <GLFW/glfw3.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include "stb_image.h"
#include "chunk.hpp"

Camera camera;

bool firstMouse = true;
int height = 900;
int width = 900 * 16 / 9;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void keyboard_callback();

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	stbi_set_flip_vertically_on_load(true);
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Voxels", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create a glfw window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize Glad\n");
		return -1;
	}

	Shader shaderProgram = Shader("../shaders/shader.vert", "../shaders/shader.frag");
	Chunk chunk = Chunk(glm::ivec3(0, 0, 0), shaderProgram);
	
	
	// set the texture wrapping/filtering options (on the currently bound texture object)

	glm::mat4 projection;
	glm::mat4 view;
	
	int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
	int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
	
	glEnable(GL_DEPTH_TEST);

	for (int x = 2; x < 14; x++)
		for (int z = 2; z < 14; z++)
			chunk.setBlock(glm::ivec3(x, 4, z), Block("stone_bricks"));
	for (int x = 3; x < 13; x++)
		for (int y = 5; y < 8; y++)
			for (int z = 3; z < 13; z++)
				chunk.setBlock(glm::ivec3(x, y, z), Block("bricks"));
	for (int x = 4; x < 12; x++)
		for (int y = 5; y < 8; y++)
			for (int z = 4; z < 12; z++)
				chunk.setBlock(glm::ivec3(x, y, z), Block("air"));
	for (int x = 2; x < 14; x++)
		for (int z = 2; z < 14; z++)
			chunk.setBlock(glm::ivec3(x, 8, z), Block("oak_planks"));
	for (int x = 3; x < 13; x++)
		for (int z = 3; z < 13; z++)
			chunk.setBlock(glm::ivec3(x, 9, z), Block("oak_planks"));
	for (int y = 5; y < 7; y++)
		for (int x = 7; x < 9; x++)
			chunk.setBlock(glm::ivec3(x, y, 3), Block("air"));

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++)
				chunk.setBlock(glm::ivec3(x, y, z), Block("bricks"));

	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window)) {
		shaderProgram.use();
		projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		chunk.renderChunk();

		view = camera.GetViewMatrix();

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	glViewport(0, 0, width, height);
}

double lastTime = glfwGetTime();
void processInput(GLFWwindow* window) {
	double deltaTime = glfwGetTime() - lastTime;
	lastTime = glfwGetTime();
	if (deltaTime <= 0) return;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, 4.0f * deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.ProcessMouseMovement(xpos, ypos);
}
