#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <thread>

#define PROFILER_ENABLED 1
#if PROFILER_ENABLED
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif

#define CREATE_MEM_METRICS 0
#if CREATE_MEM_METRICS
#define MEM_ALLOC() s_Allocator.PrintCurrentUsage();
#else
#define MEM_ALLOC()
#endif

struct AllocationMetrics {
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	const void PrintCurrentUsage() const { std::cout << "Total Memory Usage:" << (TotalAllocated - TotalFreed) << std::endl; }
};

static AllocationMetrics s_Allocator;

struct ProfileResult {
	std::string m_Name;
	long long m_Start, m_End;
	uint32_t m_ThreadID;
};

struct InstrumentationSession {
	std::string m_Name;
	InstrumentationSession(const std::string& name)
		: m_Name(name)
	{}
	~InstrumentationSession(){}
};

class Instrumentor {
private:
	Instrumentor();

	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;

public:
	static Instrumentor& Get();
	~Instrumentor();

	void BeginSession(const std::string& name, const std::string& filePath = "Profile.json");
	void EndSession();

	void WriteProfile(const ProfileResult& result);
	void WriteHeader();
	void WriteFooter();
};


class InstrumentationTimer {
public:
	InstrumentationTimer(const char* name);
	~InstrumentationTimer();

	const void Stop();

private:
	const char* m_Name;
	bool m_Stopped;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartPoint;
};