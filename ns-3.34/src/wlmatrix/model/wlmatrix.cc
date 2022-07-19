/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "wlmatrix.h"

namespace ns3 {

WLMatrix::WLMatrix ()
{
  for(uint32_t i = 0; i < 5; i++)
    {
      resp_flag[i] = false;
      init_flag[i] = false;
    }
  is_initialized = false;

  node_number = 5;
  app_number = 20;
  data_type_number = 2;
  node_id = 100;

  WLM = (int64_t***)malloc(sizeof(int64_t**)*node_number);
  for(uint32_t i =0; i < node_number; i++)
    {
      WLM[i] = (int64_t**)malloc(sizeof(int64_t*)*app_number);
      for(uint32_t j = 0; j < app_number; j++)
        {
          WLM[i][j] = (int64_t*)malloc(sizeof(int64_t)*data_type_number);
          for(uint32_t k = 0; k < data_type_number; k++)
            {
              WLM[i][j][k] = 0;
            }
        }
    }
}

WLMatrix::WLMatrix (uint32_t n_num, uint32_t a_num, uint32_t dt_num)
{
  for(uint32_t i = 0; i < 5; i++)
    {
      resp_flag[i] = false;
      init_flag[i] = false;
    }
  is_initialized = false;

  node_number = n_num;
  app_number = a_num;
  data_type_number = dt_num;
  node_id = 100;

  WLM = (int64_t***)malloc(sizeof(int64_t**)*node_number);
  for(uint32_t i =0; i < node_number; i++)
    {
      WLM[i] = (int64_t**)malloc(sizeof(int64_t*)*app_number);
      for(uint32_t j = 0; j < app_number; j++)
        {
          WLM[i][j] = (int64_t*)malloc(sizeof(int64_t)*data_type_number);
          for(uint32_t k = 0; k < data_type_number; k++)
            {
              WLM[i][j][k] = 0;
            }
        }
    }
}

WLMatrix::~WLMatrix ()
{
  for(uint32_t i =0; i < node_number; i++)
    {
      for(uint32_t j = 0; j < app_number; j++)
        {
          free(WLM[i][j]);
        }
      free(WLM[i]);
    }
  free(WLM);
}

bool 
WLMatrix::GetRespFlag ()
{
  for(uint32_t i = 0; i < 5; i++)
    {
      if(resp_flag[i] == true)
        {
          return true;
        }
    }
  return false;
}

void 
WLMatrix::SetRespFlag (uint32_t f_idx, bool f)
{
  resp_flag[f_idx] = f;
}

bool 
WLMatrix::GetInitFlag (uint32_t f_idx)
{
  return init_flag[f_idx];
}

void 
WLMatrix::SetInitFlag (uint32_t f_idx, bool f)
{
  init_flag[f_idx] = f;
}

uint32_t
WLMatrix::GetNodeID ()
{
  return node_id;
}

void
WLMatrix::SetNodeID (uint32_t n_id)
{
  node_id = n_id;
}

int64_t
WLMatrix::GetWLMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx)
{
  return WLM[n_idx][a_idx][dt_idx];
}

void
WLMatrix::SetWLMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx, int64_t ts)
{
  WLM[n_idx][a_idx][dt_idx] = ts;
}

void
WLMatrix::UpdataWLMatrix (uint32_t a_id, uint32_t dt_id, int64_t ts)
{
  if(node_id >= 100)
    {
      WLM[node_number - 1][a_id][dt_id] = ts;
    }
  else
    {
      WLM[node_id][a_id][dt_id] = ts;
    }
}

bool
WLMatrix::GetIsInitialized ()
{
  return is_initialized;
}

void
WLMatrix::SetIsInitialized ()
{
  is_initialized = true;
}

uint32_t
WLMatrix::GetNodeNumber ()
{
  return node_number;
}

uint32_t
WLMatrix::GetAppNumber ()
{
  return app_number;
}

uint32_t
WLMatrix::GetDateTypeNumber ()
{
  return data_type_number;
}

}

