#pragma once
#include "ActionPlanner.h"

class Agent {
protected:
	ActionPlanner* m_ActionPlanner;

public:
	Agent();
	Agent(const Agent&) = delete;
	~Agent();

	virtual void UpdateCurrentState();

	friend std::ostream& operator<< (std::ostream& stream, Agent& other);
};