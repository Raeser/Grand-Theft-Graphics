#pragma once
struct ID3D10Blob;

class CShaderCompiler
{
public:
	CShaderCompiler();
	~CShaderCompiler();
	bool FileExist(const char * filename) const;
	bool CompileShader(const char * aShader, const char * aMainFunction, const char * aTarget, ID3D10Blob *& aCodeBlob) const;

private:
};


