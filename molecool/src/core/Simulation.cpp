#include "mcpch.h"
#include "Simulation.h"

namespace molecool {
    
    Simulation::Simulation() 
    : thruster(ensemble), watcher(ensemble)
    {
        MC_PROFILE_FUNCTION();
        parseScript();
    }

    Simulation::~Simulation() {
        MC_PROFILE_FUNCTION();
    }
    
    void Simulation::run() {
        MC_PROFILE_FUNCTION();
        ensemble.save("initials");
        propagate();
        ensemble.save("finals");
    }

    void Simulation::propagate() {
        MC_PROFILE_FUNCTION();
        MC_CORE_TRACE("propagating {0} particles...", ensemble.getActivePopulation());

        using namespace boost::numeric::odeint;
        // using openmp algebra + standard operations + openmp system() function seems to reliably be the fastest
        // doesn't seem to be compatible with using the mkl operations, which require a vector_space_algebra
        // I think the absolute fastest would be to manually parallelize and have each thread use independent 
        // (non parallelized) unique copies of the stepper and system function, and use mkl operations
        //using stepper_type = velocity_verlet< state_type >;
        //using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, vector_space_algebra, mkl_operations >;
        using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, openmp_range_algebra >;
        stepper_type stepper;
        for (double t = tStart; t <= tEnd; t += dt) {
            // calculate the relevant quantum state populations (if appropriate)

            // advance classical states one timestep
            stepper.do_step(std::ref(thruster), std::make_pair(std::ref(ensemble.getPos()), std::ref(ensemble.getVel())), t, dt);
            
            // deploy watcher object, tracking trajectories, population statistics, etc.
            watcher.deployObservers(t);

            // check for early exit
            if (ensemble.getActivePopulation() == 0) { break; }
        }
        MC_CORE_TRACE("propagation complete, {0} particles still active", ensemble.getActivePopulation());
    }

    void Simulation::addParticles(int n, ParticleId p, PosDist xDis, VelDist vxDis, PosDist yDis, VelDist vyDis, PosDist zDis, VelDist vzDis) {
        MC_PROFILE_FUNCTION();;
        std::array< std::pair<PosDist, VelDist>, MC_DIMS > dists;
        dists[0] = std::make_pair(xDis, vxDis);
        dists[1] = std::make_pair(yDis, vyDis);
        dists[2] = std::make_pair(zDis, vzDis);
        ensemble.addParticles(n, p, dists);
    }

    void Simulation::addFilter(FilterFunction ff) {
        thruster.addFilter(ff);
    }

    void Simulation::addForce(ForceFunction ff) {
        thruster.addForce(ff);
    }

    void Simulation::addObserver(ObserverPtr obs) {
        watcher.addObserver(obs);
    }
    
    void Simulation::parseScript() {
        LuaScript script("src/simulation.lua");

        // get simulation time settings
        tStart = script.get<float>("startTime");            
        tEnd = script.get<float>("endTime");
        dt = script.get<float>("timestep");

        // get ensemble parameters
        int n = script.get<int>("ensemble.population");
        Dist xDist  = extractDist(script, "ensemble.xDistribution");
        Dist vxDist = extractDist(script, "ensemble.vxDistribution");
        Dist yDist  = extractDist(script, "ensemble.yDistribution");
        Dist vyDist = extractDist(script, "ensemble.vyDistribution");
        Dist zDist  = extractDist(script, "ensemble.zDistribution");
        Dist vzDist = extractDist(script, "ensemble.vzDistribution");
        addParticles(n, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);
        
        // getting arrays
        std::vector<int> v = script.getIntVector("array");
        std::cout << "Contents of array:";
        for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
            std::cout << *it << ",";
        }
        std::cout << std::endl;

        // getting table keys:
        std::vector<std::string> keys = script.getTableKeys("ensemble");
        std::cout << "Keys of [simulation] table:";
        for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); it++) {
            std::cout << *it << ",";
        }
        std::cout << std::endl;
        
    }

    Dist Simulation::extractDist(LuaScript& script, std::string name) {
        PDF pdf = nameToPDF( script.get<std::string>(name + ".pdf") );
        double p1 = 0.0, p2 = 0.0;  // shape parameters
        switch (pdf) {
        case PDF::delta:
            p1 = script.get<float>(name + ".center");
            break;
        case PDF::flat:
            p1 = script.get<float>(name + ".min");
            p2 = script.get<float>(name + ".max");
            break;
        case PDF::gaussian:
            p1 = script.get<float>(name + ".center");
            p2 = script.get<float>(name + ".width");
            break;
        default:
            p1 = script.get<float>(name + ".displacement");
            p2 = script.get<float>(name + ".scalefactor");
            break;
        }
        return Dist(pdf, p1, p2);
    }

    PDF Simulation::nameToPDF(std::string name) {
        if (name == "delta") {
            return PDF::delta;
        }
        else if (name == "flat") {
            return PDF::flat;
        }
        else if (name == "gaussian") {
            return PDF::gaussian;
        }
        else if (name == "exponential") {
            return PDF::exponential;
        }
        else if (name == "laplace") {
            return PDF::laplace;
        }
        else if (name == "cauchy") {
            return PDF::cauchy;
        }
        else if (name == "rayleigh") {
            return PDF::rayleigh;
        }
        else {
            MC_CORE_WARN("distribution type {0} not recognized", name);
            return PDF::delta;
        }
    }
    
}