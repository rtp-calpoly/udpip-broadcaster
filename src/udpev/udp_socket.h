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

#ifndef UDP_SOCKET_H_
#define UDP_SOCKET_H_

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>

#include "../logger.h"
#include "../execution_codes.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// SOCKET STRUCTURES
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef struct ifreq ifreq_t;				/**< Type for ifreq. */
#define LEN__IFREQ sizeof(ifreq_t)

/**
 * @brief Allocates memory for an ifreq structure.
 * @return A pointer to the newly allocated block of memory.
 */
ifreq_t *new_ifreq();

/**
 * @brief Allocates memory for an ifreq structure and initializes it with the
 * 			given interface name.
 * @return A pointer to the initialized ifreq structure.
 */
ifreq_t *init_ifreq(const char *if_name);

typedef struct sockaddr sockaddr_t;			/*!< Type for sockaddr. */
#define LEN__SOCKADDR sizeof(sockaddr_t)

/**
 * @brief Allocates memory for an sockaddr structure.
 * @return A pointer to the newly allocated block of memory.
 */
sockaddr_t *new_sockaddr();

typedef struct sockaddr_in sockaddr_in_t;		/*!< Type for sockaddr_in. */
#define LEN__SOCKADDR_IN sizeof(sockaddr_in_t)

/**
 * @brief Allocates memory for an sockaddr_in structure.
 * @return A pointer to the newly allocated block of memory.
 */
sockaddr_in_t *new_sockaddr_in();

sockaddr_in_t *new_broadcast_sockaddr_in(const int port);
sockaddr_in_t *new_any_sockaddr_in(const int port);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// UDP SOCKET MANAGEMENT
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/**
 * @brief Sets socket options for permitting this socket to send broadcast
 * 			messages.
 * @param socket_fd File descriptor of the socket.
 * @return 'EX_OK' in case everything went allright; otherwise, < 0.
 */
int set_broadcast_socket(const int socket_fd);

/**
 * @brief Sets socket options for binding this socket to the given interface.
 * @param iface Interface where the socket is expected to be bound.
 * @param socket_fd File descriptor of the socket.
 */
int set_bindtodevice_socket(const char *if_name, const int socket_fd);

/**
 * @brief Creates and binds an UDP socket that uses the given port.
 * @param port The UDP port to be used by this socket.
 * @return File descriptor of the opened UDP socket.
 */
int open_udp_socket(const int port);

/**
 * @brief Creates and binds an UDP socket that uses the given port with
 * 			broadcasting privileges.
 * @param iface Interface where the socket is expected to be bound.
 * @param port The UDP port to be used by this socket.
 * @return File descriptor of the opened UDP socket.
 */
int open_broadcast_udp_socket(const char *if_name, const int port);

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// COMMON SOCKET TOOLS
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define BYTES_PER_LINE 8	/**< Number of bytes per line to be printed. */

/**
 * @brief Prints the data field of the given IEEE 802.3 frame.
 * @param buffer The IEEE 802.3 frame whose data is to be printed.
 * @return EX_OK if everything was correct; otherwise < 0.
 */
int print_hex_data(const char *buffer, const int len);

/**
 * @brief Prints the given Ethernet address.
 * @param eth_address Ethernet address as an array.
 */
void print_eth_address(const unsigned char *eth_address);

#endif /* UDP_SOCKET_H_ */
