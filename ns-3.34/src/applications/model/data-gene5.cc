/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

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
#include "seq-ts-header.h"
#include<math.h>
#include<time.h>
#include <cstdlib>
#include <cstdio>
#include <algorithm>

#include "data-gene5.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DataGene5");

NS_OBJECT_ENSURE_REGISTERED (DataGene5);

TypeId
DataGene5::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DataGene5")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<DataGene5> ()
  ;
  return tid;
}

DataGene5::DataGene5 ()
{
  NS_LOG_FUNCTION (this);
  //m_sent = 0;
  //m_socket = 0;
  m_sendEvent = EventId ();
}

DataGene5::~DataGene5 ()
{
  NS_LOG_FUNCTION (this);
}

void
DataGene5::SetWlm (WLMatrix* wlm_global)
{
  wlm = wlm_global;
}


void
DataGene5::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
DataGene5::StartApplication (void)
{
  NS_LOG_FUNCTION (this);
  m_sendEvent = Simulator::Schedule (Seconds (0.5), &DataGene5::Gene, this);
}

void
DataGene5::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
  Simulator::Cancel (m_sendEvent);
}


int 
DataGene5::Possion(int probaility)
{
    double p = 1.0;//初始p
    int k = 0;//k
    double num;
    double L = exp(-probaility);//e-i次方,
    while(p>=L)
    {
        k++;
        num = rand()%10000;
        p*=num/10000;//不断更新pi(p=p1*p2*p3...)
    }
    return k;
}

int 
DataGene5::Berno(double p)
{
   
   int x,num=0,i=0;
   double u;
   
   for(i=0;i<100;i++)
   {
       u = rand()/(RAND_MAX+1.0);
       x = (u <= p)? 1:0;
       num+=x;
   }	
   return x;
}



void 
DataGene5::Gene (void)
{
    time_ctrl = Possion(16);
    if(time_ctrl < 16)
    {
        bytes = Possion(10);
        if(bytes > 0)
        {
            int64_t ts_temp = (Simulator::Now()).GetMilliSeconds();
            wlm->UpdataWLMatrix(4,0,ts_temp);
            Trans(bytes);
        }
    }

    m_sendEvent = Simulator::Schedule (Seconds (1.0), &DataGene5::Gene, this);
}

void 
DataGene5::Trans (uint32_t num_byte)
{        
    char ch;
    uint32_t i=0;
    FILE *fp1;
    FILE *fp2;
    fp1 = fopen("/home/houchangsheng/ns-3/tarballs/ns-allinone-3.27/ns-3.27/data-source/test15.txt", "r");
    if (fp1)
    {
         ch = getc(fp1);
         fp2 = fopen("/home/houchangsheng/ns-3/tarballs/ns-allinone-3.27/ns-3.27/data-source/test25.txt", "w+");
         while((ch != EOF)&&(i<num_byte*100))
         {
             fputc(ch, fp2);
             ch = getc(fp1);
             i++;
         }
         fclose(fp1);
         fclose(fp2);
         NS_LOG_INFO("Node: " << GetNode()->GetId() << "; Time: "<< (Simulator::Now()).GetSeconds() << "; Bytes: " << num_byte*100 << "B.");
    }
 
}

/*void
DataGene5::Trans2(uint32_t num)
{
     uint32_t j;
     for(j=0;j<num;j++)
     {
         FILE *fp1 = fopen("test1.jpg", "rb");
	 FILE *fp2 = fopen("test2.jpg", "wb");
	 if (fp1 == NULL) {
	     perror("fopen");
	 }
	 if (fp2 == NULL) {
	     perror("fopen");
	 }
	fseek(fp1, 0, SEEK_END);   //将指针对二进制文件(test.jpg)的读取位置放到末端
	int i=ftell(fp1);          //得到起始偏移量，这里也就是文件大小   
	rewind(fp1);               //将指针对二进制文件(test.jpg)的读取位置恢复到起始位置
	char *p = (char*)malloc(i);       //申请与原图片相同大小的动态内存空间
	fread(p, 1, i, fp1);              //复制        
	fwrite(p, 1, i, fp2);             //粘贴
	fclose(fp2);                      
	fclose(fp1);                      //关闭文件
     }
}*/

} // Namespace ns3
