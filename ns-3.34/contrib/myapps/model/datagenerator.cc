/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"


#include "datagenerator.h"

#include "string.h"
#include "math.h"
#include <exception> 
#include <unistd.h>
#include<random>

using namespace std;

namespace ns3 {

/* ... */
NS_LOG_COMPONENT_DEFINE ("DataGenerator");

NS_OBJECT_ENSURE_REGISTERED (DataGenerator);

TypeId DataGenerator::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::DataGenerator")
                                .SetParent<Application>()
                                .SetGroupName("Applications")
                                .AddConstructor<DataGenerator> ();
    return tid;
}

DataGenerator::DataGenerator()
{
    NS_LOG_FUNCTION(this);

    m_sendEvent = EventId();

    folder_path = getcwd(NULL, 0);
}


// DataGenerator::DataGenerator(int data_type_index_, int app_index_, char* folder_path_)
// {
//     data_type_index = data_type_index_;
//     app_index = app_index_;
//     folder_path = folder_path_;
    
//     NS_LOG_FUNCTION(this);

//     m_sendEvent = EventId();

// }

DataGenerator::~DataGenerator()
{
    NS_LOG_FUNCTION (this);
}

void DataGenerator::SetWaterLevelMatrix(WaterLevelMatrix* wlm_global)
{
    wlm = wlm_global;
}

void DataGenerator::SetDeviceStateManager(DeviceStateManager* dsm)
{
    devicestatemanager = dsm;
}

void DataGenerator::SetNodeIndex(uint32_t node_idx_)
{
    node_idx = node_idx_;
}

void DataGenerator::Set_data_type_index(int data_type_index_)
{
    data_type_index = data_type_index_;
}

void DataGenerator::Set_app_index(int app_index_)
{
    app_index = app_index_;
}

void DataGenerator::Init(WaterLevelMatrix* wlm_global, char* folder_path_, int data_type_index_, int app_index_)
{
    wlm = wlm_global;
    folder_path = folder_path_;
    data_type_index = data_type_index_;
    app_index = app_index_;
    
}

int DataGenerator::Get_data_type_index()
{
    return data_type_index;
}

int DataGenerator::Get_app_index()
{
    return app_index;
}

char* DataGenerator::Get_folder_path()
{
    return folder_path;
}

void DataGenerator::SetPeriod(float period_)
{
    period = period_;
}

float DataGenerator::GetPeriod()
{
    return period;
}

void DataGenerator::DoDispose(void)
{
    NS_LOG_FUNCTION (this);
    Application::DoDispose ();    
}

void DataGenerator::StartApplication (void)
{
    NS_LOG_FUNCTION (this);
    // m_sendEvent = Simulator::Schedule (Seconds (period), &DataGenerator::Gene, this);
    
    m_sendEvent = Simulator::Schedule (Seconds (period/2), &DataGenerator::GeneratePoissonData, this); 
    // NS_LOG_INFO("DataGenerator "<<node_idx<<":"<<app_index<<":"<<data_type_index<<" Start");
}

void DataGenerator::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
  Simulator::Cancel (m_sendEvent);
}

void DataGenerator::GeneratePoissonData()
{
    if (devicestatemanager->GetIsAwake(node_idx))
    {
        std::default_random_engine dre;
        std::poisson_distribution<uint32_t> p_distribution(5.0);
        std::uniform_int_distribution<int> u_distribution(0, 9);

        int p_generate_data = u_distribution(dre);

        if (p_generate_data < 5)
        {
            bytes = p_distribution(dre);

            if (bytes > 0)
            {
                int64_t ts_temp = (Simulator::Now()).GetMilliSeconds();
                
                wlm->UpdataWaterLevelMatrix(app_index, data_type_index, ts_temp);
                // TransTxt(bytes);
                // NS_LOG_INFO("Device "<< node_idx << ":" << app_index << ":" << data_type_index << " generate new data");    
            }
        }
    }
    m_sendEvent = Simulator::Schedule (Seconds (period), &DataGenerator::GeneratePoissonData, this); 

}

void DataGenerator::Gene (void)
{
    time_ctrl = Possion(10);
    if(time_ctrl < 10)
    {
        bytes = Possion(10);
        if(bytes > 0)
        {
            int64_t ts_temp = (Simulator::Now()).GetMilliSeconds();
            wlm->UpdataWaterLevelMatrix(app_index, data_type_index, ts_temp);
            TransTxt(bytes);
        }
    }

    m_sendEvent = Simulator::Schedule (Seconds (period), &DataGenerator::Gene, this);   
}

void DataGenerator::TransTxt (uint32_t num_byte)
{
    char ch;
    uint32_t i = 0;

    FILE* fp_src;
    FILE* fp_dst;
    try
    {
        fp_src = fopen(strcat(folder_path, strcat(src, ".txt")), "r");
        if (fp_src)
        {
            ch = getc(fp_src);
            fp_dst = fopen(strcat(folder_path, strcat(dst, ".txt")), "w+");

            while((ch != EOF) && (i < num_byte * 100))
            {
                // put one char into the destination file
                fputc(ch, fp_dst);
                // get the next char
                ch = getc(fp_src);
                i++;
            }

            fclose(fp_src);
            fclose(fp_dst);
            NS_LOG_INFO("Node: " << GetNode()->GetId() << "; Time: "<< (Simulator::Now()).GetSeconds() << "; Bytes: " << num_byte*100 << "B.");
        }
        else
        {
            throw "source file not exist.";
        }
    }
    catch(char* s){
        NS_LOG_INFO(s << strcat(folder_path, src));
    }    
}

void DataGenerator::TransImg (uint32_t num_byte)
{
    uint32_t j;

    for(j = 0; j < num_byte; j++)
    {
        FILE* fp_src = fopen(strcat(folder_path, strcat(src, ".jpg")), "rb");
        FILE* fp_dst = fopen(strcat(folder_path, strcat(dst, ".jpg")), "wb");

        if ((fp_src == NULL) || (fp_dst == NULL)) 
        {
            perror("file not exist.");
        }

        fseek(fp_src, 0, SEEK_END);
        int i = ftell(fp_src);
        rewind(fp_src);
        char* p = (char*)malloc(i);

        fread(p, 1, i, fp_src);
        fwrite(p, 1, i, fp_dst);;

        fclose(fp_src);
        fclose(fp_dst);
    }
    NS_LOG_INFO("Node: " << GetNode()->GetId() << "; Time: "<< (Simulator::Now()).GetSeconds() << "; Images Num: " << num_byte << ".");
}

int DataGenerator::Possion(int probaility)
{
    double p = 1.0;
    int k = 1;
    double num;
    double l = exp(-probaility);

    while(p >= l)
    {
        k++;
        num = rand()%10000;
        p *= num/10000;
    }

    return k;
}






}

