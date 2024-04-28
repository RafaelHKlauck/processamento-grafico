/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 14/08/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// STB Image - biblioteca para carregar imagens
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

using namespace std;
using namespace glm; // Assim não preciso ficar colocando glm:: antes de funções do glm

#include "shader/Shader.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int setupGeometry();
GLuint loadTexture(string texturePath);

const GLuint WIDTH = 800, HEIGHT = 600;

// Fun��o MAIN
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 3 -- Exercicio 1", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("exercicio1/shaders/tex.vs", "exercicio1/shaders/tex.fs");

	GLuint VAO = setupGeometry();
	GLuint texID = loadTexture("../textures/pixelWall.png");


	// Ativando o buffer de textura 0 da openGL
	glActiveTexture(GL_TEXTURE0);

	shader.Use();

	// Matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	// Enviando para o shader a matriz como uma variável uniforme
	shader.setMat4("projection", glm::value_ptr(projection)); // value_ptr converte a matriz para um vetor de 16 floats

	// Matriz de transformação do objeto (matriz de modelo)
	glm::mat4 model = glm::mat4(1);																						 // Inicializa com a matriz identidade
	model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0));							 // Translada o objeto para a posição (400, 300, 0)
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0)); // Rotaciona o objeto 90 graus em torno do eixo z
	model = glm::scale(model, glm::vec3(300.0, 300.0, 0.0));									 // Escala o objeto para o dobro do tamanho original
	shader.setMat4("model", glm::value_ptr(model));

	shader.setInt("texBuffer", 0); // 0 porque fizemos isso glActiveTexture(GL_TEXTURE0); no 0

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Modificar a rotação para animação
		for(int i=1; i<4; i++) {
			float angle = i*glfwGetTime();
			model = glm::mat4(1);																				 // Inicializa com a matriz identidade
			model = glm::translate(model, glm::vec3(i*200, i*150, 0.0));
			model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0)); 
			model = glm::scale(model, glm::vec3(i*50.0, i*50.0, 0.0));		 
			shader.setMat4("model", glm::value_ptr(model));

			glBindTexture(GL_TEXTURE_2D, texID);
			glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat vertices[] = {
			// x   y     z    r     g    b    s    t
			// Triangulo 0
			-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, // v0
			0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0,	// v1
			0.0, 0.5, 0.0, 0.0, 0.0, 1.0, 0.5, 1.0		// v2
	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Atributo 2 - coordenadas de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	// (2 paramentro, 2 coordenadas)															(6 = quantidade de floats onde começa as coordenadas de textura)
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

GLuint loadTexture(string texturePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Configuração do parâmetros Wrapping nas coordenadas S e T
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Configuração do parâmetros de Filtering na minimização e na magnificação da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) // jpg, bmp 
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}