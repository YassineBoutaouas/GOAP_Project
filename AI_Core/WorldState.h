#pragma once
#include <iostream>
#include <unordered_map>
#include "PriorityQueue.h"
#include "IState.h"

class WorldState : public IComparable {
public:
	WorldState(const WorldState& other) = delete;
	WorldState(WorldState&& source) noexcept = default;
	WorldState();
	WorldState(const std::string& name, const std::shared_ptr<BoolFunctor>& validationFunc, const std::shared_ptr<FloatFunctor>& priorityFunc);
	~WorldState();

	const std::string Name;

	std::shared_ptr<BoolFunctor> ValidationFunc;
	std::shared_ptr<FloatFunctor> PriorityFunc;

	static std::shared_ptr<BoolFunctor> True;
	static std::shared_ptr<BoolFunctor> False;
	static std::shared_ptr<FloatFunctor> DefaultPriority;

	void Add(const std::string& key, const std::shared_ptr<BoolFunctor>& state);
	void Remove(const std::string& key);

	int CompareTo(const IComparable& other);
	bool IsContained(const WorldState& other);

	bool ContainsKey(const std::string& key) const;
	IState* GetState(const std::string& key);
	const std::unordered_map<std::string, IState>& GetAllStates() const;

	int GetCount() const;
	void Clear();

	WorldState& operator=(WorldState&& other) noexcept = default;
	friend std::ostream& operator<< (std::ostream& stream, const WorldState& other);
	IState& operator[](const std::string& key);

	bool operator==(const IComparable& other) override;

private:
	std::unordered_map<std::string, IState> m_States;
};