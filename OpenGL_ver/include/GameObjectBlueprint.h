#pragma once



class GameObjectBlueprint {
private:
	std::string tag = "";
	std::vector<std::string> componentList;
	int modelID;
	float x;
	float y;
	float z;
	float scale;
	float rotation = 0.0f;
	char rotationAxis = 'x';
public:
	float GetX();
	void SetX(float xFloat);
	float GetY();
	void SetY(float yFloat);
	float GetZ();
	void SetZ(float zFloat);
	float GetScale();
	void SetScale(float scaleFloat);
	float GetRotation();
	void SetRotation(float rotationFloat);
	char GetRotationAxis();
	void SetRotationAxis(char rotation);
	void SetTag(std::string tagString);
	std::string GetTag();
	std::vector<std::string>& GetComponentList();
	void AddToComponentList(std::string newComponent);
	int GetModelID();
	void SetModelID(int id);
};

