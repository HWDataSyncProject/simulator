#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/stats-module.h"
#include "ns3/wifi-module.h"

#include "ns3/devicestate.h"
#include "ns3/waterlevelmatrix.h"
#include "ns3/device-helper.h"


#include <vector>
#include <map>

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE("HWDataSync");

int main(int argc, char *argv[])
{
    LogComponentEnable ("HWDataSync", LOG_LEVEL_INFO);
    LogComponentEnable ("DataGeneratorHelper", LOG_LEVEL_INFO);
    LogComponentEnable ("AppHelper", LOG_LEVEL_INFO);
    LogComponentEnable ("WaterLevelMatrix", LOG_LEVEL_INFO);
    LogComponentEnable ("WaterLevelMatrixCommunication", LOG_LEVEL_INFO);
    LogComponentEnable ("DataGenerator", LOG_LEVEL_INFO);
    LogComponentEnable ("DeviceStateManager", LOG_LEVEL_INFO);   

    CommandLine cmd;
    int node_num, app_num, awake_num, in_service_num;
    uint32_t dtype_num;
    cmd.AddValue("node_num", "the node number", node_num);
    cmd.AddValue("app_num", "the app number", app_num);
    cmd.AddValue("dtype_num", "the data type number", dtype_num);
    cmd.AddValue("awake_num", "the awaked number", awake_num);
    cmd.AddValue("in_service_num", "the in service number", in_service_num);
    cmd.Parse (argc, argv);


    Time::SetResolution (Time::NS);

    NodeContainer nodes;
    nodes.Create(node_num);

    // using wifi
    WifiHelper wifi;
    WifiMacHelper wifiMac;
    wifiMac.SetType ("ns3::AdhocWifiMac");    

    YansWifiPhyHelper wifiPhy;
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiPhy.SetChannel (wifiChannel.Create ());

    NetDeviceContainer nodeDevices = wifi.Install (wifiPhy, wifiMac, nodes);

    InternetStackHelper internet;
    internet.Install (nodes);

    Ipv4AddressHelper ipAddrs;
    ipAddrs.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = ipAddrs.Assign (nodeDevices);

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    positionAlloc->Add (Vector (0.0, 10.0, 0.0));
    positionAlloc->Add (Vector (0.0, 30.0, 0.0));
    positionAlloc->Add (Vector (20.0, 0.0, 0.0));
    positionAlloc->Add (Vector (20.0, 40.0, 0.0));
    positionAlloc->Add (Vector (30.0, 20.0, 0.0));
    mobility.SetPositionAllocator (positionAlloc);

    mobility.Install (nodes);


    float state_update_period = 3.0;
    float data_generator_period = 1.0;
    float waterlevelmatrix_sync_period = 1.0;

    map<uint32_t, vector<uint32_t>> app_info = {{0, {0, 1, 3}}, {1, {0, 1}}, {2, {0, 4}}, {3, {0, 1, 2}}};

    // random the device state
    DeviceStateManager dsm = DeviceStateManager(node_num, interfaces);
    dsm.CycleRandomDeviceState(state_update_period, awake_num, in_service_num);

    // waterlevelmatrix
    WaterLevelMatrix wlms[node_num];
    DeviceHelper wlmdevices[node_num];

    for(int i=0; i<node_num; i++)
    {
        wlms[i].resizeMatrix(node_num, app_num, dtype_num); 
        wlms[i].SetNodeID(i);

        wlmdevices[i].SetNode(i, nodes.Get(i));
        wlmdevices[i].SetWaterLevelMatrix(&wlms[i]);
        wlmdevices[i].SetDeviceStateManager(&dsm);

        wlmdevices[i].SetDataGeneratorApps(app_info, data_generator_period);
        wlmdevices[i].SetCommunicationApp(waterlevelmatrix_sync_period);

        // wlmdevices[i].BindAllApps(nodes.Get(i));
    }
    
    
    
    Simulator::Stop (Seconds (5.0));
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}

