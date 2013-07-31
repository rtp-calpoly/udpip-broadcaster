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

#ifndef CB_UDP_EVENTS_H_
#define CB_UDP_EVENTS_H_

#include <stdio.h>

#include "udp_events.h"

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// LIBEV CALLBACK FUNCTIONS
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/**
 * @brief Callback function that reads the given socket which has just been
 * 			put available for reading.
 * @param public_arg Public arguments for this callback function.
 */
void cb_print_recvfrom(public_ev_arg_t *arg);

/**
 * @brief Callback function that writes the given socket which has just been
 * 			put available for writing. Messages are sent in broadcast mode.
 * @param public_arg Public arguments for this callback function.
 */
void cb_broadcast_sendto(public_ev_arg_t *arg);

/**
 * @brief Callback function that forwards an UDP message that it receives to
 * 			a given forwarding socket.
 * @param public_arg Public arguments for this callback function.
 */
void cb_forward_recvfrom(public_ev_arg_t *arg);

/**
 * @brief Callback function that forwards an UDP message that it receives to
 * 			a given forwarding socket. The forwarded message is sent through
 * 			broadcast.
 * @param public_arg Public arguments for this callback function.
 */
void cb_broadcast_recvfrom(public_ev_arg_t *arg);

#endif /* CB_UDP_EVENTS_H_ */
