#include "Framework.h"
#include "AnimationRect.h"

AnimationRect::AnimationRect(Vector3 position, Vector3 size, boolean left)
	:TextureRect(position, size, 0.0f)
{
	this->position = position;
	this->left = left;
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

void AnimationRect::Update(class Animator* anim)
{
	anim->Update();

	MapVertexBuffer();
	{
		if (!left) {
			vertices[0].uv.y = anim->GetCurrentFrame().y + anim->GetTexelFrameSize().y;
			vertices[0].uv.x = anim->GetCurrentFrame().x;

			vertices[1].uv.x = anim->GetCurrentFrame().x + anim->GetTexelFrameSize().x;
			vertices[1].uv.y = anim->GetCurrentFrame().y;

			vertices[2].uv.x = anim->GetCurrentFrame().x + anim->GetTexelFrameSize().x;
			vertices[2].uv.y = anim->GetCurrentFrame().y + anim->GetTexelFrameSize().y;

			vertices[3].uv.x = anim->GetCurrentFrame().x;
			vertices[3].uv.y = anim->GetCurrentFrame().y;
		}
		else if (left) {
			vertices[0].uv.x = anim->GetCurrentFrame().x + anim->GetTexelFrameSize().x;
			vertices[0].uv.y = anim->GetCurrentFrame().y + anim->GetTexelFrameSize().y;

			vertices[1].uv.x = anim->GetCurrentFrame().x;
			vertices[1].uv.y = anim->GetCurrentFrame().y;

			vertices[2].uv.x = anim->GetCurrentFrame().x;
			vertices[2].uv.y = anim->GetCurrentFrame().y + anim->GetTexelFrameSize().y;

			vertices[3].uv.x = anim->GetCurrentFrame().x + anim->GetTexelFrameSize().x;
			vertices[3].uv.y = anim->GetCurrentFrame().y;
		}
	}
	UnmapVertexBuffer();

	__super::Update();
}

void AnimationRect::Update()
{
	animator->Update();

	MapVertexBuffer();
	{
		if (!left) {
			vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;
			vertices[0].uv.x = animator->GetCurrentFrame().x;

			vertices[1].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
			vertices[1].uv.y = animator->GetCurrentFrame().y;

			vertices[2].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
			vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

			vertices[3].uv.x = animator->GetCurrentFrame().x;
			vertices[3].uv.y = animator->GetCurrentFrame().y;
		}
		else if (left) {
			vertices[0].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
			vertices[0].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

			vertices[1].uv.x = animator->GetCurrentFrame().x;
			vertices[1].uv.y = animator->GetCurrentFrame().y;

			vertices[2].uv.x = animator->GetCurrentFrame().x;
			vertices[2].uv.y = animator->GetCurrentFrame().y + animator->GetTexelFrameSize().y;

			vertices[3].uv.x = animator->GetCurrentFrame().x + animator->GetTexelFrameSize().x;
			vertices[3].uv.y = animator->GetCurrentFrame().y;
		}
	}
	UnmapVertexBuffer();

	__super::Update();
}

void AnimationRect::Render(class Animator* anim)
{
	srv = anim->GetCurrentSRV();

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
	this->position += direction * velocity;
}
