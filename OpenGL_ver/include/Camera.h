#pragma once

// Defines several possible options for camera movement. Used as an abstraction to stay away from window-system specific input methods.
enum class CameraDirection {
    Forward,
    Backward,
    Left,
    Right
};

static const float S_YAW_DEFAULT = -90.0f;
static const float S_PITCH_DEFAULT = 0.0f;
static const float S_SPEED_DEFAULT = 2.5f;
static const float S_SENSITIVITY_DEFAULT = 0.05f;
static const float S_ZOOM_DEFAULT = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler angles, vectors and matrices for use in OpenGL.
class Camera
{
public:
	
    // constructor with vectors
    Camera(bool isometric, float yaw = S_YAW_DEFAULT, float pitch = S_PITCH_DEFAULT);
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Returns the view matrix calculated using Euler angles and the lookAt matrix.
    glm::mat4 GetViewMatrix() const;
	
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined enum (to abstract it from windowing systems).
    void ProcessKeyboard(const CameraDirection& direction, const float& deltaTime);
	
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float& xoffset, float& yoffset, const bool constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis.
    void ProcessMouseScroll(const float& yoffset);

	// getters and setters
    const glm::vec3& GetPosition() const;
    void SetPosition(const glm::vec3 position);
    const glm::vec3& GetFront() const;
    void SetFront(const glm::vec3 front);
    const glm::vec3& GetUp() const;
    void SetUp(const glm::vec3 up);
    const glm::vec3& GetRight() const;
    void SetRight(const glm::vec3 right);
    const glm::vec3& GetWorldUp() const;
    void SetWorldUp(const glm::vec3 worldUp);
    float GetYaw() const;
    void SetYaw(const float yaw);
    float GetPitch() const;
    void SetPitch(const float pitch);
    float GetMovementSpeed() const;
    void SetMovementSpeed(const float movementSpeed);
    float GetMouseSensitivity() const;
    void SetMouseSensitivity(const float mouseSensitivity);
    float GetZoom() const;
    void SetZoom(const float zoom);
    bool GetIsometric();
    void SetIsometric(bool isometric);
	
private:

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
    //variable representing camera mode
    bool isometric = false;
	
    // Calculates the front vector from the Camera's (updated) Euler angles.
    void UpdateCameraVectors();
};
