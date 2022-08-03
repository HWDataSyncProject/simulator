/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DATAGENERATOR_HELPER_H
#define DATAGENERATOR_HELPER_H
#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"

#include "ns3/datagenerator.h"
#include "ns3/devicestate.h"
#include "ns3/waterlevelmatrix.h"

# include <vector>
# include <map>

using namespace std;

namespace ns3 {

/* 
    One Node should contains more than one DataGeneratorHelper(Apps)
    One DataGeneratorHelper (App) contains more than one type of data(DataGenerator)

 */

class DataGeneratorHelper
{
    public:
        DataGeneratorHelper();
        void SetAttribute (std::string name, const AttributeValue &value);

        ApplicationContainer Init(uint32_t app_idx_, std::vector<pair<int, float>> dtype_idxs);
        ApplicationContainer Install(uint32_t node_idx, uint32_t app_idx, std::vector<uint32_t> dtype_idxs, float period_, WaterLevelMatrix* wlm_, DeviceStateManager *dsm);
        ApplicationContainer Get_data_container();
        

        // ApplicationContainer Install (NodeContainer c);
    private:
        ObjectFactory m_factory; //!< Object factory.
        Ptr<DataGenerator> prt_datagenerator; //!< The last created server application
        ApplicationContainer data_container;
        uint32_t app_idx;
};





}

#endif /* DATAGENERATOR_HELPER_H */

