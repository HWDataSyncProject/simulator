/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "waterlevelmatrix.h"

namespace ns3 {

/* ... */
NS_LOG_COMPONENT_DEFINE ("WaterLevelMatrix");

// NS_OBJECT_ENSURE_REGISTERED (WaterLevelMatrix);

WaterLevelMatrix::WaterLevelMatrix ()
{
    NS_LOG_FUNCTION(this);

    node_number = 5;
    app_number = 20;
    data_type_number = 2;
    node_id = 100;
    is_modified = false;

    matrix = NULL;

}

WaterLevelMatrix::WaterLevelMatrix (uint32_t n_num, uint32_t a_num, uint32_t dt_num)
{
    NS_LOG_FUNCTION(this);


    node_number = n_num;
    app_number = a_num;
    data_type_number = dt_num;
    node_id = 100;
    is_modified = false;

    matrix = NULL;
}

WaterLevelMatrix::~WaterLevelMatrix ()
{
    for(uint32_t i=0; i<node_number; i++)
    {
        for(uint32_t j=0; j<app_number; j++)
        {
            delete []matrix[i][j];
        }   
        delete []matrix[i];
    }
    delete []matrix;
    matrix = NULL;

}

void WaterLevelMatrix::resizeMatrix(uint32_t deviceNum, uint32_t appNum, uint32_t dataTypeNum)
{
    // if the matrix is not null, free the matrix and then build a new matrix
    if (matrix != NULL)
    {
        for(uint32_t i=0; i<node_number; i++)
        {
            for(uint32_t j=0; j<app_number; j++)
            {
                delete []matrix[i][j];
            }   
            delete []matrix[i];
        }
        delete []matrix;
        matrix = NULL;       
    }

    // build new matrix
    if (matrix == NULL)
    {
        node_number = deviceNum;
        app_number = appNum;
        data_type_number = dataTypeNum;

        matrix = new int64_t** [deviceNum];
        for(uint32_t i=0; i<node_number; i++)
        {
            matrix[i] = new int64_t*[appNum];
            for(uint32_t j=0; j<appNum; j++)
            {
                matrix[i][j] = new int64_t[dataTypeNum];
                for(uint32_t k=0; k<dataTypeNum; k++)
                {
                    matrix[i][j][k] = 0;
                }
            }
        }
    }
}

uint32_t WaterLevelMatrix::GetNodeID ()
{
    return node_id;
}

void WaterLevelMatrix::SetNodeID (uint32_t n_id)
{
    node_id = n_id;
}

int64_t WaterLevelMatrix::GetWaterLevelMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx)
{
    return matrix[n_idx][a_idx][dt_idx];
}

void WaterLevelMatrix::SetWaterLevelMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx, int64_t ts)
{
    matrix[n_idx][a_idx][dt_idx] = ts;
}

void WaterLevelMatrix::UpdataWaterLevelMatrix (uint32_t a_id, uint32_t dt_id, int64_t ts)
{
    matrix[node_id][a_id][dt_id] = ts;
}


uint32_t WaterLevelMatrix::GetNodeNumber ()
{
    return node_number;
}

uint32_t WaterLevelMatrix::GetAppNumber ()
{
    return app_number;
}

uint32_t WaterLevelMatrix::GetDateTypeNumber ()
{
    return data_type_number;
}

bool WaterLevelMatrix::GetIsModified()
{
    return is_modified;
}

void WaterLevelMatrix::SetIsModified(bool is_modified_)
{
    is_modified = is_modified_;
}


bool WaterLevelMatrix::CheckPeerWaterLevel(uint32_t peer_node_idx)
{
    bool awake_peer = false;
    if(peer_node_idx != node_id)
    {
        for(uint32_t i = 0; i < app_number; i++)
        {
            for(uint32_t j = 0; j < data_type_number; j++)
            {
                int64_t current_value = matrix[node_id][i][j];
                int64_t peer_value = matrix[peer_node_idx][i][j];

                if(current_value < peer_value)
                {
                    awake_peer = true;
                    return awake_peer;
                }
            }
        }
    }
    return false;

}

void WaterLevelMatrix::UpdataCurrentWaterLevelByPeer(uint32_t peer_node_idx)
{

    if(peer_node_idx != node_id)
    {
        for(uint32_t i = 0; i < app_number; i++)
        {
            for(uint32_t j = 0; j < data_type_number; j++)
            {
                int64_t current_value = matrix[node_id][i][j];
                int64_t peer_value = matrix[peer_node_idx][i][j];

                if(current_value < peer_value)
                {
                    matrix[node_id][i][j] = peer_value;
                }
            }
        }
    }

}

// std::vector<uint32_t> WaterLevelMatrix::GetAwakeDevices()
// {
//     for(uint32_t i=0; i<node_number; i++)
//     {
        
//     }
// }


// bool WaterLevelMatrix::GetRespFlag ()
// {
//   for(uint32_t i = 0; i < 5; i++)
//     {
//       if(resp_flag[i] == true)
//         {
//           return true;
//         }
//     }
//   return false;
// }

// void WaterLevelMatrix::SetRespFlag (uint32_t f_idx, bool f)
// {
//   resp_flag[f_idx] = f;
// }

// bool WaterLevelMatrix::GetInitFlag (uint32_t f_idx)
// {
//   return init_flag[f_idx];
// }

// void WaterLevelMatrix::SetInitFlag (uint32_t f_idx, bool f)
// {
//   init_flag[f_idx] = f;
// }


// bool WaterLevelMatrix::GetIsInitialized ()
// {
//   return is_initialized;
// }

// void WaterLevelMatrix::SetIsInitialized ()
// {
//   is_initialized = true;
// }

}

