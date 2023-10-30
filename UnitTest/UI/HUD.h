#pragma once

#define PNGNUMHUD 3
class HUD
{
public:
	HUD();
	~HUD();

	void Update();
	void Render();
	void RenderScore();
	void RenderBossHealth();//show boss health

	void UpdateScore();
	void UpdateBossHealth();//show boss health

	void LoadStateImage();
	void LoadLifeImage();
	void LoadHealthImage();
	void LoadNumberImage();
	void LoadBossHpImage();

	void SetCurrentAbility(int ability);
	void SetState(int state);
	void SetPrevState(int prevState);
	void SetEnemyDeathCnt(int enemyDeathCnt);
	void CalcScoreDigits();
	void SetNothing(bool nothing) { this->nothing = nothing; }
	void SetBossHealth(int bossHealth);
	void ChangeToBossUI();
	void ChangeToNormalUI();

private:
	class ProgressBar* frameUI = nullptr;
	vector<class ProgressBar*> health;
	vector<class ProgressBar*> number;
	vector<class ProgressBar*> life;
	vector<class ProgressBar*> stateImg;
	vector<class ProgressBar*> bossHp;

	int ability = 8;//none
	int state = 0;//idle
	int prevState = 0;//prevState
	float healthAnimStart = Time::Get()->Running();
	float lifeAnimStart = Time::Get()->Running();
	bool hpAnimIdx = false;
	int lifeAnimIdx = 0;

	int hpLeft = 6;
	int lifeLeft = 4;
	int score = 0;
	int enemyDeathCnt = 0;
	vector<int> result;
	bool nothing = false;
	int bossHealth = 28;
	bool normalUI = true;//change to false only on boss fight

	wstring statePng[6] = {
		L"stateOuch.png",
		L"stateSpark.png",
		L"stateBeam.png",
		L"stateNothing.png",
		L"stateNormal.png",
		L"stateMiss.png",
	};
	wstring numberPng[10] = {
		L"number0.png",
		L"number1.png",
		L"number2.png",
		L"number3.png",
		L"number4.png",
		L"number5.png",
		L"number6.png",
		L"number7.png",
		L"number8.png",
		L"number9.png",
	};
	wstring healthPng[3] = {
		L"healthBar1.png",
		L"healthBar2.png",
		L"healthBarGone.png",
	};
	wstring lifePng[3] = {
		L"kirbyLife1.png",
		L"kirbyLife2.png",
		L"kirbyLife3.png",
	};
	wstring bossHpPng = L"bossHp.png";
};