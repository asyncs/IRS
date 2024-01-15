# Abstract
Task and Motion Planning (TAMP) in robotics faces the challenge of synthesizing human-like intuitive decision-making processes into autonomous systems. This paper introduces Counterfactually Afforded Task and Motion Planning (CA-TAMP), a framework that integrates human-defined object affordances into TAMP solutions. Inspired by the Dual Process Theory of human cognition, CA-TAMP leverages counterfactual reasoning to generate intuitive, efficient solutions for long-horizon tasks in complex environments. Our approach employs Mini-LGP for subdividing tasks into affordance-based sub-problems and Counterfactual-LGP to integrate the Average Causal Effect, aiding in intuitive decision-making. Through a series of simulated and human experiments, we demonstrate the practicality of CA-TAMP in typical serving tasks, emphasizing its alignment with human intuition and efficiency. The results highlight the framework's capability to mirror human behavior, suggesting significant implications for the development of intuitive, human-like autonomous agents. CA-TAMP’s adaptability and effectiveness in utilizing non-task-related objects present a novel avenue in robotic task planning, resonating with the human approach to energy conservation and decision-making.

![Serving Task without the Use of Tray.](https://github.com/asyncs/CA-TAMP/assets/40043682/156626b0-dabb-428c-9ec2-080c7a2030c4)
![Serving Task with the Use of Tray.](https://github.com/asyncs/CA-TAMP/assets/40043682/25389994-edbd-444a-8129-af6e5c5a0d03)

# Installation
## Requirments
Assumes a standard Ubuntu 20.04 (or 18.04) machine.
## Quick Start
* git clone [CA-TAMP](https://github.com/asyncs/CA-TAMP.git)
* cd CA-TAMP/rai

###Dependencies for the RAI submodule.

* make -j1 printUbuntuAll
* make -j1 installUbuntuAll APTGETYES=--yes
* make -j4
* make -j4 tests bin
* make runTests

###Dependencies for the CA-TAMP.

* cd ..
* mkdir build && cd build
* cmake ..
* cmake --build .
* make
