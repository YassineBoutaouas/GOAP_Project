#pragma once
#include "Action.h"

class DoEmptyBasket : public IAction {
public:
	DoEmptyBasket(int& itemsCarried, const float& duration);
	~DoEmptyBasket();

	void OnActionEnter() override;
	bool OnActionExecute() override;
	bool Validate() override;
	int CalculateCost() override;

private:
	int& m_ItemsCarried;
	float m_Duration;
	float m_ElapsedTime;
};