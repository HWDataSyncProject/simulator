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


#include "wlmsend.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WaterLevelSend");

NS_OBJECT_ENSURE_REGISTERED (WaterLevelSend);

TypeId WaterLevelSend::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WaterLevelSend")
                            .SetParent<Application> ()
                            .SetGroupName("Applications")
                            .AddConstructor<WaterLevelSend> ();
    
    return tid;
}

WaterLevelSend::WaterLevelSend()
{
    NS_LOG_FUNCTION (this);
    // NS_LOG_INFO("WaterLevelSend");

    wlm = NULL;
    period = 2.0;
    seq = 0;
    ipv4SendSocket = 0;
    sendEvent = EventId();
}

WaterLevelSend::~WaterLevelSend()
{
    NS_LOG_FUNCTION (this);
}

void WaterLevelSend::SetWaterLevelMatrix(WaterLevelMatrix* wlm_)
{
    wlm = wlm_;
    node_idx = wlm->GetNodeID();
}

void WaterLevelSend::SetDeviceStateManager(DeviceStateManager* dsm)
{
    devicestatemanager = dsm;
    port = dsm->GetNodePort(node_idx);
}

void WaterLevelSend::SetPeriod(float period_)
{
    period = period_;
}

void WaterLevelSend::DoDispose(void)
{
    NS_LOG_FUNCTION (this);
    Application::DoDispose ();    
}

void WaterLevelSend::StartApplication (void)
{
    NS_LOG_FUNCTION (this);


    sendEvent = Simulator::Schedule (Seconds (0.0), &WaterLevelSend::Send, this);
    // NS_LOG_INFO("WaterLevelSend Start");
}

void WaterLevelSend::StopApplication (void)
{
    NS_LOG_FUNCTION (this);

    Simulator::Cancel (sendEvent);
    

}

void WaterLevelSend::Send()
{
    NS_LOG_FUNCTION (this);
    NS_ASSERT (sendEvent.IsExpired ()); 

    // TODO
    // Only send the matrix to other devices 
    // when this current device is online (connected to the network)
    // Only when the current device is in service, request to pull newest data from other device
    
    // current is online
    if (devicestatemanager->GetOnline(node_idx) && devicestatemanager->GetIsInService(node_idx))
    {
        // send the water level matrix to all online devices
        auto online_devices_id = devicestatemanager->GetCurrentOnlineDevicesID();
        for (auto iter = online_devices_id.cbegin(); iter != online_devices_id.cend(); iter++)
        {
            
            // target device information
            uint32_t target_device_id = *iter; // temp
            // no self
            if (target_device_id != node_idx)
            {
                Ipv4Address target_device_address = devicestatemanager->GetNodeIpv4Address(target_device_id);
                uint16_t target_device_port = devicestatemanager->GetNodePort(target_device_id);

                
                if (ipv4SendSocket == 0)
                {
                    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
                    ipv4SendSocket = Socket::CreateSocket (GetNode (), tid);
                    // if the address format is valid
                    
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

                
                // parsing the message
                SeqTsHeader seqTs;
                seqTs.SetSeq (seq++);

                
                uint8_t buf[4 * wlm->GetNodeNumber() * wlm->GetAppNumber() * wlm->GetDateTypeNumber() + 5];
                buf[0] = 'R';
                buf[1] = 'Q';
                buf[2] = node_idx >> 8;
                buf[3] = node_idx;
                uint32_t buf_idx = 4;



                // put the local matrix into the buf
                for(uint32_t app_idx = 0; app_idx < wlm->GetAppNumber(); app_idx++)
                {
                    for(uint32_t dtype_idx = 0; dtype_idx < wlm->GetDateTypeNumber(); dtype_idx++)
                    {
                        //TODO
                        // the buf can be compress here
                        
                        buf[buf_idx] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx) >> 24;
                        buf[buf_idx + 1] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx) >> 16;
                        buf[buf_idx + 2] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx) >> 8;
                        buf[buf_idx + 3] = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx);

                        buf_idx += 4;
                    }
                }

                // NS_LOG_INFO("1");
                // add '\0' at the last stream
                buf[buf_idx] = '\0';
                
                

                // put the buf into the packet
                Ptr<Packet> p = Create<Packet> (buf, sizeof(buf));
                p->AddHeader (seqTs);

                

                // add the target address into the string
                std::stringstream peerAddressStringStream;
                if(Ipv4Address::IsMatchingType(target_device_address))
                {
                    peerAddressStringStream << target_device_address;
                }
                
                // send the packet successfully
                if((ipv4SendSocket->Send(p)) >= 0)
                {
                    NS_LOG_INFO("Time: "<< (Simulator::Now ()).GetSeconds () <<": Device " << node_idx << " --> Device " << target_device_id);
                }
                else
                {
                    NS_LOG_INFO("Error while Device "<< node_idx << "send packet to Device " << target_device_id);

                }
                // reset the socket to send the packet to another device
                ipv4SendSocket = 0;

            }
        }
        
    }
    // else
    // {
    //     NS_LOG_INFO("NO Online Device Or Current is not in service");
    // }
    sendEvent = Simulator::Schedule (Seconds (period), &WaterLevelSend::Send, this);
}



}