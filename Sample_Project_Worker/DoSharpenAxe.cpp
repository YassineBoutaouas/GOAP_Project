#include "DoSharpenAxe.h"

DoSharpenAxe::DoSharpenAxe(int& axeSharpness, int& stamina, const float& duration)
    : IAction("DoSharpenAxe"), m_AxeSharpness(axeSharpness), m_Stamina(stamina), m_Duration(duration), m_CurrentTime(0)
{
	Preconditions.Add("AtPosition(Bench)", WorldState::True);
	Effects.Add("IsAxeSharp", WorldState::True);
}

DoSharpenAxe::~DoSharpenAxe()
{
}

void DoSharpenAxe::OnActionEnter()
{
    std::cout << "Enter Do Sharpen Axe\n";
    m_CurrentTime = 0;
}

bool DoSharpenAxe::OnActionExecute()
{
	if (m_CurrentTime >= m_Duration)
		return false;

	std::cout << "Sharpening Axe\n";
	m_AxeSharpness++;
	m_Stamina--;
	m_CurrentTime++;
	return true;
}

bool DoSharpenAxe::Validate()
{
    return true;
}

int DoSharpenAxe::CalculateCost()
{
    return 7;
}
