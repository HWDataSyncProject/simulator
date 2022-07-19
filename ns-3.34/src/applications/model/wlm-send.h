/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef WLM_SEND_H
#define WLM_SEND_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

#include "ns3/wlmatrix.h"
#include "ns3/hyperterminal.h"

namespace ns3 {

class Socket;
class Packet;

class WlmSend : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  WlmSend ();
  virtual ~WlmSend ();

  void SetWlm (WLMatrix* wlm_global);
  void SetHT (HyperTerminal* HT_global);

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void Send (void);
  void RegularSend (void);
  void Print_wlm (uint32_t p_flag);

  WLMatrix* wlm;
  HyperTerminal* HT;

  Time period; //!< Period of water level matrix transmission

  uint32_t seq;
  Ptr<Socket> m_socket_send; //!< Socket
  EventId m_sendEvent; //!< Event to send the next packet
  EventId m_sendEvent_regular;
};

} // namespace ns3

#endif /* WLM_SEND_H */
