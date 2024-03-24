#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include "shader/Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupTrianglesGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

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


	Shader shader("exercicio4/shaders/helloTriangle.vs", "exercicio4/shaders/helloTriangle.fs");

	GLuint VAO = setupTrianglesGeometry();

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	
	shader.Use();
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		glBindVertexArray(VAO);

		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para vari�vel uniform inputColor

		// Chamada de desenho - drawcall

		// Exercicio a
		// Poligono Preenchido - GL_TRIANGLES
		// glDrawArrays(GL_TRIANGLES, 0, 6); // numero de vertices a serem processados

		// Exercicio b
		// Desenhar só pointos usar GL_POINTS
		// glDrawArrays(GL_POINTS, 0, 6);

		// Exercicio c
		// Desenhar só contorno usar GL_LINE_LOOP
		// glDrawArrays(GL_LINE_LOOP, 0, 3);
		// glDrawArrays(GL_LINE_LOOP, 3, 3);

		// Exercicio d
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		glDrawArrays(GL_TRIANGLES, 0, 6); // numero de vertices a serem processados

		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		glDrawArrays(GL_POINTS, 0, 6);

		glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glDrawArrays(GL_LINE_LOOP, 3, 3);

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

int setupTrianglesGeometry()
{
	GLfloat vertices[] = {
		// x    y    z
		// Primeiro Triangulo
		-0.5, 0.5, 0.5, //v0
		 0.0, 0.0, 0.0, //v1
 		 0.5, 0.5, 0.0, //v2 
		 // Segundo Triangulo
		 0.0, 0.0, 0.0, //v3
		 -0.5, -0.5, 0.0, //v4
		 0.5, -0.5, 0.0, //v5
	};

	GLuint VBO, VAO;
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	return VAO;
}