#include "mcpch.h"
#include "Trajectorizer.h"

namespace molecool {

	int Trajectorizer::s_instance = 0;

	Trajectorizer::Trajectorizer(int nParticles)
		: m_nParticles(nParticles), m_instance(s_instance)
	{
		MC_CORE_TRACE("Creating trajectorizer, tracking first {0} trajectories", m_nParticles);
		trajectories.resize(m_nParticles);
		s_instance++;
	}

	void Trajectorizer::operator()(const Ensemble& ens, double t) {
		MC_PROFILE_FUNCTION();
		for (int i = 0; i < m_nParticles; ++i) {
			if (ens.isParticleActive(i)) {
				trajectories.at(i).push_back(std::make_pair(t, ens.getParticlePos(i)));
			}
		}
	}

	Trajectorizer::~Trajectorizer() {
		MC_PROFILE_FUNCTION();
		MC_CORE_TRACE("Destroying trajectorizer");
		std::ofstream outputStream;
		std::string filename = "output/trajectories";
		if (m_instance > 0) { filename += std::to_string(m_instance); }
		filename += ".json";
		outputStream.open(filename);
		if (!outputStream.is_open())
		{
			if (Log::getCoreLogger()) // Edge case: constructor might be before Log::init()
			{
				MC_CORE_ERROR("TrajectoryTracker could not open output file.");
			}
			exit(-1);
		}
		outputStream << std::fixed << std::setprecision(6);
		outputStream << "{\"trajectories\":[";
		for (size_t i = 0; i < trajectories.size(); ++i) {
			Trajectory t = trajectories.at(i);
			if (i > 0) { outputStream << ","; }
			outputStream << "{\"trajectory\":[";
			for (size_t j = 0; j < t.size(); ++j) {
				TrajectoryPoint p = t.at(j);
				if (j > 0) { outputStream << ","; }
				outputStream << "{\"t\":" << p.first << ",\"x\":[" << p.second << "]}";
			}
			outputStream << "]}";
		}
		outputStream << "]}";
		outputStream.flush();
		outputStream.close();
	}
}
