#ifndef LIBREDUDP_H
#define LIBREDUDP_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  const char* ip;
  uint16_t port;
} endpoint_t;

typedef struct
{
  endpoint_t endpoint;
  const char* login;
  const char* password;
} socks_proxy_t;


int init_libredudp(struct event_base* base);
uint16_t create_udp_relay(socks_proxy_t proxy, endpoint_t destination);

#ifdef __cplusplus
}
#endif

#endif
