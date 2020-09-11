#include "Engine.h"

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)


Engine::Engine(const char* title, int x, int y, uint16_t width, uint16_t height, Uint32 flags)
{
	
	initSDL(title, x, y, width, height, flags);

	//camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	inputManager = new InputManager();
	inputManager->bind(Game_State::INGAME, new Command_Definition(Game_Command::MOVE_FORWARD, "move_forward", SDL_Scancode::SDL_SCANCODE_W, true, true));
	inputManager->bind(Game_State::INGAME, new Command_Definition(Game_Command::MOVE_BACKWARD, "move_backward", SDL_Scancode::SDL_SCANCODE_S, true, true));
	inputManager->bind(Game_State::INGAME, new Command_Definition(Game_Command::MOVE_LEFT, "move_left", SDL_Scancode::SDL_SCANCODE_A, true, true));
	inputManager->bind(Game_State::INGAME, new Command_Definition(Game_Command::MOVE_RIGHT, "move_right", SDL_Scancode::SDL_SCANCODE_D, true, true));
	inputManager->bind(Game_State::INGAME, new Command_Definition(Game_Command::MOUSE_MOVE, "mouse", SDL_Scancode::SDL_SCANCODE_UNKNOWN, false, false));
	inputManager->bind(Game_State::INGAME, new Command_Definition(Game_Command::MOUSE_SCROLL, "scroll", SDL_Scancode::SDL_SCANCODE_UNKNOWN, false, false));

	//initialise the camera functions
	//inputManager->registerCommand(Game_Command::MOVE_FORWARD, [&]() { camera->ProcessKeyboardInput(Game_Command::MOVE_FORWARD); });
	//inputManager->registerCommand(Game_Command::MOVE_BACKWARD, [&]() { camera->ProcessKeyboardInput(Game_Command::MOVE_BACKWARD); });
	//inputManager->registerCommand(Game_Command::MOVE_LEFT, [&]() { camera->ProcessKeyboardInput(Game_Command::MOVE_LEFT); });
	//inputManager->registerCommand(Game_Command::MOVE_RIGHT, [&]() { camera->ProcessKeyboardInput(Game_Command::MOVE_RIGHT); });
	//inputManager->registerCommand(Game_Command::MOUSE_MOVE, [&]() { camera->ProcessMouseMovement(inputManager->xoffset, inputManager->yoffset, true); });
	//inputManager->registerCommand(Game_Command::MOUSE_SCROLL, [&]() { camera->ProcessMouseScroll(inputManager->scrolloffset); });

}
Engine::~Engine() {}

bool Engine::initSDL(const char* title, int x, int y, uint16_t width, uint16_t height, Uint32 flags)
{
	// initialise SDL Subsystems!!
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialise! SDL_Error: %s\n", SDL_GetError());
			// throw engine exception!!!
		return false;
	}	
	else {

		// Set to use OpenGL 3.3 and the Core Profile
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_CaptureMouse(SDL_TRUE);

		// initialise the window!!
		window = SDL_CreateWindow(title, x, y, width, height, flags);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			// throw engine exception!!!
			return false;
		}
		else {
			// initialise the renderer!!
			context = SDL_GL_CreateContext(window);
			if (context == NULL) {
				printf("OpenGL Context could not be created! SDL_Error: %s\n", SDL_GetError());
				// throw engine exception!!!
				return false;
			}
			else
			{
				// initialise GLEW
				glewExperimental = true;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error Initialising GLEW! %s\n", glewGetErrorString(glewError));
					// throw engine exception!!!
					return false;
				}

				glViewport(0, 0, width, height);

				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
					return false;
				}

				initGL();
			}
		}
	}
	running_ = true;
}

void Engine::initGL()
{

	glEnable(GL_DEPTH_TEST);

	shader = new Shader("Shaders/test.vert", "Shaders/test.frag");

	stbi_set_flip_vertically_on_load(true);
	if (!(image_data = stbi_load("angrylemon.png", &width, &height, &nrChannels, 0)))
	{
		printf("IMAGE DIDN't LOAD\n");
	}


	//generate VBO ID
	// build VAO first, then bind and set the VBO(s), the configure vertex attributes
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &_EBO);
	glGenTextures(1, &texture);

	glBindVertexArray(VAO);

	glBindTexture(GL_TEXTURE_2D, texture);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//colour
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);


	

	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	stbi_image_free(image_data);
}

void Engine::resize_screen(uint16_t nwidth, uint16_t nheight)
{
	glViewport(0, 0, nwidth, nheight);
}

void Engine::event()
{
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	int x, y;	
	SDL_GetMouseState(&x, &y);
	SDL_Event e;
	float wheel = 0.f;

	while (SDL_PollEvent(&e))
	{		
		switch (e.type)
		{
		case SDL_QUIT:
			running_ = false;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				resize_screen(e.window.data1, e.window.data2);
				break;
			}
		case SDL_MOUSEWHEEL:
			wheel = (float)e.wheel.y;
		case SDL_TEXTINPUT:
			if (e.text.text[0] == 'p')
			{
				wireframe = !wireframe;
				break;
			}
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				running_ = false;
				break;
			}
		}
	}
	inputManager->mouseInput(x, y, wheel);
	inputManager->handleInput(&e);
}

void Engine::update() 
{
	// update deltaTime
	float currentFrame = (float)SDL_GetPerformanceCounter();
	float deltaTime = (currentFrame - lastFrame) / (float)SDL_GetPerformanceFrequency() * 1000;
	lastFrame = currentFrame;
	//camera->getDeltaTime(deltaTime/1000);
}

void Engine::render()
{
	//clear renderer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//camera stuff
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader->use();

	//projection = glm::perspective(glm::radians(camera->Zoom()), 1920.0f / 1080.0f, 0.1f, 100.0f);
	shader->setMat4("projection", projection);
	//_model = glm::rotate(_model, ((float)SDL_GetTicks() / 1000) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	//view = camera->GetViewMatrix();
	shader->setMat4("view", view);
	model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	//Render things here!!!
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	SDL_GL_SwapWindow(window);
}

void Engine::close()
{
	delete shader;
	//delete camera;
	delete inputManager;
	// Delete the OpenGl Context
	SDL_GL_DeleteContext(context);
	printf("context deleted.\n");
	// Delete the Window
	SDL_DestroyWindow(window);
	printf("window closed.\n");
	// Quit SDL
	SDL_Quit();
	printf("SDL closed.\n");
}