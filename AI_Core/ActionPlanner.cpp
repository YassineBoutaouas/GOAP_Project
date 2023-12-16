#include "ActionPlanner.h"

ActionPlanner::ActionPlanner()
	: m_AllActions(), CurrentPlan(), CurrentAction(nullptr), m_AllWorldStates(), m_AllGoals(), m_CurrentWorldState(), m_CurrentGoal(), m_Future_Plan(), m_IsCancellable(false)
{ }

ActionPlanner::~ActionPlanner() {
	WaitForPlan();
	while (!m_AllActions.empty()) {
		delete m_AllActions.back();
		m_AllActions.pop_back();
	}

	CurrentPlan.clear();

	m_AllWorldStates.clear();
	m_AllGoals.clear();
}

#pragma region Helper Functions

void ActionPlanner::AddAction(IAction* action) {
	auto it = std::find(m_AllActions.begin(), m_AllActions.end(), action);
	if (it == m_AllActions.end())
		m_AllActions.emplace_back(action);
}

void ActionPlanner::RemoveAction(const std::string& name) {
	auto it = m_AllActions.begin();
	while (it != m_AllActions.end()) {
		if ((*it)->Name == name) {
			delete* it;
			m_AllActions.erase(it);
			return;
		}
		it++;
	}
}

void ActionPlanner::AddWorldState(WorldState&& worldState) {
	auto it(m_AllWorldStates.find(worldState.Name));
	if (it == m_AllWorldStates.end())
		m_AllWorldStates.emplace(worldState.Name, std::move(worldState));
}

void ActionPlanner::AddGoal(WorldState&& goal) {
	auto it(m_AllGoals.find(goal.Name));
	if (it == m_AllGoals.end())
		m_AllGoals.emplace(goal.Name, std::move(goal));
}

void ActionPlanner::RemoveWorldState(const std::string& name) {
	auto it(m_AllWorldStates.find(name));
	if (it != m_AllWorldStates.end())
		m_AllWorldStates.erase(name);
}

void ActionPlanner::RemoveGoal(const std::string& name) {
	auto it(m_AllGoals.find(name));
	if (it != m_AllGoals.end())
		m_AllGoals.erase(name);
}

WorldState* ActionPlanner::GetWorldState(std::string& key) {
	return &m_AllWorldStates.find(key)->second;
}

WorldState* ActionPlanner::GetGoal(std::string& key) {
	return &m_AllGoals.find(key)->second;
}

const WorldState* ActionPlanner::GetCurrentWorldState() const {
	return &m_AllWorldStates.at(m_CurrentWorldState);
}

const WorldState* ActionPlanner::GetCurrentGoal() const {
	return &m_AllGoals.at(m_CurrentGoal);
}

const bool ActionPlanner::HasActionPlan() const {
	if (!WaitForPlan())
		return false;

	return CurrentPlan.size() > 0;
}

#pragma endregion

static std::mutex s_CurrentPlanMutex;

#pragma region Private Planning Methods

const bool ActionPlanner::WaitForPlan() const {
	if (!m_Future_Plan.valid())
		return false;

	m_Future_Plan.wait();
	return true;
}

void ActionPlanner::RequestPlan(const bool& cancellable = false) {
	m_IsCancellable = cancellable;
	m_Future_Plan = std::async(std::launch::async, [this]() {
		std::lock_guard<std::mutex> lock(s_CurrentPlanMutex);
		AStar::Get().RequestPlan(m_AllActions, CurrentPlan, m_AllWorldStates, m_AllGoals, m_CurrentWorldState, m_CurrentGoal);
		});
		DEBUG_PLANNER(*this);
}

bool ActionPlanner::ValidatePlan(const bool& cancellable = false) {
	if (m_IsCancellable == false) return true;

	std::lock_guard<std::mutex> lock(s_CurrentPlanMutex);

	m_IsCancellable = cancellable;

	bool valid = true;
	m_Future_Plan = std::async(std::launch::async, [this, &valid]() {
		bool moreimportantWS = false;
		bool moreimportantGoal = false;

		for (auto& kv : m_AllWorldStates) {
			if (kv.second.PriorityFunc->operator()() > GetCurrentWorldState()->PriorityFunc->operator()()) {
				moreimportantWS = true;
				break;
			}
		}

		for (auto& kv : m_AllGoals) {
			if (kv.second.PriorityFunc->operator()() > GetCurrentWorldState()->PriorityFunc->operator()()) {
				moreimportantGoal = true;
				break;
			}
		}

		if (!(moreimportantGoal || moreimportantWS)) {
			valid = true;
			return;
		}

		std::vector<IAction*> alternativePlan;

		AStar::Get().RequestPlan(m_AllActions, alternativePlan, m_AllWorldStates, m_AllGoals, m_CurrentWorldState, m_CurrentGoal);
		if (alternativePlan.size() == 0) return;

		CurrentPlan.clear();
		for (int i = 0; i < alternativePlan.size(); i++) {
			CurrentPlan[i] = alternativePlan[i];
		}

		valid = false;
		}
	);

	m_Future_Plan.get();
	DEBUG_PLANNER(*this);
	return valid;
}

#pragma endregion

std::ostream& operator<< (std::ostream& stream, ActionPlanner& other) {
	if (!other.WaitForPlan())
		return stream;

	stream << "\n\n---WorldState & Goal---\n";
	if (other.GetCurrentWorldState() != nullptr)
		stream << other.m_AllWorldStates.at(other.m_CurrentWorldState) << '\n';
	if (other.GetCurrentGoal() != nullptr)
		stream << '\n' << other.m_AllGoals.at(other.m_CurrentGoal) << '\n';
	stream << "\n" << "----------------------\n\n";

	stream << "---Current Plan--- \n\n";
	int i = 0;
	for (auto& action : other.CurrentPlan) {
		stream << i << ": \t" << *action << '\n';
		i++;
	}

	stream << "---End Plan---\n\n\n";

	return stream;
}