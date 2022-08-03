/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WATERLEVELMATRIX_H
#define WATERLEVELMATRIX_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"


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
        bool GetRespFlag ();
        void SetRespFlag (uint32_t f_idx, bool f);
        bool GetInitFlag (uint32_t f_idx);
        void SetInitFlag (uint32_t f_idx, bool f);

        uint32_t GetNodeID ();
        void SetNodeID (uint32_t n_id);

        int64_t GetWaterLevelMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx);
        void SetWaterLevelMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx, int64_t ts);
        void UpdataWaterLevelMatrix (uint32_t a_id, uint32_t dt_id, int64_t ts);

        bool GetIsInitialized ();
        void SetIsInitialized ();

        uint32_t GetNodeNumber ();
        uint32_t GetAppNumber ();
        uint32_t GetDateTypeNumber ();

private:
        bool resp_flag[5];
        bool init_flag[5];
        bool is_initialized; //!< Flag indicating whether it has been initialized

        uint32_t    node_number;
        uint32_t    app_number;
        uint32_t    data_type_number;
        uint32_t    node_id;
        int64_t***  matrix;       

};

}

#endif /* WATERLEVELMATRIX_H */

