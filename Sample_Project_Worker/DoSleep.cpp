#include "DoSleep.h"

DoSleep::DoSleep(int& stamina, const float& targetStamina)
	: IAction("DoSleep"), m_Stamina(stamina), m_TargetStamina(targetStamina), m_CurrentTime(0)
{
	Preconditions.Add("AtPosition(Bed)", WorldState::True);
	Effects.Add("HasStamina", WorldState::True);
}

DoSleep::~DoSleep()
{ }

void DoSleep::OnActionEnter()
{
	std::cout << "Enter Sleeping\n";
	m_CurrentTime = 0;
}

bool DoSleep::OnActionExecute() {
	if (m_Stamina >= m_TargetStamina)
		return false;

	std::cout << "Sleeping\n";
	m_CurrentTime++;
	m_Stamina++;
	return true;
}

bool DoSleep::Validate()
{
	return true;
}

int DoSleep::CalculateCost()
{
	return 5;
}
