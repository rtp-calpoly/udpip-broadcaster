/*
 * @file configuration.c
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

#include "configuration.h"

/* new_configuration */
configuration_t *new_configuration()
{

	configuration_t *cfg = NULL;
	cfg = (configuration_t *)malloc(LEN__T_CONFIGURATION);

	memset(cfg, 0, LEN__T_CONFIGURATION);
	cfg->__tx_test = false;
	cfg->__verbose = false;

	return(cfg);

}

/* create_configuration */
configuration_t *create_configuration(int argc, char** argv)
{

	configuration_t *cfg = new_configuration();
	
	read_configuration(argc, argv, cfg);
	check_configuration(cfg);
	
	return(cfg);
	
}

/* read_configuration */
int read_configuration(int argc, char** argv, configuration_t* cfg)
{

	int idx = 0, read = 0;
    
    static struct option args[] =
	{
		{"help",	no_argument,		NULL, 	'h'	},
		{"verbose",	no_argument,		NULL,	'e'	},
		{"version",	no_argument,		NULL, 	'v'	},
		{"nettx",	required_argument,	NULL, 	't'	},
		{"netrx",  	required_argument,	NULL, 	'r'	},
		{"apptx",	required_argument,	NULL, 	'u'	},
		{"apprx",  	required_argument,	NULL, 	'w'	},
		{"appaddr", required_argument,  NULL,	'd'	},
		{"ifname",	required_argument,	NULL,	'i'	},
		{"txtest",  no_argument,		NULL,   's' },
		{"nec",		no_argument,		NULL,	'n' },
		{0,0,0,0}
	};
	
	while
		( ( read = getopt_long(argc, argv, "nhsevt:r:i:u:w:d:", args, &idx) )
				> -1 )
	{

		switch(read)
		{
			case 't':
				
				cfg->tx_port = atoi(optarg);
				break;
				
			case 'r':
				
				cfg->rx_port = atoi(optarg);
				break;

			case 'd':

				if ( strlen(optarg) <= 0 )
					{ handle_app_error("read_configuration: " \
										"wrong application address.\n"); }
				cfg->app_address = optarg;
				cfg->app_inet_addr = inet_addr(optarg);

				if ( 	( cfg->app_inet_addr < 0 ) ||
						( cfg->app_inet_addr == 0xFFFFFFFF )	)
					{ handle_app_error("read_configuration: " \
										"wrong application address.\n"); }

				break;

			case 'u':

				cfg->app_tx_port = atoi(optarg);
				break;

			case 'w':

				cfg->app_rx_port = atoi(optarg);
				break;

			case 's':

				cfg->__tx_test = true;
				break;

			case 'i':
			
				if ( strlen(optarg) > IF_NAMESIZE )
				{
					log_app_msg("[WARNING] if_name length = %d, maximum = %d.\
								TRUNCATING!\n", (int)strlen(optarg), \
								IF_NAMESIZE);
				}
				
				strncpy(cfg->if_name, optarg, IF_NAMESIZE);
				break;

			case 'n':

				cfg->nec_mode = true;
				break;

			case 'e':
				
				__verbose = true;
				cfg->__verbose = true;
				break;

			case 'v':
			
				print_version();
				exit(EXIT_SUCCESS);
				break;
				
			case 'h':
			default:
			
				print_help();
				exit(EXIT_SUCCESS);
				break;
		}

	};

	return(EX_OK);

}

/* check_configuration */
int check_configuration(configuration_t *cfg)
{

	if ( cfg == NULL )
	{
		handle_app_error("Configuration to be checked is NULL\n");
	}

	if ( ( cfg->tx_port <= 0 ) || ( cfg->rx_port <= 0 ) )
		{ handle_app_error("Both TX and RX port must be set.\n"); }
	
	if ( strlen(cfg->if_name) <= 0  )
		{ handle_app_error("Network interface name must be provided.\n"); }

	if ( cfg->app_address != NULL )
	{
		if ( strlen(cfg->app_address) <= 0  )
			{ handle_app_error("Application address must be provided.\n"); }
	}

	if ( ( cfg->app_tx_port <= 0 ) || ( cfg->app_rx_port <= 0 ) )
		{ handle_app_error("Both APP. TX and RX port must be set.\n"); }

	return(EX_OK);

}

/* print_configuration */
void print_configuration(const configuration_t *cfg)
{

	if ( cfg == NULL )
		{ handle_app_error("Given configuration is NULL.\n"); }
	
	log_app_msg(">>> Configuration = \n{\n");
	log_app_msg("\t.app_addr = %s\n", cfg->app_address);
	log_app_msg("\t.app_inet_addr = %.2X\n", cfg->app_inet_addr);
	log_app_msg("\t.app_tx_port = %d\n", cfg->app_tx_port);
	log_app_msg("\t.app_rx_port = %d\n", cfg->app_rx_port);
	log_app_msg("\t.tx_port = %d\n", cfg->tx_port);
	log_app_msg("\t.rx_port = %d\n", cfg->rx_port);
	log_app_msg("\t.if_name = %s\n", cfg->if_name);
	log_app_msg("\t.nec_mode = %s\n", cfg->nec_mode ? "true" : "false");
	log_app_msg("\t.__tx_test = %s\n", cfg->__tx_test ? "true" : "false");
	log_app_msg("\t.__verbose = %s\n", cfg->__verbose ? "true" : "false");
	log_app_msg("}\n");
	
}
