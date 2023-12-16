#pragma once
#include <string>
#include <iostream>
#include <sstream>

#include "Agent.h"
#include "GameManager.h"
#include "AnimationHandler.h"
#include "GoTo.h"
#include "DoEmptyBasket.h"
#include "DoSleep.h"
#include "DoSharpenAxe.h"
#include "DoMining.h"

class Worker : public Agent, public IGameObserver {
public:
	Worker(const std::string& texPath);
	Worker(const Worker& other) = delete;
	~Worker();

	void OnStart() override;
	void OnUpdate() override;
	void OnRender(sf::RenderTarget& renderTarget) override;
	void OnInput(const sf::Event& key) override;

	enum TargetType {
		None,
		AtMine,
		AtBench,
		AtBed,
		AtCrate
	};

	TargetType CurrentTarget;

private:
	sf::Font m_Font;
	sf::Text m_Text;
	std::string m_PlanData;

	sf::Texture m_Temp_Backgroundtexture;
	sf::Sprite m_Temp_Backgroundsprite;

	AnimationHandler m_AnimationHandler;
	float m_CurrentPos_X;
	float m_CurrentPos_Y;
	int m_AmountItemsCarried;
	int m_Stamina;
	int m_AxeSharpness;
};

class Priority : public FloatFunctor {
public:
	Priority(const Priority&) = delete;
	Priority(Priority&&) noexcept = default;

	Priority(int& amount, const int& threshhold)
		: FloatFunctor(1), m_CurrentAmount(amount), m_Thresshold(threshhold)
	{ }

	~Priority() { }

	const float operator()() override {
		return  m_CurrentAmount < m_Thresshold ? 2 : 0;
	}

private:
	int& m_CurrentAmount;
	const int m_Thresshold;
};

class AtPosition : public BoolFunctor {
public:
	AtPosition(const AtPosition&) = delete;
	AtPosition(AtPosition&&) noexcept = default;

	AtPosition(Worker::TargetType& currentPos, const Worker::TargetType& desiredPos)
		: BoolFunctor(true), m_CurrentPos(currentPos), m_DesiredPos(desiredPos)
	{ }

	~AtPosition() { }

	const bool operator()() override {
		return m_CurrentPos == m_DesiredPos;
	}

private:
	Worker::TargetType& m_CurrentPos;
	Worker::TargetType m_DesiredPos;
};

class Threshold : public BoolFunctor {
public:
	Threshold(const Threshold&) = delete;
	Threshold(Threshold&&) noexcept = default;

	Threshold(int& amount, const int& threshhold)
		: BoolFunctor(true), m_CurrentAmount(amount), m_Threshold(threshhold)
	{ }

	~Threshold() { }

	const bool operator()() override {
		return  m_CurrentAmount > m_Threshold;
	}

private:
	int& m_CurrentAmount;
	const int m_Threshold;
};