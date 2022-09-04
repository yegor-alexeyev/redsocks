#include "libredudp.hh"

extern app_subsys redsocks_subsys;
extern app_subsys debug_subsys;
extern app_subsys base_subsys;
extern app_subsys redudp_subsys;
extern app_subsys dnstc_subsys;
extern app_subsys dnsu2t_subsys;


int init_libredudp(struct event_base* evbase)
{
  global_evbase = evbase;

    app_subsys *subsystems[] = {
        &redsocks_subsys,
#ifdef DBG_BUILD
        &debug_subsys,
#endif
        &base_subsys,
        &redudp_subsys,
        &dnstc_subsys,
        &dnsu2t_subsys,
    };

	int error;
	app_subsys **ss;

	evutil_secure_rng_init();

	if (event_get_struct_event_size() != sizeof(struct event)) {
		puts("libevent event_get_struct_event_size() != sizeof(struct event)! Check `redsocks -v` and recompile redsocks");
		return EXIT_FAILURE;
	}

	/* FILE *f = fopen(confname, "r"); */
	/* if (!f) { */
	/* 	perror("Unable to open config file"); */
	/* 	return EXIT_FAILURE; */
	/* } */

	/* parser_context* parser = parser_start(NULL); */
	/* if (!parser) { */
	/* 	perror("Not enough memory for parser"); */
	/* 	return EXIT_FAILURE; */
	/* } */

	/* FOREACH(ss, subsystems) */
	/* 	if ((*ss)->conf_section) */
	/* 		parser_add_section(parser, (*ss)->conf_section); */
	/* error = parser_run(parser); */
	/* parser_stop(parser); */

	/* fclose(f); */


	/* memset(terminators, 0, sizeof(terminators)); */

    int i = 0;
	FOREACH(ss, subsystems) {
		if ((*ss)->init) {
			error = (*ss)->init(evbase);
			if (error)
            {
              log_error(LOG_WARNING, "subsystem %d init failed\n", i);
              return -1;
            }
		}
        i++;
	}


	/* assert(SIZEOF_ARRAY(exit_signals) == SIZEOF_ARRAY(terminators)); */
	/* for (i = 0; i < SIZEOF_ARRAY(exit_signals); i++) { */
	/* 	signal_set(&terminators[i], exit_signals[i], terminate, NULL); */
	/* 	if (signal_add(&terminators[i], NULL) != 0) { */
	/* 		log_errno(LOG_ERR, "signal_add"); */
	/* 		goto shutdown; */
	/* 	} */
	/* } */

	if (LIBEVENT_VERSION_NUMBER != event_get_version_number()) {
		log_error(LOG_WARNING, "libevent version mismatch! headers %8x, runtime %8x\n", LIBEVENT_VERSION_NUMBER, event_get_version_number());
	}
    return 0;

	/* log_error(LOG_NOTICE, "redsocks started, conn_max=%u", redsocks_conn_max()); */

	/* event_dispatch(); */

	/* log_error(LOG_NOTICE, "redsocks goes down"); */

/* shutdown: */
/* 	for (i = 0; i < SIZEOF_ARRAY(exit_signals); i++) { */
/* 		if (signal_initialized(&terminators[i])) { */
/* 			if (signal_del(&terminators[i]) != 0) */
/* 				log_errno(LOG_WARNING, "signal_del"); */
/* 			memset(&terminators[i], 0, sizeof(terminators[i])); */
/* 		} */
/* 	} */

	/* for (--ss; ss >= subsystems; ss--) */
	/* 	if ((*ss)->fini) */
	/* 		(*ss)->fini(); */

	/* event_base_free(evbase); */

	/* return !error ? EXIT_SUCCESS : EXIT_FAILURE; */
}

uint16_t create_udp_relay(socks_proxy_t proxy, endpoint_t destination)
{
	redudp_instance *instance = calloc(1, sizeof(*instance));
	if (!instance) {
        fprintf(stderr, "create_udp_relay: Not enough memory \n");
		return -1;
	}

	INIT_LIST_HEAD(&instance->list);
	INIT_LIST_HEAD(&instance->clients);


	instance->config.bindaddr.sin_family = AF_INET;
    ext_vp_in_addr(&instance->config.bindaddr.sin_addr, "127.0.0.1");
    instance->config.bindaddr.sin_port = 0;
	instance->config.bindaddr.sin_port = htons(instance->config.bindaddr.sin_port);

	instance->config.relayaddr.sin_family = AF_INET;
    ext_vp_in_addr(&instance->config.relayaddr.sin_addr, proxy.endpoint.host);
    instance->config.relayaddr.sin_port = proxy.endpoint.port;
    
    instance->config.login = strdup(proxy.login);
    instance->config.password = strdup(proxy.password);

	instance->config.relayaddr.sin_port = htons(instance->config.relayaddr.sin_port);

	instance->config.destaddr.sin_family = AF_INET;
    ext_vp_in_addr(&instance->config.destaddr.sin_addr, destination.host);
    instance->config.destaddr.sin_port = destination.port;
	instance->config.destaddr.sin_port = htons(instance->config.destaddr.sin_port);

	instance->config.max_pktqueue = 5;
	instance->config.udp_timeout = 30;
	instance->config.udp_timeout_stream = 180;

	list_add(&instance->list, &instances);

    redudp_init_instance(instance);

    return ntohs(instance->config.bindaddr.sin_port);
}
