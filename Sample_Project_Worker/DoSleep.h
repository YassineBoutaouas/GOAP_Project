#pragma once
#include "Action.h"

class DoSleep : public IAction {
public:
	DoSleep(int& stamina, const float& duration);
	~DoSleep();

	void OnActionEnter() override;
	bool OnActionExecute() override;
	bool Validate() override;
	int CalculateCost() override;

private:
	int& m_Stamina;
	float m_TargetStamina;
	float m_CurrentTime;
};