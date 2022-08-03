#ifndef WLMRECEIVE_H
#define WLMRECEIVE_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket.h"
#include "ns3/inet-socket-address.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"

#include <vector>
#include "devicestate.h"
#include "waterlevelmatrix.h"


namespace ns3 {

class Socket;
class Packet;


class WaterLevelReceive: public Application
{
    public:
        static TypeId GetTypeId (void);
        WaterLevelReceive();
        virtual ~WaterLevelReceive();

        void SetWaterLevelMatrix(WaterLevelMatrix* wlm_);

        void SetDeviceStateManager(DeviceStateManager* dsm);

        void SetPeriod(float period_);

        void Receive(Ptr<Socket> socket);

        void ParsingWaterLevelMatrix(uint8_t buf[]);
        void ParsingResponse(uint8_t buf[]);

        void Response(uint8_t response_buf[], size_t response_buf_size, uint32_t source_idx);

    protected:
        virtual void DoDispose(void);       

    private:

        virtual void StartApplication (void);
        virtual void StopApplication (void);

        EventId receiveEvent;
        // EventId offlineEvent;

        WaterLevelMatrix* wlm;
        DeviceStateManager* devicestatemanager;

        uint32_t node_idx;
        uint16_t port;

        Ptr<Socket> ipv4ReceiveSocket;
        Ptr<Socket> ipv4SendSocket_;

        float period;
        uint32_t seq;

        

};

}


#endif /* WLMRECEIVE_H */