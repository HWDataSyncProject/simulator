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
#include "ns3/networkmanager.h"


#include <vector>
#include <map>

using namespace std;
using namespace ns3;


NS_LOG_COMPONENT_DEFINE("Demo");

int main(int argc, char *argv[])
{
    
    LogComponentEnable ("Demo", LOG_LEVEL_INFO);
    LogComponentEnable ("DataGenerator", LOG_LEVEL_INFO);
    LogComponentEnable ("DataGeneratorHelper", LOG_LEVEL_INFO);
    LogComponentEnable ("AppHelper", LOG_LEVEL_INFO);
    LogComponentEnable ("WaterLevelMatrix", LOG_LEVEL_INFO);
    LogComponentEnable ("WaterLevelMatrixCommunication", LOG_LEVEL_INFO);
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

    float state_update_period = 15.0;
    float data_generator_period = 1.0;
    float waterlevelmatrix_sync_period = 2.0;
    float pull_period = 1.0;
    float run_time = 100.0;

    map<uint32_t, vector<uint32_t>> app_info = {{0, {0, 1, 3}}, {1, {0, 1}}, {2, {0, 4}}, {3, {0, 1, 2}}};

    NetworkManager manager = NetworkManager(node_num);
    manager.InitWaterLevelMatrix(app_num, dtype_num);
    manager.InitDevices(app_info, data_generator_period, waterlevelmatrix_sync_period);


    manager.StartSimulation(run_time, pull_period, state_update_period, awake_num, in_service_num);
    manager.StopSimulation();

    return 0;
}

