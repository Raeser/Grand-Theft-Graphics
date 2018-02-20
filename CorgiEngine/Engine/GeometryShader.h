#pragma once

class CGeometryShader
{
public:
	CGeometryShader();
	~CGeometryShader();
	ID3D11GeometryShader* GetShader() const;
	bool CreateShader(const char* aPath, ID3D11Device* aDevice, const CShaderCompiler& aShaderCompiler);


private:

	struct SShaderCompileData
	{
		std::string filepath;
		ID3D11Device* devicePtr;
		CShaderCompiler shaderCompiler;
	}myShaderCompileData;

	void OnShaderFileModified(const std::wstring &);
	ID3D11GeometryShader* myShader;
	bool myIsBeingFilewatched;
};

