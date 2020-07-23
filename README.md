# molecool
A computation engine for simulating experiments with cold and ultracold atoms and molecules.

## Getting Started
Visual Studio 2017 or 2019 (on Windows) is recommended, _molecool_ is currently untested on other development environments.  

### External dependencies
Currently, the external dependences include the Intel MKL and Boost libraries.  Everything else you will need to get up and running is included in the repository, 
either as part of the source code or as a git submodule.

You can clone the repository to a local destination using git, making sure to do a `recursive` clone to fetch all of the submodules:

`git clone --recursive https://github.com/ColdMatter/molecool.git`

Before working with the codebase, you must install the aforementioned external dependencies and define a few environment variables so that _molecool_ knows where to find them on your local machine.    The required environmental variables are _MKLROOT_ and _BOOSTROOT_.  For example, on my machine these environmental variables evaluate to `C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2020.1.216/windows/` and `C:/Boost/Boost_1_73_0/`.  You can either set these yourself manually or edit and run the `scripts/win_setup_env_vars.bat` batch file.   

Once  the external dependencies are installed (and built, in the case of Boost), and the environment variables set, you are ready to create your project files.  In order to support platforms other than Windows in the future, and to avoid inexorably tying the project to Visual Studio, _molecool_ uses premake as a build system. For now, to generate your Visual Studio project and solution files, run the `scripts/win_premake_generate_project.bat` batch file.

## The Plan
The plan for _molecool_ is to build a easy to use, fast, general purpose simulation engine for simulating many of the experiments 
around the world using cold and ultracold atoms and molecules.  These experiments include molecular beams, guides, slowing techniques using electric or magnetic fields, conservative traps, magneto-optical traps, Doppler and sub-Doppler cooling, and
much more.     

### Planned features
- Fast, highly parallelized, particle trajectory simulations
- Simulation of atomic and molecular quantum state populations in the presence of electromagnetic fields, including semi-classical rate equations and fully quantum mechanical optical Bloch equations
- Support for Windows, Mac, and Linux, including clusters
- Fully scripted behavior, allowing for rapid development and optimization