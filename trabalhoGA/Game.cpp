#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <ctime>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "trabalhoGA/Game.h"
#include "shader/Shader.h"
#include "trabalhoGA/Sprite.h"
#include "trabalhoGA/Bird.h"
#include "trabalhoGA/Pipe.h"

using namespace std;
using namespace glm;

static bool hasClickedSpace = false; 
static bool hasStartedGame = false;
static bool isReadyToPlay = false;
static bool isGameOver = false;
static bool hasClickedEnter = false;

const GLuint WIDTH = 800, HEIGHT = 600;

Game::Game() {}

Game::~Game() {}

void Game::initialize() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Essencial para computadores da Apple
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Flappy Bird! -- Rafael", nullptr, nullptr);
	this->window = window;
	glfwMakeContextCurrent(window);

	// Inicializar ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
};

void Game::start() {
	// Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS); // a cada ciclo

	// Habilitar o modo de transparência (blend - mistura de cores)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("shaders/tex.vs", "shaders/tex.fs");
	this->shader = &shader;

  createBackground();
	createPlayer();
	createPipes();

	glActiveTexture(GL_TEXTURE0);
	shader.Use();
  glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	shader.setMat4("projection", glm::value_ptr(projection));
	shader.setInt("texBuffer", 0);

  while (!glfwWindowShouldClose(this->window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (isReadyToPlay && !isGameOver) {
			if (hasClickedSpace) {
				bird->moveUp();
			}
		}

		background->draw();	
		
		if (isReadyToPlay) {
			glm::vec3 birdPosition = bird->getPosition();
			for (Pipe *pipe : pipes) {
				pipe->draw(score, isGameOver);
				if (pipe->getHasBirdPassed()) continue;
				
				if (!isGameOver) checkPassingThroughPipes(birdPosition, pipe);
			}
			if(!isGameOver) checkCollisionTopBottom();
		}


		if (!pipes.empty() && pipes.front()->topSprite->pos.x <= 0.0) {
			// Remover o último par de canos da lista
			delete pipes.front();
			pipes.erase(pipes.begin());

			// Gerar um novo par de canos e adicioná-lo à direita da tela
			float distanceBetweenPipes = 250.0;
			float lastPipeXPosition = pipes.back()->topSprite->pos.x;
			createPipePair(lastPipeXPosition + distanceBetweenPipes);
    }

		if (hasStartedGame && !isReadyToPlay) {
			initialAnimation();
		}
		renderScore();

		bird->draw(hasStartedGame, isReadyToPlay, isGameOver);
		
		if (isGameOver && hasClickedEnter) {
			reset();
		}

		glfwSwapBuffers(window);
	}
};

void Game::end() {
  glfwTerminate();
};

void Game::createBackground() {
	Sprite *sprite;
	sprite = new Sprite;
	GLuint backgroundTexId = loadTexture("../textures/flappyBirdBackground.png");
	
	sprite->setShader(shader);
	sprite->initialize(backgroundTexId, glm::vec3(400.0, 300.0, 0.0), glm::vec3(800.0, 600.0, 1.0));
	this->background = sprite;
};

void Game::createPlayer() {	
	Sprite *sprite;
	sprite = new Sprite;
	GLuint birdTexId = loadTexture("../textures/flappyBird.png");

	sprite->setShader(shader);
	sprite->initialize(birdTexId, glm::vec3(60.0, 300.0, 0.0), glm::vec3(birdWidth, birdHeight, 1.0));

	Bird *bird;
	bird = new Bird(sprite);
	this->bird = bird;
};

void Game::createPipes() {
	srand(static_cast<unsigned int>(time(nullptr)));
	float distanceBetweenPipes = 250.0; // Distância fixa entre os canos
	float initialXPosition = 1000.0; // Posição inicial dos canos
	float lastPipeXPosition = initialXPosition;
	
	// Gerar os primeiros pares de canos
	for (int i = 0; i <= 6; ++i) {
		createPipePair(lastPipeXPosition);
		lastPipeXPosition += distanceBetweenPipes;
	}
};

void Game::createPipePair(float xPosition) {
	float minPositionBottom = 95.0;
	float minHeight = 20.0;
	float bottomPipeMaxHeight = 400.0;

	int randomHeight = minHeight + rand() % static_cast<int>(bottomPipeMaxHeight - minHeight + 1);
	
	float bottomPipeHeight = static_cast<float>(randomHeight);
	float bottomYPosition = (minPositionBottom + bottomPipeHeight) / 2.0;

	Sprite *bottomSprite = new Sprite;
	GLuint bottomPipeTexId = loadTexture("../textures/pipeBottom.png");
	bottomSprite->setShader(shader);
	bottomSprite->initialize(bottomPipeTexId, glm::vec3(xPosition, bottomYPosition, 0.0), glm::vec3(pipeWidth, bottomPipeHeight, 1.0));

	float topPipeHeight = HEIGHT - minPositionBottom - pipeGap - bottomPipeHeight;
	float topYPosition = HEIGHT - (topPipeHeight / 2.0);

	Sprite *topSprite = new Sprite;
	GLuint topPipeTexId = loadTexture("../textures/pipeTop.png");
	topSprite->setShader(shader);
	topSprite->initialize(topPipeTexId, glm::vec3(xPosition, topYPosition, 0.0), glm::vec3(pipeWidth, topPipeHeight, 1.0));

	Pipe *pipePair = new Pipe(topSprite, bottomSprite);
	this->pipes.push_back(pipePair);
}

void Game::checkPassingThroughPipes(glm::vec3 birdPosition, Pipe *pipe) {
	glm::vec3 topPipePosition = pipe->getTopPipePosition();
	glm::vec3 bottomPipePosition = pipe->getBottomPipePosition();
	
	// Calcula as coordenadas x mínima e máxima do cano
	float pipeMinX = topPipePosition.x - pipeWidth / 2.0f;
	float pipeMaxX = topPipePosition.x + pipeWidth / 2.0f;
	
	// Calcula as coordenadas y mínima e máxima do espaço entre os canos
	float gapMinY = bottomPipePosition.y + pipeGap / 2.0f;
	float gapMaxY = topPipePosition.y - pipeGap / 2.0f;

	// Verifica se o pássaro está dentro das coordenadas do cano e do espaço entre os canos
	if (birdPosition.x + birdWidth / 2.0f >= pipeMinX && birdPosition.x - birdWidth / 2.0f <= pipeMaxX) {
		if (birdPosition.y + birdWidth / 2.0f > gapMinY && birdPosition.y - birdWidth / 2.0f < gapMaxY) {
			// O pássaro passou pelos canos
			pipe->setHasBirdPassed(true);
			score++;
		} else {
			// O pássaro colidiu com o cano
			isGameOver = true;
		}
	}
}


void Game::checkCollisionTopBottom() {
	if (bird->getPosition().y - birdWidth/2 <= 90.00 || bird->getPosition().y + birdWidth/2 >= HEIGHT) {
		isGameOver = true;
	}
}

void Game::initialAnimation() {
	static std::chrono::steady_clock::time_point animationStartTime;
	static bool animationStarted = false;

	if (!animationStarted) {
    animationStartTime = std::chrono::steady_clock::now();
    animationStarted = true;
  }
  std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	double elapsedTime = std::chrono::duration_cast<std::chrono::duration<double>>(currentTime - animationStartTime).count();

	if (elapsedTime <= animationMaxTime) {
		if (elapsedTime - animationLastTime >= animationInterval) {
			bird->sprite->pos.x += 10;
			animationLastTime = elapsedTime;
		}
	} else {
		isReadyToPlay = true; 
		animationStarted = false;
	}
}

void Game::reset() {
	score = 0;
	isGameOver = false;
	isReadyToPlay = false;
	hasStartedGame = false;
	hasClickedEnter = false;
	animationLastTime = 0.0;
	bird->sprite->pos = glm::vec3(60.0, 300.0, 0.0);
	bird->sprite->angle = 0.0;
	
	pipes.clear();
	createPipes();
}

void Game::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (isGameOver) {
			hasClickedSpace = true;
			return;
		}
		if (!hasStartedGame) {
			hasStartedGame = true;
			return;
		}
		hasClickedSpace = true;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		hasClickedSpace = false;
	}
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		if (isGameOver) {
			hasClickedEnter = true;
			return;
		}
	}
};

GLuint Game::loadTexture(string texturePath) {
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
};

void Game::renderScore() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Configura a posição e o tamanho da janela para cobrir toda a tela
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
	ImGui::SetNextWindowBgAlpha(0.0f); // Torna o fundo da janela transparente
	ImGui::Begin("Score", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	// Coloca o texto no centro em x e mais para cima em y
	ImGui::SetCursorPosX((WIDTH - ImGui::CalcTextSize("Score: 9999").x) * 0.5f);
	ImGui::SetCursorPosY((HEIGHT - ImGui::CalcTextSize("Score: 9999").y) * 0.1f);

	// Aumenta a escala da fonte
	ImGui::SetWindowFontScale(5.0f);

	// Exibe o texto da pontuação
	ImGui::Text("%d", score);

	// Redefine a escala da fonte para o tamanho padrão
	ImGui::SetWindowFontScale(1.0f);

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}