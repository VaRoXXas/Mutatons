#pragma once



class Building : public GameObject
{
private:
	std::string type = "empty";
	bool captured = false;
	std::vector<Building*> childrenObstacles;
public:
	void Reaction(GameObject* gameObject);
	void SetType(std::string typeSet);
	std::string GetType();
	void AddChildObstacle(Building* building);
	bool CheckObstacles();
};