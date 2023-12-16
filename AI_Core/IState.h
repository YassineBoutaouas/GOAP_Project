#pragma once
#include "Functors.h"
#include <iostream>

class IState {
public:
	IState(const IState&) = delete;
	IState(IState&& other) noexcept;
	IState(std::shared_ptr<BoolFunctor> boolFunctor);
	IState();
	~IState();

	std::shared_ptr<BoolFunctor> BoolFunc;

	const bool IsSatisfied(IState& other) const;

	IState& operator=(IState&& other) noexcept;
	IState& operator=(const IState& other) noexcept;
	friend std::ostream& operator<< (std::ostream& stream, const IState& other);
};