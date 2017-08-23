#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "shader.h"
#include "model.h"
#include "camera.h"
#pragma comment(lib, "glfw3.lib")
//some functions declarations
//process mouse input
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//process keyboard input
void processInput(GLFWwindow *window);
//load texture from a picture
unsigned int loadTexture(const char *path);
//set VertexsAttribute
void setVertexsAttri(GLuint &vao, GLuint &vbo, float * vertices, int len);
//draw 
void draw(GLuint vao, GLuint vbo, Shader shader, unsigned int texture, glm::mat4 &model);
//window's size setting
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int VERTICES_NUM = 6;
const unsigned int VPOSITION = 0;
const unsigned int BUFF_NUM = 1;
const unsigned int VAO_NUM = 1;
//camera setting
Camera camera(glm::vec3(-3.0f, 1.0f, 0.0f));
//Camera camera;
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
//time setting
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//set up vertices data
float cubeVertices[] = {
	//positions          // texture Coords
	//back 
	0.0f, 0.1f, 0.0f,  0.0f, 0.0f,
	0.5f, 0.1f, 0.0f,  1.0f, 0.0f,
	0.5f,  0.14f, 0.0f,  1.0f, 1.0f,
	0.5f,  0.14f, 0.0f,  1.0f, 1.0f,
	0.0f,  0.14f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.1f, 0.0f,    0.0f, 0.0f,
	//forward
	0.0f, 0.1f,  0.5f,  0.0f, 0.0f,
	0.5f, 0.1f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.14f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.14f,  0.5f,  1.0f, 1.0f,
	0.0f,  0.14f,  0.5f,  0.0f, 1.0f,
	0.0f, 0.1f,  0.5f,  0.0f, 0.0f,
	//left
	0.0f,  0.1f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.1f, 0.5f,  1.0f, 1.0f,
	0.0f, 0.14f, 0.5f,  0.0f, 1.0f,
	0.0f, 0.14f, 0.5f,  0.0f, 1.0f,
	0.0f, 0.14f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.1f,  0.0f,  1.0f, 0.0f,
	//right
	0.5f,  0.1f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.1f, 0.5f,  1.0f, 1.0f,
	0.5f,  0.14f, 0.5f,  0.0f, 1.0f,
	0.5f, 0.14f, 0.5f,  0.0f, 1.0f,
	0.5f, 0.14f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.1f,  0.0f,  1.0f, 0.0f,
	//bottom
	0.0f, 0.1f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.1f, 0.5f,  1.0f, 1.0f,
	0.5f, 0.1f,  0.5f,  1.0f, 0.0f,
	0.5f, 0.1f,  0.5f,  1.0f, 0.0f,
	0.5f, 0.1f,  0.0f,  0.0f, 0.0f,
	0.0f, 0.1f, 0.0f,  0.0f, 1.0f,
	//top
	0.0f,  0.14f, 0.0f,  0.0f, 1.0f,
	0.0f,  0.14f, 0.5f,  1.0f, 1.0f,
	0.5f,  0.14f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.14f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.14f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.14f, 0.0f,  0.0f, 1.0f
};
float bedVertices[] = {
	//back 
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	0.7f, 0.0f, 0.0f,  1.0f, 0.0f,
	0.7f,  2.0f, 0.0f,  1.0f, 1.0f,
	0.7f,  2.0f, 0.0f,  1.0f, 1.0f,
	0.0f,  2.0f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	//forward
	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	0.7f, 0.0f, 1.0f,  1.0f, 0.0f,
	0.7f,  2.0f, 1.0f,  1.0f, 1.0f,
	0.7f,  2.0f, 1.0f,  1.0f, 1.0f,
	0.0f,  2.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	//left
	0.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	0.0f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 2.0f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	//right
	0.7f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.7f,  0.0f, 1.0f,  1.0f, 1.0f,
	0.7f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.7f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.7f, 2.0f,  0.0f,  0.0f, 0.0f,
	0.7f,  0.0f,  0.0f,  1.0f, 0.0f,
	//bottom
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	0.7f, 0.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 0.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 0.0f,  0.0f,  0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	//top
	0.0f, 2.0f, 0.0f,  0.0f, 1.0f,
	0.0f, 2.0f, 1.0f,  1.0f, 1.0f,
	0.7f, 2.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 2.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 2.0f,  0.0f,  0.0f, 0.0f,
	0.0f, 2.0f, 0.0f,  0.0f, 1.0f,
};


float cabinetVertices[] = {
	//back 
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	0.7f, 0.0f, 0.0f,  1.0f, 0.0f,
	0.7f,  2.0f, 0.0f,  1.0f, 1.0f,
	0.7f,  2.0f, 0.0f,  1.0f, 1.0f,
	0.0f,  2.0f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	//forward
	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	0.7f, 0.0f, 1.0f,  1.0f, 0.0f,
	0.7f,  2.0f, 1.0f,  1.0f, 1.0f,
	0.7f,  2.0f, 1.0f,  1.0f, 1.0f,
	0.0f,  2.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	//left
	0.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
	0.0f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.0f, 2.0f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	//right
	0.7f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.7f,  0.0f, 1.0f,  1.0f, 1.0f,
	0.7f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.7f, 2.0f, 1.0f,  0.0f, 1.0f,
	0.7f, 2.0f,  0.0f,  0.0f, 0.0f,
	0.7f,  0.0f,  0.0f,  1.0f, 0.0f,
	//bottom
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	0.7f, 0.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 0.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 0.0f,  0.0f,  0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	//top
	0.0f, 2.0f, 0.0f,  0.0f, 1.0f,
	0.0f, 2.0f, 1.0f,  1.0f, 1.0f,
	0.7f, 2.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 2.0f,  1.0f,  1.0f, 0.0f,
	0.7f, 2.0f,  0.0f,  0.0f, 0.0f,
	0.0f, 2.0f, 0.0f,  0.0f, 1.0f,
};
float deskVertices[] = {
	//back 
	0.0f, 0.6f, 0.0f,  0.0f, 0.0f,
	1.0f, 0.6f, 0.0f,  1.0f, 0.0f,
	1.0f,  0.64f, 0.0f,  1.0f, 1.0f,
	1.0f,  0.64f, 0.0f,  1.0f, 1.0f,
	0.0f,  0.64f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.6f, 0.0f,  0.0f, 0.0f,
	//forward
	0.0f, 0.6f, 0.9f,  0.0f, 0.0f,
	1.0f, 0.6f, 0.9f,  1.0f, 0.0f,
	1.0f,  0.64f, 0.9f,  1.0f, 1.0f,
	1.0f,  0.64f, 0.9f,  1.0f, 1.0f,
	0.0f,  0.64f, 0.9f,  0.0f, 1.0f,
	0.0f, 0.6f, 0.9f,  0.0f, 0.0f,
	//left
	0.0f,  0.6f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.6f, 0.9f,  1.0f, 1.0f,
	0.0f, 0.64f, 0.9f,  0.0f, 1.0f,
	0.0f, 0.64f, 0.9f,  0.0f, 1.0f,
	0.0f, 0.64f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.6f,  0.0f,  1.0f, 0.0f,
	//right
	1.0f,  0.6f,  0.0f,  1.0f, 0.0f,
	1.0f,  0.6f, 0.9f,  1.0f, 1.0f,
	1.0f, 0.64f, 0.9f,  0.0f, 1.0f,
	1.0f, 0.64f, 0.9f,  0.0f, 1.0f,
	1.0f, 0.64f,  0.0f,  0.0f, 0.0f,
	1.0f,  0.6f,  0.0f,  1.0f, 0.0f,
	//bottom
	0.0f, 0.6f, 0.0f,  0.0f, 1.0f,
	1.0f, 0.6f, 0.0f,  1.0f, 1.0f,
	1.0f, 0.6f,  0.9f,  1.0f, 0.0f,
	1.0f, 0.6f,  0.9f,  1.0f, 0.0f,
	0.0f, 0.6f,  0.9f,  0.0f, 0.0f,
	0.0f, 0.6f, 0.0f,  0.0f, 1.0f,
	//top
	0.0f, 0.64f, 0.0f,  0.0f, 1.0f,
	1.0f, 0.64f, 0.0f,  1.0f, 1.0f,
	1.0f, 0.64f,  0.9f,  1.0f, 0.0f,
	1.0f, 0.64f,  0.9f,  1.0f, 0.0f,
	0.0f, 0.64f,  0.9f,  0.0f, 0.0f,
	0.0f, 0.64f, 0.0f,  0.0f, 1.0f,
};
float chairLegVertices[] = {
	//back
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.1f, -0.5f, 0.0f, 1.0f, 0.0f,
	0.1f, 0.1f, 0.0f, 1.0f, 1.0f,
	0.1f, 0.1f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.1f, 0.0f, 0.0f, 1.0f,
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
	//forward
	0.0f, -0.5f, 0.1f, 0.0f, 0.0f,
	0.1f, -0.5f, 0.1f, 1.0f, 0.0f,
	0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.0f, 0.1f, 0.1f, 0.0f, 1.0f,
	0.0f, -0.5f, 0.1f, 0.0f, 0.0f,
	//left
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.1f, 1.0f, 0.0f,
	0.0f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.0f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.0f, 0.1f, 0.0f, 0.0f, 1.0f,
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
	//right
	0.1f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.1f, -0.5f, 0.1f, 1.0f, 0.0f,
	0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.1f, 0.1f, 0.0f, 0.0f, 1.0f,
	0.1f, -0.5f, 0.0f, 0.0f, 0.0f,
	//bottom
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.0f, -0.5f, 0.1f, 1.0f, 0.0f,
	0.1f, -0.5f, 0.1f, 1.0f, 1.0f,
	0.1f, -0.5f, 0.1f, 1.0f, 1.0f,
	0.1f, -0.5f, 0.0f, 0.0f, 1.0f,
	0.0f, -0.5f, 0.0f, 0.0f, 0.0f,
	//top
	0.0f, 0.1f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.1f, 0.1f, 1.0f, 0.0f,
	0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.1f, 0.1f, 0.1f, 1.0f, 1.0f,
	0.1f, 0.1f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.1f, 0.0f, 0.0f, 0.0f,
}; float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};
float backrestVertices[] = {
	//back
	0.0f, 0.14f, 0.0f,  0.0f, 0.0f,
	0.5f, 0.14f, 0.0f,  1.0f, 0.0f,
	0.5f,  0.64f, 0.0f,  1.0f, 1.0f,
	0.5f,  0.64f, 0.0f,  1.0f, 1.0f,
	0.0f, 0.64f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.14f, 0.0f,  0.0f, 0.0f,
	//forward
	0.0f, 0.14f, 0.1f,  0.0f, 0.0f,
	0.5f, 0.14f, 0.1f,  1.0f, 0.0f,
	0.5f,  0.64f, 0.1f,  1.0f, 1.0f,
	0.5f,  0.64f, 0.1f,  1.0f, 1.0f,
	0.0f, 0.64f, 0.1f,  0.0f, 1.0f,
	0.0f, 0.14f, 0.1f,  0.0f, 0.0f,
	//left
	0.0f,  0.14f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.14f, 0.1f,  1.0f, 1.0f,
	0.0f, 0.64f, 0.1f,  0.0f, 1.0f,
	0.0f, 0.64f, 0.1f,  0.0f, 1.0f,
	0.0f, 0.64f,  0.0f,  0.0f, 0.0f,
	0.0f,  0.14f,  0.0f,  1.0f, 0.0f,
	//right
	0.5f,  0.14f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.14f, 0.1f,  1.0f, 1.0f,
	0.5f, 0.64f, 0.1f,  0.0f, 1.0f,
	0.5f, 0.64f, 0.1f,  0.0f, 1.0f,
	0.5f, 0.64f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.14f,  0.0f,  1.0f, 0.0f,
	//bottom
	0.0f, 0.14f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.14f, 0.1f,  1.0f, 1.0f,
	0.5f, 0.14f,  0.1f,  1.0f, 0.0f,
	0.5f, 0.14f,  0.1f,  1.0f, 0.0f,
	0.5f, 0.14f,  0.0f,  0.0f, 0.0f,
	0.0f, 0.14f, 0.0f,  0.0f, 1.0f,
	//top
	0.0f, 0.64f, 0.0f,  0.0f, 1.0f,
	0.0f, 0.64f, 0.1f,  1.0f, 1.0f,
	0.5f, 0.64f,  0.1f,  1.0f, 0.0f,
	0.5f, 0.64f,  0.1f,  1.0f, 0.0f,
	0.5f, 0.64f,  0.0f,  0.0f, 0.0f,
	0.0f, 0.64f, 0.0f,  0.0f, 1.0f

};
//Position data
glm::vec3 chairLegsPosition[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.40f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.4f),
	glm::vec3(0.4f, 0.0f, 0.4f)
};
glm::vec3 leftpillarsPosition[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.4f),
	glm::vec3(0.0f, 0.0f, 0.8f),
	glm::vec3(0.0f, 0.0f, 1.2f),
};
glm::vec3 pillarsPosition[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.4f, 0.0f, 0.0f),
	glm::vec3(0.8f, 0.0f, 0.0f),
	glm::vec3(1.2f, 0.0f, 0.0f),
	glm::vec3(1.6f, 0.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 0.0f),
	glm::vec3(2.4f, 0.0f, 0.0f),
	glm::vec3(2.8f, 0.0f, 0.0f),
	glm::vec3(3.2f, 0.0f, 0.0f)

};
glm::vec3 leftstairsPosition[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.5f, 0.4f),
	glm::vec3(0.0f, 1.0f, 0.8f),
	glm::vec3(0.0f, 1.5f, 1.2f),
};
glm::vec3 rightstairsPosition[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.5f, -0.4f),
	glm::vec3(0.0f, 1.0f, -0.8f),
	glm::vec3(0.0f, 1.5f, -1.2f),
};

int main(int argc, char **argv) {
	//init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//creating window and processing request
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dormitory", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	Shader shader("vertex.glsl", "fragment.glsl");
	
	//set up vao and vbo
	unsigned int cubeVAO, cubeVBO;
	unsigned int cabinetVAO, cabinetVBO;
	unsigned int chairLegsVAO, chairLegsVBO;
	unsigned int planeVAO, planeVBO;
	unsigned int backrestVAO, backrestVBO;
	unsigned int deskVAO, deskVBO;

	setVertexsAttri(cubeVAO, cubeVBO, cubeVertices, sizeof(cubeVertices));
	setVertexsAttri(cabinetVAO, cabinetVBO, cabinetVertices, sizeof(cabinetVertices));
	setVertexsAttri(chairLegsVAO, chairLegsVBO, chairLegVertices, sizeof(chairLegVertices));
	setVertexsAttri(planeVAO, planeVBO, planeVertices, sizeof(planeVertices));
	setVertexsAttri(backrestVAO, backrestVBO, backrestVertices, sizeof(backrestVertices));
	setVertexsAttri(deskVAO, deskVBO, deskVertices, sizeof(deskVertices));

	unsigned int cubeTexture = loadTexture("res//chair.png");
	unsigned int chairLegsTexture = loadTexture("res//chair.png");
	unsigned int floorTexture = loadTexture("res//metal.png");
	unsigned int backrestTexture = loadTexture("res//chair.png");
	unsigned int deskTexture = loadTexture("res//desk.png");
	unsigned int cabinetTexture = loadTexture("res//cabinet.png");
	unsigned int stairTexture = loadTexture("res//stairs.png");
	unsigned int quiltTexture = loadTexture("res//quilt.png");
	unsigned int wallTexture = loadTexture("res//wall.png");
	unsigned int doorTexture = loadTexture("res//door.png");
	unsigned int windowsillTexture = loadTexture("res//windowsill.png");
	unsigned int curtainTexture = loadTexture("res//curtain.png");

	shader.use();
	shader.setInt("texture1", 0);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		//draw floor
		model = glm::mat4();
		draw(planeVAO, planeVBO, shader, floorTexture, model);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		//draw surface of chair
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		draw(cubeVAO, cubeVBO, shader, cubeTexture, model);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		draw(cubeVAO, cubeVBO, shader, cubeTexture, model);
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, -1.8f));
		draw(cubeVAO, cubeVBO, shader, cubeTexture, model);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		draw(cubeVAO, cubeVBO, shader, cubeTexture, model);
		//draw backrest
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		draw(backrestVAO, backrestVBO, shader, backrestTexture, model);
		model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
		draw(backrestVAO, backrestVBO, shader, backrestTexture, model);
		model = glm::translate(model, glm::vec3(0.3f, 0.0f, -1.4f));
		draw(backrestVAO, backrestVBO, shader, backrestTexture, model);
		model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
		draw(backrestVAO, backrestVBO, shader, backrestTexture, model);
		//draw cabinet
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, 0.8f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.5f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-1.4f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.65f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, -0.5f, -3.2f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.5f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		//draw windowsill
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.6f, 0.75f, -1.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.03f, 2.0f));
		draw(cabinetVAO, cabinetVBO, shader, windowsillTexture, model);
		//draw curtain
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.8f, 0.75f, -1.4f));
		model = glm::scale(model, glm::vec3(0.02f, 1.0f, 2.0f));
		draw(cabinetVAO, cabinetVBO, shader, curtainTexture, model);
		//draw door
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.55f, -0.5f, -1.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.8f, 1.0f));
		draw(cabinetVAO, cabinetVBO, shader, doorTexture, model);
		//draw bed
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, 1.5f, 0.8f));
		model = glm::scale(model, glm::vec3(5.0f, 0.02f, 1.5f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-0.84f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.65f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, 1.5f, -3.2f));
		model = glm::scale(model, glm::vec3(5.0f, 0.02f, 1.5f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		//draw quilt
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0f, 1.55f, 0.8f));
		model = glm::scale(model, glm::vec3(5.0f, 0.02f, 1.5f));
		draw(cabinetVAO, cabinetVBO, shader, quiltTexture, model);
		model = glm::translate(model, glm::vec3(-0.84f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, quiltTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.65f));
		draw(cabinetVAO, cabinetVBO, shader, quiltTexture, model);
		model = glm::translate(model, glm::vec3(0.84f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, quiltTexture, model);
		//draw small cabinet
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(3.85f, -0.5f, 0.8f));
		model = glm::scale(model, glm::vec3(0.9f, 0.55f, 1.4f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-7.84f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model); 
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.85f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model); 
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(3.85f, -0.5f, -3.15f));
		model = glm::scale(model, glm::vec3(0.9f, 0.55f, 1.4f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		//draw pillars
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.5, -0.5, 0.8));
		model = glm::scale(model, glm::vec3(0.05f, 1.3f, 0.05f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-155.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(155.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.5, -0.5, 2.25));
		model = glm::scale(model, glm::vec3(0.05f, 1.3f, 0.05f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-155.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(155.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		//draw left pillars
		for (int i = 0; i < 9; ++i) {
			model = glm::mat4();
			model = glm::translate(model, pillarsPosition[i]);
			model = glm::translate(model, glm::vec3(1.0, 1.5, 0.8));
			model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);

			model = glm::translate(model, glm::vec3(-78.5f, 0.0f, 0.0f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -51.0f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);

			model = glm::translate(model, glm::vec3(78.5f, 0.0f, 0.0f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		}
		//draw top pillar
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.0, 2.1, 0.8));
		model = glm::scale(model, glm::vec3(5.0f, 0.025f, 0.05f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-0.84f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -51.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.84f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		//draw left pillars
		for (int i = 0; i < 4; ++i) {
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(4.5, 1.5, 0.8));
			model = glm::translate(model, leftpillarsPosition[i]);
			model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.05f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);

			model = glm::translate(model, glm::vec3(-155.0f, 0.0f, 0.0f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);

			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -80.0f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);

			model = glm::translate(model, glm::vec3(155.0f, 0.0f, 0.0f));
			draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		}
		//draw left top pillar
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.5, 2.1, 0.8));
		model = glm::scale(model, glm::vec3(0.05f, 0.025f, 1.5f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(-155.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.66f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		model = glm::translate(model, glm::vec3(155.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, cabinetTexture, model);
		//draw stairs
		for (int i = 0; i < 4; ++i) {
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.3, -0.5, 0.8));
			model = glm::translate(model, leftstairsPosition[i]);
			model = glm::scale(model, glm::vec3(1.0, 0.025, 0.3));
			draw(cabinetVAO, cabinetVBO, shader, stairTexture, model);
		}
		for (int i = 0; i < 4; ++i) {
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.3, -0.5, -2.0));
			model = glm::translate(model, rightstairsPosition[i]);
			model = glm::scale(model, glm::vec3(1.0, 0.025, 0.3));
			draw(cabinetVAO, cabinetVBO, shader, stairTexture, model);
		}
		//draw desk
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.7f, 0.0f, 0.8f));
		model = glm::scale(model, glm::vec3(2.8f, 1.0f, 1.5f));
		draw(deskVAO, deskVBO, shader, deskTexture, model);
		model = glm::translate(model, glm::vec3(-1.75f, 0.0f, 0.0f));
		draw(deskVAO, deskVBO, shader, deskTexture, model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.65f));
		draw(deskVAO, deskVBO, shader, deskTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(1.7f, 0.0f, -3.1f));
		model = glm::scale(model, glm::vec3(2.8f, 1.0f, 1.5f));
		draw(deskVAO, deskVBO, shader, deskTexture, model);
		//draw chairLegs
		for (int i = 0; i < 4; ++i) {
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			model = glm::translate(model, chairLegsPosition[i]);
			draw(chairLegsVAO, chairLegsVBO, shader, chairLegsTexture, model);
			model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
			draw(chairLegsVAO, chairLegsVBO, shader, chairLegsTexture, model);
			model = glm::translate(model, glm::vec3(0.3f, 0.0f, -1.8f));
			draw(chairLegsVAO, chairLegsVBO, shader, chairLegsTexture, model);
			model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
			draw(chairLegsVAO, chairLegsVBO, shader, chairLegsTexture, model);
		}
		//draw wall
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.5, -0.5, 2.3));
		model = glm::scale(model, glm::vec3(12.0, 2.0, 0.05));
		draw(cabinetVAO, cabinetVBO, shader, wallTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.5, -0.5, -3.3));
		model = glm::scale(model, glm::vec3(12.0, 2.0, 0.05));
		draw(cabinetVAO, cabinetVBO, shader, wallTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.5, 3.5, -3.3));
		model = glm::scale(model, glm::vec3(12.0, 0.05, 6.0));
		draw(cabinetVAO, cabinetVBO, shader, wallTexture, model);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(4.9, -0.5, -3.5));
		model = glm::scale(model, glm::vec3(0.05, 2.2, 6.0));
		draw(cabinetVAO, cabinetVBO, shader, wallTexture, model);
		model = glm::translate(model, glm::vec3(-170.0f, 0.0f, 0.0f));
		draw(cabinetVAO, cabinetVBO, shader, wallTexture, model);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;

}

void setVertexsAttri(GLuint &vao, GLuint &vbo, float * vertices, int len) {
	glGenVertexArrays(VAO_NUM, &vao);
	glGenBuffers(BUFF_NUM, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void draw(GLuint vao, GLuint vbo, Shader shader, unsigned int texture, glm::mat4 &model)
{
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
unsigned int loadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
