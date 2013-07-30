/**
 * @file udp_events.c
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

#include "udp_events.h"

/* new_udp_events */
udp_events_t *new_udp_events()
{
	udp_events_t *s = NULL;
	if ( ( s = (udp_events_t *)malloc(LEN__UDP_EVENTS) ) == NULL )
		{ handle_sys_error("new_udp_events: <malloc> returns NULL."); }
	if ( memset(s, 0, LEN__UDP_EVENTS) == NULL )
		{ handle_sys_error("new_udp_events: <memset> returns NULL."); }
	return(s);
}

/* init_tx_udp_events */
udp_events_t *init_tx_udp_events
	(const char* iface, const int port
			, const ev_cb_t callback, const bool broadcast)
{

	udp_events_t *s = new_udp_events();

	if ( broadcast == true )
		{ s->socket_fd = open_broadcast_udp_socket(iface, port); }
	else
		{ s->socket_fd = open_udp_socket(port); }

	if ( init_watcher(s, callback, EV_WRITE, port) < 0 )
		{ handle_app_error("init_tx_udp_events: <init_watcher> error.\n"); }

	return(s);

}

/* init_rx_udp_events */
udp_events_t *init_rx_udp_events
	(const int port, const ev_cb_t callback)
{

	udp_events_t *s = new_udp_events();
	s->socket_fd = open_udp_socket(port);

	if ( init_watcher(s, callback, EV_READ, port) < 0 )
		{ handle_app_error("init_rx_udp_events: <init_watcher> error.\n"); }

	return(s);

}

/* free_udp_events */
void free_udp_events(udp_events_t *m)
{
	free(m);
}

/* new_ev_io_arg_t */
ev_io_arg_t *new_ev_io_arg()
{
	ev_io_arg_t *s = NULL;
	if ( ( s = (ev_io_arg_t *)malloc(LEN__EV_IO_ARG) ) == NULL )
		{ handle_sys_error("new_ev_io_arg_t: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__EV_IO_ARG) == NULL )
		{ handle_sys_error("new_ev_io_arg_t: <memset> returns NULL.\n"); }
	return(s);
}

/* init_watcher */
int init_watcher(udp_events_t *m
				, const ev_cb_t callback, const int events, const int port)
{

	m->loop = EV_DEFAULT;
	ev_io_arg_t *arg = init_ev_io_arg(m, callback, port);
	m->watcher = &arg->watcher;

	ev_io_init(	m->watcher, cb_common,
				m->socket_fd, events	);
	ev_io_start(m->loop, m->watcher);

    return(EX_OK);

}

/* init_ev_io_arg */
ev_io_arg_t *init_ev_io_arg(const udp_events_t *m
							, const ev_cb_t callback, const int port)
{

	ev_io_arg_t *s = new_ev_io_arg();

	// 1) private data initialization
	s->cb_specfic = callback;

	// 2) public data initialization
	if ( ( s->public_arg.data = malloc(UDP_EVENTS_BUFFER_LEN) ) == NULL )
		{ handle_sys_error("init_ev_io_arg: <malloc> returns NULL."); }

	s->public_arg.len = 0;
	s->public_arg.socket_fd = m->socket_fd;
	s->public_arg.port = port;

	return(s);

}

/* cb_common */
void cb_common
	(struct ev_loop *loop, struct ev_io *watcher, int revents)
{

	if( EV_ERROR & revents )
		{ log_sys_error("Invalid event"); return; }

	ev_io_arg_t *arg = (ev_io_arg_t *)watcher;
	public_ev_arg_t *public_arg = &arg->public_arg;
	public_arg->socket_fd = watcher->fd;

	if ( arg->cb_specfic == NULL )
	{
		log_app_msg("cb_private_common (ev=%d,fd=%d): <cb_function> NULL!\n"
						, revents, watcher->fd);
		return;
	}

	arg->cb_specfic(public_arg);

}

/* cb_recvfrom */
void cb_recvfrom(public_ev_arg_t *arg)
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

}

#define __TX_DELAY_US 1000000	/**< (usecs) without sending. */

/* cb_broadcast_sendto */
void cb_broadcast_sendto(public_ev_arg_t *arg)
{

	int bytes_sent = 0;
	char *test = "BROADCAST-BROADCAST-BROADCAST\0";
	int len = strlen(test);
	sockaddr_t *dest_addr = (sockaddr_t *)
			new_broadcast_sockaddr_in(arg->port);

	printf(">>> BROADCAST TEST (fd = %d): sending test[%d] = %s\n"
			, arg->socket_fd, len, test);

	if ( ( bytes_sent = sendto(arg->socket_fd, test, len
								, 0, dest_addr, LEN__SOCKADDR_IN) ) < 0 )
	{
		log_sys_error("cb_broadcast_sendto (fd=%d): <sendto> ERROR.\n"
						, arg->socket_fd);
		getchar();
		return;
	}

	if ( bytes_sent < arg->len )
	{
		log_app_msg("cb_broadcast_sendto: sent %d bytes, required %d.\n"
						, bytes_sent, arg->len);
	}

	if ( usleep(__TX_DELAY_US) < 0 )
	{
		log_app_msg("Could not sleep for %d (usecs).\n", __TX_DELAY_US);
		return;
	}

}
