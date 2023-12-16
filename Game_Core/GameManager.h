#pragma once
#include <iostream>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Profiler.h"

class IGameObserver {
public:
	IGameObserver();
	virtual ~IGameObserver();

	virtual void OnStart() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender(sf::RenderTarget& renderTarget) = 0;
	virtual void OnInput(const sf::Event& key) = 0;
};

class GameManager {
public:
	GameManager(const GameManager&) = delete;
	static GameManager& Get();
	~GameManager();

	sf::View GameView;

	void RegisterListener(IGameObserver* observer);
	void UnregisterListener(IGameObserver* observer);
	void ClearListeners();

	const bool GetWindowIsOpen() const;

	void Start();
	void Update();

private:
	GameManager();

	std::list<IGameObserver*> m_GameObservers;
	sf::RenderWindow m_GameWindow;
	sf::Event m_InputEvent;

	void NotifyOnStart();
	void NotifyOnUpdate();
	void NotifyOnRender(sf::RenderTarget& renderTarget);
	void NotifyOnInput(const sf::Event& event);

	void UpdatePollEvents();
};