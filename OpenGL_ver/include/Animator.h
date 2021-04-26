#pragma once

#include <Animation.h>



class Animator
{
public:
	Animator(Animation* currentPtr);

	void UpdateAnimation(float dt);

	void PlayAnimation(Animation* pAnimationPtr);

	void CalculateBoneTransform(const AssimpNodeData* nodePtr, glm::mat4 parentTransform);

	std::vector<glm::mat4> GetPoseTransforms();

private:
	std::vector<glm::mat4> mTransforms;
	Animation* mCurrentAnimationPtr;
	float mCurrentTime;
	float mDeltaTime;

};
