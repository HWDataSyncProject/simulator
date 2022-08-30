
#include "ns3/ipv4-address.h"

#include "devicestate.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;


namespace ns3 {


NS_LOG_COMPONENT_DEFINE ("DeviceStateManager");

// NS_OBJECT_ENSURE_REGISTERED (DeviceStateManager);

DeviceStateManager::DeviceStateManager()
{
    NS_LOG_FUNCTION(this);
    nodeNum = 0;
    pull_times = 0;
    error_times = 0;



}

DeviceStateManager::DeviceStateManager(uint32_t device_num, Ipv4InterfaceContainer addrs)
{
    NS_LOG_FUNCTION(this);
    nodeNum = device_num;
    for(uint32_t i=0; i<device_num; i++)
    {
        DeviceState temp = {
            .ipv4address = addrs.GetAddress(i, 0),
            .port = (uint16_t)(2000 + i),
            .is_online = true,
            .is_awake = false,
            .node_idx = i,
            .awake_times = 0,
            .is_in_service = false,
            .baseline_awake_times = 0,
            .awake_times_waterlevelmatrix = 0,
            .awake_times_datasync = 0
        };

        devicestates.insert(pair<uint32_t, struct DeviceState>(i, temp));
        

        // init recv_wl
        std::vector<int> temp_recv_wlm(device_num, 0);
        recv_wl.insert(pair<uint32_t, std::vector<int>>(i, temp_recv_wlm));
    
    }

    pull_times = 0;
    error_times = 0;

}



DeviceStateManager::~DeviceStateManager()
{


}

void DeviceStateManager::AddDevice(DeviceState devicestate_)
{
    devicestates.insert(std::pair<int, DeviceState>(devicestate_.node_idx, devicestate_));

    nodeNum ++;
}

std::vector<struct DeviceState> DeviceStateManager::GetCurrentAwakeDevices()
{
    std::vector<struct DeviceState> temp;
    for(std::map<uint32_t, struct DeviceState>::iterator i=devicestates.begin(); i != devicestates.end(); i++)
    {

        if (i->second.is_online && i->second.is_awake)
        {
            temp.push_back(i->second);
        }
    }
    return temp;
}

std::vector<struct DeviceState> DeviceStateManager::GetCurrentOnlineDevices()
{
    std::vector<struct DeviceState> temp;
    for(std::map<uint32_t, struct DeviceState>::iterator i=devicestates.begin(); i != devicestates.end(); i++)
    {

        if (i->second.is_online)
        {
            temp.push_back(i->second);
        }
    }
    return temp;
}


std::vector<struct DeviceState> DeviceStateManager::GetAllDevices()
{
    std::vector<struct DeviceState> temp;
    for(std::map<uint32_t, struct DeviceState>::iterator i=devicestates.begin(); i != devicestates.end(); i++)
    {
        temp.push_back(i->second);      
    }
    return temp;
}

std::vector<uint32_t> DeviceStateManager::GetCurrentAwakeDevicesID()
{
    std::vector<uint32_t> temp;
    for(std::map<uint32_t, struct DeviceState>::iterator i=devicestates.begin(); i != devicestates.end(); i++)
    {
        if (i->second.is_online && i->second.is_awake)
        {
            temp.push_back(i->first);
        }
    }
    return temp;

}

std::vector<uint32_t> DeviceStateManager::GetCurrentOnlineDevicesID()
{
    std::vector<uint32_t> temp;
    for(std::map<uint32_t, struct DeviceState>::iterator i=devicestates.begin(); i != devicestates.end(); i++)
    {

        if (i->second.is_online)
        {
            temp.push_back(i->first);
        }
    }
    return temp;

}

std::vector<uint32_t> DeviceStateManager::GetAllDevicesID()
{
    std::vector<uint32_t> temp;
    for(std::map<uint32_t, struct DeviceState>::iterator i=devicestates.begin(); i != devicestates.end(); i++)
    {
        temp.push_back(i->first);
    }
    return temp;
}

void DeviceStateManager::SetPullTimes()
{
    pull_times += 1;
}

int DeviceStateManager::GetPullTimes()
{
    return pull_times;
}

void DeviceStateManager::SetErrorTimes()
{
    error_times += 1;
}

int DeviceStateManager::GetErrorTimes()
{
    return error_times;
}




int DeviceStateManager::GetNodeNum()
{
    return (int)nodeNum;
}

uint16_t DeviceStateManager::GetNodePort(uint32_t node_idx)
{
    // index is consist with the id;

    return devicestates[node_idx].port;

}

Ipv4Address DeviceStateManager::GetNodeIpv4Address(uint32_t node_idx)
{
    // index is consist with the id;

    return devicestates[node_idx].ipv4address;

}

bool DeviceStateManager::GetOnline(uint32_t node_idx)
{
    return devicestates[node_idx].is_online;
}

bool DeviceStateManager::GetIsInService(uint32_t node_idx)
{
    return devicestates[node_idx].is_in_service;
}

bool DeviceStateManager::GetIsAwake(uint32_t node_idx)
{
    return devicestates[node_idx].is_awake;
}


void DeviceStateManager::SetDeviceAwake(uint32_t node_idx)
{
    if(devicestates[node_idx].is_online)
    {
        if(devicestates[node_idx].is_awake)
        {

        }
        else
        {
            /* 
            omit the state changes, the state "is_awake" should be change to "true"
            then the state "is_awake" should be change to "false" after the responsing or some other actions. 
            */
            devicestates[node_idx].awake_times += 1;
            NS_LOG_INFO("Time: "<< (int)(Simulator::Now ()).GetSeconds () <<": Device "<< node_idx << " --> awaked");
        }

    }

}

void DeviceStateManager::SetDeviceAwakeBaseline(uint32_t node_idx_)
{
    if(devicestates[node_idx_].is_online)
    {
        if(devicestates[node_idx_].is_awake)
        {

        }
        else
        {
            devicestates[node_idx_].baseline_awake_times += 1;
        }
    }
    
}

void DeviceStateManager::SetDeviceAwakeForWaterLevelMatrix(uint32_t node_idx_)
{
    if(devicestates[node_idx_].is_online)
    {
        if(devicestates[node_idx_].is_awake)
        {

        }
        else
        {
            devicestates[node_idx_].awake_times_waterlevelmatrix += 1;
        }
    } 
    UpdateDeviceAwakeTimes(node_idx_);
}

void DeviceStateManager::SetDeviceAwakeForWaterLevelMatrixFromMultiDevices(uint32_t node_idx_, uint32_t source_node_idx)
{
    //TODO
    recv_wl[node_idx_][source_node_idx] = 1;
    
    bool all_recv = true;
    std::vector<uint32_t> online_devices = GetCurrentAwakeDevicesID();
    for(uint32_t i=0; i < online_devices.size(); i++)
    {

        if(recv_wl[node_idx_][online_devices[i]] == 0)
        {
            all_recv = false;
            break;
        }
        
    }

    // cout<<"Device " << node_idx_ << ": ";
    // for(uint32_t i = 0; i < recv_wl[node_idx_].size(); i++)
    // {
    //     cout<<recv_wl[node_idx_][i]<< " ";
    // }
    // cout << boolalpha << all_recv <<endl;

    if(all_recv)
    {
        SetDeviceAwakeForWaterLevelMatrix(node_idx_);
        std::vector<int> temp(nodeNum, 0);
        recv_wl[node_idx_] = temp;
    }

}


void DeviceStateManager::SetDeviceAwakeForDataSync(uint32_t node_idx_)
{
    if(devicestates[node_idx_].is_online)
    {
        if(devicestates[node_idx_].is_awake)
        {

        }
        else
        {
            devicestates[node_idx_].awake_times_datasync += 1;
        }
    }
    UpdateDeviceAwakeTimes(node_idx_);
}

void DeviceStateManager::UpdateDeviceAwakeTimes(uint32_t node_idx_)
{
    devicestates[node_idx_].awake_times = devicestates[node_idx_].awake_times_datasync + devicestates[node_idx_].awake_times_waterlevelmatrix;
}


void DeviceStateManager::RandomDeviceState(int awake_num, int in_service_num)
{
    // only random the online devices
    std::vector<uint32_t> online_devices = GetCurrentOnlineDevicesID();

    // reset the online devices first
    for (int i=0; i<(int)online_devices.size(); i++)
    {
        devicestates[online_devices[i]].is_awake = false;
        devicestates[online_devices[i]].is_in_service = false;
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(online_devices.begin(), online_devices.end(), std::default_random_engine(seed));

    if (awake_num > (int)online_devices.size())
    {
        awake_num = (int)online_devices.size();
        std::cout<<"The given awake_num should be smaller than current devices number."<<std::endl;
    }

    for (int i=0; i < awake_num; i++)
    {
        devicestates[online_devices[i]].is_awake = true;
    }

    // random the in service state
    if(in_service_num > awake_num)
    {
        in_service_num = 1;
    }
    for (int i=0; i<in_service_num; i++)
    {
        devicestates[online_devices[i]].is_in_service = true;
    }

}

uint32_t DeviceStateManager::RandomDevicePull()
{
    //TODO
    // randomly select some devices which want to pull, the number is limited by awake_num;
    
    //only the in_service device will pull
    for (uint32_t i=0; i < nodeNum; i++)
    {
        if (devicestates[i].is_in_service == true)
        {
            return i;
        }
    }
    return nodeNum + 1;

}

void DeviceStateManager::CycleRandomDeviceState(float cycle_period, int awake_num, int in_service_num)
{
    
    RandomDeviceState(awake_num, in_service_num);
    
    // PrintCurrentDeviceStates();
    PrintCurrentDeviceStatesSimple();

    Simulator::Schedule(Seconds(cycle_period), &DeviceStateManager::CycleRandomDeviceState, this, cycle_period, awake_num, in_service_num);
}

void DeviceStateManager::PrintCurrentDeviceStates()
{
    int state_num = 8;
    int column_width = 10;
    vector<std::string> header{"DeviceName", "IP", "Port", "isOnline", "isAwake", "InService", "AwakeTimes", "Baseline"};
    Draw_line(state_num, column_width);
    
    std::cout << "|"  << setw(3 * column_width)  << "Time: " << fixed << setprecision(3) << (Simulator::Now ()).GetSeconds () << ": Device State" << setw(5 * column_width - 3) << setiosflags(ios::right) << setfill(' ') << "|" << std::endl;

    Draw_line(state_num, column_width);
    // header
    for (int i = 0; i < state_num; i++)
    {
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << header[i] << " ";
    }
    std::cout << "|" << std::endl;
    Draw_line(state_num, column_width);

    for(uint32_t i = 0; i < nodeNum; i++)
    {
        std::cout << "|" << setw(column_width - 1) << setiosflags(ios::right) << setfill(' ') << "Device_" << devicestates[i].node_idx << " ";
        std::cout << "|" << setw(column_width - 6) << setiosflags(ios::right) << setfill(' ') << devicestates[i].ipv4address << " " ; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].port << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << boolalpha << devicestates[i].is_online << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << boolalpha << devicestates[i].is_awake << " ";    
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << boolalpha << devicestates[i].is_in_service << " " ; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].baseline_awake_times << " "; 

        std::cout << "|" << std::endl;
    }

    Draw_line(state_num, column_width);

}

void DeviceStateManager::PrintCurrentDeviceStatesSimple()
{
    int state_num = 7;
    int column_width = 10;
    vector<std::string> header{"DeviceName", "isAwake", "InService", "Baseline", "T_4_Wlm", "T_4_Sync", "AwakeTimes"};
    Draw_line(state_num, column_width);
    
    std::cout << "|"  << setw(3 * column_width)  << "Time: " << fixed << setprecision(3) << (Simulator::Now ()).GetSeconds () << ": Device State" << setw(4 * column_width - 5) << setiosflags(ios::right) << setfill(' ') << "|" << std::endl;

    Draw_line(state_num, column_width);
    // header
    for (int i = 0; i < state_num; i++)
    {
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << header[i] << " ";
    }
    std::cout << "|" << std::endl;
    Draw_line(state_num, column_width);

    for(uint32_t i = 0; i < nodeNum; i++)
    {
        std::cout << "|" << setw(column_width - 1) << setiosflags(ios::right) << setfill(' ') << "Device_" << devicestates[i].node_idx << " ";
        // std::cout << "|" << setw(column_width - 6) << setiosflags(ios::right) << setfill(' ') << devicestates[i].ipv4address << " " ; 
        // std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].port << " "; 
        // std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << boolalpha << devicestates[i].is_online << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << boolalpha << devicestates[i].is_awake << " ";    
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << boolalpha << devicestates[i].is_in_service << " " ; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].baseline_awake_times << " "; 
        
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times_waterlevelmatrix << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times_datasync << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times << " "; 

        std::cout << "|" << std::endl;
    }

    Draw_line(state_num, column_width);



}



void DeviceStateManager::Draw_line(int columns, int columns_width)
{
    for(int i = 0; i < columns; i++)
    {
        std::cout<< "+-";
        for(int j=0; j < columns_width; j++)
        {
            std::cout<<"-";
        }
    }
    std::cout << "+" <<std::endl;
}


void DeviceStateManager::Summary()
{
    int state_num = 4;
    int column_width = 11;
    int summary_wl_awake = 0;
    int summary_baseline_awake = 0;

    Draw_line(state_num, column_width);

    std::cout << "|" << setw(column_width * 3) << setiosflags(ios::right) << setfill(' ') << "Number of awakenings" << setw(column_width * 2 - 3) << setiosflags(ios::right) << setfill(' ')<< "|"<<std::endl;

    Draw_line(state_num, column_width);
    vector<std::string> header{"DeviceName", "Ours", "Baseline", "Difference"};

    for (int i = 0; i < state_num; i++)
    {
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << header[i] << " ";
    }
    std::cout << "|" << std::endl;
    Draw_line(state_num, column_width);
    for(uint32_t i = 0; i < nodeNum; i++)
    {
        std::cout << "|" << setw(column_width - 1) << setiosflags(ios::right) << setfill(' ') << "Device_" << devicestates[i].node_idx << " ";       
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].baseline_awake_times << " ";        
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << (devicestates[i].baseline_awake_times - devicestates[i].awake_times) << " "; 
    
        std::cout << "|" << std::endl;

        summary_wl_awake += devicestates[i].awake_times;
        summary_baseline_awake += devicestates[i].baseline_awake_times;

    }

    Draw_line(state_num, column_width);
    vector<int> summary_line{summary_wl_awake, summary_baseline_awake, summary_baseline_awake - summary_wl_awake};
    
    
    for(int i=0; i < state_num; i++)
    {
        if(i == 0)
        {
            std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Summary" << " ";
        }
        else
        {
            std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << summary_line[i - 1] << " ";
        }

    } 
    std::cout << "|" << std::endl;
    Draw_line(state_num, column_width);

    for(int i=0; i < state_num; i++)
    {
        if(i == 0)
        {
            std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Ratio" << " ";
        }
        else
        {
            double temp_value = (double)summary_line[i - 1]/summary_baseline_awake;
           
            
            std::cout << "|" << setw(column_width) << setiosflags(ios::fixed) << setfill(' ') << std::setprecision(4) << temp_value << " ";
        }

    } 
    std::cout << "|" << std::endl;

    Draw_line(state_num, column_width);
 

}

void DeviceStateManager::SummaryWithErrorRatio()
{
    int state_num = 6;
    int column_width = 11;


    Draw_line(state_num, column_width);
    std::cout << "|" << setw(column_width * (state_num / 2 + 1)) << setiosflags(ios::right) << setfill(' ') << "Number of awakenings" << setw(column_width * state_num/2 + 1) << setiosflags(ios::right) << setfill(' ')<< "|"<<std::endl;
    Draw_line(state_num, column_width);


    vector<std::string> header{"DeviceName", "Ours", "Baseline", "Invalid", "T_wlm", "T_datasync"};
    for (uint32_t i = 0; i < header.size(); i++)
    {
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << header[i] << " ";
    }   
    std::cout << "|" << std::endl;
    Draw_line(state_num, column_width);

    std::vector<int> summary_info(state_num - 1, 0);

    for(uint32_t i = 0; i < nodeNum; i++)
    {
        std::cout << "|" << setw(column_width - 1) << setiosflags(ios::right) << setfill(' ') << "Device_" << devicestates[i].node_idx << " ";       
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].baseline_awake_times << " ";        
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << (devicestates[i].baseline_awake_times - devicestates[i].awake_times) << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times_waterlevelmatrix << " "; 
        std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << devicestates[i].awake_times_datasync << " "; 

        std::cout << "|" << std::endl;

        summary_info[0] += devicestates[i].awake_times;
        summary_info[1] += devicestates[i].baseline_awake_times;
        summary_info[2] += devicestates[i].baseline_awake_times - devicestates[i].awake_times;
        summary_info[3] += devicestates[i].awake_times_waterlevelmatrix;
        summary_info[4] += devicestates[i].awake_times_datasync;

    }    
    Draw_line(state_num, column_width);


    for(int i=0; i < state_num; i++)
    {
        if(i == 0)
        {
            std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Summary" << " ";
        }
        else
        {
            std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << summary_info[i - 1] << " ";
        }

    } 
    std::cout << "|" << std::endl;
    Draw_line(state_num, column_width);

    for(int i=0; i < state_num; i++)
    {
        if(i == 0)
        {
            std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Ratio" << " ";
        }
        else
        {
            // summary_info --> baseline
            double temp_value = (double)summary_info[i - 1]/summary_info[1];
           
            
            std::cout << "|" << setw(column_width) << setiosflags(ios::fixed) << setfill(' ') << std::setprecision(4) << temp_value << " ";
        }

    } 
    std::cout << "|" << std::endl;

    Draw_line(state_num, column_width);

    std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Pull" << " ";
    std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << GetPullTimes() << " ";

    std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Pull Error" << " ";
    std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << GetErrorTimes() << " ";

    std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << "Ratio" << " ";
    std::cout << "|" << setw(column_width) << setiosflags(ios::right) << setfill(' ') << std::setprecision(4) << (double)GetErrorTimes() / GetPullTimes() << " ";


    std::cout << "|" << std::endl;


    Draw_line(state_num, column_width);

}

}

   