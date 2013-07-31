/**
 * @file cb_udp_events.h
 * @author Ricardo Tubío (rtpardavila[at]gmail.com)
 * @version 0.1
 *
 * @section LICENSE
 *
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

#include "cb_udp_events.h"

/* cb_print_recvfrom */
void cb_print_recvfrom(public_ev_arg_t *arg)
{

	int bytes_read = 0;
	sockaddr_t *src_addr = new_sockaddr();
	socklen_t src_addr_len = 0;

	if ( ( bytes_read = recvfrom(arg->socket_fd
									, arg->data, UDP_EVENTS_BUFFER_LEN
									, 0, src_addr, &src_addr_len) ) < 0 )
	{
		log_sys_error("cb_readfrom: wrong <recvfrom> call. ");
		return;
	}

	printf(">>> RECEIVED UDP MESSAGE >>>\n");
	print_hex_data(arg->data, bytes_read);
	printf("\n");

}

#define __TX_DELAY_US 1000000	/**< (usecs) without sending. */

/* cb_broadcast_sendto */
void cb_broadcast_sendto(public_ev_arg_t *arg)
{

	char *test = "BROADCAST-BROADCAST-BROADCAST-NUMBER=\0";
	int len = strlen(test);
	char *buffer = malloc(len);
	sockaddr_t *dest_addr = (sockaddr_t *)
			init_broadcast_sockaddr_in(arg->port);

	printf(">>> BROADCAST TEST (fd = %d): sending test[%d] = %s\n"
			, arg->socket_fd, len, test);

	send_message(dest_addr, arg->socket_fd, test, len);

	if ( usleep(__TX_DELAY_US) < 0 )
	{
		log_app_msg("Could not sleep for %d (usecs).\n", __TX_DELAY_US);
		return;
	}

}

/* cb_forward_recvfrom */
void cb_forward_recvfrom(public_ev_arg_t *arg)
{

	sockaddr_t *src_addr = new_sockaddr();
	socklen_t src_addr_len = 0;

	arg->len = 0;

	// 1) read UDP message from network level
	if ( ( arg->len = recvfrom(arg->socket_fd
									, arg->data, UDP_EVENTS_BUFFER_LEN
									, 0, src_addr, &src_addr_len) ) < 0 )
	{
		log_sys_error("cb_forward_recvfrom: wrong <recvfrom> call. ");
		return;
	}

	// 2) forward network level UDP message to application level
	int fwd_bytes = send_message
						(	arg->forwarding_addr, arg->forwarding_socket_fd,
							arg->data, arg->len	);

	if ( arg->print_forwarding_message == true )
	{
		log_app_msg(">>> fwd(net:%d>app:%d) = %d\n"
				, arg->port, arg->forwarding_port, fwd_bytes);
		print_hex_data(arg->data, arg->len);
		log_app_msg("\n");
	}

}
