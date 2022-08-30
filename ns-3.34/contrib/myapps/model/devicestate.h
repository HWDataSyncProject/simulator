/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DEVICESTATE_H
#define DEVICESTATE_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-address.h"


#include <vector>
#include <map>
using namespace std;


namespace ns3 {

/* ... */

struct DeviceState
{  
    Ipv4Address ipv4address;
    uint16_t port;
    bool is_online; // the device is connected to the network.
    bool is_awake; 
    uint32_t node_idx; 
    int awake_times;
    bool is_in_service; // if true, current device performs "pull"
    int baseline_awake_times;
    int awake_times_waterlevelmatrix;
    int awake_times_datasync;
};

class DeviceStateManager
{
    public:
        DeviceStateManager();
        DeviceStateManager(uint32_t device_num, Ipv4InterfaceContainer addrs);
        ~DeviceStateManager();

        void AddDevice(DeviceState devicestate_);
        std::vector<struct DeviceState> GetCurrentAwakeDevices();
        std::vector<struct DeviceState> GetCurrentOnlineDevices();
        std::vector<struct DeviceState> GetAllDevices();

        std::vector<uint32_t> GetCurrentAwakeDevicesID();
        std::vector<uint32_t> GetCurrentOnlineDevicesID();
        std::vector<uint32_t> GetAllDevicesID();

        void SetPullTimes();
        int GetPullTimes();
        void SetErrorTimes();
        int GetErrorTimes();

        int GetNodeNum();
        uint16_t GetNodePort(uint32_t node_idx_);
        Ipv4Address GetNodeIpv4Address(uint32_t node_idx_);
        bool GetOnline(uint32_t node_idx_);
        bool GetIsInService(uint32_t node_idx_);
        bool GetIsAwake(uint32_t node_idx_);


        void SetDeviceAwake(uint32_t node_idx_);
        void SetDeviceAwakeBaseline(uint32_t node_idx_);

        void SetDeviceAwakeForWaterLevelMatrix(uint32_t node_idx_);
        void SetDeviceAwakeForWaterLevelMatrixFromMultiDevices(uint32_t node_idx_, uint32_t source_node_idx);

        void SetDeviceAwakeForDataSync(uint32_t node_idx_);
        void UpdateDeviceAwakeTimes(uint32_t node_idx_);

        


        void RandomDeviceState(int awake_num, int in_service_num);

        uint32_t RandomDevicePull();
        
        void CycleRandomDeviceState(float cycle_period, int awake_num, int in_service_num);

        void PrintCurrentDeviceStates();
        void PrintCurrentDeviceStatesSimple();

        void Draw_line(int columns, int columns_width);

        void Summary();
        void SummaryWithErrorRatio();

    private:
        
        uint32_t nodeNum;
        // std::vector<struct DeviceState> devicestates;
        std::map<uint32_t, struct DeviceState> devicestates;

        std::map<uint32_t, vector<int>> recv_wl;

        int pull_times;
        int error_times;

};



}

#endif /* DEVICESTATE_H */

