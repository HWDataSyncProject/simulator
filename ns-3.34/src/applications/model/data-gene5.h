/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef DATA_GENE5_H
#define DATA_GENE5_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

#include "ns3/wlmatrix.h"

namespace ns3 {

class Socket;
class Packet;

/**
 * \ingroup DataGene5server
 *
 * \brief A Udp client. Sends UDP packet carrying sequence number and time stamp
 *  in their payloads
 *
 */
class DataGene5 : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

  DataGene5 ();

  void SetWlm (WLMatrix* wlm_global);

  virtual ~DataGene5 ();

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void Gene (void);
  void Trans (uint32_t num_byte);
  //void Trans2 (uint32_t num_byte);
  int  Possion (int probaility);
  int  Berno(double p);
  uint32_t time_ctrl;
  uint32_t bytes;
  
  char src1;
  char des1;
  uint32_t time_s;
  uint32_t num_byte;
  WLMatrix* wlm;
  EventId m_sendEvent; //!< Event to send the next packet

};

} // namespace ns3

#endif /* DATA_GENE5_H */