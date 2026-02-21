#ifndef TEST_NETWORK_LINK_HPP
#define TEST_NETWORK_LINK_HPP

#include <limits>
#include <cadmium/modeling/devs/coupled.hpp>
#include <cadmium/lib/iestream.hpp>
#include "../atomics/network_link_model.hpp"

using namespace cadmium;

struct TestNetworkLink : public Coupled {
    TestNetworkLink(const std::string& id) : Coupled(id) {
        auto nlm       = addComponent<NetworkLinkModel>("network_link_model", 0, 100.0, 200.0);
        auto tx_input  = addComponent<lib::IEStream<int>>(
            "tx_input", "./inputs/test_nlm_tx.txt");
        auto pos_input = addComponent<lib::IEStream<PosUpdateMsg>>(
            "pos_input", "./inputs/test_nlm_pos.txt");
        addCoupling(tx_input->out,  nlm->tx_in);
        addCoupling(pos_input->out, nlm->pos_update);
    }
};

#endif
