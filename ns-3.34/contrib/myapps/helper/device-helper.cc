#include "device-helper.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DeviceHelper");

DeviceHelper::DeviceHelper()
{
    NS_LOG_FUNCTION(this);

    
    comm_object_fac.SetTypeId(WaterLevelMatrixCommunication::GetTypeId());
    comm_app = comm_object_fac.Create<WaterLevelMatrixCommunication>();

}

DeviceHelper::~DeviceHelper()
{
    NS_LOG_FUNCTION(this);
}

void DeviceHelper::SetAttribute (std::string name, const AttributeValue &value)
{
    comm_object_fac.Set (name, value);
}

void DeviceHelper::SetNode(uint32_t node_idx_, Ptr<Node> node_)
{
    node_idx = node_idx_;
    node = node_;
}

void DeviceHelper::SetDataGeneratorApps(map<uint32_t, vector<uint32_t>> app_info, float period_)
{
    // node_idx = node_idx_;
    data_generate_period = period_;

    for(map<uint32_t, vector<uint32_t>>::iterator i = app_info.begin(); i != app_info.end(); i++)
    {
        uint32_t app_idx_ = i -> first;
        vector<uint32_t> dtype_idxs = i->second;

        DataGeneratorHelper d;
        ApplicationContainer container = d.Install(node_idx, app_idx_, dtype_idxs, data_generate_period, wlm, dsm);
        // NS_LOG_INFO("App "<< app_idx_ << "Installed");
        data_generator_apps.Add(container);
    }

    data_generator_apps.Start(Seconds (0.25));

    BindDataApps(node);

}

void DeviceHelper::SetCommunicationApp(float period_)
{
    // two app: sender and receiver

    communication_period = period_;

    comm_app->SetWaterLevelMatrix(wlm);
    comm_app->SetDeviceStateManager(dsm);
    comm_app->SetSendPeriod(communication_period);

    comm_app->SetStartTime (Seconds (0.5));

    BindCommApp(node);

}

void DeviceHelper::SetWaterLevelMatrix(WaterLevelMatrix* wlm_)
{
    wlm = wlm_;
}

void DeviceHelper::SetDeviceStateManager(DeviceStateManager* dsm_)
{
    dsm = dsm_;
}

void DeviceHelper::BindDataApps(Ptr<Node> node)
{
    // install the app into the node
    for(ApplicationContainer::Iterator i = data_generator_apps.Begin(); i != data_generator_apps.End(); ++i)
    {
        Ptr<DataGenerator> d = (*i) -> GetObject<DataGenerator>();
        node->AddApplication (d);
    }
}

void DeviceHelper::BindCommApp(Ptr<Node> node)
{
    node->AddApplication(comm_app);
}

void DeviceHelper::BindAllApps(Ptr<Node> node)
{
    BindDataApps(node);
    BindCommApp(node);
}

std::vector<uint32_t> DeviceHelper::GetDataSyncDevices()
{
    
    std::vector<uint32_t> datasyncdevices;
    uint32_t node_num = wlm->GetNodeNumber();
    for(uint32_t i = 0; i < node_num; i++)
    {
        if (wlm->CheckPeerWaterLevel(i))
        {
            datasyncdevices.push_back(i);
        }
    }

    return datasyncdevices;
}

void DeviceHelper::UpdateWaterLevelAfterDataSync(uint32_t peer_node_idx)
{
    wlm->UpdataCurrentWaterLevelByPeer(peer_node_idx);
}

int64_t DeviceHelper::GetWaterLevel(uint32_t app_idx, uint32_t dtype_idx)
{
    return wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx);
}

uint32_t DeviceHelper::GetWaterLevelMatrixAppNumber()
{
    return wlm->GetAppNumber();
}

uint32_t DeviceHelper::GetWaterLevelMatrixDataTypeNumber()
{
    return wlm->GetDateTypeNumber();
}

uint32_t DeviceHelper::GetNodeId()
{
    return node_idx;
}

}