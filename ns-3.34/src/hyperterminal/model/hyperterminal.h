/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef HYPERTERMINAL_H
#define HYPERTERMINAL_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"

#include <iostream>

namespace ns3 {

struct SimpleDevice
{
  Ipv4Address addr;
  uint16_t port;
  bool is_online;
  bool is_awake;
};

class HyperTerminal
{
public:
  HyperTerminal ();
  virtual ~HyperTerminal ();
  void Initialize (Ipv4InterfaceContainer addrs);
  void Print (void);

  uint32_t Join (Ipv4Address ip);
  void SetIsAwake (Ipv4Address ip, bool state);
  void SetIsAwake (uint32_t node_id, bool state);
  void SetIsOnline (Ipv4Address ip, bool state);
  void SetIsOnline (uint32_t node_id, bool state);
  bool GetIsAwake (Ipv4Address ip);
  bool GetIsAwake (uint32_t node_id);
  bool GetIsOnline (Ipv4Address ip);
  bool GetIsOnline (uint32_t node_id);
  uint32_t GetDeviceCount ();

  Ipv4Address GetAddr (uint32_t node_id);
  uint32_t GetNodeId (Ipv4Address ip);
  uint16_t GetPort (Ipv4Address ip);
  uint16_t GetPort (uint32_t node_id);

  void SetLeaveFlag (bool state);
  bool GetLeaveFlag ();
  void SetCOrigin (uint32_t c_temp);
  uint32_t GetCOrigin ();
  void SetCWlm (uint32_t c_temp);
  uint32_t GetCWlm ();

  void SetAppInstall (uint32_t node_id, uint32_t app_id);
  bool GetAppInstall (uint32_t node_id, uint32_t app_id);

private:
  void AddDevice (Ipv4Address ip, uint16_t port);
  //void Print (void);

  SimpleDevice TML_node[5];
  uint32_t device_count;

  bool leave_flag;
  uint32_t c_origin;
  uint32_t c_wlm;

  bool app_install[5][20];
};

}

#endif /* HYPERTERMINAL_H */

