/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef MYAPPS_HELPER_H
#define MYAPPS_HELPER_H

#include "ns3/myapps.h"
#include "datagenerator-helper.h"
#include "ns3/devicestate.h"
#include "ns3/waterlevelmatrix.h"
#include "ns3/wlmreceive.h"
#include "ns3/wlmsend.h"
#include "ns3/object-factory.h"

# include <vector>
# include <map>

using namespace std;

namespace ns3 {

/* ... */

class AppHelper
{
    public:
        AppHelper();
        ~AppHelper();

        void SetAttribute (std::string name, const AttributeValue &value);

        ApplicationContainer Init(uint32_t node_idx_, vector<int> app_idxs, vector<vector<pair<int, float>>> data_type_idxs);
        ApplicationContainer Init(uint32_t node_idx_, map<int, vector<pair<int, float>>> app_info);
        ApplicationContainer Install(Ptr<Node> node);

        void Clear_all_Apps();

        ApplicationContainer InstallApps(uint32_t node_idx_, map<uint32_t, vector<uint32_t>> dtype_idxs, float period_, WaterLevelMatrix* wlm_, DeviceStateManager* dsm);
        void BindNode(Ptr<Node> node);

    private:
        
        ObjectFactory send_factory;
        ObjectFactory receive_factory;
        // Ptr<DataGeneratorHelper> prt_apps;
        ApplicationContainer app_container;
        // ApplicationContainer trans_app_container;
        Ptr<WaterLevelSend> waterlevelmsendapp;
        Ptr<WaterLevelReceive> waterlevelreceiveapp;        
        uint32_t node_idx;

};


}

#endif /* MYAPPS_HELPER_H */

