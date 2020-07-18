#include "mcpch.h"
#include "Trajectorizer.h"

namespace molecool {

	Trajectorizer::Trajectorizer(const Ensemble& ens, int nParticles)
		: m_nParticles(std::min(nParticles, ens.getPopulation()))
	{
		MC_CORE_TRACE("Creating trajectory tracker (observer), tracking first {0} trajectories", m_nParticles);
		trajectories.resize(m_nParticles);
	}

	void Trajectorizer::operator()(const Ensemble& ensemble, double t) {
		MC_PROFILE_FUNCTION();
		for (int i = 0; i < m_nParticles; ++i) {
			trajectories.at(i).push_back(std::make_pair(t, ensemble.getParticlePos(i)));
		}
	}

	Trajectorizer::~Trajectorizer() {
		MC_CORE_TRACE("destroying trajectory tracker");
		std::ofstream outputStream;
		outputStream.open("trajectories.json");
		if (!outputStream.is_open())
		{
			if (Log::getCoreLogger()) // Edge case: constructor might be before Log::init()
			{
				MC_CORE_ERROR("TrajectoryTracker could not open output file.");
			}
			exit(-1);
		}
		//outputStream << std::fixed << std::setprecision(6);
		outputStream << "{\"trajectories\":[";
		for (size_t i = 0; i < trajectories.size(); ++i) {
			Trajectory t = trajectories.at(i);
			if (i > 0) { outputStream << ","; }
			outputStream << "{\"trajectory\":[";
			for (size_t j = 0; j < t.size(); ++j) {
				TrajectoryPoint p = t.at(j);
				if (j > 0) { outputStream << ","; }
				outputStream << "{\"time\":" << p.first << ",\"position\":[" << p.second << "]}";
			}
			outputStream << "]}";
		}
		outputStream << "]}";
		outputStream.flush();
		outputStream.close();
	}
}
