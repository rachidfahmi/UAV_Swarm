# UAV Swarm Communication with Relay Handoff — DEVS Simulation

**Course:** SYSC 5104 — Methodologies for Discrete Event Modelling and Simulation  
**Student:** Rachid Fahmi (101415381)  
**Date:** February 2026  

---

## Project Overview

This project models a UAV swarm surveillance system using the DEVS formalism and the Cadmium v2 simulation framework.

Three UAVs periodically receive surveillance tasks, collect sensor data, and transmit the data to a ground station. When a UAV moves beyond communication range, a Network Link model detects the degradation and eventual disconnection. A Handoff Coordinator then triggers a relay mode in the affected UAV’s communication module.

Two configurations are implemented and compared:

1. **Baseline (No Handoff)** — Direct transmission only  
2. **With Handoff** — Relay mode enabled upon link disconnection  

The goal is to evaluate packet delivery behavior and task completion under link degradation conditions.

---

## Model Hierarchy

The system is implemented as a four-level DEVS hierarchy:

- **Level 4 — MissionSystem**  
  Top-level model combining task generation, communication cluster, and ground station aggregation.

- **Level 3 — CommunicationCluster**  
  Three UAV nodes, three network links, and optionally a HandoffCoordinator.

- **Level 2 — MobileUAVNode (×3)**  
  Each UAV contains:
  - UAVSensorModule  
  - UAVMobilityController  
  - UAVCommunicationNode  

- **Level 1 — Atomic Models (7 total)**  
  - MissionTaskGenerator  
  - UAVSensorModule  
  - UAVMobilityController  
  - UAVCommNode  
  - NetworkLinkModel  
  - HandoffCoordinator  
  - GroundStationAggregator  

---

## Repository Structure

```text
UAV_Swarm/
├── main/
│   ├── main.cpp                 # Entry point
│   └── include/
│       ├── atomics/
│       │   ├── mission_task_generator.hpp
│       │   ├── uav_sensor_module.hpp
│       │   ├── uav_mobility_controller.hpp
│       │   ├── uav_comm_node.hpp
│       │   ├── network_link_model.hpp
│       │   ├── handoff_coordinator.hpp
│       │   └── ground_station_aggregator.hpp
│       └── coupled/
│           ├── mobile_uav_node.hpp
│           ├── communication_cluster.hpp
│           ├── communication_cluster_no_handoff.hpp
│           ├── mission_system.hpp
│           └── mission_system_no_handoff.hpp
├── logs/
│   ├── exp1a_mission_task_generator.log
│   ├── exp1b_uav_sensor_module.log
│   ├── exp1c_network_link_model.log
│   ├── exp1d_ground_station_aggregator.log
│   ├── exp1e_handoff_coordinator.log
│   ├── exp1f_uav_comm_node.log
│   ├── exp1g_uav_mobility_controller.log
│   ├── exp2_baseline_no_handoff.log
│   └── exp3_with_handoff.log
├── CMakeLists.txt
├── build_sim.sh
├── run_experiments.sh
├── DEVSmodelsForm.md
├── UAV-Swarm-DEVS-Simulation-Report.pdf
└── README.md
```


---

## Dependencies

- [Cadmium v2](https://github.com/SimulationEverywhere/cadmium_v2)
- CMake ≥ 3.14
- GCC ≥ 11 (C++17)

Cadmium v2 must be cloned at `~/cadmium_v2` or the path updated in `CMakeLists.txt`.

---

## Build Instructions

```bash
git clone https://github.com/rachidfahmi/UAV_Swarm.git
cd UAV_Swarm
source build_sim.sh
```
Running the Experiments

Run all experiments at once and save logs:

```bash
./run_experiments.sh
```

This runs:
| Script target | Experiment                   | Output log                        |
| ------------- | ---------------------------- | --------------------------------- |
| Exp 1a–1g     | Atomic unit tests (7 models) | logs/exp1a–1g_*.log               |
| Exp 2         | Baseline — no handoff        | logs/exp2_baseline_no_handoff.log |
| Exp 3         | With handoff                 | logs/exp3_with_handoff.log        |

## Running a Specific Configuration Manually

To run an individual experiment instead of using `run_experiments.sh`:

1. Open `main/main.cpp`
2. Instantiate the desired top-level model:

   - `MissionSystemNoHandoff` → for **Experiment 2 (Baseline)**
   - `MissionSystem` → for **Experiment 3 (With Handoff)**

3. Rebuild and execute:

```bash
source build_sim.sh
./bin/UAV_Swarm > logs/my_run.log
```
Log Format
```bash
time ; model_id ; model_name ; port_name ; data
```

Verifying Results

Key events to check in the logs:

| Event                | Expected time | Log file              |
| -------------------- | ------------- | --------------------- |
| First task_assigned  | t=60          | exp3_with_handoff.log |
| nl0 → Disconnected   | t=45          | exp2 and exp3         |
| handoff_in fired     | t=46          | exp3_with_handoff.log |
| First task_completed | t=126         | exp2 and exp3         |

## Notes on Relay Behavior

Relay mode updates a communication flag inside the affected UAV’s Communication Node. However, the structural couplings in the CommunicationCluster remain static, meaning packets are still routed through their original NetworkLink model.

As a result, relay mode validates the detection and control logic but does not implement dynamic two-hop rerouting. Supporting true relay routing would require structural changes to the coupling configuration and is identified as future work.

Documentation

     - UAV-Swarm-DEVS-Simulation-Report.pdf — Full modeling, implementation, and experimental analysis

     - DEVSmodelsForm.md — Formal DEVS model and port descriptions.





