#pragma once
#include <Rendering/animation/AssimpGLMHelpers.h>
/* Container for bone data */



struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, int ID, const aiNodeAnim* channelPtr);

	void Update(float animationTime);
	glm::mat4 GetLocalTransform();
	std::string GetBoneName() const;
	int GetBoneID();



	int GetPositionIndex(float animationTime);

	int GetRotationIndex(float animationTime);

	int GetScaleIndex(float animationTime);


private:

	float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

	glm::mat4 InterpolatePosition(float animationTime);

	glm::mat4 InterpolateRotation(float animationTime);

	glm::mat4 InterpolateScaling(float animationTime);

	std::vector<KeyPosition> mPositions;
	std::vector<KeyRotation> mRotations;
	std::vector<KeyScale> mScales;
	int mNumPositions;
	int mNumRotations;
	int mNumScalings;

	glm::mat4 mLocalTransform;
	std::string mName;
	int mID;
};
