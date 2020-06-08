#pragma once

namespace molecool {

    class Simulation {
    public:
        Simulation();
        virtual ~Simulation();
        void run();
    };

    // METHOS TO BE DEFINED IN CLIENT APPLICATION
    Simulation* createSimulation();

}