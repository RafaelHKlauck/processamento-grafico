#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include "shader/Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupGeometry();

const GLuint WIDTH = 600, HEIGHT = 600;

const int numberOfPoints = 1000;
const float pi = 3.14159;
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	// Essencial para computadores da Apple
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 1", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER); 
	const GLubyte* version = glGetString(GL_VERSION); 
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	Shader shader("exercicio7/shaders/helloTriangle.vs", "exercicio7/shaders/helloTriangle.fs");

	GLuint VAO = setupGeometry();

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	
	shader.Use();

	// Exercicio a - octogono

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO);

		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); //enviando cor para vari�vel uniform inputColor

		// Chamada de desenho - drawcall

		
		glDrawArrays(GL_LINE_STRIP, 0, numberOfPoints);

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat* vertices;
	const int multipliedNumberOfPoints = 3 * numberOfPoints;
	vertices = new GLfloat[multipliedNumberOfPoints];

  // x y z do centro do espiral
	vertices[0] = 0.0;
	vertices[1] = 0.0;
	vertices[2] = 0.0;

	float angle = 0.0;
	float angleIncrement = 6 * pi / (float)(numberOfPoints - 2);
	float radius = 0.0;
	float radiusIncrement = 0.5 / (float)(numberOfPoints - 2);
	for (int i = 3; i < multipliedNumberOfPoints; i += 3)
	{
		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;
		angle += angleIncrement;
		radius += radiusIncrement;
	}
	
	GLuint VBO, VAO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, multipliedNumberOfPoints * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	return VAO;
}