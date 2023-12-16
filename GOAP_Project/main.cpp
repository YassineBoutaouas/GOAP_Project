#include <iostream>
#include "SpawnConfig.h"
#include "GameManager.h"

void* operator new(size_t size) {
	s_Allocator.TotalAllocated += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size) {
	s_Allocator.TotalFreed += size;
	free(memory);
}

int main() {

	MEM_ALLOC();

	StaticSpawnConfig();
	GameManager::Get().Start();

	while (GameManager::Get().GetWindowIsOpen())
		GameManager::Get().Update();

	std::cin.get();

	MEM_ALLOC();
}