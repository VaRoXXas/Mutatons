#include "pch.h"
#include "Settings.h"
#include "Util.h"
#include "Input.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "GraphNode.h"

// Shaders
#include "VertexShaders.h"
#include "GeometryShaders.h"
#include "FragmentShaders.h"

bool sceneExplorationModeEnabled = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat currentFrame = 0.0f;
Shader* litTexturedShaderPtr;
Shader* orbitShaderPtr;
Shader* sphereShaderPtr;
Shader* litTexturedInstancedShaderPtr;
Shader* lineShaderPtr;
Shader* refractShaderPtr;
glm::mat4* transformPtr;
glm::mat4* modelPtr;
glm::mat4* viewPtr;
glm::mat4* projectionPtr;
glm::mat4 androidTransform = glm::mat4(1.0f);
glm::mat4 androidHeadTransform = glm::mat4(1.0f);
glm::vec3 lineShaderEndPointPos;
GraphNode* house; // na pojedyncze domki
GraphNode* roof; // na pojedyncze dachy
int* meshDetailLevelPtr;
bool* cursorDisabledPtr;
void (*RKeyAction)();
void (*TKeyAction)();
void (*EKeyAction)();
void (*QKeyAction)();
void (*WKeyAction)();
void (*SKeyAction)();
void (*AKeyAction)();
void (*DKeyAction)();
static GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, planeVAO, planeVBO, instancedCubeVAO, instancedPyramidVAO, instanceVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO;
static GLuint houseBase_diffuse, roof_diffuse, plane_diffuse, houseBase_specular, roof_specular, plane_specular, cubemapTexture;
static const int HOUSES_ROW_LENGTH = 200;
static const int HOUSES_COUNT = HOUSES_ROW_LENGTH * HOUSES_ROW_LENGTH; // 200 x 200 = 40000
static const float ANDROID_SPEED = 1.5f;

void ToggleCursor();
void ToggleSceneExplorationMode();
void MoveCameraForward();
void MoveCameraBackward();
void MoveCameraLeft();
void MoveCameraRight();
void RotateAndroidHeadLeft();
void RotateAndroidHeadRight();
void MoveAndroidForward();
void MoveAndroidBackward();
void MoveAndroidLeft();
void MoveAndroidRight();
void DrawOrbit(const float radius, float* color, glm::mat4 transform);
void DrawSphere(const float radius, float* color, glm::mat4 transform);
void DrawPlane(glm::mat4 transform);
void DrawCube(glm::mat4 transform);
void DrawPyramid(glm::mat4 transform);
void DrawHouses(glm::mat4 transform);
void DrawLine(glm::mat4 transform);
void DrawReflected(glm::mat4 transform);
void DrawRefracted(glm::mat4 transform);
unsigned int LoadTexture(char const* path);
unsigned int LoadCubemap(std::vector<std::string> faces);



int main()
{
	//Util::HideTerminal();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Util::FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, Input::MouseCallback);
	//glfwSetScrollCallback(window, Input::ScrollCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Blocking cursor inside a window and disabling its graphic representation.

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);



#pragma region shaders init

	glEnable(GL_DEPTH_TEST);
	Shader litTexturedShader(LitTexturedV, LitTexturedF);
	Shader orbitShader(UnlitV, OrbitG, UnlitF);
	Shader sphereShader(UnlitV, SphereG, UnlitF);
	Shader litTexturedInstancedShader(LitTexturedInstancedV, LitTexturedF);
	Shader lineShader(UnlitV, LineG, UnlitF);
	Shader skyboxShader(SkyboxV, SkyboxF);
	Shader refractShader(LitTexturedV, RefractF);
	litTexturedShaderPtr = &litTexturedShader;
	orbitShaderPtr = &orbitShader;
	sphereShaderPtr = &sphereShader;
	litTexturedInstancedShaderPtr = &litTexturedInstancedShader;
	lineShaderPtr = &lineShader;
	refractShaderPtr = &refractShader;

	litTexturedShader.Use();
	litTexturedShader.SetInt("material.diffuse", 0);
	litTexturedShader.SetInt("material.specular", 1);
	litTexturedInstancedShader.Use();
	litTexturedInstancedShader.SetInt("material.diffuse", 0);
	litTexturedInstancedShader.SetInt("material.specular", 1);

#pragma endregion



#pragma region models and textures

	// loaded models
	//Model* backpackModelPtr = new Model("res/models/backpack/backpack.obj");
	//Model* additionalModelPtr = new Model("res/models/statue/LibertStatue.obj");

	houseBase_diffuse = LoadTexture("res/textures/container_diffuse.png");
	houseBase_specular = LoadTexture("res/textures/container_specular.png");
	roof_diffuse = LoadTexture("res/textures/stone_diffuse.jpg");
	roof_specular = LoadTexture("res/textures/stone_specular.jpg");
	plane_diffuse = LoadTexture("res/textures/grass_diffuse.jpg");
	plane_specular = LoadTexture("res/textures/grass_specular.jpg");

	vector<std::string> faces
	{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
	};
	cubemapTexture = LoadCubemap(faces);

#pragma endregion



#pragma region render data

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// orbit
	GLfloat points[] = {
		//  Coordinates
			0.0f, 0.0f, 0.0f,
			0.5f, 0.0f, 0.0f, // for debugging
			0.2f, 0.4f, 0.0f  // for debugging
	};
	glGenVertexArrays(1, &orbitVAO);
	glBindVertexArray(orbitVAO);
	glGenBuffers(1, &orbitVBO);
	glBindBuffer(GL_ARRAY_BUFFER, orbitVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// sphere
	GLfloat triangles[] = {
		//   Vertex no. 1            Vertex no. 2            Vertex no. 3
			 0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,     0.0f,  1.0f,  0.0f,
			 0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,     0.0f,  1.0f,  0.0f,

			 0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,     0.0f, -1.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,     0.0f, -1.0f,  0.0f,
			 0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,     0.0f, -1.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,     0.0f, -1.0f,  0.0f
	};
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// plane
	float planeData[] = {
		// position            normal                 texture
		-0.5f, 0.0f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f,   100.0f,
		 0.5f, 0.0f, -0.5f,    0.0f,  1.0f,  0.0f,    100.0f, 100.0f,
		-0.5f, 0.0f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f,   0.0f,
						     
		 0.5f, 0.0f, -0.5f,    0.0f,  1.0f,  0.0f,    100.0f, 100.0f,
		-0.5f, 0.0f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f,   0.0f,
		 0.5f, 0.0f,  0.5f,    0.0f,  1.0f,  0.0f,    100.0f, 0.0f
	};
	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeData), planeData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Additional things for instanced houses.
	glm::vec3* translations = new glm::vec3[HOUSES_COUNT];
	int index = 0;
	float offset = 2.0f;
	for (int z = -HOUSES_ROW_LENGTH / 2; z < HOUSES_ROW_LENGTH / 2; z++)
	{
		for (int x = -HOUSES_ROW_LENGTH / 2; x < HOUSES_ROW_LENGTH / 2; x++)
		{
			glm::vec3* translation = new glm::vec3(1.0f);
			*translation = glm::vec3((float)x * offset, 0.0f, (float)z * offset);
			translations[index] = *translation;
			index++;
		}
	}
	for(int i = 0; i < HOUSES_COUNT; i++)
	{
			
	}
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * HOUSES_COUNT, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// house base (cube)
	float cubeData[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &instancedCubeVAO);
	glBindVertexArray(instancedCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// also set instance data
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
	
	// roof (pyramid)
	float pyramidData[] = {
		// position			    // normal										     // texture coord
		//base																	
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,								     0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,								     1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,								     0.0f, 1.0f,

		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,								     1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,								     0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,								     1.0f, 1.0f,

		//front
		-0.5f, -0.5f,  0.5f,    0.0f, sqrtf(1.0f / 5), 2 * sqrtf(1.0f / 5),     0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,    0.0f, sqrtf(1.0f / 5), 2 * sqrtf(1.0f / 5),     1.0f, 0.0f,
		 0.0f,  0.5f,  0.0f,    0.0f, sqrtf(1.0f / 5), 2 * sqrtf(1.0f / 5),     0.5f, 1.0f,

		// right
		 0.5f, -0.5f,  0.5f,    2 * sqrtf(1.0f / 5), sqrtf(1.0f / 5), 0.0f,     0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,    2 * sqrtf(1.0f / 5), sqrtf(1.0f / 5), 0.0f,     1.0f, 0.0f,
		 0.0f,  0.5f,  0.0f,    2 * sqrtf(1.0f / 5), sqrtf(1.0f / 5), 0.0f,     0.5f, 1.0f,

		// back
		 0.5f, -0.5f, -0.5f,    0.0f, sqrtf(1.0f / 5), -2 * sqrtf(1.0f / 5),    0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, sqrtf(1.0f / 5), -2 * sqrtf(1.0f / 5),    1.0f, 0.0f,
		 0.0f,  0.5f,  0.0f,    0.0f, sqrtf(1.0f / 5), -2 * sqrtf(1.0f / 5),    0.5f, 1.0f,

		// left
		-0.5f, -0.5f, -0.5f,    -2 * sqrtf(1.0f / 5), sqrtf(1.0f / 5), 0.0f,    0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,    -2 * sqrtf(1.0f / 5), sqrtf(1.0f / 5), 0.0f,    1.0f, 0.0f,
		 0.0f,  0.5f,  0.0f,    -2 * sqrtf(1.0f / 5), sqrtf(1.0f / 5), 0.0f,    0.5f, 1.0f
	};
	glGenVertexArrays(1, &pyramidVAO);
	glBindVertexArray(pyramidVAO);
	glGenBuffers(1, &pyramidVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidData), pyramidData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &instancedPyramidVAO);
	glBindVertexArray(instancedPyramidVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// also set instance data
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.

#pragma endregion



#pragma region imgui init

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	// imgui variables
	bool xRotationEnabled = false;
	bool yRotationEnabled = false;
	bool wireframeModeEnabled = false;
	bool cursorDisabled = false;
	cursorDisabledPtr = &cursorDisabled;
	int meshDetailLevel = 3;
	bool directionalLightEnabled = true;
	bool spotLight1Enabled = true;
	bool spotLight2Enabled = true;
	bool pointLightEnabled = true;
	bool lightsPositionsDirectionsShown = true;
	float lightsDirectionVectorAngleOffset = 0.0f;
	float* directionalLightColor = new float[4];
	float* spotLight1Color = new float[4];
	float* spotLight2Color = new float[4];
	float* pointLightColor = new float[4];

	meshDetailLevelPtr = &meshDetailLevel;
	std::vector<float*>* lightColors = new std::vector<float*>();
	lightColors->push_back(directionalLightColor);
	lightColors->push_back(spotLight1Color);
	lightColors->push_back(spotLight2Color);
	lightColors->push_back(pointLightColor);
	for (auto* colorPtr : (*lightColors))
	{
		for (unsigned short i = 0; i < 4; i++)
		{
			colorPtr[i] = 1.0f;
		}
	}
	delete lightColors;

	// Assigning keyboard actions.
	RKeyAction = &ToggleCursor;
	TKeyAction = &ToggleSceneExplorationMode;
	EKeyAction = &MoveCameraRight;
	QKeyAction = &MoveCameraLeft;
	WKeyAction = &MoveCameraForward;
	SKeyAction = &MoveCameraBackward;
	AKeyAction = &MoveCameraLeft;
	DKeyAction = &MoveCameraRight;

#pragma endregion



#pragma region mvpt matrices init

	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	transformPtr = &transform;
	modelPtr = &model;
	viewPtr = &view;
	projectionPtr = &projection;

#pragma endregion



	// game loop
	while (!glfwWindowShouldClose(window))
	{
		currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// get input
		Input::ProcessInput(window);

		// updating matrices, uniforms, vectors
		transform = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		if (xRotationEnabled)
			model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		if (yRotationEnabled)
			model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

		litTexturedShader.ApplyMVPT();
		orbitShader.ApplyMVPT();
		sphereShader.ApplyMVPT();
		litTexturedInstancedShader.ApplyMVPT();
		lineShader.ApplyMVPT();
		refractShader.ApplyMVPT();

		orbitShader.Use();
		orbitShader.SetInt("sidesCount", 64);
		sphereShader.Use();
		sphereShader.SetInt("meshDetailLevel", meshDetailLevel);
		lineShader.Use();
		lineShader.SetVecf4("color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));



#pragma region lights

		Shader& lt = litTexturedShader;
		Shader& lti = litTexturedInstancedShader;

		const float pointLightCircleRadius = 5.0f;
		const float pointLightLinearVal = 0.35f; // Im wartosc mniejsza, tym wiekszy wplyw na dalsze obiekty.
		const float pointLightQuadraticVal = 0.44f; // Im wartosc mniejsza, tym wiekszy wplyw na blizsze obiekty.
		const float spotLightCutOffAngle = 12.5f;
		float darkness[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glm::vec3 pointLightPos = glm::vec3(pointLightCircleRadius * cos((GLfloat)glfwGetTime() + 1.0f), 1.0f, pointLightCircleRadius * sin((GLfloat)glfwGetTime() + 1.0f));
		glm::vec3 spotLight1Pos = glm::vec3(1.0f, 1.0f, 0.0f);
		glm::vec3 spotLight2Pos = glm::vec3(-1.0f, 0.5f, 0.5f);
		glm::vec3 directionalLightsDirection = glm::vec3(0.2f, -1.0f, 0.3f + lightsDirectionVectorAngleOffset);
		glm::vec3 lightColor;
		glm::vec3 diffuseColor;
		glm::vec3 ambientColor;

		lt.Use();
		lt.SetVecf3("viewPos", camera.position);
		lt.SetFloat("material.shininess", 16.0f);
		lt.SetVecf3("dirLight.direction", directionalLightsDirection);
		lt.SetVecf3("pointLight.position", pointLightPos);
		lt.SetFloat("pointLight.constant", 1.0f);
		lt.SetFloat("pointLight.linear", pointLightLinearVal);
		lt.SetFloat("pointLight.constant", pointLightQuadraticVal);
		lt.SetVecf3("spotLight1.position", spotLight1Pos);
		lt.SetVecf3("spotLight2.position", spotLight2Pos);
		lt.SetVecf3("spotLight1.direction", glm::vec3(-0.2f, -1.0f, -0.3f + lightsDirectionVectorAngleOffset));
		lt.SetVecf3("spotLight2.direction", glm::vec3(0.3f, -0.7f, 1.3f + lightsDirectionVectorAngleOffset));
		lt.SetFloat("spotLight1.cutOff", glm::cos(glm::radians(spotLightCutOffAngle)));
		lt.SetFloat("spotLight2.cutOff", glm::cos(glm::radians(spotLightCutOffAngle)));
		if (directionalLightEnabled)
		{
			lightColor = glm::vec3(directionalLightColor[0], directionalLightColor[1], directionalLightColor[2]);
			diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
			lt.SetVecf3("dirLight.ambient", ambientColor);
			lt.SetVecf3("dirLight.diffuse", diffuseColor);
			lt.SetVecf3("dirLight.specular", glm::vec3(0.2f));
		}
		else
		{
			lt.SetVecf3("dirLight.ambient", darkness);
			lt.SetVecf3("dirLight.diffuse", darkness);
			lt.SetVecf3("dirLight.specular", darkness);
		}
		if(pointLightEnabled)
		{
			lightColor = glm::vec3(pointLightColor[0], pointLightColor[1], pointLightColor[2]);
			diffuseColor = lightColor * glm::vec3(0.6f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
			lt.SetVecf3("pointLight.ambient", ambientColor);
			lt.SetVecf3("pointLight.diffuse", diffuseColor);
			lt.SetVecf3("pointLight.specular", glm::vec3(0.6f));
		}
		else
		{
			lt.SetVecf3("pointLight.ambient", darkness);
			lt.SetVecf3("pointLight.diffuse", darkness);
			lt.SetVecf3("pointLight.specular", darkness);
		}
		if(spotLight1Enabled)
		{
			lightColor = glm::vec3(spotLight1Color[0], spotLight1Color[1], spotLight1Color[2]);
			diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.1f); // low influence
			lt.SetVecf3("spotLight1.ambient", ambientColor);
			lt.SetVecf3("spotLight1.diffuse", diffuseColor);
			lt.SetVecf3("spotLight1.specular", glm::vec3(1.0f));
		}
		else
		{
			lt.SetVecf3("spotLight1.ambient", darkness);
			lt.SetVecf3("spotLight1.diffuse", darkness);
			lt.SetVecf3("spotLight1.specular", darkness);
		}
		if (spotLight2Enabled)
		{
			lightColor = glm::vec3(spotLight2Color[0], spotLight2Color[1], spotLight2Color[2]);
			diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.1f); // low influence
			lt.SetVecf3("spotLight2.ambient", ambientColor);
			lt.SetVecf3("spotLight2.diffuse", diffuseColor);
			lt.SetVecf3("spotLight2.specular", glm::vec3(1.0f));
		}						  
		else					  
		{						  
			lt.SetVecf3("spotLight2.ambient", darkness);
			lt.SetVecf3("spotLight2.diffuse", darkness);
			lt.SetVecf3("spotLight2.specular", darkness);
		}

		
		lti.Use();
		lti.SetVecf3("viewPos", camera.position);
		lti.SetFloat("material.shininess", 16.0f);
		lti.SetVecf3("dirLight.direction", directionalLightsDirection);
		lti.SetVecf3("pointLight.position", pointLightPos);
		lti.SetFloat("pointLight.constant", 1.0f);
		lti.SetFloat("pointLight.linear", pointLightLinearVal);
		lti.SetFloat("pointLight.constant", pointLightQuadraticVal);
		lti.SetVecf3("spotLight1.position", spotLight1Pos);
		lti.SetVecf3("spotLight2.position", spotLight2Pos);
		lti.SetVecf3("spotLight1.direction", glm::vec3(-0.2f, -1.0f, -0.3f + lightsDirectionVectorAngleOffset));
		lti.SetVecf3("spotLight2.direction", glm::vec3(0.3f, -0.7f, 1.3f + lightsDirectionVectorAngleOffset));
		lti.SetFloat("spotLight1.cutOff", glm::cos(glm::radians(spotLightCutOffAngle)));
		lti.SetFloat("spotLight2.cutOff", glm::cos(glm::radians(spotLightCutOffAngle)));
		if(directionalLightEnabled)
		{
			lightColor = glm::vec3(directionalLightColor[0], directionalLightColor[1], directionalLightColor[2]);
			diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
			lti.SetVecf3("dirLight.ambient", ambientColor);
			lti.SetVecf3("dirLight.diffuse", diffuseColor);
			lti.SetVecf3("dirLight.specular", glm::vec3(0.2f));
		}
		else
		{
			lti.SetVecf3("dirLight.ambient", darkness);
			lti.SetVecf3("dirLight.diffuse", darkness);
			lti.SetVecf3("dirLight.specular", darkness);
		}
		if (pointLightEnabled)
		{
			lightColor = glm::vec3(pointLightColor[0], pointLightColor[1], pointLightColor[2]);
			diffuseColor = lightColor * glm::vec3(0.6f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
			lti.SetVecf3("pointLight.ambient", ambientColor);
			lti.SetVecf3("pointLight.diffuse", diffuseColor);
			lti.SetVecf3("pointLight.specular", glm::vec3(0.6f));
		}
		else
		{
			lti.SetVecf3("pointLight.ambient", darkness);
			lti.SetVecf3("pointLight.diffuse", darkness);
			lti.SetVecf3("pointLight.specular", darkness);
		}
		if (spotLight1Enabled)
		{
			lightColor = glm::vec3(spotLight1Color[0], spotLight1Color[1], spotLight1Color[2]);
			diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.1f); // low influence
			lti.SetVecf3("spotLight1.ambient", ambientColor);
			lti.SetVecf3("spotLight1.diffuse", diffuseColor);
			lti.SetVecf3("spotLight1.specular", glm::vec3(1.0f));
		}
		else
		{
			lti.SetVecf3("spotLight1.ambient", darkness);
			lti.SetVecf3("spotLight1.diffuse", darkness);
			lti.SetVecf3("spotLight1.specular", darkness);
		}
		if (spotLight2Enabled)
		{
			lightColor = glm::vec3(spotLight2Color[0], spotLight2Color[1], spotLight2Color[2]);
			diffuseColor = lightColor * glm::vec3(0.8f); // decrease the influence
			ambientColor = diffuseColor * glm::vec3(0.1f); // low influence
			lti.SetVecf3("spotLight2.ambient", ambientColor);
			lti.SetVecf3("spotLight2.diffuse", diffuseColor);
			lti.SetVecf3("spotLight2.specular", glm::vec3(1.0f));
		}
		else
		{
			lti.SetVecf3("spotLight2.ambient", darkness);
			lti.SetVecf3("spotLight2.diffuse", darkness);
			lti.SetVecf3("spotLight2.specular", darkness);
		}

#pragma endregion



		// render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Util::DisableWireframeMode();
		if (wireframeModeEnabled)
			Util::EnableWireframeMode();
		if (cursorDisabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Blocking cursor inside a window and disabling its graphic representation.
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);



		glm::mat4 planeTransform = glm::translate(transform, glm::vec3(0.0f, -0.5f * 0.2f - 0.0001f, 0.0f));
		planeTransform = glm::scale(planeTransform, glm::vec3(100.0f, 100.0f, 100.0f));
		glm::mat4 originPointTransform = glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f));

		// setting up scene graph
		GraphNode rootNode(transform);

		float originPointColor[4] = { 0.45f, 0.25f, 0.45f, 1.0f };
		PseudoMesh originPoint(DrawSphere, 0.2f, originPointColor); GraphNode originPointNode(&originPoint, glm::rotate(originPointTransform, (GLfloat)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f)));
		PseudoMesh plane(DrawPlane); GraphNode planeNode(&plane, planeTransform);
		lineShaderEndPointPos = directionalLightsDirection;
		PseudoMesh directionalLightIndicator(DrawLine); GraphNode directionalLightIndicatorNode(&directionalLightIndicator, glm::translate(transform, glm::vec3(0.0f, 2.0f, 0.0f)));
		PseudoMesh pointLightIndicator(DrawSphere, 0.2f, pointLightColor); GraphNode pointLightIndicatorNode(&pointLightIndicator, glm::translate(transform, pointLightPos));
		PseudoMesh spotLight1Indicator(DrawSphere, 0.05f, spotLight1Color); GraphNode spotLight1IndicatorNode(&spotLight1Indicator, glm::translate(transform, spotLight1Pos));
		PseudoMesh spotLight2Indicator(DrawSphere, 0.05f, spotLight2Color); GraphNode spotLight2IndicatorNode(&spotLight2Indicator, glm::translate(transform, spotLight2Pos));
		PseudoMesh reflectedObj(DrawReflected); GraphNode reflectedObjNode(&reflectedObj, glm::translate(transform, glm::vec3(3.0f, 3.0f, 0.0f)));
		PseudoMesh refractedObj(DrawRefracted); GraphNode refractedObjNode(&refractedObj, glm::translate(transform, glm::vec3(-3.0f, 3.0f, 0.0f)));
		
		glm::mat4 androidStartTransform = glm::translate(androidTransform, glm::vec3(-2.0f, 1.4f, 0.0f));
		glm::mat4 androidRightArmTransform = glm::scale(glm::translate(androidStartTransform, glm::vec3(-1.0f, 0.25f, 0.0f)), glm::vec3(1.0f, 0.25f, 0.25f));
		glm::mat4 androidLeftArmTransform = glm::translate(androidRightArmTransform, glm::vec3(2.0f, 0.0f, 0.0f));
		glm::mat4 androidRightLegTransform = glm::scale(glm::translate(androidStartTransform, glm::vec3(0.25f, -1.0f, 0.0f)), glm::vec3(0.25f, 1.0f, 0.25f));
		glm::mat4 androidLeftLegTransform = glm::translate(androidRightLegTransform, glm::vec3(-2.0f, 0.0f, 0.0f));
		//glm::mat4 androidHeadTransformFinal = glm::scale(glm::translate(androidStartTransform, glm::vec3(0.0f, 0.75f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 androidHeadTransformFinal = glm::translate(androidStartTransform, glm::vec3(0.0f, 0.75f, 0.0f));
		androidHeadTransformFinal = androidHeadTransformFinal * androidHeadTransform;
		androidHeadTransformFinal = glm::scale(androidHeadTransformFinal, glm::vec3(0.5f, 0.5f, 0.5f));
		
		GraphNode androidMainNode(transform);
		PseudoMesh androidTorso(DrawCube); GraphNode androidTorsoNode(&androidTorso, androidStartTransform);
		PseudoMesh androidRightArm(DrawRefracted); GraphNode androidRightArmNode(&androidRightArm, androidRightArmTransform);
		PseudoMesh androidLeftArm(DrawRefracted); GraphNode androidLeftArmNode(&androidLeftArm, androidLeftArmTransform);
		PseudoMesh androidRightLeg(DrawRefracted); GraphNode androidRightLegNode(&androidRightLeg, androidRightLegTransform);
		PseudoMesh androidLeftLeg(DrawRefracted); GraphNode androidLeftLegNode(&androidLeftLeg, androidLeftLegTransform);
		PseudoMesh androidHead(DrawReflected); GraphNode androidHeadNode(&androidHead, androidHeadTransformFinal);

		if(!sceneExplorationModeEnabled)
			camera.position = glm::vec3(androidStartTransform[3]) + glm::vec3(0.0f, 1.5f, 3.0f);
		
		
		// parenting
		rootNode.AddChild(&planeNode);
		//rootNode.AddChild(&originPointNode); // Wskazuje srodek swiata.
		rootNode.AddChild(&androidMainNode);
		androidMainNode.AddChild(&androidTorsoNode);
		androidMainNode.AddChild(&androidRightArmNode);
		androidMainNode.AddChild(&androidLeftArmNode);
		androidMainNode.AddChild(&androidRightLegNode);
		androidMainNode.AddChild(&androidLeftLegNode);
		androidMainNode.AddChild(&androidHeadNode);

		//rootNode.AddChild(&reflectedObjNode);
		//rootNode.AddChild(&refractedObjNode);
		
		// OPTIONAL
		if(lightsPositionsDirectionsShown)
		{
			if (directionalLightEnabled)
				rootNode.AddChild(&directionalLightIndicatorNode);
			if (pointLightEnabled)
				rootNode.AddChild(&pointLightIndicatorNode);
			if (spotLight1Enabled)
				rootNode.AddChild(&spotLight1IndicatorNode);
			if (spotLight2Enabled)
				rootNode.AddChild(&spotLight2IndicatorNode);
		}



		// drawing scene
		rootNode.Render();

		// skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.SetMat4("view", view);
		skyboxShader.SetMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default



		// UI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Checkbox("X axis rotation", &xRotationEnabled);
		ImGui::Checkbox("Y axis rotation", &yRotationEnabled);
		ImGui::Checkbox("Wireframe mode", &wireframeModeEnabled);
		ImGui::Checkbox("Cursor disabled", &cursorDisabled);
		ImGui::SliderInt("Sphere detail level", &meshDetailLevel, 0, 3);
		ImGui::Checkbox("Directional light enabled", &directionalLightEnabled);
		ImGui::Checkbox("Point light enabled", &pointLightEnabled);
		ImGui::Checkbox("Spot light 1 enabled", &spotLight1Enabled);
		ImGui::Checkbox("Spot light 2 enabled", &spotLight2Enabled);
		ImGui::Checkbox("Lights' positions/directions shown", &lightsPositionsDirectionsShown);
		ImGui::SliderFloat("Lights' direction vector angle offset", &lightsDirectionVectorAngleOffset, -5.0f, 5.0f);
		ImGui::ColorEdit4("Directional light color", directionalLightColor);
		ImGui::ColorEdit4("Spot light 1 color", spotLight1Color);
		ImGui::ColorEdit4("Spot light 2 color", spotLight2Color);
		ImGui::ColorEdit4("Point light color", pointLightColor);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocation
	glDeleteVertexArrays(1, &orbitVAO);
	glDeleteBuffers(1, &orbitVBO);
	//delete backpackModelPtr;
	//delete additionalModelPtr;
	delete[] translations;
	delete[] directionalLightColor;
	delete[] spotLight1Color;
	delete[] spotLight2Color;
	delete[] pointLightColor;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}



void ToggleCursor()
{
	(*cursorDisabledPtr) = !(*cursorDisabledPtr);
	std::this_thread::sleep_for(0.25s);
}

void ToggleSceneExplorationMode()
{
	sceneExplorationModeEnabled = !sceneExplorationModeEnabled;
	if (sceneExplorationModeEnabled)
	{
		EKeyAction = &MoveCameraRight;
		QKeyAction = &MoveCameraLeft;
		WKeyAction = &MoveCameraForward;
		SKeyAction = &MoveCameraBackward;
		AKeyAction = &MoveCameraLeft;
		DKeyAction = &MoveCameraRight;
	}
	else
	{
		EKeyAction = &RotateAndroidHeadRight;
		QKeyAction = &RotateAndroidHeadLeft;
		WKeyAction = &MoveAndroidForward;
		SKeyAction = &MoveAndroidBackward;
		AKeyAction = &MoveAndroidLeft;
		DKeyAction = &MoveAndroidRight;
	}
	std::this_thread::sleep_for(0.25s);
}

void MoveCameraForward()
{
	camera.ProcessKeyboard(Forward, deltaTime);
}

void MoveCameraBackward()
{
	camera.ProcessKeyboard(Backward, deltaTime);
}

void MoveCameraLeft()
{
	camera.ProcessKeyboard(Left, deltaTime);
}

void MoveCameraRight()
{
	camera.ProcessKeyboard(Right, deltaTime);
}

void RotateAndroidHeadLeft()
{
	androidHeadTransform = glm::rotate(androidHeadTransform, 1.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
}

void RotateAndroidHeadRight()
{
	androidHeadTransform = glm::rotate(androidHeadTransform, -1.0f * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
}

void MoveAndroidForward()
{
	androidTransform = glm::translate(androidTransform, glm::vec3(0.0f, 0.0f, -1.0f) * deltaTime * ANDROID_SPEED);
}

void MoveAndroidBackward()
{
	androidTransform = glm::translate(androidTransform, glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * ANDROID_SPEED);
}

void MoveAndroidLeft()
{
	androidTransform = glm::translate(androidTransform, glm::vec3(-1.0f, 0.0f, 0.0f) * deltaTime * ANDROID_SPEED);
}

void MoveAndroidRight()
{
	androidTransform = glm::translate(androidTransform, glm::vec3(1.0f, 0.0f, 0.00f) * deltaTime * ANDROID_SPEED);
}

void DrawOrbit(const float radius, float* color, glm::mat4 transform)
{
	orbitShaderPtr->Use();
	orbitShaderPtr->SetFloat("radius", radius);
	orbitShaderPtr->SetVecf4("color", color);
	orbitShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(orbitVAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void DrawSphere(const float radius, float* color, glm::mat4 transform)
{
	sphereShaderPtr->Use();
	sphereShaderPtr->SetFloat("radius", radius);
	sphereShaderPtr->SetVecf4("color", color);
	sphereShaderPtr->SetMat4("transform", transform);
	sphereShaderPtr->SetInt("meshDetailLevel", (*meshDetailLevelPtr));
	glBindVertexArray(sphereVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 8); // 3 vertices of the triangle * 8 triangles
	glBindVertexArray(0);
}

void DrawPlane(glm::mat4 transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, plane_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, plane_specular);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2); // 3 vertices of the triangle * 2 triangles
	glBindVertexArray(0);
}

void DrawCube(glm::mat4 transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, houseBase_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, houseBase_specular);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

void DrawPyramid(glm::mat4 transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, roof_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, roof_specular);
	litTexturedShaderPtr->Use();
	litTexturedShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(pyramidVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * (2 + 4)); // 3 vertices of the triangle * (2 triangles of base + 4 triangles of sides)
	glBindVertexArray(0);
}

void DrawHouses(glm::mat4 transform)
{
	litTexturedInstancedShaderPtr->Use();
	litTexturedInstancedShaderPtr->SetMat4("transform", transform);
	
	// bases
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, houseBase_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, houseBase_specular);
	glBindVertexArray(instancedCubeVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 2 * 6, HOUSES_COUNT);
	glBindVertexArray(0);

	// roofs
	const auto roofOffset = glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f));
	litTexturedInstancedShaderPtr->SetMat4("transform", roofOffset);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, roof_diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, roof_specular);
	glBindVertexArray(instancedPyramidVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * (2 + 4), HOUSES_COUNT);
	glBindVertexArray(0);
}

void DrawLine(glm::mat4 transform)
{
	lineShaderPtr->Use();
	lineShaderPtr->SetMat4("transform", transform);
	lineShaderPtr->SetVecf3("endPointPos", lineShaderEndPointPos);
	glBindVertexArray(orbitVAO); // Wykorzystujemy wczesniej uzywane VAO, poniewaz linia takze rysuje sie z punktu.
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void DrawReflected(glm::mat4 transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	refractShaderPtr->Use();
	refractShaderPtr->SetVecf3("viewPos", camera.position);
	refractShaderPtr->SetBool("refractMode", false);
	refractShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

void DrawRefracted(glm::mat4 transform)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	refractShaderPtr->Use();
	refractShaderPtr->SetVecf3("viewPos", camera.position);
	refractShaderPtr->SetBool("refractMode", true);
	refractShaderPtr->SetMat4("transform", transform);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6); // 3 vertices of the triangle * 2 triangles per side * 6 sides
	glBindVertexArray(0);
}

unsigned int LoadTexture(char const* path)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

unsigned int LoadCubemap(std::vector<std::string> faces)
{
	stbi_set_flip_vertically_on_load(false);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
