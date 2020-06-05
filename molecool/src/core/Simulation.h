#pragma once

namespace molecool {

    class Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();
    };

    // TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}