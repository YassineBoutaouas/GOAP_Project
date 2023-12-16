#include "Pool.h"

template<typename T>
Pool<T>::Pool(int cacheCount)
	: m_ObjectQueue()
{
	for (int i = 0; i < cacheCount; i++)
		m_ObjectQueue.push_back(new T());
}

template<typename T>
Pool<T>::~Pool() {
	Clear();
}

template<typename T>
void Pool<T>::WarmCache(int cacheCount) {
	cacheCount -= m_ObjectQueue.size();

	if (cacheCount <= 0) return;

	for (int i = 0; i < cacheCount; i++)
		m_ObjectQueue.push_back(new T());
}

template<typename T>
void Pool<T>::TrimCache(int cacheCount) {
	while (cacheCount > m_ObjectQueue.size())
	{
		delete m_ObjectQueue.front();
		m_ObjectQueue.pop_front();
	}
}

template<typename T>
T* Pool<T>::Retrieve() {
	if (m_ObjectQueue.empty())
		return new T();

	T* obj = m_ObjectQueue.front();
	m_ObjectQueue.pop_front();

	IPoolable* p = dynamic_cast<IPoolable*>(obj);
	if (p != nullptr)
		p->OnRetrieved();
	return obj;
}

template<typename T>
void Pool<T>::Free(T* obj) {
	IPoolable* p = dynamic_cast<IPoolable*>(obj);
	if (p != nullptr)
		p->OnFreed();
	m_ObjectQueue.push_back(obj);
}

template<typename T>
void Pool<T>::Clear() { 
	while (!m_ObjectQueue.empty()) {
		delete m_ObjectQueue.front();
		m_ObjectQueue.pop_front();
	}
}

template<typename T>
int Pool<T>::GetCount() const { 
	return m_ObjectQueue.size(); 
}