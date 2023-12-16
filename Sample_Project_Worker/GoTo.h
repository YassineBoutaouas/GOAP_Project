#pragma once
#include "Action.h"

class GoTo : public IAction {
public:
	GoTo(float& workerPos_X, float& workerPos_Y, const std::string& target, const float& pos_x, const float& pos_y, int& currentTarget, const int& desiredTarget, const float& duration);
	~GoTo();

	void OnActionEnter() override;
	bool OnActionExecute() override;
	bool Validate() override;
	int CalculateCost() override;

private:
	float& m_WorkerPos_X;
	float& m_WorkerPos_Y;

	float m_TargetPos_X;
	float m_TargetPos_Y;

	int& m_CurrentTarget;
	int m_DesiredTarget;

	float m_TotalDuration;
	float m_CurrentTime;
};