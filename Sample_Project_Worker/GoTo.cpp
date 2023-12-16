#include "GoTo.h"

GoTo::GoTo(float& workerPos_X, float& workerPos_Y, const std::string& target, const float& pos_x, const float& pos_y, int& currentTarget, const int& desiredTarget, const float& duration)
	: IAction("GoTo (" + target + ")"), m_WorkerPos_X(workerPos_X), m_WorkerPos_Y(workerPos_Y), m_TargetPos_X(pos_x), m_TargetPos_Y(pos_y), m_CurrentTarget(currentTarget), m_DesiredTarget(desiredTarget), m_TotalDuration(duration), m_CurrentTime(0)
{
	Effects.Add("AtPosition(" + target + ")", WorldState::True);
}

GoTo::~GoTo() {

}

void GoTo::OnActionEnter() {
	std::cout << "Enter GoTo\n";
	m_CurrentTime = 0;
}

bool GoTo::OnActionExecute() {
	if (m_CurrentTime >= m_TotalDuration) {
		m_WorkerPos_X = m_TargetPos_X;
		m_WorkerPos_Y = m_TargetPos_Y;
		m_CurrentTarget = m_DesiredTarget;
		return false;
	}

	std::cout << "Going to\n";
	m_CurrentTime++;
	return true;
}

bool GoTo::Validate() {
	return true;
}

int GoTo::CalculateCost() {
	return 10;
}