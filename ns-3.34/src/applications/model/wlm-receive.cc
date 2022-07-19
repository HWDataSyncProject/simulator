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
#include <cstdlib>
#include <cstdio>
#include <iomanip>

#include "wlm-receive.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WlmReceive");

NS_OBJECT_ENSURE_REGISTERED (WlmReceive);

TypeId
WlmReceive::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WlmReceive")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<WlmReceive> ()
  ;
  return tid;
}

WlmReceive::WlmReceive ()
{
  NS_LOG_FUNCTION (this);

  wlm = NULL;

  seq = 0;

  m_socket_send = 0;
  m_socket_receive = 0;
  m_socket_receive_6 = 0;
  m_sendEvent = EventId ();

  for(uint32_t i = 0; i < 5; i++)
    {
      rec_flag[i] = false;
    }
  rec_flag_t = true;
  m_leaveEvent = EventId ();
}

WlmReceive::~WlmReceive ()
{
  NS_LOG_FUNCTION (this);
}

void
WlmReceive::SetWlm (WLMatrix* wlm_global)
{
  wlm = wlm_global;
}

void 
WlmReceive::SetHT (HyperTerminal* HT_global)
{
  HT = HT_global;
}

void
WlmReceive::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
WlmReceive::StartApplication (void)
{
  NS_LOG_FUNCTION (this);
  if(wlm->GetNodeID() >= 100)
  {
    uint32_t n_id = HT->Join(GetNode()->GetObject<Ipv4>()->GetAddress(1,0).GetLocal());
    wlm->SetNodeID(n_id);

    if( wlm->GetNodeID() != (wlm->GetNodeNumber() - 1) )
      {
        for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
          {
            for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
              {
                int64_t ts_temp = wlm->GetWLMatrix (wlm->GetNodeNumber() - 1, j, k);
                if(ts_temp != 0)
                  {
                    wlm->SetWLMatrix (wlm->GetNodeID(), j, k, ts_temp);
                    wlm->SetWLMatrix (wlm->GetNodeNumber() - 1, j, k, 0);
                  }
              }
          }
      }
  }

  if (m_socket_receive == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket_receive = Socket::CreateSocket (GetNode (), tid);
      InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), HT->GetPort(wlm->GetNodeID()) );
      if (m_socket_receive->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
    }
  m_socket_receive->SetRecvCallback (MakeCallback (&WlmReceive::HandleRead, this));

  if (m_socket_receive_6 == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket_receive_6 = Socket::CreateSocket (GetNode (), tid);
      Inet6SocketAddress local = Inet6SocketAddress (Ipv6Address::GetAny (), HT->GetPort(wlm->GetNodeID()) );
      if (m_socket_receive_6->Bind (local) == -1)
        {
          NS_FATAL_ERROR ("Failed to bind socket");
        }
    }
  m_socket_receive_6->SetRecvCallback (MakeCallback (&WlmReceive::HandleRead, this));
}

void
WlmReceive::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
  if (m_socket_receive != 0)
    {
      m_socket_receive->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
  if (m_socket_receive_6 != 0)
    {
      m_socket_receive_6->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
    }
}

void
WlmReceive::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> packet;
  Address from;

  while ((packet = socket->RecvFrom (from)))
    {
      if (packet->GetSize () > 0)
        {
          SeqTsHeader seqTs;
          packet->RemoveHeader (seqTs);
          uint32_t currentSequenceNumber = seqTs.GetSeq ();

          packet->RemoveAllPacketTags ();
          packet->RemoveAllByteTags ();

          uint8_t buf[805];
          packet->CopyData(buf, sizeof(buf));

          if( (buf[0] == 'R' && buf[1] == 'G') && wlm->GetIsInitialized () )
            {
              uint32_t idx = 4;
              for(uint32_t i = 0; i < wlm->GetNodeNumber (); i++)
                {
                  if(wlm->GetNodeID() == i)
                    {
                      idx += 4 * wlm->GetAppNumber () * wlm->GetDateTypeNumber ();
                    }
                  else
                    {
                      for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
                        {
                          for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
                            {
                              int64_t wl_temp = (buf[idx] << 24) + (buf[idx+1] << 16) + (buf[idx+2] << 8) + buf[idx+3];
                              if(wlm->GetWLMatrix(i, j, k) < wl_temp)
                                {
                                  wlm->SetWLMatrix(i, j, k, wl_temp);
                                }
                              idx += 4;
                            }
                        }
                    }
                }
              if (InetSocketAddress::IsMatchingType (from))
                {
                  NS_LOG_INFO ("Rec Push: Node " << wlm->GetNodeID () << " receive " << idx 
                  << " bytes from " << InetSocketAddress::ConvertFrom (from).GetIpv4 () 
                  << "; Port " << InetSocketAddress::ConvertFrom (from).GetPort ()
                  << "; Sequence Number: " << currentSequenceNumber 
                  << "; Uid: " << packet->GetUid () 
                  << "; TXtime: " << seqTs.GetTs () 
                  << "; RXtime: " << (Simulator::Now ()).GetSeconds () 
                  << "; Delay: " << (Simulator::Now () - seqTs.GetTs ()).GetSeconds ()
                  << ".");
                }
              else if (Inet6SocketAddress::IsMatchingType (from))
                {
                  NS_LOG_INFO ("Rec Push: Node " << wlm->GetNodeID () << " receive " << idx 
                  << " bytes from " << Inet6SocketAddress::ConvertFrom (from).GetIpv6 () 
                  << "; Port " << Inet6SocketAddress::ConvertFrom (from).GetPort ()
                  << "; Sequence Number: " << currentSequenceNumber 
                  << "; Uid: " << packet->GetUid () 
                  << "; TXtime: " << seqTs.GetTs () 
                  << "; RXtime: " << (Simulator::Now ()).GetSeconds () 
                  << "; Delay: " << (Simulator::Now () - seqTs.GetTs ()).GetSeconds ()
                  << ".");
                }
              Print_wlm(0);

              if(!HT->GetLeaveFlag()&&((Simulator::Now ()).GetSeconds ()>10))
                {
                  Ptr<UniformRandomVariable> uv=CreateObject<UniformRandomVariable> ();
                  uv->SetAttribute ("Min", DoubleValue(0));
                  uv->SetAttribute ("Max", DoubleValue(10));
                  if(uv->GetValue () < 4)
                    {
                      HT->SetIsOnline(wlm->GetNodeID(),false);
                      for(uint32_t n_id = 0; n_id < HT->GetDeviceCount(); n_id++)
                        {
                          HT->SetIsAwake(n_id,false);
                        }
                      HT->SetLeaveFlag(true);
                      NS_LOG_INFO ("Offline: Node " << wlm->GetNodeID() << " is offline.");
                      for(uint32_t i = 0; i < 5; i++)
                        {
                          rec_flag[i] = false;
                        }
                      rec_flag_t = false;
                      Ptr<UniformRandomVariable> uv2=CreateObject<UniformRandomVariable> ();
                      uv2->SetAttribute ("Min", DoubleValue(5));
                      uv2->SetAttribute ("Max", DoubleValue(10));
                      m_leaveEvent = Simulator::Schedule (Seconds(uv2->GetValue()), &WlmReceive::Online, this);
                    }
                }
              if(!rec_flag_t)
                {
                  uint32_t n_id_temp = HT->GetNodeId( InetSocketAddress::ConvertFrom (from).GetIpv4 () );
                  rec_flag[n_id_temp] = true;
                  bool rec_flag_temp = true;
                  for(uint32_t i = 0; i < 5; i++)
                    {
                      if(i == wlm->GetNodeID())
                        {
                          continue;
                        }
                      if(rec_flag[i] == false)
                        {
                          rec_flag_temp = false;
                        }
                    }
                  if(rec_flag_temp)
                    {
                      rec_flag_t = true;

                      uint32_t c_temp = 0;
                      for(uint32_t n_id = 0; n_id < HT->GetDeviceCount(); n_id++)
                        {
                          bool c_temp_flag = false;
                          for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
                            {
                              if(!HT->GetAppInstall(wlm->GetNodeID(),j))
                                {
                                  continue;
                                }

                              for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
                                {
                                  if(wlm->GetWLMatrix (n_id, j, k) > wlm->GetWLMatrix (wlm->GetNodeID(), j, k))
                                    {
                                      c_temp_flag = true;
                                      wlm->SetWLMatrix (wlm->GetNodeID(), j, k, wlm->GetWLMatrix (n_id, j, k));
                                    }
                                }
                            }
                          if(c_temp_flag)
                            {
                              HT->SetIsAwake(n_id,true);
                              c_temp += 1;
                            }
                        }
                      HT->SetLeaveFlag(false);
                      HT->SetCOrigin(4);
                      HT->SetCWlm(c_temp);
                      NS_LOG_INFO ("Count of original solution: " << HT->GetCOrigin());
                      NS_LOG_INFO ("Count of Water Level Matrix solution: " << HT->GetCWlm());
                    }
                }
            }
          else if(buf[0] == 'R' && buf[1] == 'Q')
            {
              uint16_t peer_node_id = (buf[2] << 8) + buf[3];
              uint32_t idx = 4;
              for(uint32_t i = 0; i < wlm->GetAppNumber (); i++)
                {
                  for(uint32_t j = 0; j < wlm->GetDateTypeNumber (); j++)
                    {
                      wlm->SetWLMatrix(peer_node_id, i, j, (buf[idx] << 24) + (buf[idx+1] << 16) + (buf[idx+2] << 8) + buf[idx+3]);
                      idx += 4;
                    }
                }

              if (InetSocketAddress::IsMatchingType (from))
                {
                  NS_LOG_INFO ("Rec Init: Node " << wlm->GetNodeID () << " receive " << idx 
                  << " bytes from " << InetSocketAddress::ConvertFrom (from).GetIpv4 () 
                  << " port " << InetSocketAddress::ConvertFrom (from).GetPort ()
                  << "; Sequence Number: " << currentSequenceNumber 
                  << "; Uid: " << packet->GetUid () 
                  << "; TXtime: " << seqTs.GetTs () 
                  << "; RXtime: " << (Simulator::Now ()).GetSeconds () 
                  << "; Delay: " << (Simulator::Now () - seqTs.GetTs ()).GetSeconds ()
                  << ".");
                }
              else if (Inet6SocketAddress::IsMatchingType (from))
                {
                  NS_LOG_INFO ("Rec Init: Node " << wlm->GetNodeID () << " receive " << idx 
                  << " bytes from " << Inet6SocketAddress::ConvertFrom (from).GetIpv6 () 
                  << " port " << Inet6SocketAddress::ConvertFrom (from).GetPort ()
                  << "; Sequence Number: " << currentSequenceNumber 
                  << "; Uid: " << packet->GetUid () 
                  << "; TXtime: " << seqTs.GetTs () 
                  << "; RXtime: " << (Simulator::Now ()).GetSeconds () 
                  << "; Delay: " << (Simulator::Now () - seqTs.GetTs ()).GetSeconds ()
                  << ".");
                }
              SendRP(from);
              Print_wlm(1);
            }
          else if( (buf[0] == 'R' && buf[1] == 'P') && !wlm->GetIsInitialized () )
            {
              uint16_t peer_node_id = (buf[2] << 8) + buf[3];
              uint32_t idx = 4;
              for(uint32_t i = 0; i < wlm->GetAppNumber (); i++)
                {
                  for(uint32_t j = 0; j < wlm->GetDateTypeNumber (); j++)
                    {
                      wlm->SetWLMatrix(peer_node_id, i, j, (buf[idx] << 24) + (buf[idx+1] << 16) + (buf[idx+2] << 8) + buf[idx+3]);
                      idx += 4;
                    }
                }
              if(!wlm->GetInitFlag(peer_node_id))
                {
                  wlm->SetInitFlag (peer_node_id, true);
                  wlm->SetRespFlag (peer_node_id, false);
                }
              if(!wlm->GetRespFlag())
                {
                  wlm->SetIsInitialized ();
                }

              if (InetSocketAddress::IsMatchingType (from))
                {
                  NS_LOG_INFO ("Rec Resp: Node " << wlm->GetNodeID () << " receive " << idx 
                  << " bytes from " << InetSocketAddress::ConvertFrom (from).GetIpv4 () 
                  << " port " << InetSocketAddress::ConvertFrom (from).GetPort ()
                  << "; Sequence Number: " << currentSequenceNumber 
                  << "; Uid: " << packet->GetUid () 
                  << "; TXtime: " << seqTs.GetTs () 
                  << "; RXtime: " << (Simulator::Now ()).GetSeconds () 
                  << "; Delay: " << (Simulator::Now () - seqTs.GetTs ()).GetSeconds ()
                  << ".");
                }
              else if (Inet6SocketAddress::IsMatchingType (from))
                {
                  NS_LOG_INFO ("Rec Resp: Node " << wlm->GetNodeID () << " receive " << idx 
                  << " bytes from " << Inet6SocketAddress::ConvertFrom (from).GetIpv6 () 
                  << " port " << Inet6SocketAddress::ConvertFrom (from).GetPort ()
                  << "; Sequence Number: " << currentSequenceNumber 
                  << "; Uid: " << packet->GetUid () 
                  << "; TXtime: " << seqTs.GetTs () 
                  << "; RXtime: " << (Simulator::Now ()).GetSeconds () 
                  << "; Delay: " << (Simulator::Now () - seqTs.GetTs ()).GetSeconds ()
                  << ".");
                }
              Print_wlm(0);
            }
        }
    }
}

void 
WlmReceive::SendRP (Address from)
{
  SeqTsHeader seqTs;
  seqTs.SetSeq (seq++);

  uint8_t buf[165];
  buf[0] = 'R';
  buf[1] = 'P';
  buf[2] = wlm->GetNodeID() >> 8;
  buf[3] = wlm->GetNodeID();
  uint32_t idx = 4;
  for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
    {
      for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
        {
          buf[idx] = wlm->GetWLMatrix (wlm->GetNodeID(), j, k) >> 24;
          buf[idx+1] = wlm->GetWLMatrix (wlm->GetNodeID(), j, k) >> 16;
          buf[idx+2] = wlm->GetWLMatrix (wlm->GetNodeID(), j, k) >> 8;
          buf[idx+3] = wlm->GetWLMatrix (wlm->GetNodeID(), j, k);
          idx += 4;
        }
    }
  buf[idx] = '\0';

  Ptr<Packet> p = Create<Packet> (buf, sizeof(buf));
  p->AddHeader (seqTs);

  uint16_t peer_port = HT->GetPort(InetSocketAddress::ConvertFrom(from).GetIpv4());
  std::stringstream peerAddressStringStream;
  if (InetSocketAddress::IsMatchingType (from))
    {
      if (m_socket_send == 0)
        {
          TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
          m_socket_send = Socket::CreateSocket (GetNode (), tid);
          if (m_socket_send->Bind () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
          m_socket_send->Connect (InetSocketAddress (InetSocketAddress::ConvertFrom (from).GetIpv4 (), peer_port));
        }
      m_socket_send->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_socket_send->SetAllowBroadcast (true);

      peerAddressStringStream << Ipv4Address::ConvertFrom (InetSocketAddress::ConvertFrom (from).GetIpv4 ());

      if ((m_socket_send->Send (p)) >= 0)
        {
          NS_LOG_INFO ("Resp to Init: Node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str () 
          << "; Uid: " << p->GetUid () 
          << "; Time: " << (Simulator::Now ()).GetSeconds ()
          << ".");
        }
      else
        {
          NS_LOG_INFO ("Error while node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str ());
        }
    }
  else if (Inet6SocketAddress::IsMatchingType (from))
    {
      if (m_socket_send == 0)
        {
          TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
          m_socket_send = Socket::CreateSocket (GetNode (), tid);
          if (m_socket_send->Bind6 () == -1)
            {
              NS_FATAL_ERROR ("Failed to bind socket");
            }
          m_socket_send->Connect (Inet6SocketAddress (Inet6SocketAddress::ConvertFrom (from).GetIpv6 (), peer_port));
        }
      m_socket_send->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
      m_socket_send->SetAllowBroadcast (true);

      peerAddressStringStream << Ipv6Address::ConvertFrom (Inet6SocketAddress::ConvertFrom (from).GetIpv6 ());

      if ((m_socket_send->Send (p)) >= 0)
        {
          NS_LOG_INFO ("Resp to Init: Node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str () 
          << "; Uid: " << p->GetUid () 
          << "; Time: " << (Simulator::Now ()).GetSeconds ()
          << ".");
        }
      else
        {
          NS_LOG_INFO ("Error while node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str ());
        }
    }

  m_socket_send =  0;
}

void 
WlmReceive::Online (void)
{
  NS_LOG_FUNCTION (this);

  HT->SetIsOnline(wlm->GetNodeID(),true);
  NS_LOG_INFO ("Online: Node " << wlm->GetNodeID() << " is online.");
}


void 
WlmReceive::Print_wlm (uint32_t p_flag)
{
  if(p_flag == 0)
    {
      std::cout << "Node " << wlm->GetNodeID () << " updates after receiving the push:" << std::endl;
      std::cout << "===========================================================================================================================" << std::endl;
      std::cout << "App";
      for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
        {
          if(j < 9)
            {
              std::cout << "     ";
            }
          else
            {
              std::cout << "    ";
            }
          std::cout << j+1;
        }
      std::cout << std::endl;
      for(uint32_t i = 0; i < wlm->GetNodeNumber (); i++)
        {
          for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
            {
              if(k == 0)
                {
                  std::cout << " " << i << " ";
                }
              else
                {
                  std::cout << "   ";
                }
              for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
                {
                  double ts_temp = double( wlm->GetWLMatrix (i, j, k) ) / 1000;
                  std::cout << std::setw(6) << ts_temp;
                }
              std::cout << std::endl;
            }
        }
      std::cout << "===========================================================================================================================" << std::endl;
    }
  else if (p_flag == 1)
    {
      std::cout << "Node " << wlm->GetNodeID () << " returns the data of this node:" << std::endl;
      std::cout << "===========================================================================================================================" << std::endl;
      std::cout << "App";
      for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
        {
          if(j < 9)
            {
              std::cout << "     ";
            }
          else
            {
              std::cout << "    ";
            }
          std::cout << j+1;
        }
      std::cout << std::endl;
      for(uint32_t i = 0; i < wlm->GetNodeNumber (); i++)
        {
          if(i == wlm->GetNodeID())
            {
              for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
                {
                  if(k == 0)
                    {
                      std::cout << " " << i << " ";
                    }
                  else
                    {
                      std::cout << "   ";
                    }
                  for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
                    {
                      double ts_temp = double( wlm->GetWLMatrix (i, j, k) ) / 1000;
                      std::cout << std::setw(6) << ts_temp;
                    }
                  std::cout << std::endl;
                }
            }
        }
      std::cout << "===========================================================================================================================" << std::endl;

    }
}

} // Namespace ns3
