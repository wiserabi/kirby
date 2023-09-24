#include "stdafx.h"
#include "Gravity.h"

Gravity::Gravity()
{
	gravity = 9.8f;
	start = 0.0f;
	current = 0.0f;
	currentGravity = 0.0f;
}

void Gravity::SetGravity(float gravity)
{
	this->gravity = gravity;
}

void Gravity::GravityStart()
{
	start = Time::Get()->Running();
}

float Gravity::GetCurrentGravity()
{
	current = Time::Get()->Running();
	float elapsed = current - start;
	if (elapsed > 0.0f) {
		currentGravity = elapsed * elapsed * 0.5 * gravity;
		return currentGravity;
	}
	return 0.0f;
}

float Gravity::PredictNextGravity()
{
	current = Time::Get()->Running();
	float elapsed = current - start + Time::Get()->Delta();
	if (elapsed > 0.0f) {
		currentGravity = elapsed * elapsed * 0.5 * gravity;
		return currentGravity;
	}
	return 0.0f;
}