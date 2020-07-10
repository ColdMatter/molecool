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
        // taking discrete steps is better, we can break out early if no particles are active
        //integrate_const(stepper, ho, std::make_pair(std::ref(ensemble.getPositions()), std::ref(ensemble.getVelocities())), startT, endT, dt);
        for (double t = startT; t < endT; t += dt) {
            stepper.do_step(thruster, std::make_pair(std::ref(ensemble.getPositions()), std::ref(ensemble.getVelocities())), t, dt);
            // independent steps allow us to test for boundary conditions, lost molecules [-> set a=0,v=0], etc.
            // but it would be more efficient to do these checks in the system function!
            // that can be done (or in the observer is more natural), just requires breaking the const promise!
            // HERE WE SHOULD TEST IF WE SHOULD BREAK OUT OF THE LOOP
            // if (nActiveParticles == 0) { break; }
        }
        
        // ensemble.save("finals.txt");
        std::cout << "press any key to end" << std::endl;
        std::cin.get();
    }

    void Simulation::addParticles(size_t n, ParticleId p, PosDist xDis, VelDist vxDis, PosDist yDis, VelDist vyDis, PosDist zDis, VelDist vzDis) {
        std::array< std::pair< PosDist, VelDist>, MC_DIMS > dists;
        dists[0] = std::make_pair(xDis, vxDis);
        dists[1] = std::make_pair(yDis, vyDis);
        dists[2] = std::make_pair(zDis, vzDis);
        ensemble.addParticles(n, p, dists);
    }

}