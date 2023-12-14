#include "network.h"
#include "ipaddress.h"
#include "subnetmask.h"
#include "gateway.h"
#include "primarydns.h"
#include "secondarydns.h"

Network::Network(QObject *parent) : QObject(parent)
{
    m_protocol = true;

    m_iPAddress = new IPAddress(this);
    m_iPAddress->setObjectName("iPAddress");

    m_subnetMask = new SubnetMask(this);
    m_subnetMask->setObjectName("subnetMask");

    m_gateway = new Gateway(this);
    m_gateway->setObjectName("gateway");

    m_primaryDNS = new PrimaryDNS(this);
    m_primaryDNS->setObjectName("primaryDNS");

    m_secondaryDNS = new SecondaryDNS(this);
    m_secondaryDNS->setObjectName("secondaryDNS");
}
