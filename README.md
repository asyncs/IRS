# Interpretable Responsibility Sharing as a Heuristic for Task and Motion Planning
## Overview
<p align="center">
  <img src="https://github.com/user-attachments/assets/e17382f9-98e3-4b0f-a9aa-9f4cd9846e9f" alt="IRS"/>
</p>

This article introduces a novel heuristic for Task and Motion Planning (TAMP) named Interpretable Responsibility Sharing (IRS), which enhances planning efficiency in domestic robots by leveraging human-constructed environments and inherent biases. Utilizing auxiliary objects (e.g., trays and pitchers), which are commonly found in household settings, IRS systematically incorporates these elements to simplify and optimize task execution. The heuristic is rooted in the novel concept of Responsibility Sharing (RS), where auxiliary objects share the task's responsibility with the embodied agent, dividing complex tasks into manageable sub-problems. This division not only reflects human usage patterns but also aids robots in navigating and manipulating within human spaces more effectively. By integrating Optimized Rule Synthesis (ORS) for decision-making, IRS ensures that the use of auxiliary objects is both strategic and context-aware, thereby improving the interpretability and effectiveness of robotic planning. Experiments conducted across various household tasks demonstrate that IRS significantly outperforms traditional methods by reducing the effort required in task execution and enhancing the overall decision-making process. This approach not only aligns with human intuitive methods but also offers a scalable solution adaptable to diverse domestic environments.

## Installation
### Requirments
Assumes a standard Ubuntu 20.04 (or 18.04) machine.
The following assumes $HOME/git as your git path, and $HOME/.local to install 3rd-party libs -- please stick to this (no system-wide installs).
```
sudo apt update
sudo apt install --yes \
  g++ clang make cmake curl git wget \
  liblapack-dev libf2c2-dev libqhull-dev libeigen3-dev libann-dev libccd-dev \
  libjsoncpp-dev libyaml-cpp-dev libpoco-dev libboost-system-dev portaudio19-dev libusb-1.0-0-dev libhidapi-dev \
  libx11-dev libglu1-mesa-dev libglfw3-dev libglew-dev freeglut3-dev libpng-dev libassimp-dev
mkdir -p $HOME/git $HOME/.local
```
External libraries: You can skip librealsense and libfranka if you disable in CMake.
```
export MAKEFLAGS="-j $(command nproc --ignore 2)"
wget https://github.com/MarcToussaint/rai-extern/raw/main/install.sh; chmod a+x install.sh
./install.sh fcl
./install.sh physx
./install.sh librealsense
./install.sh libfranka  ## for OLD frankas instead:   ./install.sh -v 0.7.1 libfranka
```
You can skip this, if you disable pybind11 in CMake.
```
sudo apt install --yes python3-dev python3 python3-pip
python3 -m pip install --user numpy pybind11 pybind11-stubgen

# add the following to your .bashrc, if not done already
echo 'export PATH="${PATH}:$HOME/.local/bin"' >> ~/.bashrc
echo 'export PYTHONPATH="${PYTHONPATH}:$HOME/.local/lib"' >> ~/.bashrc
```
### Quick Start
```
cd $HOME/git
git clone --recurse-submodules -j4 https://github.com/asyncs/IRS.git
```
#### Build the RAI submodule.
```
cd IRS/rai
make -j1 printUbuntuAll
make -j1 installUbuntuAll APTGETYES=--yes
make -j4
make -j4 tests bin
make runTests
```
#### Build the IRS.
```
cd ..
mkdir build && cd build
cmake ..
cmake --build .
make
```

## Example Usage
<p float="left">
  <img src="https://github.com/asyncs/CA-TAMP/assets/40043682/89b6b435-a974-42cf-9fc1-c55a43e5784b" width="500" /> 
  <img src="https://github.com/asyncs/CA-TAMP/assets/40043682/b37f07c6-c0a1-46b1-a790-ba3cd2acf074" width="500" />
</p>

To understand how IRS operates, set the environment and task parameters e.g., in `test/serving/main.cpp`, build the executable, and then run the provided script.
``` 
cd ../test/serving
./../../build/serving 
```
To understand how ORS operates, you can navigate to its directory, 'ORS,' to take a closer look.
