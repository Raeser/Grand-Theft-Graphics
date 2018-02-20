#pragma once
struct aiScene;
struct SBone
{
	std::string Name;
	Corgi::Matrix44<float> Offset;
	Corgi::Matrix44<float> LocalTransform;
	Corgi::Matrix44<float> GlobalTransform;
	Corgi::Matrix44<float> OriginalLocalTransform;
	Corgi::GrowingArray<SBone*> Children;
	SBone* Parent;

	SBone() :Parent(nullptr) { Children.Init(1); }
	~SBone()
	{
		for (int i = 0; i < Children.Size(); i++)
		{
			delete Children[i];
		}
	}
};


class CSkeleton
{
public:
	CSkeleton();
	~CSkeleton();
	bool Init(const aiScene * aAnimationScene, const aiScene * aMeshScene);
	void Release();
	const Corgi::GrowingArray<SBone*> GetBones() const { return myBoneCopies; };
	std::map<std::string, SBone*>& GetBonesByName() { return myBonesByName; };
	void UpdateTransforms(SBone* aNode);
	SBone* GetRootNode() { return myRootBone; };
	static void TransformMatrix(Corgi::Matrix44<float>& out, const aiMatrix4x4& in);
private:
	SBone* CreateBoneTree(aiNode* pNode, SBone* pParent);
	void CalculateBoneToWorldTransform(SBone* child);
private:
	SBone* myRootBone;
	std::map<std::string, SBone*> myBonesByName;
	std::map<std::string, unsigned int> myBonesToIndex;
	Corgi::GrowingArray<SBone*> myBoneCopies; //remove these by running destructor of rootbone and it will recursively delete all bones


};

