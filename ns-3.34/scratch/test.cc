

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/stats-module.h"
#include "ns3/wifi-module.h"


#include "ns3/myapps-helper.h"
#include "ns3/datagenerator.h"
#include "ns3/device.h"

#include <vector>
#include <map>

using namespace std;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MainTest");

int main(int argc, char *argv[])
{
    CommandLine cmd;
    int node_num, dtype_num;   
    cmd.AddValue("node_num", "the node number", node_num);
    cmd.AddValue("dtype_num", "the datype number", dtype_num);
    cmd.Parse (argc, argv);

    Time::SetResolution (Time::NS);

    LogComponentEnable ("DataGenerator", LOG_LEVEL_INFO);
    LogComponentEnable ("DataGeneratorHelper", LOG_LEVEL_INFO);
    LogComponentEnable ("AppHelper", LOG_LEVEL_INFO);
    LogComponentEnable ("MainTest", LOG_LEVEL_INFO);

    std::vector<int> dtype_num_idxs{0, 1, 5};
    

    // NS_LOG_INFO ("Init DataGeneratorHelper");

    // DataGeneratorHelper data_generator_helper;
    // ApplicationContainer apps = data_generator_helper.Init(0, dtype_num_idxs);

    Device d;
    NS_LOG_INFO("name " << d.GetDeviceName());

    // ApplicationContainer::Iterator i;
    // for (i = apps.Begin(); i != apps.End(); ++i)
    // {
    //     Ptr<DataGenerator> d = (*i) -> GetObject<DataGenerator>();
    //     NS_LOG_INFO("App idx" << d-> Get_app_index() << "Data type" << d -> Get_data_type_index());
    //     NS_LOG_INFO("path" << d-> Get_folder_path());
    
    // }

    NodeContainer nodes;
    nodes.Create(5);

    // Ptr<Node> node_0 = nodes.Get(0);

    // map<int, vector<pair<int, float>>> node_app;
    // vector<pair<int, float>> app_dtypes{{0, 2.1}, {1, 2.2}, {2, 2.5}, {5, 5.2}};
    // // vector<float> periods{2.0, 2.0, 2.0};
    // node_app[0] = app_dtypes;

    // AppHelper app_helper;


    // // vector<int> app_idxs{100, 102, 103};
    // // vector<vector<int>> data_type_idxs{{0,1,2}, {2,3,4}, {0,1,2,4}};
    // // app_helper.Init(0, app_idxs, data_type_idxs);
    // // app_helper.Install(node_0);

    // app_helper.Clear_all_Apps();

    // app_helper.Init(1, node_app);
    // app_helper.Install(nodes.Get(1));



    return 0;
}
