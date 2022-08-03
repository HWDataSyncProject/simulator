/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DEVICE_H
#define DEVICE_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"

#include "ns3/waterlevelmatrix-module.h"


namespace ns3 {

/* ... */

/* 
    One node(in ns3) is a device here, 
    we use device to add the component of water level matrix
*/


class Device
{
    public:
        Device();
        ~Device();
        // static TypeId GetTypeId(void);
        char* GetDeviceName();
        void SetDeviceName(char* deviceName_);

        uint32_t GetNodeID();
        void SetNodeID(uint32_t nodeID_);  

        Ipv4Address GetIpv4Address ();
        void SetIpv4Address (Ipv4Address ipv4address_);
        
        uint16_t GetPort ();
        void SetPort (uint16_t port_);

        Ptr<Node> GetNetworkNode();

        uint32_t AwakeDevice(uint32_t awakeTimes);

        void WaterLevelPull(Device* other);

        
    private:
        char* deviceName;
        uint32_t nodeID;
        Ipv4Address ipv4address;
        uint16_t port;
        WaterLevelMatrix* wlm;
        Ptr<Node> networkNode;
        
        bool is_online; // the device is connected with the network
        bool is_awake; // the device is awaked/active.
};



}

#endif /* DEVICE_H */

