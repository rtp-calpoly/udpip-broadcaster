/**
 * @file main.c
 * @author Ricardo Tubío (rtpardavila[at]gmail.com)
 * @version 0.1
 *
 * @section LICENSE
 *
 *  Created on: May 6, 2013
 *      Author: Ricardo Tubío (rtpardavila[at]gmail.com)
 * This file is part of udpip-broadcaster.
 * udpip-broadcaster is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * udpip-broadcaster is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with udpip-broadcaster.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#include "logger.h"
#include "configuration.h"
#include "udpev/udp_events.h"

/************************************************** Application definitions */

static const char* __x_app_name = "udpip-broadcaster";
static const char* __x_app_version = "0.1";

/******************************************************* INTERNAL FUNCTIONS */

/* print_help */
void print_help()
{
	fprintf(stdout, "HELP, %s\n", __x_app_name);
}

/* print_version */
void print_version()
{
	fprintf(stdout, "Version = %s\n", __x_app_version);
}

/* main */
int main(int argc, char **argv)
{
	
	// 1) Runtime configuration is read from the CLI (POSIX.2).
	configuration_t *cfg = create_configuration(argc, argv);
	print_configuration(cfg);

	// 2) Create UDP socket event managers:

	udp_events_t *events = NULL;

	if ( cfg->__tx_test == true )
	{
		events = init_tx_udp_events(cfg->if_name, cfg->tx_port
										, cb_broadcast_sendto, true);
		printf("> UDP TX socket open, port = %d, fd = %d.\n"
					, cfg->tx_port, events->socket_fd);
	}
	else
	{
		events = init_rx_udp_events(cfg->rx_port, cb_recvfrom);
		printf("> UDP RX socket open, port = %d, fd = %d.\n"
					, cfg->rx_port, events->socket_fd);
	}

	/*
	udp_events_t *app_udp_rx = init_rx_udp_events
									(cfg->app_rx_port, cb_recvfrom);
	printf("> UDP APP RX socket open, fd = %d.\n", app_udp_rx->socket_fd);
	udp_events_t *app_udp_tx
		= init_tx_udp_events(cfg->if_name, cfg->app_tx_port, NULL, false);
	printf("> UDP APP TX socket open, fd = %d.\n", app_udp_tx->socket_fd);
	*/

	// 3) loop that waits for events to occur...
	ev_loop(events->loop, 0);

	// 4) program finalization
	exit(EXIT_SUCCESS);

}
