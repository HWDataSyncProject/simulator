/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "data-gene-helper.h"
#include "ns3/data-gene.h"
#include "ns3/data-gene2.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

DataGeneHelper::DataGeneHelper ()
{
  m_factory.SetTypeId (DataGene::GetTypeId ());
}

void
DataGeneHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGeneHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene2Helper::DataGene2Helper ()
{
  m_factory.SetTypeId (DataGene2::GetTypeId ());
}

void
DataGene2Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene2Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene2> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene3Helper::DataGene3Helper ()
{
  m_factory.SetTypeId (DataGene3::GetTypeId ());
}

void
DataGene3Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene3Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene3> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene4Helper::DataGene4Helper ()
{
  m_factory.SetTypeId (DataGene4::GetTypeId ());
}

void
DataGene4Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene4Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene4> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene5Helper::DataGene5Helper ()
{
  m_factory.SetTypeId (DataGene5::GetTypeId ());
}

void
DataGene5Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene5Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene5> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene6Helper::DataGene6Helper ()
{
  m_factory.SetTypeId (DataGene6::GetTypeId ());
}

void
DataGene6Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene6Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene6> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene7Helper::DataGene7Helper ()
{
  m_factory.SetTypeId (DataGene7::GetTypeId ());
}

void
DataGene7Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene7Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene7> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene8Helper::DataGene8Helper ()
{
  m_factory.SetTypeId (DataGene8::GetTypeId ());
}

void
DataGene8Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene8Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene8> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene9Helper::DataGene9Helper ()
{
  m_factory.SetTypeId (DataGene9::GetTypeId ());
}

void
DataGene9Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene9Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene9> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

DataGene10Helper::DataGene10Helper ()
{
  m_factory.SetTypeId (DataGene10::GetTypeId ());
}

void
DataGene10Helper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
DataGene10Helper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<DataGene10> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

} // namespace ns3
