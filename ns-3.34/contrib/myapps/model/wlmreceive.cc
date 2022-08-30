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


#include "wlmreceive.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WaterLevelReceive");

NS_OBJECT_ENSURE_REGISTERED (WaterLevelReceive);

TypeId WaterLevelReceive::GetTypeId (void)
{
    static TypeId tid = TypeId ("ns3::WaterLevelReceive")
                            .SetParent<Application> ()
                            .SetGroupName("Applications")
                            .AddConstructor<WaterLevelReceive> ();
    
    return tid;
}

WaterLevelReceive::WaterLevelReceive()
{
    NS_LOG_FUNCTION (this);
    receiveEvent = EventId();
    // offlineEvent = EventId();
    ipv4ReceiveSocket = 0;
    ipv4SendSocket_= 0;

    seq = 0;
    wlm = NULL;


}

WaterLevelReceive::~WaterLevelReceive()
{
    NS_LOG_FUNCTION (this);
}


void WaterLevelReceive::SetWaterLevelMatrix(WaterLevelMatrix* wlm_)
{
    wlm = wlm_;
    node_idx = wlm->GetNodeID();
}

void WaterLevelReceive::SetDeviceStateManager(DeviceStateManager* dsm)
{
    devicestatemanager = dsm;
    port = dsm->GetNodePort(node_idx);
}

void WaterLevelReceive::SetPeriod(float period_)
{
    period = period_;
}

void WaterLevelReceive::DoDispose(void)
{
    NS_LOG_FUNCTION (this);
    Application::DoDispose ();    
}

void WaterLevelReceive::StartApplication()
{
    NS_LOG_FUNCTION (this);

    // TODO

    // if the node first join the network, the water level matrix should be expand
    
    // TODO this part should be added in device manager

    // bind the ip
    if (ipv4ReceiveSocket == 0)
    {
        TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
        ipv4ReceiveSocket = Socket::CreateSocket (GetNode (), tid);
        // recevice any device from 
        InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), port);
        
        
        if (ipv4ReceiveSocket->Bind(local) == -1)
        {
            NS_FATAL_ERROR ("Failed to bind socket");
        }
    }
    // NS_LOG_INFO(port);
    ipv4ReceiveSocket->SetRecvCallback (MakeCallback (&WaterLevelReceive::Receive, this));
    // NS_LOG_INFO("WaterLevelReceive Start");
}

void WaterLevelReceive::StopApplication(void)
{
    NS_LOG_FUNCTION (this);
    if (ipv4ReceiveSocket!= 0) //Prt pointer, 0 represents null
    {
        ipv4ReceiveSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }

}

void WaterLevelReceive::Receive(Ptr<Socket> socket)
{
    NS_LOG_FUNCTION (this << socket);
    Ptr<Packet> packet;
    Address from;

    // NS_LOG_INFO("Receive running");
   
    while((packet = socket -> RecvFrom(from)))
    {
        
        if (packet -> GetSize() > 0)
        {

            SeqTsHeader seqTs;
            packet->RemoveHeader (seqTs);

            // uint32_t currentSequenceNumber = seqTs.GetSeq ();
            packet->RemoveAllPacketTags ();
            packet->RemoveAllByteTags ();

            
            uint8_t buf[4 * wlm->GetNodeNumber() * wlm->GetAppNumber() * wlm->GetDateTypeNumber() + 5];
            packet->CopyData(buf, sizeof(buf));

            // TODO receive the data from the sender
            
            // check the header of the packet

            /*
            header: 
                RQ
            
            */
            if (buf[0] == 'R' && buf[1]=='Q')
            {
                ParsingWaterLevelMatrix(buf);
                // NS_LOG_INFO("Receive running RQ");
            }
            else if (buf[0] == 'R' && buf[1]=='S')
            {
                /* parsing response */
                ParsingResponse(buf);
                // NS_LOG_INFO("Receive running RS");
            }
            

        }


    }
    
}


void WaterLevelReceive::ParsingWaterLevelMatrix(uint8_t buf[])
{
    // check the header again
    if(buf[0] == 'R' && buf[1] == 'Q')
    {
        bool resend = false;
        uint32_t source_node_idx = (buf[2] << 8) + buf[3];
        uint32_t buf_idx = 4;

        // declare the response 
        uint8_t response_buf[4 * wlm->GetNodeNumber() * wlm->GetAppNumber() * wlm->GetDateTypeNumber() + 5];
        response_buf[0] = 'R';
        response_buf[1] = 'S';
        response_buf[2] = source_node_idx >> 8;
        response_buf[3] = source_node_idx;
        uint32_t response_buf_idx = 4;       

        for (uint32_t app_idx = 0; app_idx < wlm->GetAppNumber(); app_idx++)
        {
            for (uint32_t dtype_idx = 0; dtype_idx < wlm ->GetDateTypeNumber(); dtype_idx++)
            {
                uint64_t value = (buf[buf_idx] << 24) + (buf[buf_idx+1] << 16) + (buf[buf_idx+2] << 8) + buf[buf_idx+3];
                
                /*
                receive the value, 
                if the value from the source is larger than current device, 
                current device status should not be change.
                if the value from the source is smaller than current device,
                current device should be awaked to send the new 

                */
                uint64_t current_value = wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx); // node idx is the id of the current device
               

                
                if(value < current_value) 
                {
                    // need to be awake and 
                    // need to response to the source
                    resend = true;
                    /*
                        ....
                        data sync
                        ....
                    
                    */        
                }
                else
                {
                    // if the value is large, the data in source more fresh then current device

                }

                // create the response as one row of the current no mater whether the device should send the response/data sync
                response_buf[response_buf_idx] = current_value >> 24;
                response_buf[response_buf_idx + 1] = current_value >> 16;          
                response_buf[response_buf_idx + 2] = current_value >> 8;
                response_buf[response_buf_idx + 3] = current_value;
                
                response_buf_idx += 4;
                buf_idx += 4;
            }
        }
        response_buf[response_buf_idx] = '\0';

        
        /*
        if the current device need to send the response
        the current is awake, the device just need to data sync the new data from the current device
        the current is sleep, the device need to be awaked and then data sync
        */
        if(resend)
        {
            Response(response_buf, sizeof(response_buf), source_node_idx);
            // NS_LOG_INFO("Response");
            devicestatemanager->SetDeviceAwake(node_idx);
            
        }

        // baseline: awake the device even if the don't have new data
        devicestatemanager->SetDeviceAwakeBaseline(node_idx);

    }
}

void WaterLevelReceive::ParsingResponse(uint8_t buf[])
{
    if(buf[0] == 'R' && buf[1] == 'S')
    {
        uint32_t buf_idx = 4;
        for(uint32_t app_idx = 0; app_idx < wlm->GetAppNumber(); app_idx++)
        {
            for(uint32_t dtype_idx = 0; dtype_idx < wlm->GetDateTypeNumber(); dtype_idx++)
            {
                //TODO
                int64_t value = (buf[buf_idx] << 24) + (buf[buf_idx + 1] << 16) + (buf[buf_idx + 2] << 8) + buf[buf_idx + 3];
                if (value >= wlm->GetWaterLevelMatrix(node_idx, app_idx, dtype_idx))
                {
                    wlm -> SetWaterLevelMatrix(node_idx, app_idx, dtype_idx, value);
                }
                buf_idx += 4;
            }
        }

    
    }





}

void WaterLevelReceive::Response(uint8_t response_buf[], size_t response_buf_size, uint32_t source_idx)
{

    if(ipv4SendSocket_ == 0)
    {
        TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
        ipv4SendSocket_ = Socket::CreateSocket (GetNode (), tid); 

        Ipv4Address source_address = devicestatemanager->GetNodeIpv4Address(source_idx);
        uint16_t source_port = devicestatemanager->GetNodePort(source_idx);
        if(Ipv4Address::IsMatchingType(source_address))
        {
            if (ipv4SendSocket_->Bind () == -1)
            {
                NS_FATAL_ERROR ("Failed to bind socket");
            }
            ipv4SendSocket_->Connect (InetSocketAddress(source_address, source_port));     
        }
        else
        {
            NS_ASSERT_MSG(false, "Incompatible address type:" << source_address);
        }
    } 

    uint32_t seq_ = 0;
    
    SeqTsHeader seqTs;
    seqTs.SetSeq (seq_++);
    Ptr<Packet> p = Create<Packet> (response_buf, response_buf_size);
    

    p->AddHeader(seqTs);
    
    
    if (ipv4SendSocket_->Send(p) >= 0)
    {
       NS_LOG_INFO("Time: "<< (int)(Simulator::Now ()).GetSeconds () <<": Response: Device " << node_idx <<" --> Device "<< source_idx); 
    }
    else
    {
        NS_LOG_INFO("Error while Device " << node_idx <<" --> Device "<< source_idx); 
    }
    ipv4SendSocket_ = 0;

}





}