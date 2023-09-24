#pragma once

class Gravity {
public:
	Gravity();
	void SetGravity(float gravity);
	void GravityStart();
	float GetCurrentGravity();
	float PredictNextGravity();
	
private:
	float gravity;
	float currentGravity;
	float start;
	float current;
};