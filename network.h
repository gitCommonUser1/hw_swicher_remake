#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
class IPAddress;
class SubnetMask;
class Gateway;
class PrimaryDNS;
class SecondaryDNS;

class Network : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool protocol READ protocol WRITE setProtocol NOTIFY protocolChanged)
    Q_PROPERTY(IPAddress* iPAddress READ iPAddress)
    Q_PROPERTY(SubnetMask* subnetMask READ subnetMask)
    Q_PROPERTY(Gateway* gateway READ gateway)
    Q_PROPERTY(PrimaryDNS* primaryDNS READ primaryDNS)
    Q_PROPERTY(SecondaryDNS* secondaryDNS READ secondaryDNS)
    bool m_protocol;

    IPAddress* m_iPAddress;

    SubnetMask* m_subnetMask;

Gateway* m_gateway;

PrimaryDNS* m_primaryDNS;

SecondaryDNS* m_secondaryDNS;

public:
    explicit Network(QObject *parent = nullptr);

bool protocol() const
{
    return m_protocol;
}

IPAddress* iPAddress() const
{
    return m_iPAddress;
}

SubnetMask* subnetMask() const
{
    return m_subnetMask;
}

Gateway* gateway() const
{
    return m_gateway;
}

PrimaryDNS* primaryDNS() const
{
    return m_primaryDNS;
}

SecondaryDNS* secondaryDNS() const
{
    return m_secondaryDNS;
}

public slots:
void setProtocol(bool protocol)
{
    if (m_protocol == protocol)
        return;

    m_protocol = protocol;
    emit protocolChanged(m_protocol);
}

signals:

void protocolChanged(bool protocol);
};

#endif // NETWORK_H
