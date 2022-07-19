/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef DATA_GENE_HELPER_H
#define DATA_GENE_HELPER_H

#include <stdint.h>
#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"
#include "ns3/ipv4-address.h"
#include "ns3/data-gene.h"
#include "ns3/data-gene2.h"
#include "ns3/data-gene3.h"
#include "ns3/data-gene4.h"
#include "ns3/data-gene5.h"
#include "ns3/data-gene6.h"
#include "ns3/data-gene7.h"
#include "ns3/data-gene8.h"
#include "ns3/data-gene9.h"
#include "ns3/data-gene10.h"

namespace ns3 {

class DataGeneHelper
{
public:
  DataGeneHelper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene> m_server; //!< The last created server application
};

class DataGene2Helper
{
public:
  DataGene2Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene2> m_server; //!< The last created server application
};

class DataGene3Helper
{
public:
  DataGene3Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene3> m_server; //!< The last created server application
};

class DataGene4Helper
{
public:
  DataGene4Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene4> m_server; //!< The last created server application
};

class DataGene5Helper
{
public:
  DataGene5Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene5> m_server; //!< The last created server application
};

class DataGene6Helper
{
public:
  DataGene6Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene6> m_server; //!< The last created server application
};

class DataGene7Helper
{
public:
  DataGene7Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene7> m_server; //!< The last created server application
};

class DataGene8Helper
{
public:
  DataGene8Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene8> m_server; //!< The last created server application
};

class DataGene9Helper
{
public:
  DataGene9Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene9> m_server; //!< The last created server application
};

class DataGene10Helper
{
public:
  DataGene10Helper ();

  void SetAttribute (std::string name, const AttributeValue &value);

  ApplicationContainer Install (NodeContainer c);
private:
  ObjectFactory m_factory; //!< Object factory.
  Ptr<DataGene10> m_server; //!< The last created server application
};

} // namespace ns3

#endif /* DATA_GENE_HELPER_H */
