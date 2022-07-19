/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "hyperterminal.h"

namespace ns3 {

HyperTerminal::HyperTerminal ()
{
  device_count = 0;

  leave_flag = false;
  c_origin = 0;
  c_wlm = 0;

  for(uint32_t i = 0; i < 5; i++)
    {
      for(uint32_t j = 0; j < 20; j++)
        {
          app_install[i][j] = false;
        }
    }
}

HyperTerminal::~HyperTerminal ()
{
}

void 
HyperTerminal::Initialize (Ipv4InterfaceContainer addrs)
{
  for(uint32_t i = 0; i < 5; i++)
    {
      AddDevice(addrs.GetAddress(i,0),2000+i*1000);
    }
  Print();
}

void 
HyperTerminal::AddDevice (Ipv4Address ip, uint16_t port)
{
  TML_node[device_count].addr = ip;
  TML_node[device_count].port = port;
  TML_node[device_count].is_online = false;
  TML_node[device_count].is_awake = false;
  device_count += 1;
}

void 
HyperTerminal::Print (void)
{
  std::cout << "HyperTerminal State:" << std::endl;
  for(uint32_t i = 0; i < 5; i++)
    {
      std::cout << "Node " << i << ": IP " << TML_node[i].addr << "; Port " << TML_node[i].port << "; ";
      if (TML_node[i].is_online)
        {
          std::cout << "is online";
          if (TML_node[i].is_awake)
            {
              std::cout << " and awake.";
            }
          else
            {
              std::cout << " and sleep.";
            }
          std::cout << std::endl;
        }
      else
        {
          std::cout << "is offline." << std::endl;
        }
    }
}

uint32_t 
HyperTerminal::Join (Ipv4Address ip)
{
  uint32_t node_id = GetNodeId (ip);
  TML_node[node_id].is_online = true;
  TML_node[node_id].is_awake = true;
  return node_id;
}

uint32_t 
HyperTerminal::GetNodeId (Ipv4Address ip)
{
  for(uint32_t i = 0; i < device_count; i++)
    {
      if(TML_node[i].addr == ip)
        {
          return i;
        }
    }
  return 9999;
}

void 
HyperTerminal::SetIsAwake (Ipv4Address ip, bool state)
{
  uint32_t node_id = GetNodeId (ip);
  TML_node[node_id].is_awake = state;
}

void 
HyperTerminal::SetIsAwake (uint32_t node_id, bool state)
{
  TML_node[node_id].is_awake = state;
}

void 
HyperTerminal::SetIsOnline (Ipv4Address ip, bool state)
{
  uint32_t node_id = GetNodeId (ip);
  TML_node[node_id].is_online = state;
}

void 
HyperTerminal::SetIsOnline (uint32_t node_id, bool state)
{
  TML_node[node_id].is_online = state;
}

bool 
HyperTerminal::GetIsAwake (Ipv4Address ip)
{
  uint32_t node_id = GetNodeId (ip);
  return TML_node[node_id].is_awake;
}

bool 
HyperTerminal::GetIsAwake (uint32_t node_id)
{
  return TML_node[node_id].is_awake;
}

bool 
HyperTerminal::GetIsOnline (Ipv4Address ip)
{
  uint32_t node_id = GetNodeId (ip);
  return TML_node[node_id].is_online;
}

bool 
HyperTerminal::GetIsOnline (uint32_t node_id)
{
  return TML_node[node_id].is_online;
}

uint32_t 
HyperTerminal::GetDeviceCount ()
{
  return device_count;
}

Ipv4Address 
HyperTerminal::GetAddr (uint32_t node_id)
{
  return TML_node[node_id].addr;
}

uint16_t 
HyperTerminal::GetPort (Ipv4Address ip)
{
  uint32_t node_id = GetNodeId (ip);
  return TML_node[node_id].port;
}

uint16_t 
HyperTerminal::GetPort (uint32_t node_id)
{
  return TML_node[node_id].port;
}

void 
HyperTerminal::SetLeaveFlag (bool state)
{
  leave_flag = state;
}

bool 
HyperTerminal::GetLeaveFlag ()
{
  return leave_flag;
}

void 
HyperTerminal::SetCOrigin (uint32_t c_temp)
{
  c_origin += c_temp;
}

uint32_t 
HyperTerminal::GetCOrigin ()
{
  return c_origin;
}

void 
HyperTerminal::SetCWlm (uint32_t c_temp)
{
  c_wlm += c_temp;
}

uint32_t 
HyperTerminal::GetCWlm ()
{
  return c_wlm;
}

void 
HyperTerminal::SetAppInstall (uint32_t node_id, uint32_t app_id)
{
  app_install[node_id][app_id] = true;
}

bool 
HyperTerminal::GetAppInstall (uint32_t node_id, uint32_t app_id)
{
  return app_install[node_id][app_id];
}

}

