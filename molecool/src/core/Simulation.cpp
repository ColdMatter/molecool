#include "mcpch.h"
#include "Simulation.h"

#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

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
            // check for early exit
            if (ensemble.getActivePopulation() == 0) { break; }

            // calculate the relevant quantum state populations (if appropriate)

            // advance classical states one timestep
            stepper.do_step(std::ref(thruster), std::make_pair(std::ref(ensemble.getPos()), std::ref(ensemble.getVel())), t, dt);
            
            // deploy watcher object, tracking trajectories, population statistics, etc.
            watcher.deployObservers(t);

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
        lua.open_libraries(sol::lib::base);
        auto result = lua.safe_script_file("src/simulation.lua", sol::script_pass_on_error);  // load and execute script from file
        if (!result.valid()) {
            sol::error err = result;
            MC_CORE_ERROR("lua script error: {0}, exiting.", err.what());
            exit(1);
        }

        tStart  = lua.get<float>("startTime");  // explicit get of a variable
        tEnd    = lua["endTime"];               // implicit conversion to end type
        dt      = lua["timestep"];

        // get ensemble parameters stored in lua "ensemble" table
        sol::table ensTbl = lua["ensemble"];
        int n = ensTbl["population"];                  
        Dist xDist  = extractDist(ensTbl["xDistribution"]);
        Dist vxDist = extractDist(ensTbl["vxDistribution"]);
        Dist yDist  = extractDist(ensTbl["yDistribution"]);
        Dist vyDist = extractDist(ensTbl["vyDistribution"]);
        Dist zDist  = extractDist(ensTbl["zDistribution"]);
        Dist vzDist = extractDist(ensTbl["vzDistribution"]);
        addParticles(n, ParticleId::CaF, xDist, vxDist, yDist, vyDist, zDist, vzDist);

        // have lua script add any filters, need to expose some kind of host addFilter() function to script
        // probably need to define everything in string format and parse it to a host-level expression

        // have lua script add any forces, need to expose some kind of host addForce() function to script

        // have lua script add any observers, need to expose some kind of host addObserver() function to script
        
    }

    // expect 
    Dist Simulation::extractDist(sol::table table) {
        PDF pdf = nameToPDF(table["pdf"]);
        double p1 = 0.0, p2 = 0.0;  // shape parameters
        switch (pdf) {
        case PDF::delta:
            p1 = table["center"];
            break;
        case PDF::flat:
            p1 = table["min"];
            p2 = table["max"];
            break;
        case PDF::gaussian:
            p1 = table["center"];
            p2 = table["width"];
            break;
        default:
            p1 = table["displacement"];
            p2 = table["scalefactor"];
            break;
        }
        return Dist(pdf, p1, p2);
    }

    // could this be cleaner using magic enum?
    // this should probably be a static method of the PDF class
    // better yet, make the PDF constructor be able to take in a name/string?
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