#include "mcpch.h"
#include "Simulation.h"

namespace molecool {
    
    Simulation::Simulation() 
    : thruster(ensemble)
    {
        MC_PROFILE_FUNCTION();
    }

    Simulation::~Simulation() {
        MC_PROFILE_FUNCTION();
    }
    
    void Simulation::run() {
        MC_PROFILE_FUNCTION();
        
        // ensemble.save("initials.txt");
        propagate();
        
        // ensemble.save("finals.txt");
        std::cout << "press any key to end" << std::endl;
        std::cin.get();
    }

    void Simulation::propagate() {
        MC_PROFILE_FUNCTION();
        MC_CORE_INFO("propogating {0} particles...", ensemble.getActivePopulation());

        using namespace boost::numeric::odeint;
        // using openmp algebra + standard operations + openmp system() function seems to reliably be the fastest
        // doesn't seem to be compatible with using the mkl operations, which require a vector_space_algebra
        // I think the absolute fastest would be to manually parallelize and have each thread use independent 
        // (non parallelized) steppers, system functions, and mkl operations
        //using stepper_type = velocity_verlet< state_type >;
        //using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, vector_space_algebra, mkl_operations >;
        using stepper_type = velocity_verlet< state_type, state_type, double, state_type, double, double, openmp_range_algebra >;
        stepper_type stepper;
        const double startT = 0.0;
        const double dt = 0.001;
        const double endT = 1.0;

        // taking discrete steps is better than using integrate(), we can break out early if no particles are active
        //integrate_const(stepper, thruster, std::make_pair(std::ref(ensemble.positions), std::ref(ensemble.velocities)), startT, endT, dt);
        for (double t = startT; t < endT; t += dt) {
            stepper.do_step(std::ref(thruster), std::make_pair(std::ref(ensemble.pos), std::ref(ensemble.vel)), t, dt);
            if (ensemble.getActivePopulation() == 0) { break; }
        }

        MC_CORE_INFO("propagation complete, {0} particles still active", ensemble.getActivePopulation());
    }

    void Simulation::addParticles(size_t n, ParticleId p, PosDist xDis, VelDist vxDis, PosDist yDis, VelDist vyDis, PosDist zDis, VelDist vzDis) {
        std::array< std::pair< PosDist, VelDist>, MC_DIMS > dists;
        dists[0] = std::make_pair(xDis, vxDis);
        dists[1] = std::make_pair(yDis, vyDis);
        dists[2] = std::make_pair(zDis, vzDis);
        ensemble.addParticles(n, p, dists);
    }

}