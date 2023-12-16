#include "DoEmptyBasket.h"

DoEmptyBasket::DoEmptyBasket(int& itemsCarried, const float& duration)
	: IAction("DoEmptyBasket"), m_ItemsCarried(itemsCarried), m_Duration(duration), m_ElapsedTime(0)
{
	Preconditions.Add("AtPosition(Crate)", WorldState::True);
	Effects.Add("ItemsCarried", WorldState::False);
}

DoEmptyBasket::~DoEmptyBasket() {

}

void DoEmptyBasket::OnActionEnter() {
	std::cout << "Enter Do Empty Basket\n";
	m_ElapsedTime = 0;
}

bool DoEmptyBasket::OnActionExecute() {
	if (m_ElapsedTime >= m_Duration) {
		m_ItemsCarried = 0;
		return false;
	}

	std::cout << "Execute Do Empty Basket\n";
	m_ElapsedTime++;
	return true;
}

bool DoEmptyBasket::Validate()
{
	return true;
}

int DoEmptyBasket::CalculateCost()
{
	return 12;
}
