#include "Profiler.h"

#pragma region Instrumentor

Instrumentor::Instrumentor()
	: m_CurrentSession(nullptr), m_ProfileCount(0)
{}

Instrumentor::~Instrumentor() {
	std::cout << "Destroyed Instrumentor" << std::endl;
	EndSession();
}

void Instrumentor::BeginSession(const std::string& name, const std::string& filePath) {
	m_OutputStream.open(filePath);
	WriteHeader();
	m_CurrentSession = new InstrumentationSession(name);
}

void Instrumentor::EndSession() {
	WriteFooter();
	m_OutputStream.close();
	delete m_CurrentSession;
	m_CurrentSession = nullptr;
	m_ProfileCount = 0;
}

void Instrumentor::WriteProfile(const ProfileResult& result) {
	if (m_ProfileCount++ > 0)
		m_OutputStream << ",";

	std::string name = result.m_Name;
	std::replace(name.begin(), name.end(), '"', '\'');

	m_OutputStream << "{";
	m_OutputStream << "\"cat\":\"function\",";
	m_OutputStream << "\"dur\":" << (result.m_End - result.m_Start) << ',';

	std::string deleted = " __cdecl";
	std::string::size_type i = name.find(deleted);

	if (i != std::string::npos)
		name.erase(i, deleted.length());

	m_OutputStream << "\"name\":\"" << name << "\",";
	m_OutputStream << "\"ph\":\"X\",";
	m_OutputStream << "\"pid\":0,";
	m_OutputStream << "\"tid\":" << result.m_ThreadID << ", ";
	m_OutputStream << "\"ts\":" << result.m_Start;
	m_OutputStream << "}";

	m_OutputStream.flush();
}

void Instrumentor::WriteHeader() {
	m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
	m_OutputStream.flush();
}

void Instrumentor::WriteFooter() {
	m_OutputStream << "]}";
	m_OutputStream.flush();
}

Instrumentor& Instrumentor::Get() {
	static Instrumentor* instance = new Instrumentor();
	return *instance;
}

#pragma endregion

#pragma region InstrumentationTimer

InstrumentationTimer::InstrumentationTimer(const char* name)
	: m_Name(name), m_Stopped(false)
{
	m_StartPoint = std::chrono::high_resolution_clock::now();
}

InstrumentationTimer::~InstrumentationTimer() {
	if (!m_Stopped)
		Stop();
}

const void InstrumentationTimer::Stop() {
	auto endPoint = std::chrono::high_resolution_clock::now();

	long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
	long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();

	uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
	Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });
	m_Stopped = true;
}

#pragma endregion