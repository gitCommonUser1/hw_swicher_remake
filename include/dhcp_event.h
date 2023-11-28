#ifndef DhcpEvent_H
#define DhcpEvent_H

#include <QObject>

class DhcpEvent : public QObject
{
    Q_OBJECT
public:
    DhcpEvent();

    int dhcp_event(char *buf, int len);

    int dhcp_event_init();
    void dhcp_event_exit();

private:
    void send_dhcp_change(bool is_add, char *netif)
    {
        if (netif == NULL
                || strlen(netif) <= 0)
            return;

        //printf("%s: is_add:%d %s\n", __func__, is_add, netif);

        emit emit_dhcp_change(is_add, netif);
    }

signals:
    /**
     * @brief emit_dhcp_change
     * @param is_add
     * @param netif
     */
    void emit_dhcp_change(bool is_add, QString netif);
};

extern DhcpEvent dhcp_event;
extern int g_dhcp_fd;

#endif // DhcpEvent_H
