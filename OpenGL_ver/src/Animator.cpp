#include "pch.h"

#include <Animation.h>
#include <Bone.h>
#include <Animator.h>



Animator::Animator(Animation* currentPtr)
{
	mCurrentAnimationPtr = currentPtr;
	mCurrentTime = 0.0;
	mTransforms.reserve(100);
	for (int i = 0; i < 100; i++)
		mTransforms.push_back(glm::mat4(1.0f));
}

void Animator::UpdateAnimation(float dt)
{
	mDeltaTime = dt;
	if (mCurrentAnimationPtr)
	{
		mCurrentTime += mCurrentAnimationPtr->GetTicksPerSecond() * dt;
		mCurrentTime = fmod(mCurrentTime, mCurrentAnimationPtr->GetDuration());
		CalculateBoneTransform(&mCurrentAnimationPtr->GetRootNode(), glm::mat4(1.0f));
	}
}

void Animator::PlayAnimation(Animation* pAnimationPtr)
{
	mCurrentAnimationPtr = pAnimationPtr;
	mCurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* nodePtr, glm::mat4 parentTransform)
{
	std::string nodeName = nodePtr->name;
	glm::mat4 nodeTransform = nodePtr->transformation;

	Bone* Bone = mCurrentAnimationPtr->FindBone(nodeName);

	if (Bone)
	{
		Bone->Update(mCurrentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform;

	auto boneInfoMap = mCurrentAnimationPtr->GetBoneIDMap();
	if (boneInfoMap.find(nodeName) != boneInfoMap.end())
	{
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;
		mTransforms[index] = globalTransformation * offset;
	}

	for (int i = 0; i < nodePtr->childrenCount; i++)
		CalculateBoneTransform(&nodePtr->children[i], globalTransformation);
}

std::vector<glm::mat4> Animator::GetPoseTransforms()
{
	return mTransforms;
}
