#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include "shader/Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupHead();
int setupBody();
int setupTail();
int setupFace();

const GLuint WIDTH = 600, HEIGHT = 600;

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


	Shader shader("exercicio9/shaders/helloTriangle.vs", "exercicio9/shaders/helloTriangle.fs");

	GLuint VAOHead = setupHead();
	GLuint VAOBody = setupBody();
	GLuint VAOTail = setupTail();
	GLuint VAOFace = setupFace();

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	
	shader.Use();

	// Exercicio a - octogono

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(4);

		// Head
		glBindVertexArray(VAOHead);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, 9);

		// Body
		glBindVertexArray(VAOBody);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, 16);

		// Tail
		glBindVertexArray(VAOTail);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_LINE_STRIP, 0, 8);

		// Eye
		glBindVertexArray(VAOFace);
		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 6);
		glDrawArrays(GL_TRIANGLES, 6, 9);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAOHead);
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupHead()
{  
	GLfloat vertices[] = {
		-0.8, 1, 0.0,
		-0.6, 0.8, 0.0,
		-0.4, 0.8, 0.0,
		-0.2, 1, 0.0,
		-0.2, 0.6, 0.0,
		-0.4, 0.4, 0.0,
		-0.6, 0.4, 0.0,
		-0.8, 0.6, 0.0,
		-0.8, 1, 0.0,
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

int setupBody()
{  
	
	GLfloat vertices[] = {
		-0.2, 0.6, 0.0,
		0.2, 0.6, 0.0,
		0.6, 0.2, 0.0,
		0.6, 0.0, 0.0,
		0.4, 0.2, 0.0,
		0.2, 0.2, 0.0,
		0.4, 0.0, 0.0,
		0.4, 0.2, 0.0,
		-0.2, 0.2, 0.0,
		-0.4, 0.0, 0.0,
		-0.6, 0.0,0.0,
		-0.4, 0.2, 0.0,
		-0.6, 0.0,0.0,
		-0.8, 0.0, 0.0,
		-0.6, 0.2, 0.0,
		-0.6, 0.4, 0.0,
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

int setupTail()
{  
	
	GLfloat vertices[] = {
		0.2, 0.6, 0.0,
		0.4, 0.8, 0.0,
		0.6, 0.8, 0.0,
		0.8, 0.6, 0.0,
		0.8, 0.4, 0.0,
		0.6, 0.6, 0.0,
		0.4, 0.6, 0.0,
		0.4, 0.4, 0.0 
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

int setupFace()
{  
	
	GLfloat vertices[] = {
		// Left eye
		-0.57, 0.67, 0.0,
		-0.63, 0.67, 0.0,
		-0.60, 0.73, 0.0,
		// Right eye
		-0.37, 0.67, 0.0,
		-0.43, 0.67, 0.0,
		-0.40, 0.73, 0.0,
		// Nose
		-0.47, 0.6, 0.0,
		-0.53, 0.6, 0.0,
		-0.50, 0.54, 0.0

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