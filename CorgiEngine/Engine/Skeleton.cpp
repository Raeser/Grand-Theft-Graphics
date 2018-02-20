#include "stdafx.h"
#include "Skeleton.h"

void CSkeleton::TransformMatrix(Corgi::Matrix44<float>& out, const aiMatrix4x4& in)
{// there is some type of alignment issue with my mat4 and the aimatrix4x4 class, so the copy must be manually //VETTE FAN OM DETTA STÄMMER KAN VARA RÄTT I VÅR MEN LOL IDK
	out.myMatrix[0] = in.a1;
	out.myMatrix[1] = in.a2;
	out.myMatrix[2] = in.a3;
	out.myMatrix[3] = in.a4;
	out.myMatrix[4] = in.b1;
	out.myMatrix[5] = in.b2;
	out.myMatrix[6] = in.b3;
	out.myMatrix[7] = in.b4;
	out.myMatrix[8] = in.c1;
	out.myMatrix[9] = in.c2;
	out.myMatrix[10] = in.c3;
	out.myMatrix[11] = in.c4;
	out.myMatrix[12] = in.d1;
	out.myMatrix[13] = in.d2;
	out.myMatrix[14] = in.d3;
	out.myMatrix[15] = in.d4;
}

CSkeleton::CSkeleton()
{
	myBoneCopies.Init(1);
}


CSkeleton::~CSkeleton()
{
}

bool CSkeleton::Init(const aiScene * aAnimationScene, const aiScene * aMeshScene)
{
	if (aMeshScene->HasAnimations())
	{
		DL_ERROR(eDebugLogType::Resource, "%s", "tried to load a Animation as a mesh wtf dude" );
		//return false;
	}
	myRootBone = CreateBoneTree(aAnimationScene->mRootNode, NULL); //creates the entire skeleton

	//this just maps all bones to the different LUTs
	myBoneCopies.Init(aMeshScene->mMeshes[0]->mNumBones);
	for (unsigned int i = 0; i < aMeshScene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = aMeshScene->mMeshes[i];

		for (unsigned int n = 0; n < mesh->mNumBones; ++n)
		{
			const aiBone* bone = mesh->mBones[n];
			std::map<std::string, SBone*>::iterator found = myBonesByName.find(bone->mName.data);
			if (found != myBonesByName.end())
			{
				bool alreadyInList = false;
				for (int j(0); j < myBoneCopies.Size(); j++)
				{
					std::string bname = bone->mName.data;
					if (myBoneCopies[j]->Name == bname) {
						alreadyInList = true;
						break;
					}
				}
				if (!alreadyInList)
				{
					std::string tes = found->second->Name;
					TransformMatrix(found->second->Offset, bone->mOffsetMatrix);
					found->second->Offset = Corgi::Matrix44<float>::Transpose(found->second->Offset);
					//found->second->Offset.Transpose();// transpoce their matrix to get in the correct format
					myBoneCopies.Add(found->second);
					myBonesToIndex[found->first] = (unsigned int)myBoneCopies.Size() - 1;
				}
			}
		}
	}
	return !myRootBone->Parent;
}

void CSkeleton::Release()
{
	delete myRootBone;// This node will delete all children recursivly
	myRootBone = nullptr;// make sure to zero it out
	myBoneCopies.RemoveAll();
	myBonesByName.clear();
}

void CSkeleton::UpdateTransforms(SBone * aNode)
{
	CalculateBoneToWorldTransform(aNode);
	for (SBone* bone : aNode->Children)
	{
		UpdateTransforms(bone);
	}
}

SBone * CSkeleton::CreateBoneTree(aiNode* pNode, SBone* pParent)
{
	SBone* internalNode = new SBone();// create a node
	internalNode->Name = pNode->mName.data;// get the name of the bone
	internalNode->Parent = pParent; //set the parent, in the case this is theroot node, it will be null

	myBonesByName[internalNode->Name] = internalNode;// use the name as a key
	TransformMatrix(internalNode->LocalTransform, pNode->mTransformation);
	internalNode->LocalTransform = Corgi::Matrix44<float>::Transpose(internalNode->LocalTransform);
	internalNode->OriginalLocalTransform = internalNode->LocalTransform;// a copy saved
	CalculateBoneToWorldTransform(internalNode);
	// continue for all child nodes and assign the created internal nodes as our children
	internalNode->Children.ReInit(pNode->mNumChildren);
	for (unsigned int a = 0; a < pNode->mNumChildren; a++)

	{// recursivly call this function on all children
		internalNode->Children.Add(CreateBoneTree(pNode->mChildren[a], internalNode));
	}
	return internalNode;
}

void CSkeleton::CalculateBoneToWorldTransform(SBone * child)
{
	child->GlobalTransform = child->LocalTransform;
	SBone* parent = child->Parent;
	while (parent)
	{
		child->GlobalTransform *= parent->LocalTransform;
		parent = parent->Parent;
	}
}
