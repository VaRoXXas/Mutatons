#include "pch.h"
#include "Settings.h"
#include "Util.h"
#include "Camera.h"
#include "Input.h"
#include "FileNames.h"
#include "DataManager.h"
#include "AudioManager.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Model.h"
#include "Rendering/PseudoMesh.h"
#include "Rendering/CustomDrawing.h"
#include "Rendering/animation/Animation.h"
#include "Rendering/animation/Animator.h"
#include "Rendering/PostProcessor.h"
#include "Scene/GraphNode.h"
#include "FrustumCulling/Frustum.h"
#include "GeometryCreation.h"
#include "MousePicker.h"

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
#include "GameObject/Building.h"
#include "LevelManager.h"

// Shaders
#include "VertexShaders.h"
#include "GeometryShaders.h"
#include "FragmentShaders.h"

// input externs
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
extern Shader* textShaderPtr;
extern Shader* postProcessingShaderPtr;
extern Shader* mainmenuShaderPtr;
extern PostProcessor* postProcessorPtr;
extern GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, boxVAO, boxVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO, textVAO, textVBO;
extern GLuint houseBaseDiffuseTexture, roofDiffuseTexture, planeDiffuseTexture, houseBaseSpecularTexture, roofSpecularTexture, planeSpecularTexture, cubemapTexture, mainmenuTexture, UITexture;
extern glm::vec3 lineShaderEndPointPos;
extern int geometryShaderPseudoMeshDetailLevel;
extern bool directionalLightEnabled;
extern bool pointLightEnabled;
extern bool spotLight1Enabled;
extern bool spotLight2Enabled;
extern bool lightsPositionsDirectionsShown;
extern const float depthMapNearPlane = 0.1f, depthMapFarPlane = 17.5f;
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
extern bool LMBreleaseEventTriggered = false;

//variable representing camera mode
bool isometric = true;

bool IMGUI_ENABLED = false;
bool sceneExplorationModeEnabled = true;
bool confusionPostProcessOn = false;
bool chaosPostProcessOn = false;
bool stateEsc = false;
DataManager dataManager = DataManager();
LevelManager levelManager;
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
std::vector<Building*> buildingVector;
GameObject* gameObjectPtr;
Crossing* crossingPtr;
Building* buildingPtr;


std::vector<std::string> capturedPoints;
std::vector<std::string> inactiveMutatons;
int maxCapturedPoints = 0;
int capturedCounter = 0;
int counter = 0;
int mutatonCounter = 0;
int maxMutatonsInLevel = 8;
int mutatonsInControl = 0;

AudioManager audioManager = AudioManager();
extern sf::SoundBuffer winningSoundBuffer, losingSoundBuffer, mutationSoundBuffer, controlPanelSoundBuffer, dyingFromLaserSoundBuffer, bridgeClickedSoundBuffer;

void RenderScene();
void DepthRenderScene();
void SpawnMutaton();
void Reset(unsigned short levelIndex);

struct TextCharacter {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};
extern std::map<GLchar, TextCharacter> TextCharacters;



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
	glfwSetMouseButtonCallback(windowPtr, Input::MouseButtonCallback);
	
	// [glad] Load all OpenGL function pointers.
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Setting window's background color.

	audioManager.Initialize();

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
	Shader textShader(s_textVertexPtr, s_textFragmentPtr);
	Shader postProcessingShader(s_postProcessingVertexPtr, s_postProcessingFragmentPtr);
	Shader mainmenuShader(s_mainmenuVertexPtr, s_mainmenuFragmentPtr);
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
	textShaderPtr = &textShader;
	postProcessingShaderPtr = &postProcessingShader;
	mainmenuShaderPtr = &mainmenuShader;

	Lighting::InitLighting(litTexturedShader);
	Lighting::InitLighting(litTexturedInstancedShader);

	postProcessorPtr = new PostProcessor(postProcessingShader, WINDOW_WIDTH, WINDOW_HEIGHT);

#pragma endregion

	

#pragma region models and textures loading

	//Data manager loads all models
	dataManager.LoadAllModels();

	//houseBaseDiffuseTexture = dataManager.LoadTexture("container_diffuse.png");
	//houseBaseSpecularTexture = dataManager.LoadTexture("container_specular.png");
	//roofDiffuseTexture = dataManager.LoadTexture("stone_diffuse.jpg");
	//roofSpecularTexture = dataManager.LoadTexture("stone_specular.jpg");
	//planeDiffuseTexture = dataManager.LoadTexture("grass_diffuse.jpg");
	//planeSpecularTexture = dataManager.LoadTexture("grass_specular.jpg");
	mainmenuTexture = dataManager.LoadTexture("res/textures/mainmenu.png");
	UITexture = dataManager.LoadTexture("res/textures/UI.png");
	
	cubemapTexture = dataManager.LoadCubemap();

#pragma endregion

	MousePicker picker(&mainCamera, windowPtr);
	glm::vec3 terrainPoint;

#pragma region game objects declarations

	//Location and size vectors' declaration
	glm::vec3 objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
	objectScalePtr = &objectScale;

	//Parent GameObject declaration
	levelManager.CreateParent();
	levelManager.LoadLevel("mainmenu");

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
	GeometryCreation::MainmenuCreation();

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
	
	//text init
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 projectionText = glm::ortho(0.0f, static_cast<GLfloat>(WINDOW_WIDTH), 0.0f, static_cast<GLfloat>(WINDOW_HEIGHT));
	textShaderPtr->Use();
	textShaderPtr->SetMat4("projection", projectionText);
	//glUniformMatrix4fv(glGetUniformLocation(textShader->Use(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "res/Sofia Pro Black Az.otf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texTexture;
		glGenTextures(1, &texTexture);
		glBindTexture(GL_TEXTURE_2D, texTexture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		TextCharacter character = {
			texTexture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		TextCharacters.insert(std::pair<GLchar, TextCharacter>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::chrono::time_point start = std::chrono::system_clock::now();
	std::chrono::time_point between = std::chrono::system_clock::now();
	double posX, posY;
	int diffSec, diffTSec, diffMinute;

	double lasttime = glfwGetTime();
	int pos = 0;

	// game loop
	while (!glfwWindowShouldClose(windowPtr))
	{
		currentFrame = static_cast<GLfloat>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//L = reset
		
		if (glfwGetKey(windowPtr, GLFW_KEY_L) == GLFW_PRESS)
		{
			//static unsigned short levelIndex = 0;
			//levelIndex++;
			//Reset(levelIndex);

			mainCamera.SetFront(glm::vec3(-1.0f, -3.0f, -1.0f));
			mainCamera.SetPosition(glm::vec3(12.0f, 10.0f, 0.0f));
			mainCamera.SetWorldUp(glm::vec3(0.0f, 1.0f, 0.0f));
			mainCamera.SetZoom(S_ZOOM_DEFAULT);

			levelManager.LoadLevel("second");
			counter = 0;
			mutatonCounter = 0;
			maxMutatonsInLevel = 9;
		}

		counter++;
		//if (counter%375 == 0 && mutatonCounter!=8 )
		if (counter % 500 == 0 && mutatonCounter != maxMutatonsInLevel)
		{
			SpawnMutaton();
			mutatonCounter++;
		}
		// getting input
		Input::ProcessInput(windowPtr);
		if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			stateEsc = true;
		}

		glfwGetCursorPos(windowPtr, &posX, &posY);

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
		glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(windowPtr, nullptr);

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
		const bool postProcessingEnabled = false;
		if(postProcessingEnabled)
		{
			postProcessorPtr->BeginRender();
			RenderScene();
			postProcessorPtr->EndRender();
			postProcessorPtr->RenderQuad(glfwGetTime());
		}
		else
		{
			RenderScene();
		}

		picker.SetProjectionMatrix(projection);

		picker.Update();

		terrainPoint = picker.GetCurrentTerrainPoint();

		//if (terrainPoint.x >= -1.f && terrainPoint.x <= 1.f && terrainPoint.z >= -1.f && terrainPoint.z <= 1.f)
		//{
		//	model = glm::translate(model, terrainPoint);
		//	if (LMBreleaseEventTriggered == true)
		//	{
		//		{
		//			modeltest.Draw(litTexturedShader);
		//		}
		//	}
		//}


		//Checking if all points are captured (may not work with more than three)
		if (capturedPoints.size() != maxCapturedPoints)
		{
			for (Building* b : buildingVector)
			{
				for (std::string s : capturedPoints)
					if ((s != b->GetTag() && b->GetCaptured()))
					{
						capturedCounter++;
						capturedPoints.push_back(b->GetTag());
					}
				if (capturedPoints.empty() && b->GetCaptured())
				{
					capturedCounter++;
					capturedPoints.push_back(b->GetTag());
				}
			}
		}

		//Checking if all mutatons are inactive and removes them from a vector
		pos = 0;
		for (GameObject* m : modifiableGameObjectVector)
		{
			for (std::string s : inactiveMutatons)
			{
				if (!m->IsActive() && !inactiveMutatons.empty() )
				{
					if (m->GetIsInControl())
						mutatonsInControl++;
					inactiveMutatons.push_back(m->GetTag());
					std::_Erase_remove(modifiableGameObjectVector, modifiableGameObjectVector[pos]);
					break;
				}
			}
			if (inactiveMutatons.empty() && !m->IsActive())
			{
				if (m->GetIsInControl())
					mutatonsInControl++;
				inactiveMutatons.push_back(m->GetTag());
				std::_Erase_remove(modifiableGameObjectVector, modifiableGameObjectVector[pos]);
				break;
			}
			pos++;
		}

		//Checking if all conditions to finish level are met 
		if(modifiableGameObjectVector.empty() && capturedPoints.size() == maxCapturedPoints)
			std::cout << "END" << std::endl;

		// And the skybox...
		CustomDrawing::DrawSkybox();

		// Drawing HUD
		between = std::chrono::system_clock::now();
		std::chrono::duration <double> diff = between - start;
		diffMinute = diff.count() / 60;
		diffTSec =  (diff.count() - diffMinute*60) / 10;
		diffSec = diff.count() - diffMinute * 60 - diffTSec * 10;

		CustomDrawing::DrawHud1();
		//CustomDrawing::DrawHud2();
		CustomDrawing::RenderText(std::to_string((int)diffMinute)+":"+ std::to_string((int)diffTSec) + std::to_string((int)diffSec), 925.0f, 100.0f, 1.0f, glm::vec3(0.1, 0.1f, 0.7f));
		CustomDrawing::RenderText(std::to_string(mutatonCounter)+'/'+std::to_string(maxMutatonsInLevel), 680.0f, 75.0f, 1.0f, glm::vec3(0.9, 0.2f, 0.7f));
		//CustomDrawing::RenderText("Mutatons left:", 1640.0f, 880.0f, 0.8f, glm::vec3(0.3, 0.7f, 0.9f));
		std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
		time_t t = std::chrono::system_clock::to_time_t(p);
		char str[26];
		ctime_s(str, sizeof str, &t);
		std::string sysTime(str);
		//CustomDrawing::RenderText("System time: " + sysTime, 500.0f, 50.0f, 1.0f, glm::vec3(0.5, 0.1f, 0.2f));
		//CustomDrawing::DrawMainmenu();
		//TextRendering::TextInitialize();

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		depthMapDebugShader.Use();
		depthMapDebugShader.SetFloat("near_plane", depthMapNearPlane);
		depthMapDebugShader.SetFloat("far_plane", depthMapFarPlane);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//CustomDrawing::DrawQuad();
		//std::cout << posX << std::endl << posY<<std::endl<<std::endl;
		if (levelManager.GetCurrScene() > 0)
		{
			if ((glfwGetKey(windowPtr, GLFW_KEY_L) == GLFW_PRESS) || (glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && 1770 < posX && posX < 1840 && 930 < posY && posY < 990))
			{
				stateEsc = false;
				levelManager.LoadLevel("mainmenu");

			}
		}
		if (levelManager.GetCurrScene() == 0)
		{
			levelManager.LoadLevel("mainmenu");
			if (glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && 1300 < posX && posX < 1800 && 450 < posY && posY < 700)
			{
				maxCapturedPoints = 2;
				capturedCounter = 0;
				counter = 0;
				mutatonCounter = 0;
				maxMutatonsInLevel = 3;
				mutatonsInControl = 0;
				start = std::chrono::system_clock::now();
				levelManager.LoadLevel("tutorial");
			}
			if ((glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_RELEASE && stateEsc == true) ||( glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && 900 < posX && posX < 1050 && 875 < posY && posY < 950))
			{
				stateEsc = false;
				glfwSetWindowShouldClose(windowPtr, true);
			
			}
		}
		if (levelManager.GetCurrScene() == 1)
		{
			if(glfwGetKey(windowPtr, GLFW_KEY_L) == GLFW_PRESS)
			{
				maxCapturedPoints = 1;
				capturedCounter = 0;
				counter = 0;
				mutatonCounter = 0;
				maxMutatonsInLevel = 8;
				mutatonsInControl = 0;
				start = std::chrono::system_clock::now();
				levelManager.LoadLevel("first");

			}
			if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_RELEASE && stateEsc == true)
			{
				stateEsc = false;
				levelManager.LoadLevel("mainmenu");
			}
		}
		if (levelManager.GetCurrScene() == 2)
		{
			if(glfwGetKey(windowPtr, GLFW_KEY_L) == GLFW_PRESS)
			{
				start = std::chrono::system_clock::now();
				levelManager.LoadLevel("first");
			}
			if (glfwGetKey(windowPtr, GLFW_KEY_ESCAPE) == GLFW_RELEASE && stateEsc == true)
			{
				stateEsc = false;
				levelManager.LoadLevel("mainmenu");
			}
		}

		//GameObject's updates
		for (GameObject* g : modifiableGameObjectVector)
		{
			g->Update(g->GetTransformComponent()->GetLocationAddr(), deltaTime);
		}
		//Choosing model for the gameobject
		//modifiableGameObjectVector.front()->GetGraphicsComponent()->SetModel(vecModel[modelID]);
		// ImGui (UI for debugging purposes)
		if(IMGUI_ENABLED)
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Checkbox("Wireframe mode", &wireframeModeEnabled);
			ImGui::Checkbox("Directional light enabled", &directionalLightEnabled);
			ImGui::Checkbox("Lights' positions/directions shown", &lightsPositionsDirectionsShown);
			
			//gameobject editor
			ImGui::SliderFloat("lightsDirectionVectorAngleOffset", &lightsDirectionVectorAngleOffset, -10.0f, 10.0f);

			// PostProcessing
			if(ImGui::Button("Shake it"))
			{
				// shaking the screen
				postProcessorPtr->SetShakeTime(0.5f);
				postProcessorPtr->shake = GL_TRUE;
			}
			if (postProcessorPtr->GetShakeTime() > 0.0f)
			{
				postProcessorPtr->SetShakeTime(postProcessorPtr->GetShakeTime() - deltaTime);
				if (postProcessorPtr->GetShakeTime() <= 0.0f)
					postProcessorPtr->shake = GL_FALSE;
			}
			ImGui::Checkbox("Confusion post process", &confusionPostProcessOn);
			ImGui::Checkbox("Chaos post process", &chaosPostProcessOn);

			if (ImGui::Button("SoundTest"))
			{
				audioManager.PlaySfSound(losingSoundBuffer);
			}
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		//Checking if any crossing is colliding with moving gameobjects
		for (Crossing *c : crossingVector)
		{
			for (GameObject* g : modifiableGameObjectVector)
			{
				c->ChangeDirection(g);
				c->CheckIfBlocked(g);
			}
			
			c->CheckInput(terrainPoint);
			c->InputDirection();
		}
		//objectScalePtr = new glm::vec3(0.1f);
		//modifiableGameObjectVector[0]->GetTransformComponent()->SetScale(*objectScalePtr);

		for (Building* b : buildingVector)
		{
			for (GameObject* g : modifiableGameObjectVector)
			{
				b->Reaction(g);
			}
		}

		while (glfwGetTime() < lasttime + 1.0 / 60) 
		{
		}
		lasttime += 1.0 / 60;

		audioManager.TryClean();
		
		// [glfw] Swapping buffers and polling IO events...
		LMBreleaseEventTriggered = false;
		glfwSwapBuffers(windowPtr);
		glfwPollEvents();
	}

	//glDeleteQueries(1, &queryName);

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
	gameObjectVector[0]->Render(gameObjectVector[0]->GetTransformComponent()->GetTransform());
}

void DepthRenderScene()
{
	gameObjectVector[0]->DepthRender();
}

void SpawnMutaton()
{
	gameObjectPtr = new GameObject;
	gameObjectPtr->SetTag("mutaton"+std::to_string(mutatonCounter));
	gameObjectPtr->SetActive();
	gameObjectPtr->SetVelocity(10.0f);
	gameObjectPtr->AddComponent(std::make_shared<TransformComponent>(glm::vec3(10.0f, 1.0f, -7.0f)));
	gameObjectPtr->AddComponent(std::make_shared<GraphicsComponent>());
	gameObjectPtr->AddComponent(std::make_shared<ColliderComponent>());
	gameObjectPtr->SetUpdate();
	gameObjectPtr->GetTransformComponent()->SetScale(*objectScalePtr);
	gameObjectPtr->GetGraphicsComponent()->SetModel(vecAnimModel[1]);
	gameObjectPtr->GetGraphicsComponent()->SetOversized(true);
	gameObjectPtr->GetGraphicsComponent()->InitializeAnimation(ANIM_CREATURE_BASIC);
	modifiableGameObjectVector.push_back(gameObjectPtr);
	gameObjectVector[0]->AddChild(gameObjectPtr);
	std::cout << gameObjectPtr->GetTag() << std::endl;
}

void Reset(unsigned short levelIndex)
{
	counter = 0;
	mutatonCounter = 0;
	
	mainCamera.SetFront(glm::vec3(-1.0f, -3.0f, -1.0f));
	mainCamera.SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	mainCamera.SetWorldUp(glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera.SetZoom(S_ZOOM_DEFAULT);

	modifiableGameObjectVector.clear();
	crossingVector.clear();
	buildingVector.clear();
	gameObjectVector[0]->ClearChildren();

	//Location and size vectors' declaration
	glm::vec3 objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
	objectScalePtr = &objectScale;

	//Parent GameObject declaration
	gameObjectPtr = new GameObject;
	gameObjectPtr->SetActive();
	gameObjectPtr->SetTag("Parent");
	gameObjectPtr->AddComponent(std::make_shared<TransformComponent>());
	gameObjectVector.push_back(gameObjectPtr);

	//Loading gameobjects from file
	GameObjectLoader loader;
	std::string extension = ".txt";
	std::string levelPath = "res/level";
	std::string levelBuildingsPath = "res/level_buildings";
	levelPath += std::to_string(levelIndex) + extension;
	levelBuildingsPath += std::to_string(levelIndex) + extension;
	loader.LoadGameObjects(levelPath, *gameObjectVector[0]);
	loader.LoadGameObjects(levelBuildingsPath, *gameObjectVector[0]);

}
