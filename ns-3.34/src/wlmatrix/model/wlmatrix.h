/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WLMATRIX_H
#define WLMATRIX_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"

#include <iostream>

namespace ns3 {

class WLMatrix
{
public:
  WLMatrix ();
  virtual ~WLMatrix ();
  WLMatrix (uint32_t n_num, uint32_t a_num, uint32_t dt_num);

  bool GetRespFlag ();
  void SetRespFlag (uint32_t f_idx, bool f);
  bool GetInitFlag (uint32_t f_idx);
  void SetInitFlag (uint32_t f_idx, bool f);

  uint32_t GetNodeID ();
  void SetNodeID (uint32_t n_id);

  int64_t GetWLMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx);
  void SetWLMatrix (uint32_t n_idx, uint32_t a_idx, uint32_t dt_idx, int64_t ts);
  void UpdataWLMatrix (uint32_t a_id, uint32_t dt_id, int64_t ts);

  bool GetIsInitialized ();
  void SetIsInitialized ();

  uint32_t GetNodeNumber ();
  uint32_t GetAppNumber ();
  uint32_t GetDateTypeNumber ();

private:
  bool resp_flag[5];
  bool init_flag[5];
  bool is_initialized; //!< Flag indicating whether it has been initialized

  uint32_t node_number;
  uint32_t app_number;
  uint32_t data_type_number;
  uint32_t node_id;
  int64_t ***WLM;
};

}

#endif /* WLMATRIX_H */

