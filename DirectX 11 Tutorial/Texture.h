#pragma once

namespace radx
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		ID3D11ShaderResourceView* GetTexture();
		void Release();

		bool Initialize(ID3D11Device* device, const TCHAR* filename);

	private:
		ID3D11ShaderResourceView* mpTexture;
	};
}