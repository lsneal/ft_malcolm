#include "ft_malcolm.h"

void    parse_arg(char **argv, struct malcolm *arp) {

    arp->source.ip = strdup(argv[1]);
    arp->source.mac = strdup(argv[2]);
    arp->target.ip = strdup(argv[3]);
    arp->target.mac = strdup(argv[4]);
}