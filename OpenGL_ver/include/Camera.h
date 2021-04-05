#pragma once

// Defines several possible options for camera movement. Used as an abstraction to stay away from window-system specific input methods.
enum class CameraDirection {
    Forward,
    Backward,
    Left,
    Right
};

static const float YAW_DEFAULT = -90.0f;
static const float PITCH_DEFAULT = 0.0f;
static const float SPEED_DEFAULT = 2.5f;
static const float SENSITIVITY_DEFAULT = 0.05f;
static const float ZOOM_DEFAULT = 45.0f;



// An abstract camera class that processes input and calculates the corresponding Euler angles, vectors and matrices for use in OpenGL.
class Camera
{
public:
    // attributes
    glm::vec3 position = glm::vec3();
    glm::vec3 front = glm::vec3();
    glm::vec3 up = glm::vec3();
    glm::vec3 right = glm::vec3();
    glm::vec3 worldUp = glm::vec3();
	
    // Euler angles
    float yaw = 0.0f;
    float pitch = 0.0f;
	
    // options
    float movementSpeed = 0.0f;
    float mouseSensitivity = 0.0f;
    float zoom = 0.0f;

	
    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW_DEFAULT, float pitch = PITCH_DEFAULT);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Returns the view matrix calculated using Euler angles and the lookAt matrix.
    const glm::mat4& GetViewMatrix() const;
	
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined enum (to abstract it from windowing systems).
    void ProcessKeyboard(const CameraDirection& direction, const float& deltaTime);
	
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float& xoffset, float& yoffset, const GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis.
    void ProcessMouseScroll(const float& yoffset);

	
private:
    // Calculates the front vector from the Camera's (updated) Euler angles.
    void UpdateCameraVectors();
};
