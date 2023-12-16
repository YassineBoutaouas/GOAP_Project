#include "Node.h"

Node::Node()
	: NodeState(),
	CostSoFar(0),
	Heuristic(0),
	TotalCost(0),
	Action(nullptr),
	ParentNode(nullptr),
	Depth(0)
{ }

Node::~Node() {
	Reset();
}

void Node::Set(IAction* action_ptr, int costSoFar, int heuristic, int totalCost, Node* parent, int depth) {
	NodeState.Clear();
	CostSoFar = costSoFar;
	Heuristic = heuristic;
	TotalCost = totalCost;
	Action = action_ptr;
	ParentNode = parent;
	Depth = depth;
}

void Node::Reset() {
	NodeState.Clear();
	CostSoFar = 0;
	Heuristic = 0;
	TotalCost = 0;
	Action = nullptr;
	ParentNode = nullptr;
	Depth = 0;
}

void Node::OnFreed() {
	Reset();
}

int Node::CompareTo(const IComparable& other) {
	Node& o = (Node&)other;
	if (TotalCost > o.TotalCost)
		return 1;
	else if (TotalCost < o.TotalCost)
		return -1;
	return 0;
}

bool Node::operator==(const IComparable& other) {
	Node& otherNode = (Node&)other;
	return NodeState == otherNode.NodeState;
}