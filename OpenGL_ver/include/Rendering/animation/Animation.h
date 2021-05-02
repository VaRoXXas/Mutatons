#pragma once

#include <Rendering/animation/Bone.h>
#include <Rendering/Model.h>

//This class is designed to set up animation

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};



class Animation
{
public:
	Animation() = default;

	Animation(const std::string& animationPath, Model* modelPtr);

	~Animation()
	{
	}

	Bone* FindBone(const std::string& name);


	inline float GetTicksPerSecond()
	{
		return mTicksPerSecond;
	}

	inline float GetDuration()
	{
		return mDuration;
	}

	inline const AssimpNodeData& GetRootNode()
	{
		return mRootNode;
	}

	inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return mBoneInfoMap;
	}

private:
	void SetupBones(const aiAnimation* animationPtr, Model& model);

	void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* srcPtr);

	float mDuration;
	int mTicksPerSecond;
	std::vector<Bone> mBones;
	AssimpNodeData mRootNode;
	std::map<std::string, BoneInfo> mBoneInfoMap;
};