#include "WorldState.h"

#pragma region Constructors & Destructors
WorldState::WorldState()
	: Name(),
	ValidationFunc(WorldState::True),
	PriorityFunc(WorldState::DefaultPriority),
	m_States()
{ }

WorldState::WorldState(const std::string& name, const std::shared_ptr<BoolFunctor>& validationFunc, const std::shared_ptr<FloatFunctor>& priorityFunc)
	: Name(name),
	ValidationFunc(validationFunc),
	PriorityFunc(priorityFunc),
	m_States()
{ }

WorldState::~WorldState() {
	m_States.clear();
}
#pragma endregion

std::shared_ptr<BoolFunctor> WorldState::True = std::make_shared<BoolFunctor>(true);
std::shared_ptr<BoolFunctor> WorldState::False = std::make_shared<BoolFunctor>(false);
std::shared_ptr<FloatFunctor> WorldState::DefaultPriority = std::make_shared<FloatFunctor>(1.0f);

void WorldState::Add(const std::string& key, const std::shared_ptr<BoolFunctor>& state) {
	auto it(m_States.find(key));
	if (it == m_States.end())
		m_States.emplace(key, std::move(IState(state)));
}

void WorldState::Remove(const std::string& key) {
	auto it(m_States.find(key));
	if (it != m_States.end())
		m_States.erase(key);
}

int WorldState::CompareTo(const IComparable& other) {
	WorldState& otherWorldState = (WorldState&)other;

	float prio = (*PriorityFunc)();
	float otherPrio = otherWorldState.PriorityFunc->operator()();

	if (prio < otherPrio)
		return 1;
	else if (prio == otherPrio)
		return 0;

	return -1;
}

bool WorldState::IsContained(const WorldState& other) {
	if (m_States.size() == 0 || other.m_States.size() == 0) return false;

	for (auto& keyValue : m_States)
	{
		auto it(other.m_States.find(keyValue.first));
		if (it == other.m_States.end())
			return false;

		if (!it->second.IsSatisfied(m_States[keyValue.first]))
			return false;
	}

	return true;
}

bool WorldState::ContainsKey(const std::string& key) const {
	return m_States.find(key) != m_States.end();
}

IState* WorldState::GetState(const std::string& key) {
	auto state = m_States.find(key);
	if (state != m_States.end())
		return &state->second;

	return nullptr;
}

const std::unordered_map<std::string, IState>& WorldState::GetAllStates() const {
	return m_States;
}

int WorldState::GetCount() const {
	return m_States.size();
}

void WorldState::Clear() {
	m_States.clear();
}

bool WorldState::operator==(const IComparable& other) {
	WorldState& otherWorldstate = (WorldState&)other;

	if (m_States.size() != otherWorldstate.m_States.size()) return false;

	return IsContained(otherWorldstate);
}

IState& WorldState::operator[](const std::string& key) {
	return m_States[key];
}

std::ostream& operator<< (std::ostream& stream, const WorldState& other) {
	stream << other.Name << " - ";

	if (other.GetCount() == 0)
		return stream << "None";

	stream << "States: [";

	for (auto& keyValue : other.m_States) {
		stream << "(" << keyValue.first << " : " << keyValue.second << ") ";
	}
	stream << "] ";

	if (other.PriorityFunc != nullptr)
		stream << " - Priority: " << other.PriorityFunc->operator()();

	if (other.ValidationFunc != nullptr)
		stream << " - Validation: " << (other.ValidationFunc->operator()() ? "True" : "False");

	return stream;
}