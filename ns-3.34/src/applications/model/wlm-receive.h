/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef WLM_RECEIVE_H
#define WLM_RECEIVE_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/random-variable-stream.h"

#include "ns3/wlmatrix.h"
#include "ns3/hyperterminal.h"

namespace ns3 {

class Socket;
class Packet;

class WlmReceive : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  WlmReceive ();
  virtual ~WlmReceive ();

  void SetWlm (WLMatrix* wlm_global);
  void SetHT (HyperTerminal* HT_global);

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void HandleRead (Ptr<Socket> socket);
  void SendRP (Address from);
  void Online (void);
  void Print_wlm (uint32_t p_flag);

  WLMatrix* wlm;
  HyperTerminal* HT;

  uint32_t seq;

  Ptr<Socket> m_socket_send; //!< Socket
  Ptr<Socket> m_socket_receive; //!< IPv4 Socket
  Ptr<Socket> m_socket_receive_6; //!< IPv6 Socket
  EventId m_sendEvent; //!< Event to send the next packet

  bool rec_flag[5];
  bool rec_flag_t;
  EventId m_leaveEvent;
};

} // namespace ns3

#endif /* WLM_RECEIVE_H */
