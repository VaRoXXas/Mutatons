#pragma once



//Crossing class is a collider, which causes moving gameobjects to change move direction
class Crossing : public GameObject
{
private:
	bool once = false;
	std::string dir = "right";
	std::vector<std::string> availableDirs;
	std::vector<GameObject*> bridges;
	GameObject* directionSwitch;
	bool firstTime = true;
	bool blocked = false;
	bool colided = false;
	int count = 0;
	std::shared_ptr<ColliderComponent> bridgeOneCollider = std::make_shared<ColliderComponent>();
	std::shared_ptr<ColliderComponent> bridgeTwoCollider = std::make_shared<ColliderComponent>();
	glm::vec3* mutatonPosPtr;
	glm::vec3* mutatonSizePtr;
	std::string mutatonTag = "mutaton0";
public:
	~Crossing();
	void ChangeDirection(GameObject *gameObject);
	void InputDirection();
	void AddSwitch();
	void SetDir(std::string str);
	void AddDir(std::string str);
	void CheckIfBlocked(GameObject* mutaton);
};

