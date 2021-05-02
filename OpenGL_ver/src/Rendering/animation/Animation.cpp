#include "pch.h"


#include <Rendering/animation/AnimData.h>
#include <Rendering/Model.h>
#include <Rendering/animation/Animation.h>



Animation::Animation(const std::string& animationPath, Model* modelPtr)
{
	{
		Assimp::Importer importer;
		const aiScene* scenePtr = importer.ReadFile(animationPath, aiProcess_Triangulate);
		assert(scenePtr && scenePtr->mRootNode);
		auto animation = scenePtr->mAnimations[0];
		mDuration = animation->mDuration;
		mTicksPerSecond = animation->mTicksPerSecond;
		aiMatrix4x4 globalTransformation = scenePtr->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHeirarchyData(mRootNode, scenePtr->mRootNode);
		SetupBones(animation, *modelPtr);
	}
}

Bone* Animation::FindBone(const std::string& name)
{
	auto iter = std::find_if(mBones.begin(), mBones.end(),
		[&](const Bone& Bone)
		{
			return Bone.GetBoneName() == name;
		}
	);
	if (iter == mBones.end()) return nullptr;
	else return &(*iter);
}





void Animation::SetupBones(const aiAnimation* animationPtr, Model& model)
{
	int size = animationPtr->mNumChannels;

	auto& boneInfoMap = model.GetOffsetMatMap();
	int& boneCount = model.GetBoneCount();

	for (int i = 0; i < size; i++)
	{
		auto channel = animationPtr->mChannels[i];
		std::string boneName = channel->mNodeName.data;

		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		mBones.push_back(Bone(channel->mNodeName.data,
			boneInfoMap[channel->mNodeName.data].id, channel));
	}

	mBoneInfoMap = boneInfoMap;
}

void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* srcPtr)
{
	assert(srcPtr);

	dest.name = srcPtr->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(srcPtr->mTransformation);
	dest.childrenCount = srcPtr->mNumChildren;

	for (int i = 0; i < srcPtr->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHeirarchyData(newData, srcPtr->mChildren[i]);
		dest.children.push_back(newData);
	}
}
