#include "networkmanager.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("NetworkManager");

NetworkManager::NetworkManager()
{
    node_num = 5;
    nodes.Create(node_num);

    ConfigNS3Network();

    InitDeviceStateManager();


}

NetworkManager::~NetworkManager()
{
    delete []wlms;
    delete []wlmdevices;
}

NetworkManager::NetworkManager(int node_num_)
{
    node_num = 5;
    nodes.Create(node_num_);

    ConfigNS3Network();

    InitDeviceStateManager();

}

void NetworkManager::ConfigNS3Network()
{
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
    
    interfaces = ipAddrs.Assign (nodeDevices);

    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    
    for(int i=0; i < node_num; i++)
    {
        positionAlloc->Add (Vector (i * 10.0, (i + 1) * 10.0, 0.0));
    }
    

    mobility.SetPositionAllocator (positionAlloc);

    mobility.Install (nodes);

}

void NetworkManager::InitDeviceStateManager()
{
    dsm = DeviceStateManager(node_num, interfaces);
}

void NetworkManager::InitWaterLevelMatrix(int app_num, int dtype_num)
{
    wlms = new WaterLevelMatrix[node_num];

    for(int i = 0; i < node_num; i++)
    { 
        wlms[i].resizeMatrix(node_num, app_num, dtype_num); 
        wlms[i].SetNodeID(i);
    }
}

void NetworkManager::InitDevices(map<uint32_t, vector<uint32_t>> app_info, float datagenerator_period, float wlm_sync_period)
{
    wlmdevices = new DeviceHelper[node_num];

    for(int i = 0; i < node_num; i++)
    {
        wlmdevices[i].SetNode(i, nodes.Get(i));
        wlmdevices[i].SetWaterLevelMatrix(&wlms[i]);
        wlmdevices[i].SetDeviceStateManager(&dsm);

        wlmdevices[i].SetDataGeneratorApps(app_info, datagenerator_period);
        wlmdevices[i].SetCommunicationApp(wlm_sync_period);

    }

}

void NetworkManager::RequestPull()
{   
    //get the node id which is ready to pull from  device state manager 
    uint32_t device_idx = dsm.RandomDevicePull();
    // if the device id is valid
    if (device_idx != (uint32_t)(node_num + 1))
    {
        std::vector<uint32_t> datasyncdevices = wlmdevices[device_idx].GetDataSyncDevices();
        // TODO 
        if (! datasyncdevices.empty())
        {
            for(uint32_t i = 0;i < datasyncdevices.size(); i++)
            {
                dsm.SetDeviceAwakeForDataSync(datasyncdevices[i]);
                // cout<<datasyncdevices[i]<<endl;
                /*
                    Data Sync ... ...
                */
                wlmdevices[device_idx].UpdateWaterLevelAfterDataSync(datasyncdevices[i]);
            }
            //check the peers' waterlevel 
            CheckLastDataSync(device_idx);
        }

        // baseline: awake the devices which is not awaked
        for(uint32_t i=0; i < (uint32_t)node_num; i++)
        {
            dsm.SetDeviceAwakeBaseline(i);
        }

        dsm.SetPullTimes();

    }

}

void NetworkManager::CheckLastDataSync(uint32_t source_node_idx)
{
    
    uint32_t app_num =  wlmdevices[0].GetWaterLevelMatrixAppNumber();
    uint32_t dtype_num = wlmdevices[0].GetWaterLevelMatrixDataTypeNumber();
    
    bool is_error = false;
    for(int peer_node = 0; peer_node < node_num; peer_node++)
    {
        
        if(wlmdevices[peer_node].GetNodeId() != source_node_idx)
        {
            for(uint32_t i = 0; i < app_num; i++)
            {
                for(uint32_t j = 0; j < dtype_num; j++)
                {

                    int64_t source_device_value = wlmdevices[source_node_idx].GetWaterLevel(i, j);
                    int64_t peer_device_value = wlmdevices[peer_node].GetWaterLevel(i, j);
                    if(source_device_value < peer_device_value)
                    {
                        is_error = true;
                        cout<<"DataSync error in: "<< source_node_idx << "<--pull--"<<peer_node<<endl;
                    }
                }
            }

        }

    }
    if(is_error)
    {
        dsm.SetErrorTimes();
    }

    

}


void NetworkManager::StartSimulation(float runtime, float pull_period, float state_period, int awake_num, int in_service_num)
{
    
    dsm.CycleRandomDeviceState(state_period, awake_num, in_service_num);
    CycleRequestPull(pull_period);
    Simulator::Stop(Seconds (runtime));
    Simulator::Run();
}

void NetworkManager::StopSimulation()
{
    Simulator::Destroy ();

    dsm.SummaryWithErrorRatio();
}

void NetworkManager::CycleRequestPull(float pull_period)
{
    RequestPull();
    Simulator::Schedule(Seconds(pull_period), &NetworkManager::CycleRequestPull, this, pull_period);
}


}