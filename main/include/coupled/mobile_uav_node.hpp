#ifndef MOBILE_UAV_NODE_HPP
#define MOBILE_UAV_NODE_HPP

#include <cadmium/modeling/devs/coupled.hpp>
#include "../atomics/uav_sensor_module.hpp"
#include "../atomics/uav_mobility_controller.hpp"
#include "../atomics/uav_comm_node.hpp"

using namespace cadmium;

struct MobileUAVNode : public Coupled {
    Port<int>          task_in;
    Port<HandoffMsg>   handoff_in;
    Port<int>          tx_out;
    Port<PosUpdateMsg> pos_update;

    MobileUAVNode(const std::string& id,
                  int uav_id = 0,
                  double x = 0.0, double y = 0.0,
                  double vx = 0.0, double vy = 0.0)
        : Coupled(id) {

        task_in    = addInPort<int>("task_in");
        handoff_in = addInPort<HandoffMsg>("handoff_in");
        tx_out     = addOutPort<int>("tx_out");
        pos_update = addOutPort<PosUpdateMsg>("pos_update");

        auto sm = addComponent<UAVSensorModule>("sensor_module");
        auto mc = addComponent<UAVMobilityController>(
                      "mobility_controller", x, y, vx, vy);
        auto cn = addComponent<UAVCommNode>("comm_node", uav_id);

        // EIC
        addCoupling(task_in,    sm->task_in);
        addCoupling(handoff_in, cn->handoff_in);

        // IC
        addCoupling(sm->data_packet, cn->data_packet);

        // EOC
        addCoupling(cn->tx_out,     tx_out);
        addCoupling(mc->pos_update, pos_update);
    }
};

#endif
