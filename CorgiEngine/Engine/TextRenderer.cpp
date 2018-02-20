#include "stdafx.h"
#include "TextRenderer.h"
#include "FontLoader.h"
#define FONT_BUFF_SIZE 4096
#define MAX_AMOUNT_OF_CHARACTERS 4096


CTextRenderer::CTextRenderer(CEngine& aEngine):
	myEngine(aEngine)
{
}


CTextRenderer::~CTextRenderer()
{
	myTextInputLayout->Release();
	myTextInputLayout = nullptr;
	myTextInstanceBuffer->Release();
	myTextInstanceBuffer = nullptr;
	myTextVertexBuffer->Release();
	myTextVertexBuffer = nullptr;
	myTextPixelShader->Release();
	myTextPixelShader = nullptr;
	myTextGeometryShader->Release();
	myTextGeometryShader = nullptr;
	myTextVertexShader->Release();
	myTextVertexShader = nullptr;
}

void CTextRenderer::Init()
{
	myBufferedObjects.Init(FONT_BUFF_SIZE + 1);
	CreateTextVertexBuffer();
	LoadTextShaders();
	CreateTextInstanceBuffer();
}

void CTextRenderer::Render(SSimplifiedText & aTextToRender, CFontData & aFontToUse)
{
	size_t textSize = aTextToRender.bufferedText.size();
	if (textSize <= 0)
	{
		return;
	}
	myBufferedObjects.ReInit(aTextToRender.bufferedText.size());
	BuildText(aTextToRender.bufferedText, aTextToRender.color,  aTextToRender.scale, aFontToUse);

	float screenSizeX = static_cast<float>(myEngine.GetWindowSize().x);
	float screenSizeY = static_cast<float>(myEngine.GetWindowSize().y);
	const float screenWidthOffsetSpace = 0.05f;
	float positionOffsetX = (screenSizeX - (screenSizeX * screenWidthOffsetSpace));

	if (aFontToUse.myRenderCountChanged)
	{
		aFontToUse.myRenderCountChanged = false;
	}
	myCurrentCharacterAmount = 0;

	for (int i = 0; i < myBufferedObjects.Size(); i++)
	{
		if (i > FONT_BUFF_SIZE)
		{
			break;
		}
		STextToRender& textToRender = myBufferedObjects[i];
		if (myCurrentCharacterAmount < MAX_AMOUNT_OF_CHARACTERS)
		{
			textToRender.myPosition.x /= positionOffsetX;
			textToRender.myPosition.y /= screenSizeY;
			
			textToRender.mySize.x /= screenSizeX;
			textToRender.mySize.y /= screenSizeY;

			myCurrentCharacterAmount++;
		}
		else
		{
			DL_ERROR(eDebugLogType::Resource, "%s%s", "Text exceeds maximum amount of allowed characters! Keep it under 2048 unless you're writing a fucking book... text was: ", aTextToRender.bufferedText);
		}
	}
	UpdateTextBuffers();
	UpdateInstanceBuffer(aTextToRender.myScreenPosition);
	ID3D11DeviceContext* context = myEngine.GetDXFrameWork()->GetContext();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->IASetInputLayout(myTextInputLayout);
	context->VSSetShader(myTextVertexShader, nullptr, 0);
	context->PSSetShader(myTextPixelShader, nullptr, 0);
	context->GSSetShader(myTextGeometryShader, nullptr, 0);
	context->VSSetConstantBuffers(1, 1, &myTextInstanceBuffer);
	context->GSSetConstantBuffers(1, 1, &myTextInstanceBuffer);
	context->PSSetShaderResources(0, 1, &aFontToUse.myAtlasView);

	context->IASetVertexBuffers(0, 1, &myTextVertexBuffer, &myStride, &myOffset);
	UINT CharacterAmountToRender = 0;
	if (aTextToRender.textType == eText_Scrolling)
	{
		CharacterAmountToRender = static_cast<UINT>(aTextToRender.scrollingIndex);
	}
	if (aTextToRender.textType == eText_Normal || CharacterAmountToRender > myBufferedObjects.Size())
	{
		CharacterAmountToRender = max(static_cast<UINT>(myBufferedObjects.Size()), 0);
	}
	context->Draw(CharacterAmountToRender, 0);

}

void CTextRenderer::BuildText(const std::string & aText, Vector4f aColor, float aSize, CFontData& aFontData)
{
	std::string safeText = aText;
	int count = static_cast<int>(safeText.length());
	
	if (aFontData.myLastRenderCount != count)
	{
		aFontData.myLastRenderCount = count;
		aFontData.myRenderCountChanged = true;
	}
	float drawX = 0.f;
	float drawY = -5.f;
	float maxDrawY = 0.f;


	for (int i = 0, row = 0; i < safeText.length(); i++)
	{
		SCharData& charData = aFontData.myCharData[(int)safeText[i]];

		if (maxDrawY < charData.myHeight)
		{
			maxDrawY = static_cast<float>(charData.myHeight);
		}


		if (safeText[i] == '\n')
		{
			drawX = 0;
			drawY -= (maxDrawY + 6);
			row--;
			continue;
		}


		if (safeText[i] == ' ')
		{
			drawX += aFontData.myWordSpacing;
		}

		float top = drawY + charData.myBearingY;
		float bottom = top + charData.myHeight;

		STextToRender renderText;

		renderText.myPosition = { drawX * aSize, -bottom * aSize };
		renderText.mySize = { static_cast<float>(charData.myWidth), static_cast<float>(charData.myHeight) };


		renderText.myUV = charData.myTopLeftUV;
		renderText.uvScale = charData.myBottomRightUV;
		renderText.myColor = aColor;

		myBufferedObjects.Add(renderText);
		drawX += charData.myAdvanceX;
	}
}

bool CTextRenderer::LoadTextShaders()
{
	HRESULT result;
	ID3D11Device* device = myEngine.GetDXFrameWork()->GetDevice();

	//std::ifstream vsFile;
	//vsFile.open("../Assets/Shaders/VS_Text.hlsl", std::ios::binary);
	//std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };


	ID3D10Blob* VSBlob = nullptr;
	myShaderCompiler.CompileShader("../Assets/Shaders/VS_Text.hlsl", "main", "vs_5_0", VSBlob);
	result = device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, &myTextVertexShader);


	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "failed to load Text VertexShader");
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "loading of Text VertexShader Succeded")
	}
	//vsFile.close();

	//std::ifstream psFile;
	//psFile.open("../Assets/Shaders/PS_Text.hlsl", std::ios::binary);
	//std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	//result = device->CreatePixelShader(psData.data(), psData.size(), nullptr, &myTextPixelShader);


	ID3D10Blob* PSBlob = nullptr;
	myShaderCompiler.CompileShader("../Assets/Shaders/PS_Text.hlsl", "main", "ps_5_0", PSBlob);
	result = device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, &myTextPixelShader);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "failed to load Text VertexShader");
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "loading of Text PixelShader Succeded")
	}
	//psFile.close();

	//std::ifstream gsFile;
	//gsFile.open("../Assets/Shaders/GS_Text.hlsl", std::ios::binary);
	//std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
	//result = device->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &myTextGeometryShader);

	ID3D10Blob* GSBlob = nullptr;
	myShaderCompiler.CompileShader("../Assets/Shaders/GS_Text.hlsl", "main", "gs_5_0", GSBlob);
	result = device->CreateGeometryShader(GSBlob->GetBufferPointer(), GSBlob->GetBufferSize(), nullptr, &myTextGeometryShader);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "failed to load Text VertexShader");
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "loading of Text PixelShader Succeded")
	}
	//gsFile.close();
	bool inputResult = CreateTextInputLayout(VSBlob);

	VSBlob->Release();
	VSBlob = 0;
	GSBlob->Release();
	GSBlob = 0;
	PSBlob->Release();
	PSBlob = 0;

	return inputResult;
}

bool CTextRenderer::CreateTextVertexBuffer()
{
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = static_cast<UINT>(MAX_AMOUNT_OF_CHARACTERS + 1) * sizeof(STextToRender);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = myEngine.GetDXFrameWork()->GetDevice()->CreateBuffer(&bufferDescription, nullptr, &myTextVertexBuffer);
	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "failed to initialize TextVertexBuffer");
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "loading of TextVertexBuffer Succeded")
	}
	myStride = sizeof(STextToRender);
	myOffset = 0;
	return true;
}

bool CTextRenderer::CreateTextInstanceBuffer()
{
	ID3D11Device* device = myEngine.GetDXFrameWork()->GetDevice();
	HRESULT result;
	D3D11_BUFFER_DESC bufferDescription = {};
	bufferDescription.ByteWidth = sizeof(Vector4f);
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = device->CreateBuffer(&bufferDescription, nullptr, &myTextInstanceBuffer);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "TextRenderer failed to create PositionBuffer");
		if (result == E_INVALIDARG)
		{
			DL_ERROR(eDebugLogType::Render, "%s", "invalidArgument !?");
		}
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "initialization of textinstancebuffer succeded in textRenderer!");
	}
	return true;
}


bool CTextRenderer::CreateTextInputLayout(ID3D10Blob* aBlob)
{
	ID3D11Device* device = myEngine.GetDXFrameWork()->GetDevice();
	HRESULT result = S_OK;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "UVTOPLEFT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "UVTOPRIGHT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA },
	};
	result = device->CreateInputLayout(layout, 5, aBlob->GetBufferPointer(), aBlob->GetBufferSize(), &myTextInputLayout);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "Create Text InputLayout failed to load");
		return false;
	}
	else
	{
		DL_SUCCESS(eDebugLogType::Render, "%s", "Create Text InputLayout load succeded");
		return true;
	}
}

bool CTextRenderer::UpdateTextBuffers()
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE data;
	STextToRender* dataToMemcpy = myBufferedObjects.GetPureData();
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = myEngine.GetDXFrameWork()->GetContext()->Map(myTextVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "TextRenderer failed to map buffers in UpdateVertexData");
		return false;
	}
	memcpy(data.pData, dataToMemcpy, (sizeof(STextToRender) * (myBufferedObjects.Size())));
	myEngine.GetDXFrameWork()->GetContext()->Unmap(myTextVertexBuffer, 0);
	return true;
}

bool CTextRenderer::UpdateInstanceBuffer(const Vector4f& aPosition)
{
	ID3D11DeviceContext* context = myEngine.GetDXFrameWork()->GetContext();

	D3D11_MAPPED_SUBRESOURCE data;
	HRESULT result;

	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	result = context->Map(myTextInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);

	if (FAILED(result))
	{
		DL_ERROR(eDebugLogType::Render, "%s", "sprite renderer failed to update spritebuffer");
		return false;
	}


	memcpy(data.pData, &aPosition, sizeof(Vector4f));
	context->Unmap(myTextInstanceBuffer, 0);

	return true;
}

