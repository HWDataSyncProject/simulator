/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/stats-module.h"
#include "ns3/wifi-module.h"

#include "ns3/wlmatrix.h"
#include "ns3/hyperterminal.h"

#include <iostream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Main");

int main (int argc, char *argv[])
{
  CommandLine cmd;
  int test;
  cmd.AddValue("test", "the test command", test);
  cmd.Parse (argc, argv);

  
  
  //time resolution
  Time::SetResolution (Time::NS);
  //log component
  LogComponentEnable ("WlmSend", LOG_LEVEL_INFO);
  LogComponentEnable ("WlmReceive", LOG_LEVEL_INFO);
  LogComponentEnable ("DataGene", LOG_LEVEL_INFO);
  LogComponentEnable ("DataGene2", LOG_LEVEL_INFO);
  LogComponentEnable ("Main", LOG_LEVEL_INFO);

  NS_LOG_INFO ("Count of original solution: "<< test);


  NS_LOG_INFO ("Creating Nodes.");
  //topology helper for nodes
  NodeContainer nodes;
  //create two nodes with topology helper
  nodes.Create (5);

  NS_LOG_INFO ("Installing WiFi and Internet Stack.");
  //install the protocol stack
  WifiHelper wifi;

  WifiMacHelper wifiMac;
  wifiMac.SetType ("ns3::AdhocWifiMac");

  YansWifiPhyHelper wifiPhy;
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
  wifiPhy.SetChannel (wifiChannel.Create ());

  NetDeviceContainer nodeDevices = wifi.Install (wifiPhy, wifiMac, nodes);

  //assign IP address
  InternetStackHelper internet;
  internet.Install (nodes);

  Ipv4AddressHelper ipAddrs;
  ipAddrs.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = ipAddrs.Assign (nodeDevices);


  NS_LOG_INFO ("Installing Static Mobility.");
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  positionAlloc->Add (Vector (0.0, 10.0, 0.0));
  positionAlloc->Add (Vector (0.0, 30.0, 0.0));
  positionAlloc->Add (Vector (20.0, 0.0, 0.0));
  positionAlloc->Add (Vector (20.0, 40.0, 0.0));
  positionAlloc->Add (Vector (30.0, 20.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  //mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);


  NS_LOG_INFO ("Installing the Applications.");
  WLMatrix wlm[5];
  HyperTerminal HT;
  HT.Initialize(interfaces);

  WlmSendHelper wlm_send;
  ApplicationContainer send_apps = wlm_send.Install (nodes);

  Ptr<WlmSend> send_app_0 = send_apps.Get(0)->GetObject<WlmSend>();
  send_app_0->SetWlm(&wlm[0]);
  send_app_0->SetHT(&HT);
  send_apps.Get (0)->SetStartTime (Seconds (1.0));
  send_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<WlmSend> send_app_1 = send_apps.Get(1)->GetObject<WlmSend>();
  send_app_1->SetWlm(&wlm[1]);
  send_app_1->SetHT(&HT);
  send_apps.Get (1)->SetStartTime (Seconds (1.0));
  send_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<WlmSend> send_app_2 = send_apps.Get(2)->GetObject<WlmSend>();
  send_app_2->SetWlm(&wlm[2]);
  send_app_2->SetHT(&HT);
  send_apps.Get (2)->SetStartTime (Seconds (3.0));
  send_apps.Get (2)->SetStopTime (Seconds (600.0));

  Ptr<WlmSend> send_app_3 = send_apps.Get(3)->GetObject<WlmSend>();
  send_app_3->SetWlm(&wlm[3]);
  send_app_3->SetHT(&HT);
  send_apps.Get (3)->SetStartTime (Seconds (5.0));
  send_apps.Get (3)->SetStopTime (Seconds (600.0));

  Ptr<WlmSend> send_app_4 = send_apps.Get(4)->GetObject<WlmSend>();
  send_app_4->SetWlm(&wlm[4]);
  send_app_4->SetHT(&HT);
  send_apps.Get (4)->SetStartTime (Seconds (7.0));
  send_apps.Get (4)->SetStopTime (Seconds (600.0));

  WlmReceiveHelper wlm_receive;
  ApplicationContainer receive_apps = wlm_receive.Install (nodes);

  Ptr<WlmReceive> receive_app_0 = receive_apps.Get(0)->GetObject<WlmReceive>();
  receive_app_0->SetWlm(&wlm[0]);
  receive_app_0->SetHT(&HT);
  receive_apps.Get (0)->SetStartTime (Seconds (1.0));
  receive_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<WlmReceive> receive_app_1 = receive_apps.Get(1)->GetObject<WlmReceive>();
  receive_app_1->SetWlm(&wlm[1]);
  receive_app_1->SetHT(&HT);
  receive_apps.Get (1)->SetStartTime (Seconds (1.0));
  receive_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<WlmReceive> receive_app_2 = receive_apps.Get(2)->GetObject<WlmReceive>();
  receive_app_2->SetWlm(&wlm[2]);
  receive_app_2->SetHT(&HT);
  receive_apps.Get (2)->SetStartTime (Seconds (3.0));
  receive_apps.Get (2)->SetStopTime (Seconds (600.0));

  Ptr<WlmReceive> receive_app_3 = receive_apps.Get(3)->GetObject<WlmReceive>();
  receive_app_3->SetWlm(&wlm[3]);
  receive_app_3->SetHT(&HT);
  receive_apps.Get (3)->SetStartTime (Seconds (5.0));
  receive_apps.Get (3)->SetStopTime (Seconds (600.0));

  Ptr<WlmReceive> receive_app_4 = receive_apps.Get(4)->GetObject<WlmReceive>();
  receive_app_4->SetWlm(&wlm[4]);
  receive_app_4->SetHT(&HT);
  receive_apps.Get (4)->SetStartTime (Seconds (7.0));
  receive_apps.Get (4)->SetStopTime (Seconds (600.0));

  DataGeneHelper DG;
  ApplicationContainer dg_apps = DG.Install (nodes);

  Ptr<DataGene> dg_app_0 = dg_apps.Get(0)->GetObject<DataGene>();
  dg_app_0->SetWlm(&wlm[0]);
  dg_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene> dg_app_1 = dg_apps.Get(1)->GetObject<DataGene>();
  dg_app_1->SetWlm(&wlm[1]);
  dg_apps.Get (1)->SetStartTime (Seconds (3.0));
  dg_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene> dg_app_2 = dg_apps.Get(2)->GetObject<DataGene>();
  dg_app_2->SetWlm(&wlm[2]);
  dg_apps.Get (2)->SetStartTime (Seconds (5.0));
  dg_apps.Get (2)->SetStopTime (Seconds (600.0));

  Ptr<DataGene> dg_app_3 = dg_apps.Get(3)->GetObject<DataGene>();
  dg_app_3->SetWlm(&wlm[3]);
  dg_apps.Get (3)->SetStartTime (Seconds (7.0));
  dg_apps.Get (3)->SetStopTime (Seconds (600.0));

  Ptr<DataGene> dg_app_4 = dg_apps.Get(4)->GetObject<DataGene>();
  dg_app_4->SetWlm(&wlm[4]);
  dg_apps.Get (4)->SetStartTime (Seconds (1.0));
  dg_apps.Get (4)->SetStopTime (Seconds (600.0));

  DataGene2Helper DG2;
  ApplicationContainer dg2_apps = DG2.Install (nodes);

  Ptr<DataGene2> dg2_app_0 = dg2_apps.Get(0)->GetObject<DataGene2>();
  dg2_app_0->SetWlm(&wlm[0]);
  dg2_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg2_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene2> dg2_app_1 = dg2_apps.Get(1)->GetObject<DataGene2>();
  dg2_app_1->SetWlm(&wlm[1]);
  dg2_apps.Get (1)->SetStartTime (Seconds (3.0));
  dg2_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene2> dg2_app_2 = dg2_apps.Get(2)->GetObject<DataGene2>();
  dg2_app_2->SetWlm(&wlm[2]);
  dg2_apps.Get (2)->SetStartTime (Seconds (5.0));
  dg2_apps.Get (2)->SetStopTime (Seconds (600.0));

  Ptr<DataGene2> dg2_app_3 = dg2_apps.Get(3)->GetObject<DataGene2>();
  dg2_app_3->SetWlm(&wlm[3]);
  dg2_apps.Get (3)->SetStartTime (Seconds (7.0));
  dg2_apps.Get (3)->SetStopTime (Seconds (600.0));

  Ptr<DataGene2> dg2_app_4 = dg2_apps.Get(4)->GetObject<DataGene2>();
  dg2_app_4->SetWlm(&wlm[4]);
  dg2_apps.Get (4)->SetStartTime (Seconds (1.0));
  dg2_apps.Get (4)->SetStopTime (Seconds (600.0));

  NodeContainer n012 = NodeContainer(nodes.Get(0), nodes.Get(1), nodes.Get(2));
  NodeContainer n013 = NodeContainer(nodes.Get(0), nodes.Get(1), nodes.Get(3));
  NodeContainer n014 = NodeContainer(nodes.Get(0), nodes.Get(1), nodes.Get(4));
  NodeContainer n023 = NodeContainer(nodes.Get(0), nodes.Get(2), nodes.Get(3));
  NodeContainer n024 = NodeContainer(nodes.Get(0), nodes.Get(2), nodes.Get(4));
  NodeContainer n034 = NodeContainer(nodes.Get(0), nodes.Get(3), nodes.Get(4));
  NodeContainer n123 = NodeContainer(nodes.Get(1), nodes.Get(2), nodes.Get(3));
  NodeContainer n124 = NodeContainer(nodes.Get(1), nodes.Get(2), nodes.Get(4));
  HT.SetAppInstall(0,0);
  HT.SetAppInstall(1,0);
  HT.SetAppInstall(2,0);
  HT.SetAppInstall(3,0);
  HT.SetAppInstall(4,0);
  HT.SetAppInstall(0,1);
  HT.SetAppInstall(1,1);
  HT.SetAppInstall(2,1);
  HT.SetAppInstall(3,1);
  HT.SetAppInstall(4,1);

  DataGene3Helper DG3;
  ApplicationContainer dg3_apps = DG3.Install (n012);
  HT.SetAppInstall(0,2);
  HT.SetAppInstall(1,2);
  HT.SetAppInstall(2,2);

  Ptr<DataGene3> dg3_app_0 = dg3_apps.Get(0)->GetObject<DataGene3>();
  dg3_app_0->SetWlm(&wlm[0]);
  dg3_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg3_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene3> dg3_app_1 = dg3_apps.Get(1)->GetObject<DataGene3>();
  dg3_app_1->SetWlm(&wlm[1]);
  dg3_apps.Get (1)->SetStartTime (Seconds (3.0));
  dg3_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene3> dg3_app_2 = dg3_apps.Get(2)->GetObject<DataGene3>();
  dg3_app_2->SetWlm(&wlm[2]);
  dg3_apps.Get (2)->SetStartTime (Seconds (5.0));
  dg3_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene4Helper DG4;
  ApplicationContainer dg4_apps = DG4.Install (n013);
  HT.SetAppInstall(0,3);
  HT.SetAppInstall(1,3);
  HT.SetAppInstall(3,3);

  Ptr<DataGene4> dg4_app_0 = dg4_apps.Get(0)->GetObject<DataGene4>();
  dg4_app_0->SetWlm(&wlm[0]);
  dg4_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg4_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene4> dg4_app_1 = dg4_apps.Get(1)->GetObject<DataGene4>();
  dg4_app_1->SetWlm(&wlm[1]);
  dg4_apps.Get (1)->SetStartTime (Seconds (3.0));
  dg4_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene4> dg4_app_3 = dg4_apps.Get(2)->GetObject<DataGene4>();
  dg4_app_3->SetWlm(&wlm[3]);
  dg4_apps.Get (2)->SetStartTime (Seconds (7.0));
  dg4_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene5Helper DG5;
  ApplicationContainer dg5_apps = DG5.Install (n014);
  HT.SetAppInstall(0,4);
  HT.SetAppInstall(1,4);
  HT.SetAppInstall(4,4);

  Ptr<DataGene5> dg5_app_0 = dg5_apps.Get(0)->GetObject<DataGene5>();
  dg5_app_0->SetWlm(&wlm[0]);
  dg5_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg5_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene5> dg5_app_1 = dg5_apps.Get(1)->GetObject<DataGene5>();
  dg5_app_1->SetWlm(&wlm[1]);
  dg5_apps.Get (1)->SetStartTime (Seconds (3.0));
  dg5_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene5> dg5_app_4 = dg5_apps.Get(2)->GetObject<DataGene5>();
  dg5_app_4->SetWlm(&wlm[4]);
  dg5_apps.Get (2)->SetStartTime (Seconds (1.0));
  dg5_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene6Helper DG6;
  ApplicationContainer dg6_apps = DG6.Install (n023);
  HT.SetAppInstall(0,5);
  HT.SetAppInstall(2,5);
  HT.SetAppInstall(3,5);

  Ptr<DataGene6> dg6_app_0 = dg6_apps.Get(0)->GetObject<DataGene6>();
  dg6_app_0->SetWlm(&wlm[0]);
  dg6_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg6_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene6> dg6_app_2 = dg6_apps.Get(1)->GetObject<DataGene6>();
  dg6_app_2->SetWlm(&wlm[2]);
  dg6_apps.Get (1)->SetStartTime (Seconds (5.0));
  dg6_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene6> dg6_app_3 = dg6_apps.Get(2)->GetObject<DataGene6>();
  dg6_app_3->SetWlm(&wlm[3]);
  dg6_apps.Get (2)->SetStartTime (Seconds (7.0));
  dg6_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene7Helper DG7;
  ApplicationContainer dg7_apps = DG7.Install (n024);
  HT.SetAppInstall(0,6);
  HT.SetAppInstall(2,6);
  HT.SetAppInstall(4,6);

  Ptr<DataGene7> dg7_app_0 = dg7_apps.Get(0)->GetObject<DataGene7>();
  dg7_app_0->SetWlm(&wlm[0]);
  dg7_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg7_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene7> dg7_app_2 = dg7_apps.Get(1)->GetObject<DataGene7>();
  dg7_app_2->SetWlm(&wlm[2]);
  dg7_apps.Get (1)->SetStartTime (Seconds (5.0));
  dg7_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene7> dg7_app_4 = dg7_apps.Get(2)->GetObject<DataGene7>();
  dg7_app_4->SetWlm(&wlm[4]);
  dg7_apps.Get (2)->SetStartTime (Seconds (1.0));
  dg7_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene8Helper DG8;
  ApplicationContainer dg8_apps = DG8.Install (n034);
  HT.SetAppInstall(0,7);
  HT.SetAppInstall(3,7);
  HT.SetAppInstall(4,7);

  Ptr<DataGene8> dg8_app_0 = dg8_apps.Get(0)->GetObject<DataGene8>();
  dg8_app_0->SetWlm(&wlm[0]);
  dg8_apps.Get (0)->SetStartTime (Seconds (1.0));
  dg8_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene8> dg8_app_3 = dg8_apps.Get(1)->GetObject<DataGene8>();
  dg8_app_3->SetWlm(&wlm[3]);
  dg8_apps.Get (1)->SetStartTime (Seconds (7.0));
  dg8_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene8> dg8_app_4 = dg8_apps.Get(2)->GetObject<DataGene8>();
  dg8_app_4->SetWlm(&wlm[4]);
  dg8_apps.Get (2)->SetStartTime (Seconds (1.0));
  dg8_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene9Helper DG9;
  ApplicationContainer dg9_apps = DG9.Install (n123);
  HT.SetAppInstall(1,8);
  HT.SetAppInstall(2,8);
  HT.SetAppInstall(3,8);

  Ptr<DataGene9> dg9_app_1 = dg9_apps.Get(0)->GetObject<DataGene9>();
  dg9_app_1->SetWlm(&wlm[1]);
  dg9_apps.Get (0)->SetStartTime (Seconds (3.0));
  dg9_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene9> dg9_app_2 = dg9_apps.Get(1)->GetObject<DataGene9>();
  dg9_app_2->SetWlm(&wlm[2]);
  dg9_apps.Get (1)->SetStartTime (Seconds (5.0));
  dg9_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene9> dg9_app_3 = dg9_apps.Get(2)->GetObject<DataGene9>();
  dg9_app_3->SetWlm(&wlm[3]);
  dg9_apps.Get (2)->SetStartTime (Seconds (7.0));
  dg9_apps.Get (2)->SetStopTime (Seconds (600.0));

  DataGene10Helper DG10;
  ApplicationContainer dg10_apps = DG10.Install (n124);
  HT.SetAppInstall(1,9);
  HT.SetAppInstall(2,9);
  HT.SetAppInstall(4,9);

  Ptr<DataGene10> dg10_app_1 = dg10_apps.Get(0)->GetObject<DataGene10>();
  dg10_app_1->SetWlm(&wlm[1]);
  dg10_apps.Get (0)->SetStartTime (Seconds (3.0));
  dg10_apps.Get (0)->SetStopTime (Seconds (600.0));

  Ptr<DataGene10> dg10_app_2 = dg10_apps.Get(1)->GetObject<DataGene10>();
  dg10_app_2->SetWlm(&wlm[2]);
  dg10_apps.Get (1)->SetStartTime (Seconds (5.0));
  dg10_apps.Get (1)->SetStopTime (Seconds (600.0));

  Ptr<DataGene10> dg10_app_4 = dg10_apps.Get(2)->GetObject<DataGene10>();
  dg10_app_4->SetWlm(&wlm[4]);
  dg10_apps.Get (2)->SetStartTime (Seconds (1.0));
  dg10_apps.Get (2)->SetStopTime (Seconds (600.0));

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (600.0));
  Simulator::Run ();
  Simulator::Destroy ();

  NS_LOG_INFO ("Count of original solution: " << HT.GetCOrigin());
  NS_LOG_INFO ("Count of Water Level Matrix solution: " << HT.GetCWlm());

  return 0;
}
