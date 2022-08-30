/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "myapps-helper.h"
#include "ns3/wlmreceive.h"
#include "ns3/wlmsend.h"


namespace ns3 {

/* ... */

NS_LOG_COMPONENT_DEFINE ("AppHelper");


AppHelper::AppHelper()
{
    NS_LOG_FUNCTION(this);
    // NS_LOG_INFO("Construct");

    
    // NS_LOG_INFO("send Construct");
    send_factory.SetTypeId (WaterLevelSend::GetTypeId ());
    waterlevelmsendapp = send_factory.Create<WaterLevelSend>();

    
    // NS_LOG_INFO("receive Construct");
    receive_factory.SetTypeId (WaterLevelReceive::GetTypeId ());
    waterlevelreceiveapp = receive_factory.Create<WaterLevelReceive>();

    
}

AppHelper::~AppHelper()
{
    NS_LOG_FUNCTION(this);
}

void AppHelper::SetAttribute (std::string name, const AttributeValue &value)
{
    send_factory.Set (name, value);
}

ApplicationContainer AppHelper::Init(uint32_t node_idx_, vector<int> app_idxs, vector<vector<pair<int, float>>> data_type_idxs)
{
    node_idx = node_idx_;

    int app_num = app_idxs.size();

    for(int i = 0; i < app_num; i++)
    {
        int app_idx = app_idxs[i];
        vector<pair<int, float>> app_data_type = data_type_idxs[i];

        DataGeneratorHelper d;
        ApplicationContainer container = d.Init(app_idx, app_data_type);

        app_container.Add(container);

    }
       
    return app_container;
}

ApplicationContainer AppHelper::Init(uint32_t node_idx_, map<int, vector<pair<int, float>>> app_info)
{
    node_idx = node_idx_;
    map<int, vector<pair<int, float>>> temp = app_info;

    for (map<int, vector<pair<int, float>>>::iterator i = temp.begin(); i != temp.end(); i++)
    {
        int app_idx = i->first;
        vector<pair<int, float>> app_data_type = i->second;

        DataGeneratorHelper d;
        ApplicationContainer container = d.Init(app_idx, app_data_type);

        app_container.Add(container);

    }

    return app_container;

}

ApplicationContainer AppHelper::Install(Ptr<Node> node)
{

    for(ApplicationContainer::Iterator i = app_container.Begin(); i != app_container.End(); ++i)
    {
        Ptr<DataGenerator> d = (*i) -> GetObject<DataGenerator>();
        node->AddApplication (d);
        NS_LOG_INFO("node: "<< node_idx << "Install app: " << d-> Get_app_index() << " data type: "<< d -> Get_data_type_index() << "period: "<< d -> GetPeriod());
    }

    return app_container;
}

void AppHelper::Clear_all_Apps()
{
    ApplicationContainer temp;
    app_container = temp;
}

ApplicationContainer AppHelper::InstallApps(uint32_t node_idx_, map<uint32_t, vector<uint32_t>> dtype_idxs, float period_, WaterLevelMatrix* wlm_, DeviceStateManager* dsm_)
{
    node_idx = node_idx_;
   
    for(map<uint32_t, vector<uint32_t>>::iterator i = dtype_idxs.begin(); i != dtype_idxs.end(); i++)
    {
        uint32_t app_idx_ = i -> first;
        vector<uint32_t> dtype_idx = i->second;

        DataGeneratorHelper d;
        ApplicationContainer container = d.Install(node_idx_, app_idx_, dtype_idx, period_, wlm_, dsm_);
        // NS_LOG_INFO("App "<< app_idx_ << "Installed");
        app_container.Add(container);
    }

    // 
    // NS_LOG_INFO("node ");

    waterlevelmsendapp->SetWaterLevelMatrix(wlm_);
    waterlevelmsendapp->SetDeviceStateManager(dsm_);
    waterlevelmsendapp->SetPeriod(period_);
    
    waterlevelreceiveapp->SetWaterLevelMatrix(wlm_);
    waterlevelreceiveapp->SetDeviceStateManager(dsm_);
    waterlevelreceiveapp->SetPeriod(period_);   

    return app_container; 
}

void AppHelper::BindNode(Ptr<Node> node)
{
    for(ApplicationContainer::Iterator i = app_container.Begin(); i != app_container.End(); ++i)
    {
        Ptr<DataGenerator> d = (*i) -> GetObject<DataGenerator>();
        node->AddApplication (d);
        // NS_LOG_INFO("App Installed "<< node_idx << ":" << d-> Get_app_index() << ":"<< d -> Get_data_type_index());
    }
    // bind send and receive to the node
    node->AddApplication(waterlevelmsendapp);
    node->AddApplication(waterlevelreceiveapp);
    // NS_LOG_INFO("APP LOAD SUCCESSFULLY");

}

// void AppHelper::InitCommunicationApp()
// {
//     //TODO
// }


// void AppHelper::AddCommunicationApp()
// {
//     //TODO
// }



}