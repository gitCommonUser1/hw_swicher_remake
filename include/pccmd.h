#ifndef _PCCMD_H
#define _PCCMD_H

#include <QObject>

#include "pc_cmd.h"
#include "tcp_socket.h"


class PcCmd : public QObject
{
    Q_OBJECT
public:
    PcCmd();

    int _do_control(const int fd, uint8_t *buf, int len);

private:
    int _set_sn(const int fd, uint8_t *buf, int len);
    int _set_mac(const int fd, uint8_t *buf, int len);

    int _get_sn(const int fd, uint8_t *buf, int len);
    int _get_mac(const int fd, uint8_t *buf, int len);
    int _get_id(const int fd, uint8_t *buf, int len);
    int _set_license(const int fd, uint8_t *buf, int len);
    int _get_license(const int fd, uint8_t *buf, int len);
    int _get_license_status(const int fd, uint8_t *buf, int len);

signals:
    void emit_sn_change(QString sn);
    void emit_mac_change(uint8_t mac[6]);
};


extern PcCmd pccmd;


int do_tcp_process(struct tcp_client_t *client);


#endif // _PCCMD_H
