/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/string.h"

#include "datagenerator-helper.h"
using namespace std;


namespace ns3 {

/* ... */
NS_LOG_COMPONENT_DEFINE ("DataGeneratorHelper");


DataGeneratorHelper::DataGeneratorHelper()
{
    NS_LOG_FUNCTION(this);
    m_factory.SetTypeId (DataGenerator::GetTypeId ());
}

void DataGeneratorHelper::SetAttribute(std::string name, const AttributeValue &value)
{
    m_factory.Set (name, value);
}

ApplicationContainer DataGeneratorHelper::Init(uint32_t app_idx_, std::vector<pair<int, float>> dtype_idxs)
{
    app_idx = app_idx_;
    for(uint32_t i = 0; i < dtype_idxs.size();i++)
    {
        prt_datagenerator = m_factory.Create<DataGenerator> ();
        prt_datagenerator->Set_app_index(app_idx);
        prt_datagenerator->Set_data_type_index(dtype_idxs[i].first);
        prt_datagenerator->SetPeriod(dtype_idxs[i].second);
        data_container.Add(prt_datagenerator);

    }

    return data_container;
}

ApplicationContainer DataGeneratorHelper::Install(uint32_t node_idx_, uint32_t app_idx_, std::vector<uint32_t> dtype_idxs, float period_, WaterLevelMatrix* wlm_, DeviceStateManager* dsm)
{
    for(uint32_t i = 0; i < dtype_idxs.size();i++)
    {
        prt_datagenerator = m_factory.Create<DataGenerator> ();

        prt_datagenerator->SetNodeIndex(node_idx_);
        prt_datagenerator->Set_app_index(app_idx_);
        prt_datagenerator->Set_data_type_index(dtype_idxs[i]);
        prt_datagenerator->SetPeriod(period_);
        prt_datagenerator->SetWaterLevelMatrix(wlm_);
        prt_datagenerator->SetDeviceStateManager(dsm);
        data_container.Add(prt_datagenerator);
    }

    return data_container;    
}


ApplicationContainer DataGeneratorHelper::Get_data_container()
{
    return data_container;
}

// ApplicationContainer DataGeneratorHelper::Install(NodeContainer c)
// {
//     ApplicationContainer apps;
//     for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
//         {
//             Ptr<Node> node = *i;

//             prt_datagenerator = m_factory.Create<DataGenerator> ();
//             node->AddApplication (prt_datagenerator);
//             apps.Add(prt_datagenerator);

//         }
//     return apps;

// }






}

