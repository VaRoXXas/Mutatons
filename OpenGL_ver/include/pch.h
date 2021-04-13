#pragma once

// Standard library / STL

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <random>
#include <stack>
#include <string>
#include <thread>
#include <typeinfo>
#include <queue>
#include <vector>

// Windows

#include <Windows.h>

// 3rd party libraries

#ifdef _DEBUG
#include <glad/gladDEBUG.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
