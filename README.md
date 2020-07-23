# molecool
A computation engine for simulating experiments with cold and ultracold atoms and molecules.

## Getting Started
Visual Studio 2017 or 2019 (on Windows) is recommended, _molecool_ is currently untested on other development environments.  

You can clone the repository to a local destination using git, making sure to do a `recursive` clone to fetch all of the submodules:

`git clone --recursive https://github.com/ColdMatter/molecool.git`

## The Plan
The plan for _molecool_ is to build a easy to use, fast, general purpose simulation engine for simulating many of the experiments 
around the world using cold and ultracold atoms and molecules.  These experiments include molecular beams, guides, slowing techniques using electric or magnetic fields, conservative traps, magneto-optical traps, Doppler and sub-Doppler cooling, and
much more.     

### Planned features
- Fast, highly parallelized, particle trajectory simulations
- Simulation of atomic and molecular quantum state populations in the presence of electromagnetic fields, including semi-classical rate equations and fully quantum mechanical optical Bloch equations
- Support for Windows, Mac, and Linux, including clusters
- Fully scripted behavior, allowing for rapid development and optimization