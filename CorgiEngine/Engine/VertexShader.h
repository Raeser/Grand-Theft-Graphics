#pragma once


class CVertexShader
{
public:
	CVertexShader();
	~CVertexShader();
	ID3D11VertexShader* GetShader() const;
	ID3D11InputLayout* GetInputLayout() const;
	bool CreateShader(const char* aPath, ID3D11Device* aDevice, D3D11_INPUT_ELEMENT_DESC* aInputLayout, int aLayoutSize, const CShaderCompiler& aShaderCompiler);

private:
	void OnShaderFileModified(const std::wstring& afile);

	struct SShaderCompileData
	{
		std::string filePath;
		ID3D11Device* devicePtr;
		D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc = nullptr;
		int layoutSize;
		CShaderCompiler shaderCompiler;
	} myShaderCompileData;


	bool myIsBeingFilewatched;

	ID3D11VertexShader* myShader;
	ID3D11InputLayout* myInputLayout;
};

