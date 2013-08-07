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

	bool blocked = false;
	arg->len = 0;

	// 1) read UDP message from network level
	//		(self-broadcast messages are not received)
	if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header
								, arg->local_addr->sin_addr.s_addr
								, &blocked) ) < 0 )
	{
		log_app_msg("cb_print_recvfrom: <recv_msg> " \
						"Could not receive message.\n");
		return;
	}

	printf(">>> RECEIVED UDP MESSAGE >>>\n");
	print_hex_data(arg->data, arg->len);
	printf("\n");

}

#define __TX_DELAY_US 1000000	/**< (usecs) without sending. */

#define __DATA_BUFFER_LEN 100			/**< Length of the buffer. */
char data[__DATA_BUFFER_LEN];		/**< Static buffer. */

/* cb_broadcast_sendto */
void cb_broadcast_sendto(public_ev_arg_t *arg)
{

	sprintf(data, "BROADCAST-TEST#%d", arg->__test_number++);
	int len = strlen(data);

	sockaddr_t *dest_addr = (sockaddr_t *)
			init_broadcast_sockaddr_in(arg->port);

	printf(">>> BROADCAST TEST (fd = %d): sending test[%d] = %s\n"
			, arg->socket_fd, len, data);

	send_message(dest_addr, arg->socket_fd, data, len);

	if ( usleep(__TX_DELAY_US) < 0 )
	{
		log_app_msg("Could not sleep for %d (usecs).\n", __TX_DELAY_US);
		return;
	}

}

/* cb_forward_recvfrom */
void cb_forward_recvfrom(public_ev_arg_t *arg)
{

	bool blocked = false;
	arg->len = 0;

	printf("### 1\n");

	// 1) read UDP message from network level
	//		(self-broadcast messages are not received)
	if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header
								, arg->local_addr->sin_addr.s_addr
								, &blocked) ) < 0 )
	{
		log_app_msg("cb_forward_recvfrom: <recv_msg> " \
						"Could not receive message.\n");
		return;
	}

	printf("### 2\n");

	// 2) in case the message comes from the localhost, it is discarded
	if ( blocked == true )
	{
		log_app_msg(">>>@cb_forward_recvfrom: Message blocked!\n");
		return;
	}

	printf("### 3\n");

	// 3) forward network level UDP message to application level
	int fwd_bytes = send_message
						(	(sockaddr_t *)arg->forwarding_addr,
							arg->forwarding_socket_fd,
							arg->data, arg->len	);

	if ( arg->print_forwarding_message == true )
	{
		log_app_msg(">>> fwd(net:%d>app:%d), msg[%.2d] = {"
				, arg->port, arg->forwarding_port, fwd_bytes);
		print_hex_data(arg->data, arg->len);
		log_app_msg("}\n");
	}

}

/* cb_forward_recvfrom */
void cb_broadcast_recvfrom(public_ev_arg_t *arg)
{

	bool blocked = false;
	arg->len = 0;

	// 1) read UDP message from application level
	//		(self-broadcast messages are not received)
	if ( ( arg->len = recv_msg(arg->socket_fd, arg->msg_header
								, arg->local_addr->sin_addr.s_addr
								, &blocked) ) < 0 )
	{
		log_app_msg("cb_broadcast_recvfrom: <recv_msg> " \
						"Could not receive message.\n");
		return;
	}

	// 2) broadcast application level UDP message to network level
	int fwd_bytes = send_message
						(	(sockaddr_t *)arg->forwarding_addr,
							arg->forwarding_socket_fd,
							arg->data, arg->len	);

	if ( arg->print_forwarding_message == true )
	{
		log_app_msg(">>> BROADCAST(app:%d>net:%d), msg[%.2d] = {"
				, arg->port, arg->forwarding_port, fwd_bytes);
		print_hex_data(arg->data, arg->len);
		log_app_msg("}\n");
	}

}
