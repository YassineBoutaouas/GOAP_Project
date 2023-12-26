#pragma once
#include "ActionPlanner.h"

class Agent {
protected:
	ActionPlanner* m_ActionPlanner;
	virtual void UpdateCurrentState();

public:
	Agent();
	Agent(const Agent&) = delete;
	~Agent();

	friend std::ostream& operator<< (std::ostream& stream, Agent& other);
};