#pragma once



class GameObjectBlueprint {
private:
	std::string tag = "";
	std::vector<std::string> componentList;
	int modelID;
	float x;
	float y;
	float z;
	float colX;
	float colY;
	float colZ;
	float scale;
	float rotation = 0.0f;
	char rotationAxis = 'x';
	bool hasInput = 0;
	//crossings
	std::vector<std::string> dirs;
	std::string currentDir;
	bool ifSwitch = 0;
public:
	float GetX();
	void SetX(float xFloat);
	float GetY();
	void SetY(float yFloat);
	float GetZ();
	void SetZ(float zFloat);
	float GetColX();
	void SetColX(float xFloat);
	float GetColY();
	void SetColY(float yFloat);
	float GetColZ();
	void SetColZ(float zFloat);
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
	void SetInput(std::string ifInput);
	bool GetInput();
	//crossings
	void AddDir(std::string direction);
	std::vector<std::string>& GetDirs();
	void SetCurDir(std::string direction);
	std::string GetCurrentDir();
	void SetSwitch(std::string switchString);
	bool GetIfSwitch();
};

