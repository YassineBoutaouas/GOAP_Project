#include "Worker.h"

Worker::Worker(const std::string& texturePath)
	: m_AnimationHandler("Resources/worker.png", 6, 7, 200, 700, 8, 8), m_CurrentPos_X(0), m_CurrentPos_Y(0), m_AmountItemsCarried(0), m_Stamina(2), m_AxeSharpness(2), CurrentTarget(Worker::AtCrate),
	m_Temp_Backgroundsprite(), m_Temp_Backgroundtexture(), m_Font(), m_Text(), 
	m_PlanData("Press [Space] to request a plan\nVisual assets are placeholders\n\nThe current plan will be displayed in the console window.\n\nThe project does not implement actual behavior. Actions are simply timebased.")
{
	m_Font.loadFromFile("Resources/font.ttf");
	m_Text.setFont(m_Font);
	m_Text.setString(m_PlanData);
	m_Text.setCharacterSize(40.0f);
	m_Text.setPosition(60.0f, 60.0f);

	m_Temp_Backgroundtexture.loadFromFile("Resources/background.png");
	m_Temp_Backgroundsprite.setTexture(m_Temp_Backgroundtexture);
	m_Temp_Backgroundsprite.setScale(7.0f, 7.0f);
}

Worker::~Worker() { }

void Worker::OnStart() {
	WorldState worldState("WorldState", WorldState::True, WorldState::DefaultPriority);
	worldState.Add("IsMining", WorldState::False);
	worldState.Add("HasStamina", std::make_shared<Threshold>(m_Stamina, 0));
	worldState.Add("ItemsCarried", std::make_shared<Threshold>(m_AmountItemsCarried, 0));
	worldState.Add("IsAxeSharp", std::make_shared<Threshold>(m_AxeSharpness, 0));

	worldState.Add("AtPosition(Bed)", WorldState::False);
	worldState.Add("AtPosition(Crate)", WorldState::False);
	worldState.Add("AtPosition(Bench)", WorldState::False);
	worldState.Add("AtPosition(Mine)", WorldState::False);

	WorldState doWorkGoal("WorkGoal", WorldState::True, WorldState::DefaultPriority);
	doWorkGoal.Add("IsMining", WorldState::True);

	WorldState doSharpenAxeGoal("DoSharpenAxeGoal", WorldState::True, std::make_shared<Priority>(m_AxeSharpness, 0));
	doSharpenAxeGoal.Add("IsAxeSharp", WorldState::True);

	WorldState doSleepGoal("SleepGoal", WorldState::True, std::make_shared<Priority>(m_Stamina, 0));
	doSleepGoal.Add("HasStamina", WorldState::True);

	m_ActionPlanner->AddWorldState(std::move(worldState));
	m_ActionPlanner->AddGoal(std::move(doWorkGoal));
	m_ActionPlanner->AddGoal(std::move(doSharpenAxeGoal));
	m_ActionPlanner->AddGoal(std::move(doSleepGoal));

	GoTo* gotoBench = new GoTo(m_CurrentPos_X, m_CurrentPos_Y, "Bench", 10, 10, (int&)CurrentTarget, AtBench, 1);
	GoTo* gotoBed = new GoTo(m_CurrentPos_X, m_CurrentPos_Y, "Bed", 5, 5, (int&)CurrentTarget, AtBed, 1);
	GoTo* gotoCrate = new GoTo(m_CurrentPos_X, m_CurrentPos_Y, "Crate", 15, 15, (int&)CurrentTarget, AtCrate, 1);
	GoTo* gotoMine = new GoTo(m_CurrentPos_X, m_CurrentPos_Y, "Mine", 20, 20, (int&)CurrentTarget, AtMine, 1);

	m_ActionPlanner->AddAction(gotoMine);
	m_ActionPlanner->AddAction(gotoBench);
	m_ActionPlanner->AddAction(gotoCrate);
	m_ActionPlanner->AddAction(gotoBed);

	m_ActionPlanner->AddAction(new DoMining(m_AxeSharpness, m_Stamina, m_AmountItemsCarried, 1, (int&)CurrentTarget));
	m_ActionPlanner->AddAction(new DoSharpenAxe(m_AxeSharpness, m_Stamina, 1));
	m_ActionPlanner->AddAction(new DoEmptyBasket(m_AmountItemsCarried, 1));
	m_ActionPlanner->AddAction(new DoSleep(m_Stamina, 2));
}

void Worker::OnUpdate() {
	UpdateCurrentState();
}

void Worker::OnRender(sf::RenderTarget& renderTarget) {
	renderTarget.draw(m_Temp_Backgroundsprite);
	m_AnimationHandler.Draw(renderTarget);
	renderTarget.draw(m_Text);
}

void Worker::OnInput(const sf::Event& event) {
	if (event.key.code == sf::Keyboard::Space)
		m_ActionPlanner->RequestPlan(false);

}