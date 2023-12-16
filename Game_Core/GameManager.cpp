#include "GameManager.h"

IGameObserver::IGameObserver() {
	GameManager::Get().RegisterListener(this);
}

IGameObserver::~IGameObserver() {
	GameManager::Get().UnregisterListener(this);
}

GameManager::GameManager() 
	: m_GameWindow(sf::RenderWindow(sf::VideoMode(2240, 1260), "GOAP Prototype", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize)), m_InputEvent(), GameView(sf::Vector2f(1120, 630), sf::Vector2f(2240,1260))
{
	m_GameWindow.setFramerateLimit(60);
	m_GameWindow.setView(GameView);
}

GameManager& GameManager::Get() {
	static GameManager instance;
	return instance;
}

GameManager::~GameManager() {
	ClearListeners();
	MEM_ALLOC();
}

void GameManager::RegisterListener(IGameObserver* observer) {
	m_GameObservers.emplace_back(observer);
}

void GameManager::UnregisterListener(IGameObserver* observer) {
	m_GameObservers.remove(observer);
}

void GameManager::ClearListeners() {
	while (!m_GameObservers.empty()) {
		delete m_GameObservers.front();
	}
}

const bool GameManager::GetWindowIsOpen() const {
	return m_GameWindow.isOpen();
}

void GameManager::Start() {
	NotifyOnStart();
}

void GameManager::UpdatePollEvents() {
	while (m_GameWindow.pollEvent(m_InputEvent)) {
		switch (m_InputEvent.type)
		{
		case sf::Event::Closed:
			m_GameWindow.close();
			break;

		case sf::Event::KeyPressed:
			NotifyOnInput(m_InputEvent);
			break;
		case sf::Event::KeyReleased:
			break;
		case sf::Event::MouseWheelMoved:
			NotifyOnInput(m_InputEvent);
			break;
		default:
			break;
		}
	}
}

void GameManager::Update() {
	UpdatePollEvents();
	NotifyOnUpdate();

	m_GameWindow.clear(sf::Color(80, 80, 80));
	NotifyOnRender(m_GameWindow);
	m_GameWindow.setView(GameView);
	m_GameWindow.display();
}

void GameManager::NotifyOnStart() {
	for (IGameObserver* observer : m_GameObservers)
		observer->OnStart();
}

void GameManager::NotifyOnUpdate() {
	for (IGameObserver* observer : m_GameObservers)
		observer->OnUpdate();
}

void GameManager::NotifyOnRender(sf::RenderTarget& renderTarget) {
	for (IGameObserver* observer : m_GameObservers)
		observer->OnRender(renderTarget);
}

void GameManager::NotifyOnInput(const sf::Event& event) {
	for (IGameObserver* observer : m_GameObservers)
		observer->OnInput(event);
}
