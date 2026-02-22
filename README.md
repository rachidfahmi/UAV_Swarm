UAV Swarm Communication with Relay Handoff — DEVS Simulation

Course: SYSC 5104 — Methodologies for Discrete Event Modelling and Simulation
Student: Rachid Fahmi (101415381)
Date: February 2026

Project Overview

This project models a UAV swarm surveillance system using the DEVS formalism and the Cadmium v2 simulation framework.

Three UAVs periodically receive surveillance tasks, collect sensor data, and transmit the data to a ground station. When a UAV moves beyond communication range, a Network Link model detects the degradation and eventual disconnection. A Handoff Coordinator then triggers a relay mode in the affected UAV’s communication module.

Two configurations are implemented and compared:

Baseline (No Handoff) — Direct transmission only

With Handoff — Relay mode enabled upon link disconnection

The goal is to evaluate packet delivery behavior and task completion under link degradation conditions.

Model Hierarchy

The system is implemented as a four-level DEVS hierarchy:

Level 4 — MissionSystem
Top-level model combining task generation, communication cluster, and ground station aggregation.

Level 3 — CommunicationCluster
Three UAV nodes, three network links, and optionally a HandoffCoordinator.

Level 2 — MobileUAVNode (×3)
Each UAV contains:

UAVSensorModule

UAVMobilityController

UAVCommunicationNode

Level 1 — Atomic Models (7 total)

MissionTaskGenerator

UAVSensorModule

UAVMobilityController

UAVCommNode

NetworkLinkModel

HandoffCoordinator

GroundStationAggregator

Repository Structure
UAV_Swarm/
├── main/
│   ├── main.cpp
│   └── include/
│       ├── atomics/
│       └── coupled/
├── logs/
├── CMakeLists.txt
├── build_sim.sh
├── run_experiments.sh
├── DEVSmodelsForm.md
├── UAV-Swarm-DEVS-Simulation-Report.pdf
└── README.md

atomics/ contains the 7 atomic DEVS models.

coupled/ contains hierarchical model compositions.

logs/ contains experiment output logs.

run_experiments.sh executes all defined experiments.

The PDF report documents modeling, implementation, and results.

Dependencies

Cadmium v2
https://github.com/SimulationEverywhere/cadmium_v2

CMake ≥ 3.14

GCC ≥ 11 (C++17)

Ensure Cadmium is available locally (e.g., ~/cadmium_v2) or update the include path in CMakeLists.txt.

Build Instructions

Clone the repository and build:

git clone https://github.com/rachidfahmi/UAV_Swarm.git
cd UAV_Swarm
source build_sim.sh

The executable will be created at:

bin/UAV_Swarm
Running the Experiments

To run all experiments and generate logs:

./run_experiments.sh

This executes:

Script Target	Description	Output
Exp 1a–1g	Atomic model unit tests	logs/exp1*.log
Exp 2	Baseline (no handoff)	logs/exp2_baseline_no_handoff.log
Exp 3	With handoff enabled	logs/exp3_with_handoff.log

To run a specific configuration manually:

Edit main/main.cpp and instantiate:

MissionSystemNoHandoff for Experiment 2

MissionSystem for Experiment 3

Rebuild and execute:

source build_sim.sh
./bin/UAV_Swarm > logs/my_run.log
Log Format

Simulation logs follow this format:

time ; model_id ; model_name ; port_name ; data

If port_name is empty, the line represents a state update.

If port_name contains a value, it represents an output event.

Key Events to Verify

When reviewing logs, check for:

Event	Expected Time	Log
First task_assigned	t = 60	exp3_with_handoff.log
NL0 → Disconnected	t = 45	exp2 / exp3
handoff_in fired	t = 46	exp3_with_handoff.log
First task_completed	t = 126	exp2 / exp3
Notes on Relay Behavior

Relay mode updates a communication flag inside the affected UAV’s Communication Node. However, the structural couplings remain static. Therefore, packets are still routed through their original NetworkLink model.

As a result, relay mode validates the detection-and-control chain but does not implement dynamic two-hop rerouting. Extending the model to support full relay routing is identified as future work.

Documentation

UAV-Swarm-DEVS-Simulation-Report.pdf — Full modeling, implementation, and experimental analysis

DEVSmodelsForm.md — Formal DEVS model description
