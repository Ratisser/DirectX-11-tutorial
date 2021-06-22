#include "pch.h"
#include "Texture.h"

namespace radx
{
	Texture::Texture()
		: mpTexture(nullptr)
	{
	}

	Texture::~Texture()
	{
	}

	ID3D11ShaderResourceView* Texture::GetTexture()
	{
		return mpTexture;
	}

	void Texture::Release()
	{
		if (mpTexture)
		{
			mpTexture->Release();
			mpTexture = nullptr;
		}
		delete this;
	}

	bool Texture::Initialize(ID3D11Device* device, const TCHAR* filename)
	{
		HRESULT hr = S_OK;

		hr = D3DX11CreateShaderResourceViewFromFile(device, filename, nullptr, nullptr, &mpTexture, nullptr);
		if (FAILED(hr))
		{
			return false;
		}
		return true;
	}
}