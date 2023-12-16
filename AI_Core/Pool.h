#pragma once
#include <iostream>
#include <list>

class IPoolable {
public:
	virtual void OnRetrieved() = 0;
	virtual void OnFreed() = 0;
};

template<typename T>
class Pool 
{
private:
	std::list<T*> m_ObjectQueue;

public:
	Pool<T>(const Pool<T>& other) = delete;
	Pool<T>(int cacheCount);
	~Pool();

	void WarmCache(int cacheCount);
	void TrimCache(int cacheCount);

	T* Retrieve();
	void Free(T* obj);

	int GetCount() const;
	void Clear();
};