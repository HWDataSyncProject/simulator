/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef WLM_HELPER_H
#define WLM_HELPER_H

#include <stdint.h>

#include "ns3/core-module.h"
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/wlm-send.h"
#include "ns3/wlm-receive.h"

#include "ns3/wlmatrix.h"

namespace ns3 {

class WlmSendHelper
{
public:
  WlmSendHelper ();
  WlmSendHelper (Time period);

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
};

class WlmReceiveHelper
{
public:
  WlmReceiveHelper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
};

} // namespace ns3

#endif /* WLM_HELPER_H */
