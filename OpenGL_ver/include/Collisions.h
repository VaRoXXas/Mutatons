#pragma once



class Collisions
{
public:
	//glm::vec3 pos;
	//glm::vec3 size;
	//glm::vec3 separation;

	glm::vec3 PosCalc(glm::mat4& mat);
	glm::vec3 SizeCalc(glm::mat4& mat);
	//void PosSizeCalc(glm::mat4& mat);
	bool RectCol();
	bool Collides(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize);
	bool Trigger(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize);
	
	glm::vec3 Separation(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize);
	//glm::vec3 SetSeparation(glm::vec3& onePos, glm::vec3& oneSize, glm::vec3& twoPos, glm::vec3& twoSize)
};

