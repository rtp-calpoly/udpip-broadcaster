/*
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

/**************************************************** Application definitions */
static const char* __x_app_name = "udpip-broadcaster";
static const char* __x_app_version = "0.1";

/********************************************************* INTERNAL FUNCTIONS */

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

	configuration_t *cfg = NULL;
	
	/* 1) Runtime configuration is read from the CLI (POSIX.2). */
	cfg = create_configuration(argc, argv);
	print_configuration(cfg);

	exit(EXIT_SUCCESS);

}
