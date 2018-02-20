#pragma once

class CPixelShader
{
public:
	CPixelShader();
	~CPixelShader();
	ID3D11PixelShader* GetShader() const;
	bool CreateShader(const char* aPath, ID3D11Device* aDevice, const CShaderCompiler& aShaderCompiler);



private:

	void OnShaderFileModified(const std::wstring &);
	struct SCompileShaderData
	{
		std::string filepath;
		ID3D11Device* devicePtr;
		CShaderCompiler shaderCompiler;
	} myShaderCompileData;

	bool myIsBeingFilewatched;
	ID3D11PixelShader* myShader;
};

