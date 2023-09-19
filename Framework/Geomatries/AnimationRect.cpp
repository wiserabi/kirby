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

		Texture2D* srcTex = new Texture2D(TexturePath + L"kirbyAnim/kirbywalkright.png");
		AnimationClip* RunR = new AnimationClip(L"RunR", srcTex, 4,
			Vector2(0, srcTex->GetHeight() * 0.0f),
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));
		AnimationClip* RunL = new AnimationClip(L"RunL", srcTex, 4,
			Values::ZeroVec2,
			Vector2(srcTex->GetWidth(), srcTex->GetHeight() * 1.0f));

		animator->AddAnimClip(RunR);
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

	if (key->Press(VK_RIGHT)) {
		position.x += 100 * delta;
		pressed += 1;
	}
	else if (key->Press(VK_LEFT)) {
		position.x -= 100 * delta;
		pressed += 1;
	}

	if (pressed == 1) {
		animator->SetCurrentAnimClip(L"RunR");
	}

}
