#ifndef WLMSEND_H
#define WLMSEND_H

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


namespace ns3 {

class Socket;
class Packet;

class WaterLevelSend: public Application
{
    public:
        static TypeId GetTypeId (void);
        WaterLevelSend();
        virtual ~WaterLevelSend();

        bool isTimeToSend();

        void Send();

        void SetWaterLevelMatrix(WaterLevelMatrix* wlm_);
        void SetDeviceStateManager(DeviceStateManager* dsm);
        void SetPeriod(float period_);

    protected:
        virtual void DoDispose(void);       

    private:

        virtual void StartApplication (void);
        virtual void StopApplication (void);

        EventId sendEvent;
        // EventId regularSendEvent;

        WaterLevelMatrix* wlm;
        DeviceStateManager* devicestatemanager;

        uint32_t node_idx;
        uint16_t port;

        // Ptr<Socket> ipv4ReceiveSocket;
        Ptr<Socket> ipv4SendSocket;

        uint32_t seq;
        float period;
};



}



#endif /* WLMSEND_H */