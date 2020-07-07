#pragma once

#define MC_PROFILE 1			// enable/disable global profiling/timers (statistics outputs enabled)
#define MC_PROFILE_VERBOSE 1	// enable/disable detailed profile .json output (along with statistics)

#if MC_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define MC_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define MC_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define MC_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define MC_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define MC_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define MC_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define MC_FUNC_SIG __func__
#else
#define MC_FUNC_SIG "MC_FUNC_SIG unknown!"
#endif

#define MC_PROFILE_BEGIN_SESSION(name) ::molecool::Profiler::get().beginSession(name)
#define MC_PROFILE_END_SESSION() ::molecool::Profiler::get().endSession()
#define MC_PROFILE_SCOPE(name) constexpr auto fixedName = ::molecool::ProfilerUtils::cleanupOutputString(name, "__cdecl ");\
									::molecool::Timer timer##__LINE__(fixedName.data)
#define MC_PROFILE_FUNCTION() MC_PROFILE_SCOPE(MC_FUNC_SIG)
#else
#define MC_PROFILE_BEGIN_SESSION(name, filepath)
#define MC_PROFILE_END_SESSION()
#define MC_PROFILE_SCOPE(name)
#define MC_PROFILE_FUNCTION()
#endif

#include <algorithm>
#include <chrono>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/sum.hpp>

namespace molecool {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	// accumulator for statistics associated with a particular profile timer
	struct TimerStats {
		TimerStats(std::string name, double time)
		: name(name)
		{
			acc(time);
		}

		std::string name;
		boost::accumulators::accumulator_set<
			double,
			boost::accumulators::stats <
			boost::accumulators::tag::count,
			boost::accumulators::tag::min,
			boost::accumulators::tag::mean,
			boost::accumulators::tag::max,
			boost::accumulators::tag::sum
			>
		> acc;

		inline void addData(double t/* time (us) */) { acc(t); }
		inline double getMean() { return boost::accumulators::mean(acc); }
		inline double getMin() { return boost::accumulators::extract_result< boost::accumulators::tag::min >(acc); }
		inline double getMax() { return boost::accumulators::extract_result< boost::accumulators::tag::max >(acc); }
		inline size_t getCount() { return boost::accumulators::count(acc); }
		inline double getSum() { return boost::accumulators::sum(acc); }
	};

	struct ProfileResult
	{
		std::string name;
		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;
		std::thread::id threadID;
	};


	struct ProfilerSession
	{
		std::string name;
	};


	class Profiler
	{
	private:
		std::mutex m_mutex;
		ProfilerSession* m_currentSession;
		std::ofstream m_outputStream;
		std::ofstream m_statsStream;
		std::vector<molecool::TimerStats> m_stats;
	public:
		Profiler()
			: m_currentSession(nullptr)
		{
		}

		void beginSession(const std::string& name)
		{
			std::lock_guard lock(m_mutex);
			if (m_currentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::getCoreLogger()) // Edge case: beginSession() might be before Log::init()
				{
					MC_CORE_ERROR("Profiler::beginSession('{0}') when session '{1}' already open.", name, m_currentSession->name);
				}
				internalEndSession();
			}
			std::string outputFile = "profile_" + name + ".json";
			std::string statsFile = "profile_" + name + "_stats.txt";
			m_outputStream.open(outputFile);
			m_statsStream.open(statsFile);

			if (m_outputStream.is_open() && m_statsStream.is_open())
			{
				m_currentSession = new ProfilerSession({ name });
				writeHeader();
			}
			else
			{
				if (Log::getCoreLogger()) // Edge case: beginSession() might be before Log::init()
				{
					MC_CORE_ERROR("Profiler could not open results file(s).");
				}
			}
		}

		void endSession()
		{
			std::lock_guard lock(m_mutex);
			internalEndSession();
		}

		void writeProfile(const ProfileResult& result)
		{
			std::stringstream json;
			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.elapsedTime.count()) << ',';
			json << "\"name\":\"" << result.name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.threadID << ",";
			json << "\"ts\":" << result.start.count();
			json << "}";

			std::lock_guard lock(m_mutex);
			if (m_currentSession)
			{
				m_outputStream << json.str();
				m_outputStream.flush();
			}
		}

		void gatherStats(const std::string name, double time)
		{
			auto it = std::find_if(m_stats.begin(), m_stats.end(), [&name](const TimerStats& obj) {return obj.name == name; });
			if (it != m_stats.end()) {
				// this timer has previously reported, add the data value
				it->addData(time);
			}
			else {
				// first time this timer has reported a value
				m_stats.push_back(TimerStats(name, time));
			}
		}

		static Profiler& get()
		{
			static Profiler instance;
			return instance;
		}

	private:

		void writeHeader()
		{
			m_outputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_outputStream.flush();
		}

		void writeFooter()
		{
			m_outputStream << "]}";
			m_outputStream.flush();
		}

		void writeStatistics() {
			for (TimerStats tStats : m_stats) {
				std::stringstream stats;
				stats << std::setprecision(3) << std::fixed;
				stats << tStats.name;
				stats << "\n\tcalled " << tStats.getCount() << " times";
				stats << "\n\taverage: " << format(tStats.getMean());
				stats << "\n\tminimum: " << format(tStats.getMin());
				stats << "\n\tmaximum: " << format(tStats.getMax());
				stats << "\n\t  total: " << format(tStats.getSum());
				stats << std::endl << std::endl;
				m_statsStream << stats.str();
				m_statsStream.flush();
			}
		}

		// format double (us) as value and appropriate time unit for easy reading
		std::string format(double t) {
			double div = 1.0;
			char* unit = " us";
			if (t > 1e6) {
				unit = " s";
				div = 1e6;
			}
			else if (t > 1e3) {
				unit = " ms";
				div = 1e3;
			}
			std::stringstream timeStr;
			timeStr << std::setprecision(3) << std::fixed;
			timeStr << t / div << unit;
			return timeStr.str();
		}

		// Note: you must already own lock on m_mutex before
		// calling internalEndSession()
		void internalEndSession()
		{
			if (m_currentSession)
			{
				writeStatistics();
				m_statsStream.close();
				m_stats.clear();

				writeFooter();
				m_outputStream.close();

				delete m_currentSession;
				m_currentSession = nullptr;
			}
			
		}

	};

	class Timer
	{
	public:
		Timer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Profiler::get().gatherStats(m_Name, (double)elapsedTime.count());
			if (MC_PROFILE_VERBOSE) {
				Profiler::get().writeProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
			}
			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace ProfilerUtils {

		template <size_t N>
		struct ChangeResult
		{
			char data[N];
		};

		template <size_t N, size_t K>
		constexpr auto cleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}


