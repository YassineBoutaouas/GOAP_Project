#pragma once
#include <iostream>
#include <vector>

class IComparable {
public:
	IComparable(const IComparable& other) = delete;
	IComparable(IComparable&& source) noexcept = default;
	IComparable(){}
	~IComparable(){}

	virtual int CompareTo(const IComparable& other) = 0;

	IComparable& operator=(IComparable&& other) noexcept = default;
	friend std::ostream& operator<<(std::ostream& stream, IComparable& other);
	virtual bool operator==(const IComparable& other) = 0;
};

class PriorityQueue
{
	
public:
	PriorityQueue(PriorityQueue& other) = delete;
	PriorityQueue();
	~PriorityQueue();

	void Enqueue(IComparable& obj);
	IComparable* Dequeue();
	int Contains(IComparable& obj);

	int GetCount() const;
	bool IsConsistent() const;
	const IComparable* Peek() const;
	bool Any() const;
	void Clear();

	friend std::ostream& operator<< (std::ostream& stream, const PriorityQueue& other);

private:
	std::vector<IComparable*> m_Data;
	void TriangleSwap(int i, int j);
};