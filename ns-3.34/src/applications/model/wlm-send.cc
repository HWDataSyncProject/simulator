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

#include "wlm-send.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WlmSend");

NS_OBJECT_ENSURE_REGISTERED (WlmSend);

TypeId
WlmSend::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WlmSend")
    .SetParent<Application> ()
    .SetGroupName("Applications")
    .AddConstructor<WlmSend> ()
    .AddAttribute ("Period",
                   "The period of water level matrix push.", TimeValue (Seconds (2.0)),
                   MakeTimeAccessor (&WlmSend::period),
                   MakeTimeChecker ())
  ;
  return tid;
}

WlmSend::WlmSend ()
{
  NS_LOG_FUNCTION (this);

  wlm = NULL;

  period = Seconds (2.0);

  seq = 0;
  m_socket_send = 0;
  m_sendEvent = EventId ();
  m_sendEvent_regular = EventId ();
}

WlmSend::~WlmSend ()
{
  NS_LOG_FUNCTION (this);
  wlm = NULL;
}

void
WlmSend::SetWlm (WLMatrix* wlm_global)
{
  wlm = wlm_global;
}

void 
WlmSend::SetHT (HyperTerminal* HT_global)
{
  HT = HT_global;
}

void
WlmSend::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
WlmSend::StartApplication (void)
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

  m_sendEvent = Simulator::Schedule (Seconds (0.0), &WlmSend::Send, this);
}

void
WlmSend::StopApplication (void)
{
  NS_LOG_FUNCTION (this);
  Simulator::Cancel (m_sendEvent);
  Simulator::Cancel (m_sendEvent_regular);
}

void
WlmSend::Send (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_sendEvent.IsExpired ());

  if (!wlm->GetIsInitialized ())
    {
      for(uint32_t i = 0; i < HT->GetDeviceCount(); i++)
        {
          if( (wlm->GetNodeID() != i) && (HT->GetIsOnline(i) == true) && !wlm->GetInitFlag(i) )
            {
              wlm->SetRespFlag(i,true);
              if (m_socket_send == 0)
                {
                  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
                  m_socket_send = Socket::CreateSocket (GetNode (), tid);
                  if (Ipv4Address::IsMatchingType(HT->GetAddr(i)) == true)
                    {
                      if (m_socket_send->Bind () == -1)
                        {
                          NS_FATAL_ERROR ("Failed to bind socket");
                        }
                      m_socket_send->Connect (InetSocketAddress (HT->GetAddr(i), HT->GetPort(i)));
                    }
                  else if (Ipv6Address::IsMatchingType(HT->GetAddr(i)) == true)
                    {
                      if (m_socket_send->Bind6 () == -1)
                        {
                          NS_FATAL_ERROR ("Failed to bind socket");
                        }
                      m_socket_send->Connect (Inet6SocketAddress (Ipv6Address::MakeIpv4MappedAddress(HT->GetAddr(i)), HT->GetPort(i)));
                    }
                  else
                    {
                      NS_ASSERT_MSG (false, "Incompatible address type: " << HT->GetAddr(i));
                    }
                }
              m_socket_send->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
              m_socket_send->SetAllowBroadcast (true);

              SeqTsHeader seqTs;
              seqTs.SetSeq (seq++);

              uint8_t buf[165];
              buf[0] = 'R';
              buf[1] = 'Q';
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

              std::stringstream peerAddressStringStream;
              if (Ipv4Address::IsMatchingType (HT->GetAddr(i)))
                {
                  peerAddressStringStream << HT->GetAddr(i);
                }
              else if (Ipv6Address::IsMatchingType (HT->GetAddr(i)))
                {
                  peerAddressStringStream << HT->GetAddr(i);
                }

              if ((m_socket_send->Send (p)) >= 0)
                {
                  NS_LOG_INFO ("Req Init: Node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str ()
                  << "; Uid: " << p->GetUid () 
                  << "; Time: " << (Simulator::Now ()).GetSeconds ()
                  << ".");
                }
              else
                {
                  NS_LOG_INFO ("Error while node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str ());
                }

              m_socket_send = 0;
            }
        }

      Print_wlm(1);

      m_sendEvent = Simulator::Schedule (Seconds (0.2), &WlmSend::Send, this);
    }
  else
    {
      m_sendEvent_regular = Simulator::Schedule (Seconds (0.5), &WlmSend::RegularSend, this);
    }
}

void
WlmSend::RegularSend (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (m_sendEvent_regular.IsExpired ());

  if(HT->GetIsOnline(wlm->GetNodeID()))
    {
      for(uint32_t i = 0; i < HT->GetDeviceCount(); i++)
        {
          if( (wlm->GetNodeID() != i) && (HT->GetIsOnline(i) == true) )
            {
              if (m_socket_send == 0)
                {
                  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
                  m_socket_send = Socket::CreateSocket (GetNode (), tid);
                  if (Ipv4Address::IsMatchingType(HT->GetAddr(i)) == true)
                    {
                      if (m_socket_send->Bind () == -1)
                        {
                          NS_FATAL_ERROR ("Failed to bind socket");
                        }
                      m_socket_send->Connect (InetSocketAddress (HT->GetAddr(i), HT->GetPort(i)));
                    }
                  else if (Ipv6Address::IsMatchingType(HT->GetAddr(i)) == true)
                    {
                      if (m_socket_send->Bind6 () == -1)
                        {
                          NS_FATAL_ERROR ("Failed to bind socket");
                        }
                      m_socket_send->Connect (Inet6SocketAddress (Ipv6Address::MakeIpv4MappedAddress(HT->GetAddr(i)), HT->GetPort(i)));
                    }
                  else
                    {
                      NS_ASSERT_MSG (false, "Incompatible address type: " << HT->GetAddr(i));
                    }
                }
              m_socket_send->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
              m_socket_send->SetAllowBroadcast (true);

              SeqTsHeader seqTs;
              seqTs.SetSeq (seq++);

              uint8_t buf[805];
              buf[0] = 'R';
              buf[1] = 'G';
              buf[2] = 0;
              buf[3] = 0;
              uint32_t idx = 4;
              for(uint32_t i = 0; i < wlm->GetNodeNumber (); i++)
                {
                  for(uint32_t j = 0; j < wlm->GetAppNumber (); j++)
                    {
                      for(uint32_t k = 0; k < wlm->GetDateTypeNumber (); k++)
                        {
                          buf[idx] = wlm->GetWLMatrix (i, j, k) >> 24;
                          buf[idx+1] = wlm->GetWLMatrix (i, j, k) >> 16;
                          buf[idx+2] = wlm->GetWLMatrix (i, j, k) >> 8;
                          buf[idx+3] = wlm->GetWLMatrix (i, j, k);
                          idx += 4;
                        }
                    }
                }
              buf[idx] = '\0';

              Ptr<Packet> p = Create<Packet> (buf, sizeof(buf));
              p->AddHeader (seqTs);

              std::stringstream peerAddressStringStream;
              if (Ipv4Address::IsMatchingType (HT->GetAddr(i)))
                {
                  peerAddressStringStream << HT->GetAddr(i);
                }
              else if (Ipv6Address::IsMatchingType (HT->GetAddr(i)))
                {
                  peerAddressStringStream << HT->GetAddr(i);
                }

              if ((m_socket_send->Send (p)) >= 0)
                {
                  NS_LOG_INFO ("Reg Push: Node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str ()
                  << "; Uid: " << p->GetUid () 
                  << "; Time: " << (Simulator::Now ()).GetSeconds () 
                  << ".");
                }
              else
                {
                  NS_LOG_INFO ("Error while node " << wlm->GetNodeID () << " send " << idx << " bytes to " << peerAddressStringStream.str ());
                }

              m_socket_send = 0;
            }
        }
      Print_wlm(0);
    }
  else
    {
      NS_LOG_INFO ("Node " << wlm->GetNodeID() << " is offline.");
    }

  if (wlm->GetIsInitialized ())
    {
      m_sendEvent_regular = Simulator::Schedule (period, &WlmSend::RegularSend, this);
    }
}

void 
WlmSend::Print_wlm (uint32_t p_flag)
{
  if(p_flag == 0)
    {
      std::cout << "Node " << wlm->GetNodeID () << " regular push:" << std::endl;
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
      std::cout << "Node " << wlm->GetNodeID () << " sends the data of this node:" << std::endl;
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
