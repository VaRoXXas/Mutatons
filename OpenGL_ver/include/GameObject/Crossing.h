#pragma once



//Crossing class is a collider, which causes moving gameobjects to change move direction
class Crossing : public GameObject
{
private:
	std::string dir = "right";
public:
	void ChangeDirection(GameObject *gameObject);
	void InputDirection();
	void SetDir(std::string str);
};

