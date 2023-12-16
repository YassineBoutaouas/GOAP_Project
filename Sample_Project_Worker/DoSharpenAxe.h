#pragma once
#include "Action.h"

class DoSharpenAxe : public IAction {
public:
	DoSharpenAxe(int& axeSharpness, int& stamina, const float& duration);
	~DoSharpenAxe();

	void OnActionEnter() override;
	bool OnActionExecute() override;
	bool Validate() override;
	int CalculateCost() override;

private:
	int& m_AxeSharpness;
	int& m_Stamina;
	float m_Duration;
	float m_CurrentTime;
};