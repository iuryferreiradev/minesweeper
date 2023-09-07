#include "game.hpp"

void error_callback(int error, const char* description)
{
  std::cout << "ERROR::GLFW::CALLBACK\n" << description << std::endl;
}

Game::Game(int width, int height, const char* name)
{
  this->Height = height;
  this->Width = width;
  this->Name = name;
  this->window = nullptr;

  this->init();
}

Game::~Game()
{
  this->terminate();
}

void Game::terminate()
{
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

void Game::setupWindow()
{
  glfwSetErrorCallback(error_callback);

  // Initializing window manager library
  if(!glfwInit())
  {
    std::cout << "ERROR::GLFW::INIT" << std::endl;
    this->terminate();
  }

  // Creating game window
  this->window = glfwCreateWindow(this->Width, this->Height, this->Name, NULL, NULL);
  if(!window)
  {
    std::cout << "ERROR::GLFW::WINDOW" << std::endl;
    this->terminate();
  }

  // Setting opengl version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Disabling window resizing
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // Make the context of the window the current context
  glfwMakeContextCurrent(window);

  
  // Load opengl
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "ERROR::GLAD::LOAD" << std::endl;
    this->terminate();
  }

  glViewport(0, 0, this->Width, this->Height);
}

void Game::Run()
{
  float deltaTime = 0.0f;
  float lastTime = glfwGetTime();

  while(!glfwWindowShouldClose(this->window))
  {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    this->processInputs();
    this->update(deltaTime);
    this->render();
  }
}


void Game::init()
{
  this->setupWindow();
  ResourceManager::LoadShader("rect", "assets/shaders/quad.vert", "assets/shaders/quad.frag");
  float size = 24.0f;
  float offsetX = 40.0f;
  float offsetY = 176.0f;

  for(int j = 0; j < 30; j++)
  {
    for(int i = 0; i < 16; i++)
    {
      bool isEven = (i + j) % 2 == 0;
      glm::vec4 color = isEven ? GREEN : LIGHTGREEN;
      float x = offsetX + j * size;
      float y = offsetY + i * size;
      this->tiles.push_back(Tile(x, y, size, size, color));
    }
  }
}

void Game::render()
{
  Renderer renderer;
  // Set the clear color to white
  glClearColor(BACKGROUND.x, BACKGROUND.y, BACKGROUND.z, BACKGROUND.w);
  // Clear the window drawings
  glClear(GL_COLOR_BUFFER_BIT);

  for(auto tile : this->tiles)
  {
    tile.Render(renderer);
  }

  renderer.DrawRect(glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, 80.0f), DARKGREEN);
  // Swap the front buffer with the back buffer
  glfwSwapBuffers(window);

  // V-Sync, i think..
  glfwSwapInterval(1);
}

void Game::update(float deltaTime)
{

}

void Game::processInputs()
{
  // Manage window events: mousemove, clicks, keyboard, resize, close...
  glfwPollEvents();
}