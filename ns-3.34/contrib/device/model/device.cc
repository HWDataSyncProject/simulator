/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "device.h"

namespace ns3 {

/* ... */

Device::Device()
{   
    deviceName = (char*)"default_device";
}

Device::~Device()
{

}

char* Device::GetDeviceName()
{
    return deviceName;
}

void Device::SetDeviceName(char* deviceName_)
{
    deviceName = deviceName_;
}

uint32_t Device::GetNodeID()
{
    return nodeID;
}

void Device::SetNodeID(uint32_t nodeID_)
{
    nodeID = nodeID_;
}

Ipv4Address Device::GetIpv4Address ()
{
    return ipv4address;
}

void Device::SetIpv4Address(Ipv4Address ipv4address_)
{
    ipv4address = ipv4address_;
}

uint16_t Device::GetPort ()
{
    return port;
}

void Device::SetPort (uint16_t port_)
{
    port = port_;
}

Ptr<Node> Device::GetNetworkNode()
{
    return networkNode;
}



uint32_t Device::AwakeDevice(uint32_t awakeTimes)
{
    if(is_online)
    {
        is_awake = true;
        awakeTimes = awakeTimes + 1;
    }
    return awakeTimes;
}


void Device::WaterLevelPull(Device* other)
{

}



}

