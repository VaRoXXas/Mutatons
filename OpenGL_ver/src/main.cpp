#include "pch.h"
#include "Settings.h"
#include "Util.h"
#include "Camera.h"
#include "Input.h"
#include "DataManager.h"
#include "Rendering/Shader.h"
#include "Rendering/deprecated/Mesh.h" // TODO: Change this to new Mesh class file.
#include "Rendering/deprecated/Model.h" // TODO: Change this to new Model class file.
#include "Rendering/PseudoMesh.h"
#include "Rendering/CustomDrawing.h"
#include "Scene/GraphNode.h"

#include "Collisions.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "DataManager.h"
#include "GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"

// Shaders
#include "VertexShaders.h"
#include "GeometryShaders.h"
#include "FragmentShaders.h"

// input externs
extern bool cursorEnabled;
extern void (*wKeyActionPtr)();
extern void (*sKeyActionPtr)();
extern void (*aKeyActionPtr)();
extern void (*dKeyActionPtr)();
extern void (*rKeyActionPtr)();
extern void (*tKeyActionPtr)();
extern void (*eKeyActionPtr)();
extern void (*qKeyActionPtr)();

// renderer externs
extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;
extern Shader* lineShaderPtr;
extern Shader* refractShaderPtr;
extern Shader* skyboxShaderPtr;
extern Shader* unlitTexturedAnimatedPtr;
extern GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO;
extern GLuint houseBaseDiffuseTexture, roofDiffuseTexture, planeDiffuseTexture, houseBaseSpecularTexture, roofSpecularTexture, planeSpecularTexture, cubemapTexture;
extern glm::vec3 lineShaderEndPointPos;
extern int geometryShaderPseudoMeshDetailLevel;
extern bool directionalLightEnabled;
extern bool pointLightEnabled;
extern bool spotLight1Enabled;
extern bool spotLight2Enabled;
extern bool lightsPositionsDirectionsShown;
extern float lightsDirectionVectorAngleOffset;
extern float* directionalLightColorPtr;
extern float* spotLight1ColorPtr;
extern float* spotLight2ColorPtr;
extern float* pointLightColorPtr;
extern glm::vec3 directionalLightsDirection;
extern glm::vec3 pointLightPos;
extern glm::vec3 spotLight1Pos;
extern glm::vec3 spotLight2Pos;



bool IMGUI_ENABLED = true;
bool sceneExplorationModeEnabled = true;
DataManager dataManager = DataManager();
Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat deltaTime = 0.0f; // the difference between the current and the last frame
GLfloat lastFrame = 0.0f;
GLfloat currentFrame = 0.0f;
glm::mat4* transformMatrixPtr;
glm::mat4* modelMatrixPtr;
glm::mat4* viewMatrixPtr;
glm::mat4* projectionMatrixPtr;

glm::vec3* modelLocationPtr;
glm::vec3* gameObjectLocationPtr;
glm::vec3* objectScalePtr;
extern Model* modelPtr;

int main()
{
	//Util::HideTerminal();
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* windowPtr = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	
	if (windowPtr == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(windowPtr);
	glfwSetFramebufferSizeCallback(windowPtr, Util::FramebufferSizeCallback);
	glfwSetCursorPosCallback(windowPtr, Input::CursorPosCallback);
	glfwSetScrollCallback(windowPtr, Input::ScrollCallback);
	
	// [glad] Load all OpenGL function pointers.
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Setting window's background color.



#pragma region shaders init

	glEnable(GL_DEPTH_TEST);
	Shader litTexturedShader(s_litTexturedVertexPtr, s_litTexturedFragmentPtr);
	Shader orbitShader(s_unlitVertexPtr, s_orbitGeometryPtr, s_unlitFragmentPtr);
	Shader sphereShader(s_unlitVertexPtr, s_sphereGeometryPtr, s_unlitFragmentPtr);
	Shader litTexturedInstancedShader(s_litTexturedInstancedVertexPtr, s_litTexturedFragmentPtr);
	Shader lineShader(s_unlitVertexPtr, s_lineGeometryPtr, s_unlitFragmentPtr);
	Shader skyboxShader(s_skyboxVertexPtr, s_skyboxFragmentPtr);
	Shader refractShader(s_litTexturedVertexPtr, s_refractFragmentPtr);
	Shader unlitTexturedAnimated(s_unlitTexturedAnimatedVertexPtr, s_unlitTexturedFragmentPtr);
	litTexturedShaderPtr = &litTexturedShader;
	orbitShaderPtr = &orbitShader;
	sphereShaderPtr = &sphereShader;
	litTexturedInstancedShaderPtr = &litTexturedInstancedShader;
	lineShaderPtr = &lineShader;
	skyboxShaderPtr = &skyboxShader;
	refractShaderPtr = &refractShader;
	unlitTexturedAnimatedPtr = &unlitTexturedAnimated;

	Lighting::InitLighting(litTexturedShader);
	Lighting::InitLighting(litTexturedInstancedShader);

#pragma endregion

	//Data manager loads all models
	dataManager.LoadAllModels();
	//Location and size vectors' declaration
	glm::vec3 objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 modelLocation = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 gameObjectLocation = glm::vec3(1.0f, 1.0f, 5.0f);
	objectScalePtr = &objectScale;
	modelLocationPtr = &modelLocation;
	gameObjectLocationPtr = &gameObjectLocation;

	//First GameObject declaration
	GameObject* gameObjectPtr;
	GameObject objekt;
	objekt.SetTag("PierwszyGameObject");
	objekt.SetActive();
	//Adding components and setting up behavior
	std::shared_ptr<TransformComponent> tc = std::make_shared<TransformComponent>(*modelLocationPtr);
	objekt.AddComponent(tc);
	objekt.SetUpdate();
	objekt.SetDirection("forward");
	std::shared_ptr<GraphicsComponent> gp = std::make_shared<GraphicsComponent>();
	std::shared_ptr<ColliderComponent> col = std::make_shared<ColliderComponent>();
	objekt.AddComponent(gp);
	objekt.AddComponent(col);
	gameObjectPtr = &objekt;

	
	//First GameObject declaration
	GameObject* gameObjectPtr_2;
	GameObject objekt_2;
	objekt_2.SetActive();
	//Adding components and setting up behavior
	std::shared_ptr<TransformComponent> tc2 = std::make_shared<TransformComponent>(*gameObjectLocationPtr);
	objekt_2.AddComponent(tc2);
	objekt_2.SetUpdate();
	objekt_2.SetDirection("back");
	std::shared_ptr<GraphicsComponent> gp2 = std::make_shared<GraphicsComponent>();
	std::shared_ptr<ColliderComponent> col2 = std::make_shared<ColliderComponent>();
	objekt_2.AddComponent(gp2);
	objekt.AddComponent(col2);
	gameObjectPtr_2 = &objekt_2;

#pragma region models and textures loading

	houseBaseDiffuseTexture = dataManager.LoadTexture("container_diffuse.png");
	houseBaseSpecularTexture = dataManager.LoadTexture("container_specular.png");
	roofDiffuseTexture = dataManager.LoadTexture("stone_diffuse.jpg");
	roofSpecularTexture = dataManager.LoadTexture("stone_specular.jpg");
	planeDiffuseTexture = dataManager.LoadTexture("grass_diffuse.jpg");
	planeSpecularTexture = dataManager.LoadTexture("grass_specular.jpg");

	
	cubemapTexture = dataManager.LoadCubemap();

#pragma endregion



#pragma region custom render data

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat))); // Static cast surprisingly doesn't work.
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat))); // Static cast surprisingly doesn't work.
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat))); // Static cast surprisingly doesn't work.
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0); // Static cast surprisingly doesn't work.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float))); // Static cast surprisingly doesn't work.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float))); // Static cast surprisingly doesn't work.
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0); // Static cast surprisingly doesn't work.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float))); // Static cast surprisingly doesn't work.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float))); // Static cast surprisingly doesn't work.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0); // Static cast surprisingly doesn't work.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float))); // Static cast surprisingly doesn't work.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float))); // Static cast surprisingly doesn't work.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#pragma endregion



#pragma region imgui init

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
	ImGui_ImplOpenGL3_Init((char*)(glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS))); // C++ style casting doesn' work here.

	// imgui variables
	bool xRotationEnabled = false;
	bool yRotationEnabled = false;
	bool wireframeModeEnabled = false;
	int meshDetailLevel = 3;

	geometryShaderPseudoMeshDetailLevel = meshDetailLevel;
	auto* lightColors = new std::vector<float*>();
	lightColors->push_back(directionalLightColorPtr);
	lightColors->push_back(spotLight1ColorPtr);
	lightColors->push_back(spotLight2ColorPtr);
	lightColors->push_back(pointLightColorPtr);
	for (auto* colorPtr : (*lightColors))
	{
		for (unsigned short i = 0; i < 4; i++)
		{
			colorPtr[i] = 1.0f;
		}
	}
	delete lightColors;

	// Assigning keyboard actions.
	rKeyActionPtr = &Input::ToggleCursor;
	tKeyActionPtr = &Input::ToggleSceneExplorationMode;
	eKeyActionPtr = &Input::MoveCameraRight;
	qKeyActionPtr = &Input::MoveCameraLeft;
	wKeyActionPtr = &Input::MoveCameraForward;
	sKeyActionPtr = &Input::MoveCameraBackward;
	aKeyActionPtr = &Input::MoveCameraLeft;
	dKeyActionPtr = &Input::MoveCameraRight;

#pragma endregion



#pragma region mvpt matrices init

	glm::mat4 transform = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	transformMatrixPtr = &transform;
	modelMatrixPtr = &model;
	viewMatrixPtr = &view;
	projectionMatrixPtr = &projection;

#pragma endregion



	// game loop
	while (!glfwWindowShouldClose(windowPtr))
	{
		currentFrame = static_cast<GLfloat>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// getting input
		Input::ProcessInput(windowPtr);

		// updating matrices, uniforms, vectors
		transform = glm::mat4(1.0f);
		model = glm::mat4(1.0f);
		view = mainCamera.GetViewMatrix();
		projection = glm::perspective(glm::radians(mainCamera.GetZoom()), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);

		if (xRotationEnabled)
			model = glm::rotate(model, static_cast<GLfloat>(glfwGetTime()) * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		if (yRotationEnabled)
			model = glm::rotate(model, static_cast<GLfloat>(glfwGetTime()) * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

		// updating shaders
		litTexturedShader.ApplyMvptMatrices();
		orbitShader.ApplyMvptMatrices();
		sphereShader.ApplyMvptMatrices();
		litTexturedInstancedShader.ApplyMvptMatrices();
		lineShader.ApplyMvptMatrices();
		refractShader.ApplyMvptMatrices();
		// We do not apply all matrices to the skybox shader, because of its nature.

		orbitShader.Use();
		orbitShader.SetInt("sidesCount", 64);
		sphereShader.Use();
		sphereShader.SetInt("meshDetailLevel", meshDetailLevel);
		lineShader.Use();
		lineShader.SetVecf4("color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		Lighting::UpdateLighting(litTexturedShader);
		Lighting::UpdateLighting(litTexturedInstancedShader);



		// Rendering...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (wireframeModeEnabled)
			Util::EnableWireframeMode();
		else
			Util::DisableWireframeMode();
		if (!cursorEnabled)
			glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Blocking cursor inside a window and disabling its graphic representation.
		else
			glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);



		glm::mat4 planeTransform = glm::translate(transform, glm::vec3(0.0f, -0.5f * 0.2f - 0.0001f, 0.0f)); // Setting "terrain's" plane a little bit below, so it doesn't intersect with models set on the "ground".
		planeTransform = glm::scale(planeTransform, glm::vec3(100.0f, 100.0f, 100.0f)); // Scaling our "terrain" to make it bigger.

		// Setting up scene graph...
		glm::mat4 rootNodeTransform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
		GraphNode rootNode(rootNodeTransform);

		PseudoMesh plane(CustomDrawing::DrawPlane); GraphNode planeNode(&plane, planeTransform);
		lineShaderEndPointPos = directionalLightsDirection;
		PseudoMesh directionalLightIndicator(CustomDrawing::DrawLine); GraphNode directionalLightIndicatorNode(&directionalLightIndicator, glm::translate(transform, glm::vec3(0.0f, 2.0f, 0.0f)));
		
		//Setting up GameObjects in scene
		tc->SetScale(*objectScalePtr);
		gp->SetModel(modelPtr);
		gameObjectPtr->Render();
		gameObjectPtr->Update(*modelLocationPtr);

		tc2->SetScale(*objectScalePtr);
		gp2->SetModel(modelPtr);
		gameObjectPtr_2->Render();
		gameObjectPtr_2->Update(*gameObjectLocationPtr);
		
		if (col->col.Collides(*modelLocationPtr, *objectScalePtr, *gameObjectLocationPtr, *objectScalePtr))
			std::cout << "KOLIZJA" << std::endl;


		// Parenting...
		rootNode.AddChild(&planeNode);
		
		if(lightsPositionsDirectionsShown)
		{
			// Enabling/disabling all lights' displays...
			if (directionalLightEnabled)
				rootNode.AddChild(&directionalLightIndicatorNode);
		}



		// Drawing the scene...
		rootNode.Render();

		// And the skybox...
		CustomDrawing::DrawSkybox();



		// ImGui (UI for debugging purposes)
		if(IMGUI_ENABLED)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Checkbox("X axis rotation", &xRotationEnabled);
			ImGui::Checkbox("Y axis rotation", &yRotationEnabled);
			ImGui::Checkbox("Wireframe mode", &wireframeModeEnabled);
			ImGui::Checkbox("Cursor enabled", &cursorEnabled);
			ImGui::SliderInt("Sphere detail level", &meshDetailLevel, 0, 3);
			ImGui::Checkbox("Directional light enabled", &directionalLightEnabled);
			ImGui::Checkbox("Lights' positions/directions shown", &lightsPositionsDirectionsShown);
			ImGui::ColorEdit4("Directional light color", directionalLightColorPtr);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}



		// [glfw] Swapping buffers and polling IO events...
		glfwSwapBuffers(windowPtr);
		glfwPollEvents();
	}

	// de-allocation
	delete[] directionalLightColorPtr;
	delete[] spotLight1ColorPtr;
	delete[] spotLight2ColorPtr;
	delete[] pointLightColorPtr;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return EXIT_SUCCESS;
}
