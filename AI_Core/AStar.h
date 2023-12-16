#pragma once

#include <thread>
#include <future>
#include <unordered_set>
#include "Pool.cpp"
#include "Node.h"

#include "Profiler.h"

class AStar {

public:
	AStar(const AStar&) = delete;
	static AStar& Get();
	~AStar();

	void RequestPlan(std::vector<IAction*>& allActions, std::vector<IAction*>& currPlan, std::unordered_map<std::string, WorldState>& allWorldStates, std::unordered_map<std::string, WorldState>& allGoals, std::string& currWorldState, std::string& currGoal);

private:
	AStar();

	const int m_MaxIterations;
	Pool<Node> m_PooledNodes;

	void GetEntryNodes(std::vector<IAction*>& viableActions, WorldState* goal, WorldState* globalWorldState, PriorityQueue& openNodes);

	void AccumulatePreviousStates(Node* target, Node* parentNode, WorldState* globalWorldState, IAction* action);
	void AccumulatePreconditions(IAction* action, Node* target, WorldState* globalWorldState);
	int CalculateHeuristic(Node* node);

	void TryGetPlan(std::vector<IAction*> viableActions, PriorityQueue& viableGoals, std::vector<IAction*>& allActions, std::vector<IAction*>& currPlan, WorldState* currWorldState, std::string& goal_Name);
	void Plan(std::vector<IAction*>& viableActions, WorldState* globalWorldState, WorldState* goal, std::vector<IAction*>& currentPlan);
	void ReconstructPlan(Node* goalNode, std::vector<IAction*>& currentPlan);

	void Clear(PriorityQueue& openNodes, std::unordered_set<Node*>& closedNodes);
};