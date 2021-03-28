#pragma once

// Standard library / STL

#include <iostream>
#include <array>
#include <cstdint>
#include <chrono>
#include <algorithm>
#include <vector>
#include <map>
#include <stack>
#include <filesystem>
#include <thread>
#include <fstream>

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
