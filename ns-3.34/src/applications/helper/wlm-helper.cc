/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "wlm-helper.h"
#include "ns3/wlm-send.h"
#include "ns3/wlm-receive.h"
#include "ns3/udp-trace-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

WlmSendHelper::WlmSendHelper ()
{
  m_factory.SetTypeId (WlmSend::GetTypeId ());
}

WlmSendHelper::WlmSendHelper (Time period)
{
  m_factory.SetTypeId (WlmSend::GetTypeId ());
  SetAttribute ("Period", TimeValue (period));
}

void
WlmSendHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
WlmSendHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  uint32_t idx = 0;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      Ptr<WlmSend> client = m_factory.Create<WlmSend> ();
      node->AddApplication (client);
      apps.Add (client);

      idx += 1;
    }
  return apps;
}


WlmReceiveHelper::WlmReceiveHelper ()
{
  m_factory.SetTypeId (WlmReceive::GetTypeId ());
}

void
WlmReceiveHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
WlmReceiveHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  uint32_t idx = 0;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      Ptr<WlmReceive> server = m_factory.Create<WlmReceive> ();
      node->AddApplication (server);
      apps.Add (server);

      idx += 1;
    }
  return apps;
}

} // namespace ns3
