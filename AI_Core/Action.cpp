#include "Action.h"

IAction::IAction(const std::string& name)
	: Name(name),
	Preconditions("Preconditions", WorldState::True, WorldState::DefaultPriority),
	Effects("Effects", WorldState::True, WorldState::DefaultPriority)
{ }

IAction::~IAction() { 
	Preconditions.Clear();
	Effects.Clear();
}

std::ostream& operator<<(std::ostream& stream, IAction& other) {
	stream << "[" << other.Name << "] - Cost: " << other.CalculateCost() << "; Validation: " << (other.Validate() ? "True \n" : "False \n");
	stream << "\t" << other.Preconditions << "\n";
	stream << "\t" << other.Effects << "\n";
	return stream;
}

bool IAction::operator==(const IAction& other) {
	return Name == other.Name;
}