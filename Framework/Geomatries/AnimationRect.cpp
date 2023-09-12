#include "Framework.h"
#include "AnimationRect.h"

#include "Utilities/Animator.h"
AnimationRect::AnimationRect(Vector3 position, Vector3 size)
	:TextureRect(position, size, 0.0f)
{
	animator = new Animator();

	{
		//Texture2D* srcTex = new Texture2D(TexturePath + L"player.png");
		//AnimationClip* RunR = new AnimationClip(L"RunR", srcTex, 10,
		//	Values::ZeroVec2,
		//	Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.5f));
		//
		//AnimationClip* RunL = new AnimationClip(L"RunL", srcTex, 10,
		//	Vector2(0, srcTex->GetHeight() * 0.5f),
		//	Vector2(srcTex->GetWidth(), srcTex->GetHeight()));

		Texture2D* srcTex = new Texture2D(TexturePath + L"jelda.png");
		AnimationClip* RunR = new AnimationClip(L"RunR", srcTex, 10,
			Vector2(0, srcTex->GetHeight() * 0.75f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));

		AnimationClip* RunL = new AnimationClip(L"RunL", srcTex, 10,
			Vector2(0, srcTex->GetHeight() * 0.25f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.5f));

		AnimationClip* RunD = new AnimationClip(L"RunD", srcTex, 10,
			Values::ZeroVec2,
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.25f));

		AnimationClip* RunU = new AnimationClip(L"RunU", srcTex, 10,
			Vector2(0, srcTex->GetHeight() * 0.5f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 0.75f));

		animator->AddAnimClip(RunR);
		animator->AddAnimClip(RunL);
		animator->AddAnimClip(RunD);
		animator->AddAnimClip(RunU);
		animator->SetCurrentAnimClip(L"RunR");
		SAFE_DELETE(srcTex);
	}


	{
		D3D11_SAMPLER_DESC desc;
		States::GetSamplerDesc(&desc);
		States::CreateSampler(&desc, &point[0]);

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		States::CreateSampler(&desc, &point[1]);
		// 0 = 선형보간
		// 1 = 포인트 보간
	}
	//Blend
	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &bpoint[0]);

		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &bpoint[1]);

	}
}

AnimationRect::~AnimationRect()
{
}

void AnimationRect::Update()
{
	animator->Update();

	MapVertexBuffer();
	{
		vertices[0].uv.x = animator->GetCurrentFrame().x;
		vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

		vertices[1].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
		vertices[1].uv.y = animator->GetCurrentFrame().y;

		vertices[2].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
		vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

		vertices[3].uv.x = animator->GetCurrentFrame().x;
		vertices[3].uv.y = animator->GetCurrentFrame().y;

	}
	UnmapVertexBuffer();

	__super::Update();
}

void AnimationRect::Render()
{
	srv = animator->GetCurrentSRV();

	/*
	DC->PSSetSamplers(0, 1, &point[0]); // 선형보간
	DC->OMSetBlendState(bpoint[0], nullptr, (UINT)0xFFFFFFFF);//블렌드 비활성화

	DC->PSSetSamplers(0, 1, &point[1]); // 포인트 보간
	DC->OMSetBlendState(bpoint[0], nullptr, (UINT)0xFFFFFFFF);//블렌드 비활성화

	*/

	DC->PSSetSamplers(0, 1, &point[1]);// 포인트 보간
	DC->OMSetBlendState(bpoint[1], nullptr, (UINT)0xFFFFFFFF);//블렌드 활성화

	__super::Render();
}

void AnimationRect::Move()
{
	auto key = Keyboard::Get();
	float delta = Time::Delta();
	int pressed = 0;

	if (key->Press('D')) {
		position.x += 100 * delta;
		pressed += 1;
	}
	else if (key->Press('A')) {
		position.x -= 100 * delta;
		pressed += 2;
	}
	
	if (key->Press('W')) {
		position.y += 100 * delta;
		pressed += 4;
	}
	else if (key->Press('S')) {
		position.y -= 100 * delta;
		pressed += 8;
	}

	if (pressed == 1) {
		animator->SetCurrentAnimClip(L"RunR");
	}
	else if (pressed == 2) {
		animator->SetCurrentAnimClip(L"RunL");
	}
	else if (pressed == 4) {
		animator->SetCurrentAnimClip(L"RunU");
	}
	else if (pressed == 8) {
		animator->SetCurrentAnimClip(L"RunD");
	}
	else if (pressed == 9 || pressed == 10) {
		animator->SetCurrentAnimClip(L"RunD");
	}
	else if (pressed == 5 || pressed == 6) {
		animator->SetCurrentAnimClip(L"RunU");
	}
}
