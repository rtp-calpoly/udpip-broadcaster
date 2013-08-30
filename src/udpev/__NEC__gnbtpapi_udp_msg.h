/*
 * @file __NEC__gnbtpapi_udp_msg.h
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

#ifndef UDP_NEC_GNBTPAPI_H_
#define UDP_NEC_GNBTPAPI_H_

#include <stdio.h>
#include <stdint.h>

typedef struct __NEC__gnbtpapi_basic_header
{

	uint8_t header_type;
	uint8_t header_subtype;
	uint8_t hop_limit;
	uint8_t flags;
	uint16_t payload_length;
	uint8_t traffic_class;
	uint8_t btp_type;

} __NEC__gnbtpapi_basic_header_t;

typedef struct __NEC__gnbtpapi_extended_header
{

	__NEC__gnbtpapi_basic_header_t header;

	uint32_t repetition_interval;
	uint32_t max_lifetime;

} __NEC__gnbtpapi_extended_header_t;

typedef struct __NEC__gnbtpapi_rx_info_header
{

	uint64_t destination_gn_address;
	uint32_t timestamp;
	uint32_t latitude;
	uint32_t longitude;
	uint16_t speed;
	uint16_t heading;
	uint16_t altitude;
	uint16_t accuracies;

} __NEC__gnbtpapi_rx_info_header_t;

#define LEN____NEC__GNBTPAPI_BASIC_HEADER \
	sizeof(__NEC__gnbtpapi_basic_header_t)

#define LEN____NEC__GNBTPAPI_EXTENDED_HEADER \
	sizeof(__NEC__gnbtpapi_extended_header_t)

#define LEN____NEC__GNBTPAPI_RX_INFO_HEADER \
	sizeof(__NEC__gnbtpapi_rx_info_header_t)

typedef struct __NEC__gnbtpapi_tsb_tx_header
{

	__NEC__gnbtpapi_extended_header_t header;

	uint16_t destination_port;
	uint16_t source_port;

	void* payload;

} __NEC__gnbtpapi_tsb_tx_header_t;

typedef struct __NEC__gnbtpapi_geounicast_tx_header
{

	__NEC__gnbtpapi_extended_header_t header;

	uint32_t destination_gn_address;
	uint16_t destination_port;
	uint16_t source_port;

	void* payload;

} __NEC__gnbtpapi_geounicast_tx_header_t;

typedef struct __NEC__gnbtpapi_geocast_tx_header
{

	__NEC__gnbtpapi_extended_header_t header;

	uint32_t latitude;
	uint32_t longitude;

	uint16_t distance_a;
	uint16_t distance_b;
	uint16_t orientation;
	uint16_t reserved;

	uint16_t destination_port;
	uint16_t source_port;

	void* payload;

} __NEC__gnbtpapi_geocast_tx_header_t;

#define LEN____NEC__GNBTPAPI_TSB_TX_HEADER \
	sizeof(__NEC__gnbtpapi_tsb_tx_header_t)
#define LEN____NEC__GNBTPAPI_GEOUNICAST_TX_HEADER \
	sizeof(__NEC__gnbtpapi_geounicast_tx_header_t)
#define LEN____NEC__GNBTPAPI_GEOCAST_TX_HEADER\
	sizeof(__NEC__gnbtpapi_geocast_tx_header_t)

typedef struct __NEC__gnbtpapi_geounicast_rx_header
{

	__NEC__gnbtpapi_basic_header_t header;
	__NEC__gnbtpapi_rx_info_header_t rx_info_header;

	uint16_t destination_port;
	uint16_t destination_port_info;

	void* payload;

} __NEC__gnbtpapi_geounicast_rx_header_t;

typedef struct __NEC__gnbtpapi_geocast_rx_header
{

	__NEC__gnbtpapi_basic_header_t header;
	__NEC__gnbtpapi_rx_info_header_t rx_info_header;

	uint32_t repetition_interval;
	uint32_t maxlife_time;

	uint16_t distance_a;
	uint16_t distance_b;
	uint16_t orientation;
	uint16_t reserved;

	uint16_t destination_port;
	uint16_t source_port;

	void* payload;

} __NEC__gnbtpapi_geocast_rx_header_t;

#define LEN____NEC__GNBTPAPI_GEOUNICAST_RX_HEADER \
	sizeof(__NEC__gnbtpapi_geounicast_rx_header_t)
#define LEN____NEC__GNBTPAPI_GEOCAST_RX_HEADER \
	sizeof(__NEC__gnbtpapi_geocast_rx_header_t)

void print__NEC__basic_header();
void print__NEC__extended_header();

#endif /* UDP_NEC_GNBTPAPI_H_ */
