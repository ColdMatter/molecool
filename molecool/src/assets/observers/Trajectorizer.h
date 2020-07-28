#pragma once

#include "core/Watcher.h"
#include "core/Vector.h"

namespace molecool {
    
    class Trajectorizer : public Observer
    {
	public:

		using TrajectoryPoint = std::pair<double, Position>;
		using Trajectory = std::vector<TrajectoryPoint>;

		Trajectorizer(int nParticles);
		~Trajectorizer();
		void operator()(const Ensemble& ens, double t) override;

		// a factory-like function that knows how to create this object (on the heap)
		static ObserverPtr make(int n) {
			return std::make_shared<Trajectorizer>(n);
		}

	private:
		int m_nParticles;
		std::vector<Trajectory> trajectories;
		int m_instance = 0;
		static int s_instance;

    };
}


