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
#include "udpev/cb_udp_events.h"

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
	log_app_msg(">>> Reading configuration...\n");
	configuration_t *cfg = create_configuration(argc, argv);
	log_app_msg(">>> Configuration read! Printing data...\n");
	print_configuration(cfg);

	// 2) Create UDP socket event managers:

	udp_events_t *net_events = NULL;
	udp_events_t *app_events = NULL;

	if ( cfg->__tx_test == true )
	{

		net_events = init_tx_udp_events(cfg->if_name, cfg->tx_port
										, cb_broadcast_sendto, true);
		printf("> UDP TX socket open, port = %d, fd = %d.\n"
					, cfg->tx_port, net_events->socket_fd);

	}
	else
	{

		log_app_msg(">>> Opening UDP NET RX socket...\n");
		net_events = init_net_udp_events
						(cfg->rx_port, cfg->if_name
								, cfg->app_address, cfg->app_rx_port
								, cb_forward_recvfrom);
		log_app_msg(">>> UDP NET RX socket open!\n");
		print_udp_events(net_events, cfg->rx_port, cfg->app_rx_port);

		log_app_msg(">>> Opening UDP APP RX socket...\n");
		app_events = init_app_udp_events
						(cfg->app_tx_port, cfg->if_name, cfg->tx_port
								, cb_broadcast_recvfrom);
		log_app_msg(">>> UDP APP RX socket open!\n");
		print_udp_events(app_events, cfg->app_tx_port, cfg->tx_port);

	}

	// 3) loop that waits for net_events to occur...
	ev_loop(net_events->loop, 0);

	// 4) program finalization
	exit(EXIT_SUCCESS);

}
