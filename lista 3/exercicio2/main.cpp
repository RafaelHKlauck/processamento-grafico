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
#include "SpriteAula.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

GLuint loadTexture(string texturePath);

const GLuint WIDTH = 800, HEIGHT = 600;
SpriteAula spr;

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

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 3 -- Exercicio 2", nullptr, nullptr);
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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); //a cada ciclo

	//Habilitar o modo de transparância (blend - mistura de cores)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("exercicio2/shaders/tex.vs", "exercicio2/shaders/tex.fs");

	GLuint texID = loadTexture("../textures/pixelWall.png");

	// Sprite aqui
	spr.setShader(&shader);
	spr.initialize(texID, glm::vec3(400.0, 300.0, 0.0), glm::vec3(100.0, 100.0, 1.0), 0.0);

	// Ativando o buffer de textura 0 da openGL
	glActiveTexture(GL_TEXTURE0);

	shader.Use();

	// Matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	// Enviando para o shader a matriz como uma variável uniforme
	shader.setMat4("projection", glm::value_ptr(projection)); // value_ptr converte a matriz para um vetor de 16 floats

	shader.setInt("texBuffer", 0); // 0 porque fizemos isso glActiveTexture(GL_TEXTURE0); no 0

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height); //unidades de tela: pixel
		
		// Limpa o buffer de cor
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		spr.draw();

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D)
		spr.moveRight();
	
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A)
		spr.moveLeft();

	if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
		spr.moveUp();

	if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
		spr.moveDown();
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