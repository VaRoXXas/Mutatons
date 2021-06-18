#pragma once


class LevelManager
{
private:
	int currScene = -1;
	GameObjectLoader loader;
	int mutatonCounter = 0;
public:
	void CreateParent();
	void LoadLevel(std::string level);
	int GetCurrScene();
	void SetCurrScene(int scene);
	//void SpawnMutaton();
};

