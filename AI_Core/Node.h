#pragma once
#include "Pool.h"
#include "Action.h"

class Node : public IComparable, public IPoolable
{
public:
	Node(const Node& other) = delete;
	Node(Node&& other) noexcept = default;
	Node();
	~Node();

	WorldState NodeState;
	int CostSoFar;
	int Heuristic;
	int TotalCost;
	IAction* Action;
	Node* ParentNode;
	int Depth;

	void Set(IAction* action_ptr, int costSoFar, int heuristic, int totalCost, Node* parent, int depth);

	int CompareTo(const IComparable& other) override;

	void OnRetrieved() override {}
	void OnFreed() override;

	bool operator==(const IComparable& other) override;

private:
	void Reset();
};