#pragma once
class CSpriteInstance;
class CDXFramework;
class CSpriteLoader;
struct ID3D11Buffer;

class CSpriteRenderer
{
public:
	CSpriteRenderer(CSpriteLoader& aSpriteLoader);
	~CSpriteRenderer();
	bool Init(CDXFramework* aDXFramework);
	bool InitSpriteDataBuffer();

	CSpriteInstance* CreateInstance();
	CSpriteInstance* CreateInstance(long long aSpriteID);
	CSpriteInstance* CreateInstance(const char* aSpritePath);

	void Render(const Vector2int& aActiveTargetSize);
	void Render(CSpriteInstance* aSpriteInstance, const Vector2int& aActiveTargetSize);

	void AssignFramework(CDXFramework* aFramework);

private:
	bool UpdateSpriteBuffer(const Vector2f& aSpriteSize ,const Vector3f& aPos, const Vector4f& aColor, const Vector2f& aUVOffset, const Vector2int& aActiveTargetSize);

	struct SSpriteStruct
	{
		Vector4f Color;
		Vector3f Position;
		float trash;
		Vector2f TargetSize;
		Vector2f UVOffset;

	};
	SSpriteStruct mySpriteData;
	CSpriteLoader& mySpriteLoader;
	CDXFramework* myDXFramework;
	ID3D11Buffer* mySpriteBuffer;

private:
	struct Vertex
	{
		float x, y, z, w;
		float r, g, b, a;
		float u, v;
	};
};

