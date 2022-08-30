#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"

#include "ns3/seq-ts-header.h"


#include "wlmcommunication.h"
#include <iomanip>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WaterLevelMatrixCommunication");

NS_OBJECT_ENSURE_REGISTERED (WaterLevelMatrixCommunication);

TypeId WaterLevelMatrixCommunication::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WaterLevelMatrixCommunication")
                            .SetParent<Application> ()
                            .SetGroupName("Applications")
                            .AddConstructor<WaterLevelMatrixCommunication> ();
    
    return tid;
}

WaterLevelMatrixCommunication::WaterLevelMatrixCommunication()
{
    NS_LOG_FUNCTION (this);

    wlm = NULL;
    devicestatemanager = NULL;
    
    seq = 0;
    period = 2.0;

    sendEvent = EventId();

    ipv4SendSocket = 0;
    ipv4RecvSocket = 0;

    // function_type = false; //defualt is a receiver

}

WaterLevelMatrixCommunication::~WaterLevelMatrixCommunication()
{
    NS_LOG_FUNCTION (this);
}

// void WaterLevelMatrixCommunication::SetFunctionType(bool function_type_)
// {
//     function_type = function_type_;
// }


void WaterLevelMatrixCommunication::SetWaterLevelMatrix(WaterLevelMatrix* wlm_)
{
    wlm = wlm_;
    node_idx = wlm->GetNodeID();
}

void WaterLevelMatrixCommunication::SetDeviceStateManager(DeviceStateManager* dsm_)
{
    devicestatemanager = dsm_;
    port = dsm_->GetNodePort(node_idx);
}

void WaterLevelMatrixCommunication::SetSendPeriod(float period_)
{
    period = period_;
}

void WaterLevelMatrixCommunication::DoDispose(void)
{
    NS_LOG_FUNCTION (this);
    Application::DoDispose();
}

void WaterLevelMatrixCommunication::StartApplication(void)
{
    NS_LOG_FUNCTION (this);

    // if(function_type)
    // {
    //     cout<<"send app"<<endl;
    //     sendEvent = Simulator::Schedule (Seconds (0.0), &WaterLevelMatrixCommunication::CycleSend, this);

    // }
    // else
    // {
    //     cout<<"receive app"<<endl;
    //     BindRecvSocket();
    //     ipv4RecvSocket->SetRecvCallback (MakeCallback (&WaterLevelMatrixCommunication::Receive, this));

    // }

    sendEvent = Simulator::Schedule (Seconds (0.0), &WaterLevelMatrixCommunication::CycleSend, this);
    BindRecvSocket();
    ipv4RecvSocket->SetRecvCallback (MakeCallback (&WaterLevelMatrixCommunication::Receive, this));

    
}

void WaterLevelMatrixCommunication::StopApplication(void)
{
    NS_LOG_FUNCTION (this);

    Simulator::Cancel (sendEvent);

    if (ipv4RecvSocket!= 0) //Prt pointer, 0 represents null
    {
        ipv4RecvSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }

}

void WaterLevelMatrixCommunication::ResetSendSocket()
{
    ipv4SendSocket = 0;
}

void WaterLevelMatrixCommunication::BindSendSocket(uint32_t target_device_id)
{
    // if the target is not self
    if (target_device_id != node_idx)
    {
        Ipv4Address target_device_address = devicestatemanager->GetNodeIpv4Address(target_device_id);
        uint16_t target_device_port = devicestatemanager->GetNodePort(target_device_id);

        if (ipv4SendSocket == 0)
        {
            TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
            ipv4SendSocket = Socket::CreateSocket (GetNode (), tid);

            // check the target device ip 
            if (Ipv4Address::IsMatchingType(target_device_address))
            {
                if (ipv4SendSocket->Bind () == -1)
                {
                    NS_FATAL_ERROR ("Failed to bind socket");
                }  

                // connet to target device ip:port
                ipv4SendSocket->Connect (InetSocketAddress (target_device_address, target_device_port));  

            }
            else
            {
                NS_ASSERT_MSG (false, "Incompatible address type: " << target_device_address);
            }    
        }

        ipv4SendSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
        ipv4SendSocket->SetAllowBroadcast (true);

    }

}

void WaterLevelMatrixCommunication::BindRecvSocket()
{
    if (ipv4RecvSocket == 0)
    {
        TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
        ipv4RecvSocket = Socket::CreateSocket (GetNode (), tid);
        // the node can receive the packet from any devices/nodes.
        InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), port);

        if (ipv4RecvSocket->Bind(local) == -1)
        {
            NS_FATAL_ERROR ("Failed to bind socket");
        }

    }

}

void WaterLevelMatrixCommunication::SendPacket(uint8_t buf[], size_t buf_size, uint32_t target_device_id)
{
    // bind the send socket with the target device
    ResetSendSocket();
    BindSendSocket(target_device_id);

    uint32_t seq_ = 0;
    SeqTsHeader seqTs;
    seqTs.SetSeq (seq_++);

    // create packet
    Ptr<Packet> p = Create<Packet> (buf, buf_size);

    p->AddHeader(seqTs);

    //send 
    if (ipv4SendSocket->Send(p) >= 0)
    {
        std::cout<<"Time: "<< fixed <<  setprecision(3) <<(Simulator::Now ()).GetSeconds () <<": Device " << node_idx <<" --Send--> Device "<< target_device_id<<std::endl; 
    }
    else
    {
        NS_LOG_INFO("Error while Device " << node_idx <<" --> Device "<< target_device_id); 
    }

    //reset the send
    ResetSendSocket();
}

bool WaterLevelMatrixCommunication::IsTimeToSend()
{
    bool istimetosend = false;
    if (devicestatemanager->GetOnline(node_idx))
    {
        if (devicestatemanager->GetIsAwake(node_idx))
        {
            if (wlm->GetIsModified())
            {
                
                istimetosend = true;
            }
            // istimetosend = true;
        }
    }
    return istimetosend;   
}

void WaterLevelMatrixCommunication::LoadWaterLevelMatrixToPacket(uint8_t buf[])
{

    // add the header into the buf
    buf[0] = 'S'; //Send
    buf[1] = 'W'; //WaterLevelMatrix

    buf[2] = node_idx >> 8;
    buf[3] = node_idx;
    
    uint32_t buf_idx = 4;

    //put the waterlevel matrix into the buf
    for (uint32_t app_idx = 0; app_idx < wlm->GetAppNumber(); app_idx++)
    {
        for (uint32_t dtype_idx = 0; dtype_idx < wlm->GetDateTypeNumber(); dtype_idx++)
        {
            buf[buf_idx] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx) >> 24;
            buf[buf_idx + 1] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx) >> 16;
            buf[buf_idx + 2] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx) >> 8;
            buf[buf_idx + 3] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx);

            buf_idx += 4;
        }
    }

    buf[buf_idx] = '\0';

}

void WaterLevelMatrixCommunication::ReceivedWaterLevelMatrix(uint8_t buf[])
{
    if (buf[0] == 'S' && buf[1] == 'W')
    {
        uint32_t source_device_id = (buf[2] << 8) + buf[3];
        uint32_t buf_idx = 4;

        for (uint32_t app_idx = 0; app_idx < wlm->GetAppNumber(); app_idx++)
        {
            for (uint32_t dtype_idx = 0; dtype_idx < wlm->GetDateTypeNumber(); dtype_idx++)
            {
                int64_t value = (buf[buf_idx] << 24) + (buf[buf_idx + 1] << 16) + (buf[buf_idx + 2] << 8) + buf[buf_idx + 3];

                wlm->SetWaterLevelMatrix(source_device_id, app_idx, dtype_idx, value);

                buf_idx += 4;

            }
        }

        // the current device should be awaked for waterlevelmatrix


        devicestatemanager->SetDeviceAwakeForWaterLevelMatrixFromMultiDevices(node_idx, source_device_id);

        std::cout<<"Time: " << fixed << setprecision(3) << (Simulator::Now ()).GetSeconds () <<": Device " << node_idx <<" <--Recv-- Device "<< source_device_id << std::endl; 
    }
}

void WaterLevelMatrixCommunication::CycleSend()
{
    Send();
    Simulator::Schedule(Seconds(period), &WaterLevelMatrixCommunication::CycleSend, this);

}

void WaterLevelMatrixCommunication::Send()
{
    NS_LOG_FUNCTION (this);
    NS_ASSERT (sendEvent.IsExpired ());

    if (IsTimeToSend())
    {
        // send the water level matrix to all online devices
        auto online_devices_id = devicestatemanager->GetCurrentOnlineDevicesID();
        for (auto iter = online_devices_id.cbegin(); iter != online_devices_id.cend(); iter ++)
        {
            uint32_t target_device_id = *iter;
            //exclude self

            if (target_device_id != node_idx)
            {
                uint8_t buf[16 * wlm->GetAppNumber() * wlm->GetDateTypeNumber() + 5];

                LoadWaterLevelMatrixToPacket(buf);
                
                SendPacket(buf, sizeof(buf), target_device_id);
            }
                       
        }
    }

}

void WaterLevelMatrixCommunication::Receive(Ptr<Socket> socket)
{
    
    NS_LOG_FUNCTION (this << socket);

    Ptr<Packet> packet;
    Address from;

    while ((packet = socket->RecvFrom(from)))
    {
    
        if (packet->GetSize() > 0)
        {
            /* copy the data in packet into buf */
            SeqTsHeader seqTs;
            packet->RemoveHeader (seqTs);
            packet->RemoveAllPacketTags ();
            packet->RemoveAllByteTags ();  

            // one uint64_t = 16 * uint8_t
            uint8_t buf[16 * wlm->GetAppNumber() * wlm->GetDateTypeNumber() + 5];
            packet->CopyData(buf, sizeof(buf));      
           

            //check the header of the packet
            if (buf[0] == 'S' && buf[1] == 'W')
            {
                ReceivedWaterLevelMatrix(buf);
            }



        }
        
    }
    

}


}