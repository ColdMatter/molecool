#pragma once

#include "core/Watcher.h"
#include "core/Vector.h"
#include "core/Ensemble.h"

namespace molecool {
    
    class Trajectorizer : public Observer
    {
	public:

		using TrajectoryPoint = std::pair<double, Position>;
		using Trajectory = std::vector<TrajectoryPoint>;

		Trajectorizer(const Ensemble& ens, int nParticles);
		~Trajectorizer();
		void operator()(const Ensemble& ensemble, double t);

	private:
		int m_nParticles;
		std::vector<Trajectory> trajectories;
		int m_instance = 0;
		static int s_instance;

    };
}


