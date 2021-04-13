#include "pch.h"
#include "Settings.h"
#include "Util.h"
#include "Camera.h"
#include "Input.h"
#include "Rendering/Shader.h"
#include "Rendering/deprecated/Mesh.h" // TODO: Change this to new Mesh class file.
#include "Rendering/deprecated/Model.h" // TODO: Change this to new Model class file.
#include "Rendering/PseudoMesh.h"
#include "Scene/GraphNode.h"

// Shaders
#include "VertexShaders.h"
#include "GeometryShaders.h"
#include "FragmentShaders.h"
#include "Rendering/CustomDrawing.h"

// input externs
extern bool cursorEnabled;
extern void (*wKeyAction)();
extern void (*sKeyAction)();
extern void (*aKeyAction)();
extern void (*dKeyAction)();
extern void (*rKeyAction)();
extern void (*tKeyAction)();
extern void (*eKeyAction)();
extern void (*qKeyAction)();

// renderer externs
extern Shader* litTexturedShaderPtr;
extern Shader* litTexturedInstancedShaderPtr;
extern Shader* orbitShaderPtr;
extern Shader* sphereShaderPtr;
extern Shader* lineShaderPtr;
extern Shader* refractShaderPtr;
extern Shader* skyboxShaderPtr;
extern GLuint orbitVAO, orbitVBO, sphereVAO, sphereVBO, cubeVAO, cubeVBO, planeVAO, planeVBO, pyramidVAO, pyramidVBO, skyboxVAO, skyboxVBO;
extern GLuint houseBase_diffuse, roof_diffuse, plane_diffuse, houseBase_specular, roof_specular, plane_specular, cubemapTexture;
extern std::vector<GLuint*> customVAOs, customVBOs;
extern glm::vec3 lineShaderEndPointPos;
extern int geometryShaderPseudoMeshDetailLevel;



bool sceneExplorationModeEnabled = true;
Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat deltaTime = 0.0f; // the difference between the current and the last frame
GLfloat lastFrame = 0.0f;
GLfloat currentFrame = 0.0f;
glm::mat4* transformMatrixPtr;
glm::mat4* modelMatrixPtr;
glm::mat4* viewMatrixPtr;
glm::mat4* projectionMatrixPtr;


// TODO: Move to DataManager or delete.
unsigned int LoadTexture(char const* pathPtr);
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
	glfwSetCursorPosCallback(window, Input::CursorPosCallback);
	glfwSetScrollCallback(window, Input::ScrollCallback);
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
	Shader litTexturedShader(s_litTexturedVertexPtr, s_litTexturedFragmentPtr);
	Shader orbitShader(s_unlitVertexPtr, s_orbitGeometryPtr, s_unlitFragmentPtr);
	Shader sphereShader(s_unlitVertexPtr, s_sphereGeometryPtr, s_unlitFragmentPtr);
	Shader litTexturedInstancedShader(s_litTexturedInstancedVertexPtr, s_litTexturedFragmentPtr);
	Shader lineShader(s_unlitVertexPtr, s_lineGeometryPtr, s_unlitFragmentPtr);
	Shader skyboxShader(s_skyboxVertexPtr, s_skyboxFragmentPtr);
	Shader refractShader(s_litTexturedVertexPtr, s_refractFragmentPtr);
	litTexturedShaderPtr = &litTexturedShader;
	orbitShaderPtr = &orbitShader;
	sphereShaderPtr = &sphereShader;
	litTexturedInstancedShaderPtr = &litTexturedInstancedShader;
	lineShaderPtr = &lineShader;
	refractShaderPtr = &refractShader;
	skyboxShaderPtr = &skyboxShader;

	litTexturedShader.Use();
	litTexturedShader.SetInt("material.diffuse", 0);
	litTexturedShader.SetInt("material.specular", 1);
	litTexturedInstancedShader.Use();
	litTexturedInstancedShader.SetInt("material.diffuse", 0);
	litTexturedInstancedShader.SetInt("material.specular", 1);

#pragma endregion



#pragma region models and textures

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

	geometryShaderPseudoMeshDetailLevel = meshDetailLevel;
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
	rKeyAction = &Input::ToggleCursor;
	tKeyAction = &Input::ToggleSceneExplorationMode;
	eKeyAction = &Input::MoveCameraRight;
	qKeyAction = &Input::MoveCameraLeft;
	wKeyAction = &Input::MoveCameraForward;
	sKeyAction = &Input::MoveCameraBackward;
	aKeyAction = &Input::MoveCameraLeft;
	dKeyAction = &Input::MoveCameraRight;

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
		view = mainCamera.GetViewMatrix();
		projection = glm::perspective(glm::radians(mainCamera.GetZoom()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

		if (xRotationEnabled)
			model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		if (yRotationEnabled)
			model = glm::rotate(model, (GLfloat)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

		litTexturedShader.ApplyMvptMatrices();
		orbitShader.ApplyMvptMatrices();
		sphereShader.ApplyMvptMatrices();
		litTexturedInstancedShader.ApplyMvptMatrices();
		lineShader.ApplyMvptMatrices();
		refractShader.ApplyMvptMatrices();

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
		lt.SetVecf3("viewPos", mainCamera.GetPosition());
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
		lti.SetVecf3("viewPos", mainCamera.GetPosition());
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



		// Rendering...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (wireframeModeEnabled)
			Util::EnableWireframeMode();
		else
			Util::DisableWireframeMode();
		if (!cursorEnabled)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Blocking cursor inside a window and disabling its graphic representation.
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);



		glm::mat4 planeTransform = glm::translate(transform, glm::vec3(0.0f, -0.5f * 0.2f - 0.0001f, 0.0f));
		planeTransform = glm::scale(planeTransform, glm::vec3(100.0f, 100.0f, 100.0f));

		// Setting up scene graph...
		GraphNode rootNode(transform);

		PseudoMesh plane(CustomDrawing::DrawPlane); GraphNode planeNode(&plane, planeTransform);
		lineShaderEndPointPos = directionalLightsDirection;
		PseudoMesh directionalLightIndicator(CustomDrawing::DrawLine); GraphNode directionalLightIndicatorNode(&directionalLightIndicator, glm::translate(transform, glm::vec3(0.0f, 2.0f, 0.0f)));


		
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
		ImGui::ColorEdit4("Directional light color", directionalLightColor);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



		// [glfw] Swapping buffers and polling IO events...
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocation
	for(const auto* vaoPtr : customVAOs)
		glDeleteVertexArrays(1, vaoPtr);
	for (const auto* vboPtr : customVBOs)
		glDeleteVertexArrays(1, vboPtr);
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



unsigned int LoadTexture(char const* pathPtr)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(pathPtr, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 0;
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
		std::cout << "Texture failed to load at path: " << pathPtr << std::endl;
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
