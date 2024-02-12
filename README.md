# Intuitive Planning: Counterfactually Afforded Task and Motion Planning
## Overview
![CA-TAMP](https://github.com/asyncs/CA-TAMP/assets/40043682/0bb7475c-d81a-47bb-907e-a512241c7c04)
Task and Motion Planning (TAMP) in robotics faces the challenge of synthesizing human-like intuitive decision-making processes into autonomous systems. This paper introduces Counterfactually Afforded Task and Motion Planning (CA-TAMP), a framework that integrates human-defined object affordances into TAMP solutions. Inspired by the Dual Process Theory of human cognition, CA-TAMP leverages counterfactual reasoning to generate intuitive, efficient solutions for long-horizon tasks in complex environments. Our approach employs Mini-LGP for subdividing tasks into affordance-based sub-problems and Counterfactual-LGP to integrate the  Effect of Treatment on the Treated, aiding in intuitive decision-making. Through a series of simulated and human experiments, we demonstrate the practicality of CA-TAMP in typical serving tasks, emphasizing its alignment with human intuition and efficiency. The results highlight the framework's capability to mirror human behavior, suggesting significant implications for the development of intuitive, human-like autonomous agents. CA-TAMP’s adaptability and effectiveness in utilizing non-task-related objects present a novel avenue in robotic task planning, resonating with the human approach to energy conservation and decision-making.

## Example Usage
![Serving Task with the Use of Tray.](https://github.com/asyncs/CA-TAMP/assets/40043682/619b3953-f97a-4778-9ccb-0b3725c371a7)
![Serving Task without the Use of Tray.](https://github.com/asyncs/CA-TAMP/assets/40043682/02f80296-cfc7-4e9a-a71e-693b9deed0de)
To understand how CA-TAMP operates, set the environment and task parameters in `test/main.cpp`, build the executable, and then run the provided script.
``` 
cd test/serving
./../../build/serving 
```
## Installation
### Requirments
Assumes a standard Ubuntu 20.04 (or 18.04) machine.
### Quick Start
``` 
git clone --recursive -j4 https://github.com/asyncs/CA-TAMP.git
```
#### Build the RAI submodule.
```
cd CA-TAMP/rai
make -j1 printUbuntuAll
make -j1 installUbuntuAll APTGETYES=--yes
make -j4
make -j4 tests bin
make runTests
```
#### Build the CA-TAMP.
```
cd ..
mkdir build && cd build
cmake ..
cmake --build .
make
```
