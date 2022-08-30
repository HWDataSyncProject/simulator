#ifndef DEVICE_HELPER_H
#define DEVICE_HELPER_H


#include "ns3/object-factory.h"

#include "datagenerator-helper.h"
#include "ns3/devicestate.h"
#include "ns3/waterlevelmatrix.h"
#include "ns3/wlmcommunication.h"

# include <vector>
# include <map>

using namespace std;

namespace ns3 {

class DeviceHelper
{
    public:
        DeviceHelper();
        ~DeviceHelper();

        void SetAttribute (std::string name, const AttributeValue &value);

        void SetNode(uint32_t node_idx_, Ptr<Node> node_);
        void SetDataGeneratorApps(map<uint32_t, vector<uint32_t>> app_info, float period_);
        void SetCommunicationApp(float period_);

        void SetWaterLevelMatrix(WaterLevelMatrix* wlm_);
        void SetDeviceStateManager(DeviceStateManager* dsm_);

        void BindDataApps(Ptr<Node> node);
        void BindCommApp(Ptr<Node> node);
        void BindAllApps(Ptr<Node> node);

        std::vector<uint32_t> GetDataSyncDevices();
        void UpdateWaterLevelAfterDataSync(uint32_t peer_node_idx);
        int64_t GetWaterLevel(uint32_t app_idx, uint32_t dtype_idx);


        uint32_t GetWaterLevelMatrixAppNumber();
        uint32_t GetWaterLevelMatrixDataTypeNumber();

        uint32_t GetNodeId(); 

    private:
        
        ApplicationContainer data_generator_apps;
        Ptr<WaterLevelMatrixCommunication> comm_app;
        Ptr<Node> node;

        float data_generate_period;
        float communication_period;
        WaterLevelMatrix* wlm;
        DeviceStateManager* dsm;


        uint32_t node_idx;
        
        ObjectFactory comm_object_fac;
};



}



#endif /* DEVICE_HELPER_H */