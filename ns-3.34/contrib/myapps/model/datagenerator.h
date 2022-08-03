/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include "ns3/application.h"
#include "ns3/wlmatrix.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

// #include "ns3/waterlevelmatrix-module.h"
#include "wlmreceive.h"
#include "devicestate.h"


namespace ns3 {

/* ... */
class Socket;
class Packet;

class DataGenerator: public Application
{
    public:
        static TypeId GetTypeId(void);
        DataGenerator();
        // DataGenerator(int data_type_index_, int app_index_, char* folder_path_);
        virtual ~DataGenerator();

        void SetWaterLevelMatrix(WaterLevelMatrix* wlm_global);
        
        void SetDeviceStateManager(DeviceStateManager* dsm);

        void SetNodeIndex(uint32_t node_idx_);

        void Set_data_type_index(int data_type_index_);
        
        void Set_app_index(int app_index_);

        void Init(WaterLevelMatrix* wlm_global, char* folder_path_, int data_type_index_, int app_index_);

        int Get_data_type_index();
        int Get_app_index();
        char* Get_folder_path();

        void SetPeriod(float period_);
        float GetPeriod();


    protected:
        virtual void DoDispose(void);
    
    private:

        virtual void StartApplication (void);
        virtual void StopApplication (void);

        void Gene(void);

        void GeneratePoissonData();

        void TransTxt (uint32_t num_byte);
        void TransImg (uint32_t num_byte);
        int  Possion (int probaility);

        uint32_t time_ctrl;
        uint32_t bytes;
    
        uint32_t time_s;
        uint32_t num_byte;

        WaterLevelMatrix* wlm;
        DeviceStateManager* devicestatemanager;

        EventId m_sendEvent; //!< Event to send the next packet

        uint32_t node_idx;
        int data_type_index;
        int app_index;
        float period;

        char* folder_path;
        char* src = (char*)"/src";
        char* dst = (char*)"/dst";
        

};


}

#endif /* DATAGENERATOR_H */

