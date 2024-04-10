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

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;
using namespace glm; // Assim não preciso ficar colocando glm:: antes de funções do glm

#include "shader/Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupGeometry();
void drawScene(int VAO);

const GLuint WIDTH = 800, HEIGHT = 600;

// Fun��o MAIN
int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	//Essencial para computadores da Apple
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
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
	

	Shader shader("exercicio5/shaders/ortho.vs", "exercicio5/shaders/ortho.fs");

	GLuint VAO = setupGeometry();
	
	
	shader.Use();
	
	// Matriz de projeção paralela ortográfica
	glm::mat4 projection = glm::ortho(0.0, 800.0, 600.0, 0.0,-1.0,1.0);

	// Enviando para o shader a matriz como uma variável uniforme
	shader.setMat4("projection", glm::value_ptr(projection)); // value_ptr converte a matriz para um vetor de 16 floats

	glm::mat4 model = glm::mat4(1); // Inicializa com a matriz identidade
	model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0)); // Translada o objeto para a posição (400, 300, 0)
	model = glm::scale(model, glm::vec3(400.0, 300.0, 0.0)); // Escala o objeto para o dobro do tamanho original
	shader.setMat4("model", glm::value_ptr(model));


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);
		glViewport(0, 0, width, height);

		glBindVertexArray(VAO); 
		glDrawArrays(GL_LINE_LOOP, 0, 2);
		glDrawArrays(GL_LINE_LOOP, 2, 2);
		glBindVertexArray(0);

		// Primeira viewport (quadrante inferior esquerdo)
		glViewport(0, 0, width / 2, height / 2);
		drawScene(VAO);

		// Segunda viewport (quadrante inferior direito)
		glViewport(width / 2, 0, width / 2, height / 2);
		drawScene(VAO);

		// Terceira viewport (quadrante superior esquerdo)
		glViewport(0, height / 2, width / 2, height / 2);
		drawScene(VAO);

		// Quarta viewport (quadrante superior direito)
		glViewport(width / 2, height / 2, width / 2, height / 2);
		drawScene(VAO);

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

	GLfloat vertices[] = {
		//x   y     z
		0.0, 1.0, 0.0, 0.6, 0.8, 0.9, //v0
		0.0, -1.0, 0.0, 0.6, 0.8, 0.9, //v1
		1.0, 0.0, 0.0, 0.6, 0.8, 0.9, //v2
		-1.0, 0.0, 0.0, 0.6, 0.8, 0.9, //v3
		0.0, -0.5, 0.0, 1.0, 0.0, 1.0, //v4
		-0.5, 0.5, 0.0, 1.0, 0.0, 1.0, //v5
		0.5, 0.5, 0.0, 1.0, 0.0, 1.0, //v6

	};

	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
 
	glBindVertexArray(VAO);

	// Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	return VAO;
}


void drawScene(int VAO)
{
	glBindVertexArray(VAO); 

	glDrawArrays(GL_TRIANGLES, 4, 3);

	glBindVertexArray(0);
}