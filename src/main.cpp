#include "glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "block.hpp"
#include "glm/trigonometric.hpp"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include "world.hpp"

Camera *camera = nullptr;

bool firstMouse = true;
int height = 900;
int width = 900 * 16 / 9;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
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
	glfwSetScrollCallback(window, scroll_wheel_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize Glad\n");
		return -1;
	}

	camera = new Camera(0.0f, 16.0f, 16.0f, 0.0f, 1.0f, 0.0f, glm::radians(0.0f), 0.0f);
	camera->useCameraShader();
	Block::init();
	World world = World();
	
	// set the texture wrapping/filtering options (on the currently bound texture object)

	glm::mat4 projection;
	glm::mat4 view;
	
	int viewLoc = glGetUniformLocation(camera->getCurrentShader().ID, "view");
	int projectionLoc = glGetUniformLocation(camera->getCurrentShader().ID, "projection");
	
	glEnable(GL_DEPTH_TEST);

	for (int x = 0; x < 32; x+=4)
		for (int y = 0; y < 32; y+=4)
			for (int z = 0; z < 32; z+=4) {
				int blockTypeNum = rand() % 5 + 1; 
				world.setBlock(glm::ivec3(x, y, z), (BlockType)(blockTypeNum + !!blockTypeNum));
			}

	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window)) {
		projection = glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 1000.0f);
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		world.render();

		view = camera->GetViewMatrix();
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
		camera->ProcessKeyboard(FORWARD, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->ProcessKeyboard(LEFT, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->ProcessKeyboard(BACKWARD, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->ProcessKeyboard(RIGHT, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera->ProcessKeyboard(UP, 4.0f * deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera->ProcessKeyboard(DOWN, 4.0f * deltaTime);
	}
}

void scroll_wheel_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera->ProcessMouseMovement(xpos, ypos);
}
