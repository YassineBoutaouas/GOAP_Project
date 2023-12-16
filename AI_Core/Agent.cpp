#include "Agent.h"

Agent::Agent()
	: m_ActionPlanner(new ActionPlanner())
{ }

Agent::~Agent() {
	delete m_ActionPlanner;
}

void Agent::UpdateCurrentState() {
	if (m_ActionPlanner->CurrentAction != NULL)
		if(m_ActionPlanner->CurrentAction->OnActionExecute())
			return;

	if (!m_ActionPlanner->HasActionPlan()) {
		m_ActionPlanner->CurrentAction = NULL;
		return;
	}

	m_ActionPlanner->CurrentAction = m_ActionPlanner->CurrentPlan.front();
	m_ActionPlanner->CurrentPlan.erase(m_ActionPlanner->CurrentPlan.begin());
	m_ActionPlanner->CurrentAction->OnActionEnter();
}

std::ostream& operator<< (std::ostream& stream, Agent& other) {
	stream << *other.m_ActionPlanner;
	return stream;
}