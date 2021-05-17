#include "pch.h"
#include "Settings.h"
#include "Util.h"
#include "Camera.h"
#include "Input.h"
#include "FileNames.h"
#include "DataManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Model.h"
#include "Rendering/PseudoMesh.h"
#include "Rendering/CustomDrawing.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Scene/GraphNode.h"
#include "FrustumCulling/Frustum.h"
#include "GeometryCreation.h"

#include "Collisions.h"
#include "Component.h"
#include "Components/GraphicsComponent.h"
#include "DataManager.h"
#include "GameObject/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/ColliderComponent.h"

#include "ObjectTags.h"
#include "GameObject/GameObjectBlueprint.h"
#include "GameObject/BlueprintObjectParser.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/GameObjectLoader.h"
#include "GameObject/Crossing.h"

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
extern Shader* unlitTexturedAnimatedShaderPtr;
extern Shader* simpleDepthShaderPtr;
extern Shader* depthMapDebugShaderPtr;
extern Shader* hud1ShaderPtr;
extern GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, boxVAO, boxVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO;
extern GLuint houseBaseDiffuseTexture, roofDiffuseTexture, planeDiffuseTexture, houseBaseSpecularTexture, roofSpecularTexture, planeSpecularTexture, cubemapTexture;
extern glm::vec3 lineShaderEndPointPos;
extern int geometryShaderPseudoMeshDetailLevel;
extern bool directionalLightEnabled;
extern bool pointLightEnabled;
extern bool spotLight1Enabled;
extern bool spotLight2Enabled;
extern bool lightsPositionsDirectionsShown;
extern const float depthMapNearPlane = 1.0f, depthMapFarPlane = 7.5f;
extern float lightsDirectionVectorAngleOffset;
extern float* directionalLightColorPtr;
extern float* spotLight1ColorPtr;
extern float* spotLight2ColorPtr;
extern float* pointLightColorPtr;
extern glm::mat4 directionalLightSpaceMatrix;
extern glm::vec3 directionalLightPos;
extern glm::vec3 directionalLightsDirection;
extern glm::vec3 pointLightPos;
extern glm::vec3 spotLight1Pos;
extern glm::vec3 spotLight2Pos;
extern std::vector<Model*> vecModel;
extern std::vector<Model*> vecAnimModel;
extern Frustum frustum;
extern GLuint queryName;

//variable representing camera mode
bool isometric = false;
//variables used in cullings
int queryNumber = 0, frustumNumber = 0;

bool IMGUI_ENABLED = true;
bool sceneExplorationModeEnabled = true;
DataManager dataManager = DataManager();
Camera mainCamera(isometric);
GLfloat deltaTime = 0.0f; // the difference between the current and the last frame
GLfloat lastFrame = 0.0f;
GLfloat currentFrame = 0.0f;
glm::mat4* transformMatrixPtr;
glm::mat4* modelMatrixPtr;
glm::mat4* viewMatrixPtr;
glm::mat4* projectionMatrixPtr;

glm::vec3* modelLocationPtr;
glm::vec3* objectScalePtr;
extern Model* modelPtr;

std::vector<GameObject*> gameObjectVector;
std::vector<GameObject*> modifiableGameObjectVector;
std::vector<Crossing*> crossingVector;
GameObject* gameObjectPtr;
Crossing* crossingPtr;


void RenderScene();
void DepthRenderScene();



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
	Shader simpleDepthShader(s_simpleDepthVertexPtr, s_emptyFragmentPtr);
	Shader depthMapDebugShader(s_depthMapDebugVertexPtr, s_depthMapDebugFragmentPtr);
	Shader hud1Shader(s_hud1VertexPtr, s_hud1FragmentPtr);
	litTexturedShaderPtr = &litTexturedShader;
	orbitShaderPtr = &orbitShader;
	sphereShaderPtr = &sphereShader;
	litTexturedInstancedShaderPtr = &litTexturedInstancedShader;
	lineShaderPtr = &lineShader;
	skyboxShaderPtr = &skyboxShader;
	refractShaderPtr = &refractShader;
	unlitTexturedAnimatedShaderPtr = &unlitTexturedAnimated;
	simpleDepthShaderPtr = &simpleDepthShader;
	depthMapDebugShaderPtr = &depthMapDebugShader;
	hud1ShaderPtr = &hud1Shader;


	Lighting::InitLighting(litTexturedShader);
	Lighting::InitLighting(litTexturedInstancedShader);

#pragma endregion

	

#pragma region models and textures loading

	//Data manager loads all models
	dataManager.LoadAllModels();

	houseBaseDiffuseTexture = dataManager.LoadTexture("container_diffuse.png");
	houseBaseSpecularTexture = dataManager.LoadTexture("container_specular.png");
	roofDiffuseTexture = dataManager.LoadTexture("stone_diffuse.jpg");
	roofSpecularTexture = dataManager.LoadTexture("stone_specular.jpg");
	planeDiffuseTexture = dataManager.LoadTexture("grass_diffuse.jpg");
	planeSpecularTexture = dataManager.LoadTexture("grass_specular.jpg");

	
	cubemapTexture = dataManager.LoadCubemap();

#pragma endregion



#pragma region game objects declarations

	//Location and size vectors' declaration
	glm::vec3 objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
	objectScalePtr = &objectScale;

	//Parent GameObject declaration
	gameObjectPtr = new GameObject;
	gameObjectPtr->SetActive();
	gameObjectPtr->SetTag("Parent");
	gameObjectVector.push_back(gameObjectPtr);

	//Loading gameobjects from file
	GameObjectLoader loader;
	loader.LoadGameObjects("res/level.txt", gameObjectVector, *gameObjectVector[0]);
	//objectScalePtr = new glm::vec3(0.01f, 0.01f, 0.01f);
	//Testing gameobjects' declaration
	gameObjectPtr = new GameObject;
	gameObjectPtr->SetActive();
	gameObjectPtr->SetVelocity(15.0f);
	gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
	gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
	gameObjectPtr->AddComponent(std::make_shared<ColliderComponent>());
	gameObjectPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	gameObjectPtr->GetGraphicsComponent()->SetModel(vecAnimModel[0]);
	gameObjectPtr->GetGraphicsComponent()->InitializeAnimation(ANIM);
	gameObjectPtr->GetColliderComponent()->Initialize(gameObjectPtr->GetTransformComponent());
	modifiableGameObjectVector.push_back(gameObjectPtr);
	gameObjectVector[0]->AddChild(gameObjectPtr);

	objectScalePtr = &objectScale;
	//Crossings declaration
	//TODO: parser interprets crossings in file
	crossingPtr = new Crossing;
	crossingPtr->SetActive();
	crossingPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(9.0f,1.0f,-2.0f)));
	crossingPtr->AddComponent(std::make_shared<ColliderComponent>());
	crossingPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	crossingPtr->GetColliderComponent()->Initialize(crossingPtr->GetTransformComponent());
	crossingVector.push_back(crossingPtr);
	gameObjectVector[0]->AddChild(crossingPtr);

	crossingPtr = new Crossing;
	crossingPtr->SetActive();
	crossingPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(5.0f, 1.0f, -3.0f)));
	crossingPtr->AddComponent(std::make_shared<ColliderComponent>());
	crossingPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	crossingPtr->GetColliderComponent()->Initialize(crossingPtr->GetTransformComponent());
	crossingPtr->SetDir("back");
	crossingVector.push_back(crossingPtr);
	gameObjectVector[0]->AddChild(crossingPtr);

	crossingPtr = new Crossing;
	crossingPtr->SetActive();
	crossingPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(6.0f, 1.0f, -7.0f)));
	crossingPtr->AddComponent(std::make_shared<ColliderComponent>());
	crossingPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	crossingPtr->GetColliderComponent()->Initialize(crossingPtr->GetTransformComponent());
	crossingPtr->SetDir("right");
	crossingVector.push_back(crossingPtr);
	gameObjectVector[0]->AddChild(crossingPtr);

	crossingPtr = new Crossing;
	crossingPtr->SetActive();
	crossingPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(3.0f, 1.0f, -6.0f)));
	crossingPtr->AddComponent(std::make_shared<ColliderComponent>());
	crossingPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	crossingPtr->GetColliderComponent()->Initialize(crossingPtr->GetTransformComponent());
	crossingPtr->SetDir("forward");
	crossingVector.push_back(crossingPtr);
	gameObjectVector[0]->AddChild(crossingPtr);

	crossingPtr = new Crossing;
	crossingPtr->SetActive();
	crossingPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(4.0f, 1.0f, 0.0f)));
	crossingPtr->AddComponent(std::make_shared<ColliderComponent>());
	crossingPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	crossingPtr->GetColliderComponent()->Initialize(crossingPtr->GetTransformComponent());
	crossingPtr->SetDir("left");
	crossingVector.push_back(crossingPtr);
	gameObjectVector[0]->AddChild(crossingPtr);

	crossingPtr = new Crossing;
	crossingPtr->SetActive();
	crossingPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(9.0f, 1.0f, -1.0f)));
	crossingPtr->AddComponent(std::make_shared<ColliderComponent>());
	crossingPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	crossingPtr->GetColliderComponent()->Initialize(crossingPtr->GetTransformComponent());
	crossingPtr->SetDir("forward");
	crossingVector.push_back(crossingPtr);
	gameObjectVector[0]->AddChild(crossingPtr);
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
	
	// 2x1x1 box
	float boxData[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  1.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  1.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  1.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  1.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  1.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  1.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	glGenVertexArrays(1, &boxVAO);
	glBindVertexArray(boxVAO);
	glGenBuffers(1, &boxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxData), boxData, GL_STATIC_DRAW);
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

	// for shadowmapping

	// configure depth map FBO
	// -----------------------
	//const unsigned int SHADOW_WIDTH = 16384, SHADOW_HEIGHT = 16384;
	const unsigned int SHADOW_WIDTH = 8192, SHADOW_HEIGHT = 8192;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// configuring shader for shadows
	litTexturedShader.Use();
	litTexturedShader.SetInt("diffuseTexture", 0);
	litTexturedShader.SetInt("shadowMap", 1);
	litTexturedInstancedShader.Use();
	litTexturedInstancedShader.SetInt("diffuseTexture", 0);
	litTexturedInstancedShader.SetInt("shadowMap", 1);
	depthMapDebugShader.Use();
	depthMapDebugShader.SetInt("depthMap", 0);

	GeometryCreation::Hud1Creation();

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

	//editor propeties
	float posFloat[] = { 1.0,1.0,1.0 };
	bool update = modifiableGameObjectVector.front()->HasUpdate();
	glm::vec3 pos = modifiableGameObjectVector.front()->GetTransformComponent()->GetLocation();
	bool once = false;
	bool reset = true;
	static const char * items[] = { "forward", "back", "right", "left" };
	static int selectedItem = 0;
	int modelID = 0;

	//generate queries to occlusion culling
	glGenQueries(1, &queryName);
	
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
		//frustum culling
		frustum.SetCamInternals(glm::radians(mainCamera.GetZoom()), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);

		// updating shaders
		litTexturedShader.ApplyMvptMatrices();
		orbitShader.ApplyMvptMatrices();
		litTexturedInstancedShader.ApplyMvptMatrices();
		lineShader.ApplyMvptMatrices();
		refractShader.ApplyMvptMatrices();
		unlitTexturedAnimated.ApplyMvptMatrices();
		// We do not apply all matrices to the skybox shader, because of its nature.

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
		{
			glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Blocking cursor inside a window and disabling its graphic representation.
			glfwSetCursorPosCallback(windowPtr, Input::CursorPosCallback);
		}
		else
		{
			glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(windowPtr, nullptr);
		}

		lineShaderEndPointPos = directionalLightsDirection;
		PseudoMesh directionalLightIndicator(CustomDrawing::DrawLine);

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		// render scene from light's point of view
		simpleDepthShader.Use();
		simpleDepthShader.SetMat4("model", model);
		simpleDepthShader.SetMat4("transform", transform);
		simpleDepthShader.SetMat4("lightSpaceMatrix", directionalLightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		DepthRenderScene();
		glCullFace(GL_BACK); // don't forget to reset original culling face
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		litTexturedShader.Use();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene();



		// And the skybox...
		CustomDrawing::DrawSkybox();

		// Drawing HUD
		
		CustomDrawing::DrawHud1();

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		depthMapDebugShader.Use();
		depthMapDebugShader.SetFloat("near_plane", depthMapNearPlane);
		depthMapDebugShader.SetFloat("far_plane", depthMapFarPlane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//CustomDrawing::DrawQuad();


		//GameObject editor options, first one determines if gameobject has Update
		if (update && !once)
		{
			modifiableGameObjectVector.front()->SetUpdate();
			once = true;
		}
		else if (!update && once)
		{
			posFloat[0] = pos.x;
			posFloat[1] = pos.y;
			posFloat[2] = pos.z;
			modifiableGameObjectVector.front()->SetUpdate();
			once = false;
		}
		else if (!update)
		{
			pos = glm::vec3(posFloat[0], posFloat[1], posFloat[2]);
		}

		//Setting up movedirection of gameobject
		if (reset)
		{
			switch (selectedItem)
			{
			case 0:
				modifiableGameObjectVector.front()->SetDirection("forward");
				reset = false;
				break;
			case 1:
				modifiableGameObjectVector.front()->SetDirection("back");
				reset = false;
				break;
			case 2:
				modifiableGameObjectVector.front()->SetDirection("right");
				reset = false;
				break;
			case 3:
				modifiableGameObjectVector.front()->SetDirection("left");
				reset = false;
				break;
			}
		}

		//GameObject's updates
		modifiableGameObjectVector.front()->Update(pos,deltaTime);

		//Choosing model for the gameobject
		//modifiableGameObjectVector.front()->GetGraphicsComponent()->SetModel(vecModel[modelID]);
		// ImGui (UI for debugging purposes)
		if(IMGUI_ENABLED)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Checkbox("Wireframe mode", &wireframeModeEnabled);
			ImGui::Checkbox("Cursor enabled", &cursorEnabled);
			ImGui::Checkbox("Directional light enabled", &directionalLightEnabled);
			ImGui::Checkbox("Lights' positions/directions shown", &lightsPositionsDirectionsShown);
			ImGui::ColorEdit4("Directional light color", directionalLightColorPtr);
			
			//gameobject editor
			ImGui::Checkbox("GameObject Update", &update);
			ImGui::SliderFloat3("GameObject position", posFloat , -10.0f, 10.0f);
			ImGui::SliderFloat("lightsDirectionVectorAngleOffset", &lightsDirectionVectorAngleOffset, -10.0f, 10.0f);
			ImGui::Combo("MoveDirection", &selectedItem, items, IM_ARRAYSIZE(items));
			ImGui::Checkbox("MoveDirection reset", &reset);
			ImGui::SliderInt("GameObject model", &modelID, 0, 29);
			ImGui::Text("Number of hidden object due to occlusion culling: %i", queryNumber);
			ImGui::Text("Number of object rendered with frustum culling: %i", frustumNumber);

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		//Checking if any crossing is colliding with moving gameobjects
		for (Crossing *c : crossingVector)
		{
			for (GameObject* g : modifiableGameObjectVector)
			{
				c->ChangeDirection(g);
			}
		}
		// [glfw] Swapping buffers and polling IO events...
		glfwSwapBuffers(windowPtr);
		glfwPollEvents();
		
	}

	glDeleteQueries(1, &queryName);

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



void RenderScene()
{
	gameObjectVector[0]->Render();
}

void DepthRenderScene()
{
	gameObjectVector[0]->DepthRender();
}
