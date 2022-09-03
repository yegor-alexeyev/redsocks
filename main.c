/* redsocks - transparent TCP-to-proxy redirector
 * Copyright (C) 2007-2018 Leonid Evdokimov <leon@darkk.net.ru>
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <assert.h>
#include <event.h>
#include "log.h"
#include "main.h"
#include "utils.h"
#include "version.h"
#include "config.h"
#include "base.h"

extern app_subsys redsocks_subsys;
extern app_subsys debug_subsys;
extern app_subsys base_subsys;
extern app_subsys redudp_subsys;
extern app_subsys dnstc_subsys;
extern app_subsys dnsu2t_subsys;

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

static const char *confname = "redsocks.conf";
static const char *pidfile = NULL;

static void terminate(int sig, short what, void *_arg)
{
	if (event_loopbreak() != 0)
		log_error(LOG_WARNING, "event_loopbreak");
}


/* vim:set tabstop=4 softtabstop=4 shiftwidth=4: */
/* vim:set foldmethod=marker foldlevel=32 foldmarker={,}: */
