#include "stdafx.h"
#include "KirbyCharacter.h"
#include "Geomatries/AnimationRect.h"
#include "UI/HUD.h"

KirbyCharacter::KirbyCharacter()
{
	kirbyAnimation = new AnimationRect({ 720, 360, 1.0 }, { 45, 45, 1 });

}

KirbyCharacter::~KirbyCharacter()
{
	SAFE_DELETE(kirbyAnimation);
}

void KirbyCharacter::Init()
{
	
}

void KirbyCharacter::Destroy()
{
}

void KirbyCharacter::Update()
{
	kirbyAnimation->Update();
}

void KirbyCharacter::Render()
{
	kirbyAnimation->Render();
}

void KirbyCharacter::PostRender()
{
}

void KirbyCharacter::GUI()
{
}

void KirbyCharacter::Move()
{
	kirbyAnimation->Move();
}

void KirbyCharacter::Inhale()
{
}

void KirbyCharacter::Exhale()
{
}

void KirbyCharacter::Swallow()
{
}

void KirbyCharacter::Attack()
{
}
