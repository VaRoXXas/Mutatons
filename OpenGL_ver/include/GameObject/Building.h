#pragma once



class Building : public GameObject
{
private:
	std::string type = "empty";
	int element = 0;
	bool captured = false;
	std::vector<Building*> childrenObstacles;
public:
	void Reaction(GameObject* gameObject);
	void SetType(std::string typeSet);
	void SetBuildingElement(int elem);
	std::string GetType();
	void AddChildObstacle(Building* building);
	bool CheckObstacles();
};