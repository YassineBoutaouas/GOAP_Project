#include "IState.h"

IState::IState(IState&& other) noexcept
	: BoolFunc(other.BoolFunc)
{
	other.BoolFunc = nullptr;
}

IState::IState(std::shared_ptr<BoolFunctor> boolFunctor)
	: BoolFunc(boolFunctor)
{ }

IState::IState()
	: BoolFunc(nullptr)
{ }

IState::~IState(){ 
	BoolFunc.reset();
}

const bool IState::IsSatisfied(IState& other) const {
	return (*BoolFunc)() == other.BoolFunc->operator()();
}

IState& IState::operator=(IState&& other) noexcept {
	if (this == &other)
		return *this;

	BoolFunc = other.BoolFunc;
	other.BoolFunc.reset();

	return *this;
}

IState& IState::operator=(const IState& other) noexcept {
	this->BoolFunc = other.BoolFunc;
	return *this;
}

std::ostream& operator<< (std::ostream& stream, const IState& other) {
	stream << (other.BoolFunc->operator()() ? "True" : "False");
	return stream;
}