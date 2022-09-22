#ifndef LIBREDUDP_H
#define LIBREDUDP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*relay_shutdown_callback_t)(uint16_t);

typedef struct
{
  const char* host;
  uint16_t port;
} endpoint_t;

typedef struct
{
  endpoint_t endpoint;
  const char* login;
  const char* password;
} socks_proxy_t;


int init_libredudp();
uint16_t create_udp_relay(struct event_base* evbase, socks_proxy_t proxy, endpoint_t destination, relay_shutdown_callback_t shutdown_callback);

#ifdef __cplusplus
}
#endif

#endif
