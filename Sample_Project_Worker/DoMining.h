#pragma once
#include "Worker.h"

class DoMining : public IAction {
public:
	DoMining(int& axeSharpness, int& stamina, int& itemsCarried, const float& duration, int& target);
	~DoMining();

	void OnActionEnter() override;
	bool OnActionExecute() override;
	bool Validate() override;
	int CalculateCost() override;

private:
	int& m_AxeSharpness;
	int& m_Stamina;
	int& m_AmountItemsCarried;
	float m_Duration;
	float m_ElapsedTime;

	int& m_CurrentTarget;
};