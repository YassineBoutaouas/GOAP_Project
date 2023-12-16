#include "DoMining.h"

DoMining::DoMining(int& axeSharpness, int& stamina, int& itemsCarried, const float& duration, int& target)
    : IAction("DoMining"), m_AxeSharpness(axeSharpness), m_Stamina(stamina), m_AmountItemsCarried(itemsCarried), m_Duration(duration), m_ElapsedTime(0), m_CurrentTarget(target)
{
    Preconditions.Add("AtPosition(Mine)", WorldState::True);
    Preconditions.Add("ItemsCarried", WorldState::False);

    Effects.Add("IsMining", WorldState::True);
    Effects.Add("ItemsCarried", WorldState::True);
}

DoMining::~DoMining()
{
}

void DoMining::OnActionEnter()
{
    std::cout << "Enter Do Mining\n";
    m_ElapsedTime = 0;
}

bool DoMining::OnActionExecute()
{
    if (m_ElapsedTime >= m_Duration) {
        m_CurrentTarget = 0;
        return false;
    }

    std::cout << "Mining\n";
    m_AxeSharpness--;
    m_Stamina--;
    m_AmountItemsCarried++;
    m_ElapsedTime++;
    return true;
}

bool DoMining::Validate()
{
    return true;
}

int DoMining::CalculateCost()
{
    return 17;
}
