#include "AStar.h"

AStar::AStar()
	: m_PooledNodes(10),
	m_MaxIterations(10000)
{ }

AStar& AStar::Get()
{
	static AStar instance;
	return instance;
}

AStar::~AStar() {
	m_PooledNodes.Clear();
}

void AStar::RequestPlan(std::vector<IAction*>& allActions, std::vector<IAction*>& currPlan, std::unordered_map<std::string, WorldState>& allWorldStates, std::unordered_map<std::string, WorldState>& allGoals, std::string& currWorldState, std::string& currGoal) {
	PROFILE_FUNCTION();
	PriorityQueue goals;
	std::vector<IAction*> viableActions;

	WorldState* worldState_ptr = &allWorldStates.begin()->second;

	for (auto& kv : allWorldStates) {
		if (!kv.second.ValidationFunc->operator()()) continue;

		if (kv.second.PriorityFunc->operator()() > worldState_ptr->PriorityFunc->operator()())
			worldState_ptr = &kv.second;
	}

	currWorldState = worldState_ptr->Name;

	for (auto& kv : allGoals) {
		if (kv.second.ValidationFunc->operator()())
			goals.Enqueue(kv.second);
	}

	for (int i = 0; i < allActions.size(); i++) {
		if (allActions[i]->Validate())
			viableActions.emplace_back(allActions[i]);
	}

	TryGetPlan(viableActions, goals, allActions, currPlan, worldState_ptr, currGoal);

	goals.Clear();
	viableActions.clear();
}

void AStar::TryGetPlan(std::vector<IAction*> viableActions, PriorityQueue& viableGoals, std::vector<IAction*>& allActions, std::vector<IAction*>& currPlan, WorldState* currWorldState, std::string& goal_Name) {
	if (!viableGoals.Any()) return;

	WorldState* currGoal = (WorldState*)viableGoals.Dequeue();

	goal_Name = currGoal->Name;

	Plan(viableActions, currWorldState, currGoal, currPlan);

	if (currPlan.size() > 0) return;

	TryGetPlan(viableActions, viableGoals, allActions, currPlan, currWorldState, goal_Name);
}

void AStar::Plan(std::vector<IAction*>& viableActions, WorldState* globalWorldState, WorldState* goal, std::vector<IAction*>& currentPlan) {
	PriorityQueue openNodes;
	std::unordered_set<Node*> closedNodes;

	GetEntryNodes(viableActions, goal, globalWorldState, openNodes);

	if (openNodes.GetCount() == 0) return;

	int counter(0);

	while (openNodes.Any() && ++counter < m_MaxIterations) {
		Node* currentNode((Node*)openNodes.Dequeue());
		closedNodes.emplace(currentNode);

		if (CalculateHeuristic(currentNode) == 0) {
			ReconstructPlan(currentNode, currentPlan);
			break;
		}

		for (int i = 0; i < viableActions.size(); i++) {
			IAction* action_ptr = viableActions[i];

			if (!action_ptr->Effects.IsContained(currentNode->NodeState)) continue;

			Node* newNeighbor(m_PooledNodes.Retrieve());
			newNeighbor->Set(action_ptr, 0, 0, 0, currentNode, currentNode->Depth + 1);

			AccumulatePreviousStates(newNeighbor, currentNode, globalWorldState, action_ptr);
			AccumulatePreconditions(action_ptr, newNeighbor, globalWorldState);

			if (closedNodes.find(newNeighbor) != closedNodes.end()) {
				m_PooledNodes.Free(newNeighbor);
				continue;
			}

			int neighborCostSoFar = currentNode->CostSoFar + action_ptr->CalculateCost();
			int neighborHeuristic = CalculateHeuristic(newNeighbor);
			int neighborTotalCost = neighborCostSoFar + neighborHeuristic;

			int openNodeIndex = openNodes.Contains((IComparable&)*newNeighbor);

			if (openNodeIndex == -1 || currentNode->CostSoFar + neighborHeuristic < neighborCostSoFar) {
				newNeighbor->CostSoFar = neighborCostSoFar;
				newNeighbor->Heuristic = neighborHeuristic;
				newNeighbor->TotalCost = neighborTotalCost;
				newNeighbor->ParentNode = currentNode;
				newNeighbor->Depth = currentNode->Depth + 1;

				if (openNodeIndex == -1) {
					openNodes.Enqueue(*newNeighbor);
					continue;
				}
			}
		}
	}

	Clear(openNodes, closedNodes);
}

void AStar::GetEntryNodes(std::vector<IAction*>& viableActions, WorldState* goal, WorldState* globalWorldState, PriorityQueue& openNodes) {
	for (int i = 0; i < viableActions.size(); i++) {
		IAction* action_ptr = viableActions[i];
		if (!goal->IsContained(action_ptr->Effects)) continue;

		Node* goalNode(m_PooledNodes.Retrieve());
		goalNode->Action = action_ptr;
		goalNode->Depth = 1;

		AccumulatePreconditions(goalNode->Action, goalNode, globalWorldState);

		goalNode->Heuristic = CalculateHeuristic(goalNode);
		openNodes.Enqueue(*goalNode);
	}
}

void AStar::AccumulatePreconditions(IAction* action, Node* target, WorldState* globalWorldState) {
	for (auto& keyValue : action->Preconditions.GetAllStates()) {
		IState* correspondingEffect = action->Effects.GetState(keyValue.first);
		if (correspondingEffect != NULL && keyValue.second.IsSatisfied(*correspondingEffect))
			continue;

		IState* correspondingGlobalWorldState = globalWorldState->GetState(keyValue.first);
		if (correspondingGlobalWorldState != NULL && keyValue.second.IsSatisfied(*correspondingGlobalWorldState))
			continue;

		target->NodeState[keyValue.first] = keyValue.second;
	}
}

void AStar::AccumulatePreviousStates(Node* target, Node* parentNode, WorldState* globalWorldState, IAction* action) {
	for (auto& keyValue : parentNode->NodeState.GetAllStates()) {
		if (action->Effects.ContainsKey(keyValue.first))
			continue;

		target->NodeState[keyValue.first] = keyValue.second;
	}
}

void AStar::ReconstructPlan(Node* goalNode, std::vector<IAction*>& currentPlan) {
	currentPlan.clear();
	Node* currentNode = goalNode;

	for (int i = 0; i < goalNode->Depth; i++) {
		currentPlan.emplace_back(currentNode->Action);
		currentNode = currentNode->ParentNode;
	}
}

int AStar::CalculateHeuristic(Node* node) {
	return node->NodeState.GetCount();
}

void AStar::Clear(PriorityQueue& openNodes, std::unordered_set<Node*>& closedNodes) {
	while (openNodes.Any())
		m_PooledNodes.Free(((Node*)openNodes.Dequeue()));

	for (auto& closed : closedNodes)
		m_PooledNodes.Free(closed);
}