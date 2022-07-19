/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef DATA_GENE_H
#define DATA_GENE_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-address.h"

#include "ns3/wlmatrix.h"

namespace ns3 {

class Socket;
class Packet;

class DataGene : public Application
{
public:
  static TypeId GetTypeId (void);

  DataGene ();

  void SetWlm (WLMatrix* wlm_global);

  virtual ~DataGene ();

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void Gene (void);
  void Trans (uint32_t num_byte);
  void Trans2 (uint32_t num_byte);
  int  Possion (int probaility);
  
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

#endif /* DATA_GENE_H */
