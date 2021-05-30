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
public:
	~Crossing();
	void ChangeDirection(GameObject *gameObject);
	void InputDirection();
	void AddSwitch();
	void SetDir(std::string str);
	void AddDir(std::string str);
	void UpdateTransform();
};

