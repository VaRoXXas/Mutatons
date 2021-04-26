#include "pch.h"
#include <Bone.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>




Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channelPtr) : mName(name), mID(ID), mLocalTransform(1.0f)
{
	mNumPositions = channelPtr->mNumPositionKeys;

	for (int positionIndex = 0; positionIndex < mNumPositions; ++positionIndex)
	{
		aiVector3D aiPosition = channelPtr->mPositionKeys[positionIndex].mValue;
		float timeStamp = channelPtr->mPositionKeys[positionIndex].mTime;
		KeyPosition data;
		data.position = AssimpGLMHelpers::GetGLMVec(aiPosition);
		data.timeStamp = timeStamp;
		mPositions.push_back(data);
	}

	mNumRotations = channelPtr->mNumRotationKeys;
	for (int rotationIndex = 0; rotationIndex < mNumRotations; ++rotationIndex)
	{
		aiQuaternion aiOrientation = channelPtr->mRotationKeys[rotationIndex].mValue;
		float timeStamp = channelPtr->mRotationKeys[rotationIndex].mTime;
		KeyRotation data;
		data.orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
		data.timeStamp = timeStamp;
		mRotations.push_back(data);
	}

	mNumScalings = channelPtr->mNumScalingKeys;
	for (int keyIndex = 0; keyIndex < mNumScalings; ++keyIndex)
	{
		aiVector3D scale = channelPtr->mScalingKeys[keyIndex].mValue;
		float timeStamp = channelPtr->mScalingKeys[keyIndex].mTime;
		KeyScale data;
		data.scale = AssimpGLMHelpers::GetGLMVec(scale);
		data.timeStamp = timeStamp;
		mScales.push_back(data);
	}
}

void Bone::Update(float animationTime)
{
	glm::mat4 translation = InterpolatePosition(animationTime);
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScaling(animationTime);
	mLocalTransform = translation * rotation * scale;
}

glm::mat4 Bone::GetLocalTransform()
{
	return mLocalTransform;
}

std::string Bone::GetBoneName() const
{
	return mName;
}

int Bone::GetBoneID()
{
	return mID;
}

int Bone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < mNumPositions - 1; ++index)
	{
		if (animationTime < mPositions[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < mNumRotations - 1; ++index)
	{
		if (animationTime < mRotations[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

int Bone::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < mNumScalings - 1; ++index)
	{
		if (animationTime < mScales[index + 1].timeStamp)
			return index;
	}
	assert(0);
}

float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Bone::InterpolatePosition(float animationTime)
{
	if (1 == mNumPositions)
		return glm::translate(glm::mat4(1.0f), mPositions[0].position);

	int p0Index = GetPositionIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(mPositions[p0Index].timeStamp,
		mPositions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(mPositions[p0Index].position, mPositions[p1Index].position
		, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
	if (1 == mNumRotations)
	{
		auto rotation = glm::normalize(mRotations[0].orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = GetRotationIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(mRotations[p0Index].timeStamp,
		mRotations[p1Index].timeStamp, animationTime);
	glm::quat finalRotation = glm::slerp(mRotations[p0Index].orientation, mRotations[p1Index].orientation
		, scaleFactor);
	finalRotation = glm::normalize(finalRotation);
	return glm::toMat4(finalRotation);

}

glm::mat4 Bone::InterpolateScaling(float animationTime)
{
	if (1 == mNumScalings)
		return glm::scale(glm::mat4(1.0f), mScales[0].scale);

	int p0Index = GetScaleIndex(animationTime);
	int p1Index = p0Index + 1;
	float scaleFactor = GetScaleFactor(mScales[p0Index].timeStamp,
		mScales[p1Index].timeStamp, animationTime);
	glm::vec3 finalScale = glm::mix(mScales[p0Index].scale, mScales[p1Index].scale
		, scaleFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}
