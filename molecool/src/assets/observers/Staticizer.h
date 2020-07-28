#pragma once

#include "core/Watcher.h"

namespace molecool {
    
    class Staticizer : public Observer
    {
	public:

		using LifetimePoint = std::pair<double, int>;
		using Lifetime = std::vector<LifetimePoint>;

		Staticizer();
		~Staticizer();
		void operator()(const Ensemble& ens, double t) override;

		static ObserverPtr make() {
			return std::make_shared<Staticizer>();
		}

	private:
		Lifetime lifetime;

    };
}


