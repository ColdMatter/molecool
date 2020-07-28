#include "mcpch.h"
#include "Staticizer.h"

namespace molecool {

	Staticizer::Staticizer()
	{
		MC_CORE_TRACE("Creating staticizer");;
	}

	void Staticizer::operator()(const Ensemble& ens, double t) {
		MC_PROFILE_FUNCTION();
		lifetime.push_back(std::make_pair(t, ens.getPopulation()));
	}

	Staticizer::~Staticizer() {
		MC_PROFILE_FUNCTION();
		MC_CORE_TRACE("Destroying staticizer");
		std::ofstream outputStream;
		outputStream.open("output/statistics.json");
		if (!outputStream.is_open())
		{
			if (Log::getCoreLogger()) // Edge case: destructor might be before Log::init()
			{
				MC_CORE_ERROR("Staticizer could not open output file.");
			}
			exit(-1);
		}
		outputStream << std::fixed << std::setprecision(6);
		outputStream << "{\"statistics\":[";
		outputStream << "{\"lifetime\":[";
		for (size_t j = 0; j < lifetime.size(); ++j) {
			LifetimePoint p = lifetime.at(j);
			if (j > 0) { outputStream << ","; }
			outputStream << "{\"t\":" << p.first << ",\"pop\":" << p.second << "}";
		}
		outputStream << "]}";
		outputStream << "]}";
		outputStream.flush();
		outputStream.close();
	}
}
