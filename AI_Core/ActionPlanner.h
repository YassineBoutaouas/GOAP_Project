#pragma once
#include <thread>
#include <future>
#include <iostream>
#include <unordered_map>
#include "AStar.h"

#define DEBUG_ENABLED 1
#if DEBUG_ENABLED
#define DEBUG_PLANNER(actionPlanner) std::cout << actionPlanner << std::endl;
#else
#define DEBUG_PLANNER(actionPlanner)
#endif

class ActionPlanner {
public:
	ActionPlanner(const ActionPlanner&) = delete;
	ActionPlanner();
	~ActionPlanner();

	std::vector<IAction*> CurrentPlan;
	IAction* CurrentAction;

	void AddAction(IAction* action);
	void RemoveAction(const std::string& name);

	void AddWorldState(WorldState&& worldState);
	void AddGoal(WorldState&& goal);
	void RemoveWorldState(const std::string& name);
	void RemoveGoal(const std::string& name);
	WorldState* GetWorldState(std::string& key);
	WorldState* GetGoal(std::string& key);

	const bool WaitForPlan() const;
	void RequestPlan(const bool& cancellable);
	bool ValidatePlan(const bool& cancellable);

	const bool HasActionPlan() const;
	const WorldState* GetCurrentWorldState() const;
	const WorldState* GetCurrentGoal() const;

	friend std::ostream& operator<< (std::ostream& stream, ActionPlanner& other);

private:
	std::vector<IAction*> m_AllActions;
	std::unordered_map<std::string, WorldState> m_AllWorldStates;
	std::unordered_map<std::string, WorldState> m_AllGoals;

	std::string m_CurrentWorldState;
	std::string m_CurrentGoal;

	std::future<void> m_Future_Plan;

	bool m_IsCancellable;
};