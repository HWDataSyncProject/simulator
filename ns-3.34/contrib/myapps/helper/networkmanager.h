#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"

#include "ns3/devicestate.h"
#include "ns3/waterlevelmatrix.h"
#include "ns3/device-helper.h"

#include <map>
#include <vector>

using namespace std;


namespace ns3 {

class NetworkManager
{
    public:
        NetworkManager();
        ~NetworkManager();
        NetworkManager(int node_num_);

        void ConfigNS3Network();
        void InitDeviceStateManager();
        
        void InitWaterLevelMatrix(int app_num, int dtype_num);
        void InitDevices(map<uint32_t, vector<uint32_t>> app_info, float datagenerator_period, float wlm_sync_period);

        void RequestPull();
        void CheckLastDataSync(uint32_t source_node_idx);

        void CycleRequestPull(float pull_period);


        void StartSimulation(float runtime, float pull_period, float state_period, int awake_num, int in_service_num);
        void StopSimulation();

    private:
        DeviceStateManager dsm;
        WaterLevelMatrix* wlms;
        DeviceHelper* wlmdevices;
        NodeContainer nodes;
        Ipv4InterfaceContainer interfaces;
        int node_num;

};


}

#endif /* NETWORKMANAGER_H */
