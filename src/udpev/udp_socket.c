/**
 * @file udp_socket.c
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

#include "udp_socket.h"

/* new_ifreq */
ifreq_t *new_ifreq()
{

	ifreq_t *s = NULL;

	if ( ( s = (ifreq_t *)malloc(LEN__IFREQ) ) == NULL )
		{ handle_sys_error("new_ifreq: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__IFREQ) == NULL )
		{ handle_sys_error("new_ifreq: <memset> returns NULL.\n"); }

	return(s);

}

/* init_ifreq */
ifreq_t *init_ifreq(const char *if_name)
{

	ifreq_t *s = new_ifreq();
	int if_name_len = 0;
	int ifr_name_len = sizeof(s->ifr_name);

	if ( ( if_name_len = strlen(if_name) ) < 0 )
		{ handle_sys_error("init_ifreq: <strlen> returns < 0.\n"); }
	if ( if_name_len > ifr_name_len )
		{ handle_app_error("init_ifreq: if_name's length bigger than ifr's " \
							"buffer.\n"); }

	if ( strncpy(s->ifr_name, if_name, if_name_len) == NULL )
		{ handle_sys_error("init_ifreq: <strncpy> returns NULL.\n"); }

	return(s);

}

/* new_sockaddr */
sockaddr_t *new_sockaddr()
{

	sockaddr_t *s = NULL;

	if ( ( s = (sockaddr_t *)malloc(LEN__SOCKADDR) ) == NULL )
		{ handle_sys_error("new_sockaddr: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__SOCKADDR) == NULL )
		{ handle_sys_error("new_sockaddr: <memset> returns NULL.\n"); }

	return(s);

}

/* new_sockaddr_in */
sockaddr_in_t *new_sockaddr_in()
{

	sockaddr_in_t *s = NULL;

	if ( ( s = (sockaddr_in_t *)malloc(LEN__SOCKADDR_IN) ) == NULL )
		{ handle_sys_error("new_sockaddr_in: <malloc> returns NULL.\n"); }
	if ( memset(s, 0, LEN__SOCKADDR) == NULL )
		{ handle_sys_error("new_sockaddr_in: <memset> returns NULL.\n"); }

	return(s);

}

/* init_broadcast_sockaddr_in */
sockaddr_in_t *init_broadcast_sockaddr_in(const int port)
{

	sockaddr_in_t *s = new_sockaddr_in();

	s->sin_family = AF_INET;
	s->sin_port = (in_port_t)htons(port);
	s->sin_addr.s_addr = htonl(INADDR_BROADCAST);

	return(s);

}

/* init_any_sockaddr_in */
sockaddr_in_t *init_any_sockaddr_in(const int port)
{

	sockaddr_in_t *s = new_sockaddr_in();

	s->sin_family = AF_INET;
	s->sin_port = (in_port_t)htons(port);
	s->sin_addr.s_addr = htonl(INADDR_ANY);

	return(s);

}

/* init_sockaddr_in */
sockaddr_in_t *init_sockaddr_in(const char *address, const int port)
{

	sockaddr_in_t *s = new_sockaddr_in();

	s->sin_family = AF_INET;
	s->sin_port = (in_port_t)htons(port);

	if ( ( s->sin_addr.s_addr = inet_addr(address) ) < 0 )
		{ handle_sys_error("init_sockaddr_in: " \
							"<inet_addr> returns error.\n"); }

	return(s);

}

/* open_receiver_udp_socket */
int open_receiver_udp_socket(const int port)
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }

	// 2) local address for binding
	sockaddr_in_t* addr = init_any_sockaddr_in(port);
	if ( bind(fd, (sockaddr_t *)addr, LEN__SOCKADDR_IN) < 0 )
		{ handle_sys_error("open_udp_socket: <bind> returns error.\n"); }

	return(fd);

}

/* open_transmitter_udp_socket */
int open_transmitter_udp_socket(const int port)
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }

	return(fd);

}

/* open_broadcast_udp_socket */
int open_broadcast_udp_socket(const char *iface, const int port)
{

	int fd = -1;

	// 1) socket creation
	if ( ( fd = socket(AF_INET, SOCK_DGRAM, 0) ) < 0 )
		{ handle_sys_error("open_udp_socket: <socket> returns error.\n"); }

	// 2) set broadcast socket options
	if ( set_broadcast_socket(fd) < 0 )
	{
		handle_app_error("open_broadcast_udp_socket: " \
							"<set_broadcast_socket> returns error.\n");
	}

	// 3) broadcast socket must be bound to a specific network interface
	if ( set_bindtodevice_socket(iface, fd) < 0 )
	{
		handle_app_error("open_broadcast_udp_socket: " \
							"<set_bindtodevice_socket> returns error.\n");
	}

	return(fd);

}

/* set_broadcast_socket */
int set_broadcast_socket(const int socket_fd)
{

	int bcast = 1;

	if ( setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(int))
			< 0 )
		{ handle_sys_error("set_broadcast_socket: " \
							"<setsockopt> returns error."); }

	return(EX_OK);

}

/* set_bindtodevice_socket */
int set_bindtodevice_socket(const char *if_name, const int socket_fd)
{

	ifreq_t *ifr = init_ifreq(if_name);

	if ( setsockopt(socket_fd, SOL_SOCKET, SO_BINDTODEVICE, ifr, LEN__IFREQ)
			< 0 )
		{ handle_sys_error("set_bindtodevice_socket: " \
							"<setsockopt> returns error"); }

	return(EX_OK);

}

/* send_message */
int send_message(	const sockaddr_t* dest_addr, const int socket_fd,
					const void *buffer, const int len	)
{

	int sent_bytes = 0;

	if ( ( sent_bytes = sendto(socket_fd, buffer, len
								, 0, dest_addr, LEN__SOCKADDR_IN) ) < 0 )
	{
		log_sys_error("cb_broadcast_sendto (fd=%d): <sendto> ERROR.\n"
						, socket_fd);
		getchar();
		return(EX_ERR);
	}

	if ( sent_bytes < len )
	{
		log_app_msg("send_message: sent %d bytes, requested %d.\n"
						, sent_bytes, len);
		return(EX_ERR);
	}

	return(EX_OK);

}

/* print_hex_data */
int print_hex_data(const char *buffer, const int len)
{

	int last_byte = len - 1;
	if ( len < 0 ) { return(EX_WRONG_PARAM); }

	for ( int i = 0; i < len; i++ )
	{
		if ( ( i % BYTES_PER_LINE ) == 0 ) { log_app_msg("\n\t\t\t"); }
		log_app_msg("%02X", 0xFF & (unsigned int)buffer[i]);
		if ( i < last_byte ) { log_app_msg(":"); }
	}

	return(EX_OK);

}

/* print_eth_address */
void print_eth_address(const unsigned char *eth_address)
{

	printf("%02X:%02X:%02X:%02X:%02X:%02X",
  			(unsigned char) eth_address[0],
  			(unsigned char) eth_address[1],
  			(unsigned char) eth_address[2],
  			(unsigned char) eth_address[3],
  			(unsigned char) eth_address[4],
  			(unsigned char) eth_address[5]);

}
