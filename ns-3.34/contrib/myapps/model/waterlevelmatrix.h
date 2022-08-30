/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WATERLEVELMATRIX_H
#define WATERLEVELMATRIX_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"

#include <vector>
using namespace std;

namespace ns3 {

/* ... */

// One device contains a water level matrix

class WaterLevelMatrix
{
    public:
        WaterLevelMatrix();
        virtual ~WaterLevelMatrix();
        WaterLevelMatrix(uint32_t deviceNum, uint32_t appNum, uint32_t dataTypeNum);   
        
        void resizeMatrix(uint32_t deviceNum, uint32_t appNum, uint32_t dataTypeNum);

        uint32_t GetNodeID ();
        void SetNodeID (uint32_t n_id);

        int64_t GetWaterLevelMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx);
        void SetWaterLevelMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx, int64_t ts);
        void UpdataWaterLevelMatrix (uint32_t a_id, uint32_t dt_id, int64_t ts);

        uint32_t GetNodeNumber ();
        uint32_t GetAppNumber ();
        uint32_t GetDateTypeNumber ();
        
        bool GetIsModified();
        void SetIsModified(bool is_modified_);

        bool CheckPeerWaterLevel(uint32_t peer_node_idx);
        void UpdataCurrentWaterLevelByPeer(uint32_t peer_node_idx);

        // std::vector<uint32_t> GetAwakeDevices();


        // bool GetRespFlag ();
        // void SetRespFlag (uint32_t f_idx, bool f);
        // bool GetInitFlag (uint32_t f_idx);
        // void SetInitFlag (uint32_t f_idx, bool f);
        // bool GetIsInitialized ();
        // void SetIsInitialized ();

private:
        // bool resp_flag[5];
        // bool init_flag[5];
        // bool is_initialized; //!< Flag indicating whether it has been initialized

        bool is_modified;

        uint32_t    node_number;
        uint32_t    app_number;
        uint32_t    data_type_number;
        uint32_t    node_id;
        int64_t***  matrix;       

};

}

#endif /* WATERLEVELMATRIX_H */

