#include "PriorityQueue.h"

#pragma region IComparable
std::ostream& operator<<(std::ostream& stream, IComparable& other) {
	stream << other;
	return stream;
}
#pragma endregion

#pragma region PriorityQueue

PriorityQueue::PriorityQueue()
	: m_Data() { }

PriorityQueue::~PriorityQueue() {
	Clear();
}

void PriorityQueue::TriangleSwap(int i, int j) {
	IComparable* temp = m_Data[i];
	m_Data[i] = m_Data[j];
	m_Data[j] = temp;
}

void PriorityQueue::Enqueue(IComparable& obj) {
	m_Data.emplace_back(&obj);
	int childIndex = m_Data.size() - 1;
	int parentIndex;

	while (childIndex > 0) {
		parentIndex = (childIndex - 1) / 2;

		if (m_Data[childIndex]->CompareTo(*m_Data[parentIndex]) >= 0) break;

		TriangleSwap(childIndex, parentIndex);

		childIndex = parentIndex;
	}
}

IComparable* PriorityQueue::Dequeue() {
	int lastIndex = m_Data.size() - 1;

	if (lastIndex < 0) return nullptr;

	IComparable* frontObject = m_Data[0];
	m_Data[0] = m_Data[lastIndex];

	m_Data.erase(m_Data.end() - 1);

	lastIndex--;

	int parentIndex = 0;

	while (true) {
		int leftChildIndex = parentIndex * 2 + 1;
		int rightChildIndex = leftChildIndex + 1;

		if (leftChildIndex > lastIndex) break;

		if (rightChildIndex <= lastIndex && m_Data[rightChildIndex]->CompareTo(*m_Data[leftChildIndex]) < 0)
			leftChildIndex = rightChildIndex;

		if (m_Data[parentIndex]->CompareTo(*m_Data[leftChildIndex]) <= 0) break;

		TriangleSwap(parentIndex, leftChildIndex);

		parentIndex = leftChildIndex;
	}

	return frontObject;
}

bool PriorityQueue::IsConsistent() const {
	if (m_Data.size() == 0) return true;

	int lastIndex = m_Data.size() - 1;

	for (int parentIndex = 0; parentIndex <= lastIndex; parentIndex++)
	{
		int leftChildIndex = parentIndex * 2 + 1;
		int rightChildIndex = leftChildIndex + 1;

		if (leftChildIndex <= lastIndex && m_Data[parentIndex]->CompareTo(*m_Data[leftChildIndex]) > 0) return false;
		if (rightChildIndex <= lastIndex && m_Data[parentIndex]->CompareTo(*m_Data[leftChildIndex]) > 0) return false;
	}

	return true;
}

int PriorityQueue::Contains(IComparable& obj) {
	for (int i = 0; i < m_Data.size(); i++)
	{
		if (*m_Data[i] == obj)
			return i;
	}

	return -1;
}

int PriorityQueue::GetCount() const {
	return m_Data.size();
}

const IComparable* PriorityQueue::Peek() const {
	return m_Data[0];
}

bool PriorityQueue::Any() const {
	return m_Data.size() > 0;
}

void PriorityQueue::Clear() {
	m_Data.clear();
}

std::ostream& operator<<(std::ostream& stream, const PriorityQueue& other) {
	stream << "PriorityQueue: [";

	for (int i = 0; i < other.m_Data.size(); i++)
		stream << other.m_Data[i] << (i == other.m_Data.size() - 1 ? "]" : ", ");

	return stream;
}
#pragma endregion