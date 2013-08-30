/*
 * @file __NEC__gnbtpapi_udp_msg.c
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

#include "__NEC__gnbtpapi_udp_msg.h"

/* print__NEC__basic_header */
void print__NEC__basic_header(const void* buffer)
{

	__NEC__gnbtpapi_basic_header_t* h
		= (__NEC__gnbtpapi_basic_header_t *)buffer;

	printf("\t* header_type = %.2X\n", h->header_type);
	printf("\t* header_subtype = %.2X\n", h->header_subtype);
	printf("\t* hop_limit = %d\n", h->hop_limit);
	printf("\t* flags = %.2X\n", h->flags);
	printf("\t* payload_length = %d\n", h->payload_length);
	printf("\t* traffic_class = %.2X\n", h->traffic_class);
	printf("\t* btp_type = %.2X\n", h->btp_type);

}

/* print__NEC__extended_header */
void print__NEC__extended_header(const void* buffer)
{

	__NEC__print_basic_header(buffer);

	__NEC__gnbtpapi_extended_header_t* h
		= (__NEC__gnbtpapi_extended_header_t *)buffer;

	printf("\t* repetition_interval = %d\n", h->repetition_interval);
	printf("\t* max_lifetime = %d\n", h->max_lifetime);

}
