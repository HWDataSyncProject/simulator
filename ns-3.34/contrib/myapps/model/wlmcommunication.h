#ifndef WLMCOMMUNICATION_H
#define WLMCOMMUNICATION_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"


#include "devicestate.h"
#include "waterlevelmatrix.h"

using namespace std;
namespace ns3 {

class Socket;
class Packet;

class WaterLevelMatrixCommunication: public Application
{
    public:
        static TypeId GetTypeId (void);
        WaterLevelMatrixCommunication();
        virtual ~WaterLevelMatrixCommunication();

        // void SetFunctionType(bool function_type_);

        void SetWaterLevelMatrix(WaterLevelMatrix* wlm_);
        void SetDeviceStateManager(DeviceStateManager* dsm_);
        void SetSendPeriod(float period_);

        void ResetSendSocket();

        void BindSendSocket(uint32_t target_device_id);
        void BindRecvSocket();
        
        void SendPacket(uint8_t buf[], size_t buf_size, uint32_t target_device_id);

        bool IsTimeToSend();

        void LoadWaterLevelMatrixToPacket(uint8_t buf[]);
        void ReceivedWaterLevelMatrix(uint8_t buf[]);

        void CycleSend();

        void Send();
        void Receive(Ptr<Socket> socket);

        

    protected:
        virtual void DoDispose(void);       


    private:

        virtual void StartApplication (void);
        virtual void StopApplication (void);

        // bool function_type; // true -> send; false -> receive

        EventId sendEvent;

        Ptr<Socket> ipv4SendSocket;
        Ptr<Socket> ipv4RecvSocket;
        
        WaterLevelMatrix* wlm;
        DeviceStateManager* devicestatemanager;

        uint32_t node_idx;
        uint16_t port;
        uint32_t seq;
        float period;
        
         

};




}






#endif /* WLMCOMMUNICATION_H */