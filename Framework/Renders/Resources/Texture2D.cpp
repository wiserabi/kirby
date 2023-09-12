#include "Framework.h"
#include "Texture2D.h"

using namespace DirectX;
vector<TextureDesc> Textures::descs;

void Textures::Delete()
{
	for (TextureDesc desc : descs) {
		SAFE_RELEASE(desc.srv);
	}
}

void Textures::Load(Texture2D* texture)
{
	HRESULT hr;
	TexMetadata metaData;

	wstring ext = Path::GetExtension(texture->filePath);
	if (ext == L"tga")
	{
		hr = GetMetadataFromTGAFile(texture->filePath.c_str(), metaData);
		CHECK(hr);
	}
	else if (ext == L"dds") {
		hr = GetMetadataFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, metaData);
		CHECK(hr);
	}
	else {
		hr = GetMetadataFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, metaData);
		CHECK(hr);
	}
	uint width = metaData.width;
	uint height = metaData.height;

	TextureDesc desc;
	desc.filePath = texture->filePath;
	desc.width = width;
	desc.height = height;

	TextureDesc exist;
	bool bExist = false;
	for (TextureDesc temp : descs) {
		if (desc == temp) {
			bExist = true;
			exist = temp;

			break;
		}
	}

	if (bExist == true) {
		texture->srv = exist.srv;
		texture->metaData = metaData;
	}
	else {
		ScratchImage image;
		if (ext == L"tga")
		{
			hr = LoadFromTGAFile(texture->filePath.c_str(), &metaData, image);
			CHECK(hr);
		}
		else if (ext == L"dds") {
			hr = LoadFromDDSFile(texture->filePath.c_str(), DDS_FLAGS_NONE, &metaData, image);
			CHECK(hr);
		}
		else {
			hr = LoadFromWICFile(texture->filePath.c_str(), WIC_FLAGS_NONE, &metaData, image);
			CHECK(hr);
		}
		ID3D11ShaderResourceView* srv = nullptr;
		hr = CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(), metaData, &srv);
		CHECK(hr);

		desc.srv = srv;
		texture->srv = srv;
		texture->metaData = metaData;

		descs.push_back(desc);
	}
}

Texture2D::Texture2D(wstring filePath)
	: filePath(filePath)
{
	Textures::Load(this);
}

void Texture2D::ReadPixel(vector<Color>* pixels)
{
	ID3D11Texture2D* texture;
	srv->GetResource((ID3D11Resource**)&texture);

	ReadPixel(texture, pixels);
}

void Texture2D::ReadPixel(ID3D11Texture2D* texture, vector<Color>* pixels)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	texture->GetDesc(&srcDesc);

	D3D11_TEXTURE2D_DESC distDesc;
	ZeroMemory(&distDesc, sizeof(distDesc));

	distDesc.Usage = D3D11_USAGE_STAGING;
	distDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	distDesc.Width = srcDesc.Width;
	distDesc.Height = srcDesc.Height;
	distDesc.MipLevels= 1;
	distDesc.ArraySize = 1;
	distDesc.Format = srcDesc.Format;
	distDesc.SampleDesc = srcDesc.SampleDesc;

	ID3D11Texture2D* distTex = nullptr;

	HRESULT hr = DEVICE->CreateTexture2D(&distDesc, nullptr, &distTex);
	CHECK(hr);

	D3DX11LoadTextureFromTexture(DC, texture, nullptr, distTex);

	int pixelNum = distDesc.Width * distDesc.Height;

	uint* colors = new uint[pixelNum];

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(distTex, 0, D3D11_MAP_READ, 0, &subResource); 
	{
		memcpy(colors, subResource.pData, sizeof(uint) * distDesc.Width * distDesc.Height);
	}
	DC->Unmap(distTex, 0);

	uint* a = new uint[pixelNum];
	uint* r = new uint[pixelNum];
	uint* g = new uint[pixelNum];
	uint* b = new uint[pixelNum];

	for (int i = 0; i < pixelNum; i++) {
		r[i] = (colors[i] & 0xff000000);
		g[i] = (colors[i] & 0x00ff0000);
		b[i] = (colors[i] & 0x0000ff00);
		a[i] = (colors[i] & 0x000000ff);

		r[i] = colors[i] >> 16;
		b[i] = colors[i] << 16;

		colors[i] = a[i] | r[i] | g[i] | b[i];
	}

	pixels->assign(&colors[0], &colors[pixelNum - 1]);

	SAFE_DELETE_ARRAY(colors);
	SAFE_DELETE(distTex);

}
