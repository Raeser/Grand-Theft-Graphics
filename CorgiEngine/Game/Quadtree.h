#pragma once
#include "..\Engine\Subscriber.h"
#include "..\Utilities\GrowingArray.h"


namespace Corgi
{
	template <class T>
	class Quadtree
	{

	public:


		struct AABB
		{
			Vector2f topRight;
			Vector2f bottomLeft;

			Vector2f GetCenter()
			{
				return bottomLeft + ((topRight - bottomLeft) / 2.0f);
			}
		};

		struct LooseQuadTreeNode
		{
			AABB aabb;
			AABB looseArea;
			LooseQuadTreeNode* children[4];
			LooseQuadTreeNode* parent;
			Corgi::GrowingArray<T> objects;
			Corgi::GrowingArray<Vector2f> positionOfObjects;
		};
			

		Quadtree<T>();
		~Quadtree<T>();

		void Init();				

		void AddObject(T aObject, const Vector3f& aPosition);
		void AddToLooseAreas(LooseQuadTreeNode& aNode, T& aObject, const Vector2f& aPosition);

		LooseQuadTreeNode* ClosestChildNode(LooseQuadTreeNode* aCurrentQuadTreeNode, const Vector2f& aPosition);
		
		void Split(LooseQuadTreeNode& aCurrentQuadTreeNode);
		void ChechIfTimeToSplit(LooseQuadTreeNode& aCurrentQuadTreeNode);
		
		void AddNodeChildren(LooseQuadTreeNode& aCurrentQuadTreeNode);
		void AddNodeLooseArea(LooseQuadTreeNode& aCurrentQuadTreeNode);
		
		LooseQuadTreeNode* GetClosestTreeNode(const Vector2f& aPosition);
		Corgi::GrowingArray<T>& GetObjectsInClosestTreeNode(const Vector2f& aPosition);

		bool CheckCollisionWithLooseArea(LooseQuadTreeNode* aCurrentQuadTreeNode, const Vector2f& aPosition);
		

	private:
		LooseQuadTreeNode myQuadTree;
		float myNodeSize;
		bool myInitialized;

		int myDebugNbrOfSplits;

	};


	template<class T>
	inline Quadtree<T>::Quadtree()
	{
		myInitialized = false;
	}

	template<class T>
	inline Quadtree<T>::~Quadtree()
	{
	}

	template<class T>
	inline void Quadtree<T>::Init()
	{
		myDebugNbrOfSplits = 0;

		myNodeSize = 10000.0f;

		myQuadTree.objects.Init(9999);
		myQuadTree.positionOfObjects.Init(9999);
		myQuadTree.parent = nullptr;

		myQuadTree.aabb.topRight = { myNodeSize / 2.0f, myNodeSize / 2.0f };
		myQuadTree.aabb.bottomLeft = { -myNodeSize / 2.0f, -myNodeSize / 2.0f };

		myQuadTree.looseArea.topRight = { myNodeSize, myNodeSize };
		myQuadTree.looseArea.bottomLeft = { -myNodeSize, -myNodeSize };

		for (unsigned short nbrOfChildren = 0; nbrOfChildren < 4; ++nbrOfChildren)
		{
			myQuadTree.children[nbrOfChildren] = nullptr;
		}

		myInitialized = true;
	}

	template<class T>
	inline void Quadtree<T>::AddObject(T aObject, const Vector3f& aPosition)
	{
		assert(myInitialized && "Quadtree has not been initialized");
		
		Vector2f newPosition(aPosition.x, aPosition.z);

		LooseQuadTreeNode* currentQuadTreeNode = &myQuadTree;

		AddToLooseAreas(*currentQuadTreeNode, aObject, newPosition);
	}

	template<class T>
	inline void Quadtree<T>::AddToLooseAreas(LooseQuadTreeNode& aNode, T& aObject, const Vector2f& aPosition)
	{
		if (aNode.children[0] != nullptr)
		{
			for (unsigned short checkOtherChildren = 0; checkOtherChildren < 4; ++checkOtherChildren)
			{
				if (CheckCollisionWithLooseArea(aNode.children[checkOtherChildren], aPosition))
				{
					AddToLooseAreas(*aNode.children[checkOtherChildren], aObject, aPosition);
				}
			}
		}
		else
		{
			aNode.objects.Add(aObject);
			aNode.positionOfObjects.Add(aPosition);
			ChechIfTimeToSplit(aNode);
		}
	}

	template<class T>
	inline typename Quadtree<T>::LooseQuadTreeNode* Quadtree<T>::ClosestChildNode(LooseQuadTreeNode* aCurrentQuadTreeNode, const Vector2f& aPosition)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		Quadtree<T>::LooseQuadTreeNode* returnNode = aCurrentQuadTreeNode;

		if (aCurrentQuadTreeNode->children[0] != nullptr)
		{
			float shortestDistance = 99999.0f;

			for (unsigned short nbrOfChildren = 0; nbrOfChildren < 4; ++nbrOfChildren)
			{
				float distance = std::fabs((aPosition - aCurrentQuadTreeNode->children[nbrOfChildren]->aabb.GetCenter()).Length());
				if (shortestDistance > distance)
				{
					shortestDistance = distance;
					returnNode = aCurrentQuadTreeNode->children[nbrOfChildren];
				}
			}
		}

		return returnNode;
	}

	template<class T>
	inline void Quadtree<T>::Split(LooseQuadTreeNode& aCurrentQuadTreeNode)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		for (unsigned short nbrOfChildren = 0; nbrOfChildren < 4; ++nbrOfChildren)
		{
			aCurrentQuadTreeNode.children[nbrOfChildren] = new LooseQuadTreeNode();
			aCurrentQuadTreeNode.children[nbrOfChildren]->objects.Init(9999);
			aCurrentQuadTreeNode.children[nbrOfChildren]->positionOfObjects.Init(9999);
			aCurrentQuadTreeNode.children[nbrOfChildren]->parent = &aCurrentQuadTreeNode;

			for (unsigned short nbrOfChildrenChildrens = 0; nbrOfChildrenChildrens < 4; ++nbrOfChildrenChildrens)
			{
				aCurrentQuadTreeNode.children[nbrOfChildren]->children[nbrOfChildrenChildrens] = nullptr;
			}

		}

		AddNodeChildren(aCurrentQuadTreeNode);
		AddNodeLooseArea(aCurrentQuadTreeNode);

		unsigned int totalObjects = aCurrentQuadTreeNode.objects.Size();

		for (unsigned int nbrOfObjects = totalObjects; nbrOfObjects > 0; --nbrOfObjects)
		{
			LooseQuadTreeNode* addToThisNode = nullptr;

			addToThisNode = ClosestChildNode(&aCurrentQuadTreeNode, aCurrentQuadTreeNode.positionOfObjects[nbrOfObjects - 1]);

			addToThisNode->objects.Add(aCurrentQuadTreeNode.objects[nbrOfObjects - 1]);
			addToThisNode->positionOfObjects.Add(aCurrentQuadTreeNode.positionOfObjects[nbrOfObjects - 1]);

			if (addToThisNode->parent != nullptr)
			{
				for (unsigned short checkOtherChildren = 0; checkOtherChildren < 4; ++checkOtherChildren)
				{
					if (addToThisNode->parent->children[checkOtherChildren] == addToThisNode)
					{
						continue;
					}

					if (CheckCollisionWithLooseArea(addToThisNode->parent->children[checkOtherChildren], aCurrentQuadTreeNode.positionOfObjects[nbrOfObjects - 1]))
					{
						addToThisNode->parent->children[checkOtherChildren]->objects.Add(aCurrentQuadTreeNode.objects[nbrOfObjects - 1]);
						addToThisNode->parent->children[checkOtherChildren]->positionOfObjects.Add(aCurrentQuadTreeNode.positionOfObjects[nbrOfObjects - 1]);
						ChechIfTimeToSplit(*addToThisNode->parent->children[checkOtherChildren]);
					}
				}
			}

			aCurrentQuadTreeNode.objects.RemoveCyclicAtIndex(nbrOfObjects - 1);
			aCurrentQuadTreeNode.positionOfObjects.RemoveCyclicAtIndex(nbrOfObjects - 1);
		}
	}

	template<class T>
	inline void Quadtree<T>::ChechIfTimeToSplit(LooseQuadTreeNode& aCurrentQuadTreeNode)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		if (aCurrentQuadTreeNode.objects.Size() == 51 &&
			aCurrentQuadTreeNode.children[0] == nullptr &&
			(aCurrentQuadTreeNode.aabb.topRight.x - aCurrentQuadTreeNode.aabb.bottomLeft.x) > 40.0f)
			//Gör så att quadTrees aldrig blir mindre än 50x50x50 i storlek. Annars blir det risk för infinite loop om man skapar 99st på exakt samma plats.
		{
			++myDebugNbrOfSplits;

			//std::cout << "Number of splits: " << myDebugNbrOfSplits << std::endl;

			Split(aCurrentQuadTreeNode);

			for (unsigned short nbrOfChildren = 0; nbrOfChildren < 4; ++nbrOfChildren)
			{
				if (aCurrentQuadTreeNode.children[nbrOfChildren] != nullptr)
				{
					ChechIfTimeToSplit(*aCurrentQuadTreeNode.children[nbrOfChildren]);
				}
			}			
		}
	}

	template<class T>
	inline void Quadtree<T>::AddNodeChildren(LooseQuadTreeNode& aCurrentQuadTreeNode)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		float halfXarea = (aCurrentQuadTreeNode.aabb.topRight.x - aCurrentQuadTreeNode.aabb.bottomLeft.x) / 2.0f;
		float halfZarea = (aCurrentQuadTreeNode.aabb.topRight.y - aCurrentQuadTreeNode.aabb.bottomLeft.y) / 2.0f;


		aCurrentQuadTreeNode.children[0]->aabb.topRight = aCurrentQuadTreeNode.aabb.topRight;
		aCurrentQuadTreeNode.children[0]->aabb.bottomLeft = {
			aCurrentQuadTreeNode.aabb.bottomLeft.x + halfXarea,
			aCurrentQuadTreeNode.aabb.bottomLeft.y + halfZarea
		};

		
		aCurrentQuadTreeNode.children[1]->aabb.topRight = {
			aCurrentQuadTreeNode.aabb.bottomLeft.x + halfXarea,
			aCurrentQuadTreeNode.aabb.topRight.y
		};
		aCurrentQuadTreeNode.children[1]->aabb.bottomLeft = {
			aCurrentQuadTreeNode.aabb.bottomLeft.x,
			aCurrentQuadTreeNode.aabb.bottomLeft.y + halfZarea,
		};
		

		aCurrentQuadTreeNode.children[2]->aabb.topRight = {
			aCurrentQuadTreeNode.aabb.topRight.x,
			aCurrentQuadTreeNode.aabb.bottomLeft.y + halfZarea
		};
		aCurrentQuadTreeNode.children[2]->aabb.bottomLeft = {
			aCurrentQuadTreeNode.aabb.bottomLeft.x + halfXarea,
			aCurrentQuadTreeNode.aabb.bottomLeft.y 
		};


		aCurrentQuadTreeNode.children[3]->aabb.topRight = {
			aCurrentQuadTreeNode.aabb.bottomLeft.x + halfXarea,
			aCurrentQuadTreeNode.aabb.bottomLeft.y + halfZarea
		};
		aCurrentQuadTreeNode.children[3]->aabb.bottomLeft = aCurrentQuadTreeNode.aabb.bottomLeft;
	}

	template<class T>
	inline void Quadtree<T>::AddNodeLooseArea(LooseQuadTreeNode& aCurrentQuadTreeNode)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		float looseAreaOffsetX = (aCurrentQuadTreeNode.looseArea.topRight.x - aCurrentQuadTreeNode.looseArea.bottomLeft.x) / 8.0f;
		float looseAreaOffsetZ = (aCurrentQuadTreeNode.looseArea.topRight.y - aCurrentQuadTreeNode.looseArea.bottomLeft.y) / 8.0f;


		for (unsigned short child = 0; child < 4; ++child)
		{
			aCurrentQuadTreeNode.children[child]->looseArea.topRight =
			{
				aCurrentQuadTreeNode.children[child]->aabb.topRight.x + looseAreaOffsetX,
				aCurrentQuadTreeNode.children[child]->aabb.topRight.y + looseAreaOffsetZ
			};
			aCurrentQuadTreeNode.children[child]->looseArea.bottomLeft = {
				aCurrentQuadTreeNode.children[child]->aabb.bottomLeft.x - looseAreaOffsetX,
				aCurrentQuadTreeNode.children[child]->aabb.bottomLeft.y - looseAreaOffsetZ
			};
		}
	}

	template<class T>
	inline typename Quadtree<T>::LooseQuadTreeNode* Quadtree<T>::GetClosestTreeNode(const Vector2f& aPosition)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		LooseQuadTreeNode* currentQuadTreeNode = &myQuadTree;

		while (currentQuadTreeNode->children[0] != nullptr)
		{
			currentQuadTreeNode = ClosestChildNode(currentQuadTreeNode, aPosition);
		}

		return currentQuadTreeNode;
	}

	template<class T>
	inline Corgi::GrowingArray<T>& Quadtree<T>::GetObjectsInClosestTreeNode(const Vector2f& aPosition)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		LooseQuadTreeNode* currentQuadTreeNode = &myQuadTree;

		while (currentQuadTreeNode->children[0] != nullptr)
		{
			currentQuadTreeNode = ClosestChildNode(currentQuadTreeNode, aPosition);
		}

		return currentQuadTreeNode->objects;
	}

	template<class T>
	inline bool Quadtree<T>::CheckCollisionWithLooseArea(LooseQuadTreeNode* aCurrentQuadTreeNode, const Vector2f& aPosition)
	{
		assert(myInitialized && "Quadtree has not been initialized");

		if (aPosition.x > aCurrentQuadTreeNode->looseArea.bottomLeft.x &&
			aPosition.x < aCurrentQuadTreeNode->looseArea.topRight.x &&
			aPosition.y > aCurrentQuadTreeNode->looseArea.bottomLeft.y &&
			aPosition.y < aCurrentQuadTreeNode->looseArea.topRight.y)
		{
			return true;
		}
		
		return false;
	}

}