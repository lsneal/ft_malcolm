#include "ft_malcolm.h"

void    parse_arg(char **argv, struct malcolm *arp) {

    arp->source.ip = strdup(argv[1]);
    arp->source.mac = strdup(argv[2]);
    arp->target.ip = strdup(argv[3]);
    arp->target.mac = strdup(argv[4]);
}

void    free_interface(struct malcolm *arp) {

    for (int i = 0; arp->interface[i]; i++) {
        free(arp->interface[i]);
    }
    free(arp->interface);
}

int			sizelist(struct ifaddrs **head)
{
	struct ifaddrs		*cursor;
	int			count;

	count = 0;
	cursor = *head;
	while (cursor)
	{
		cursor = cursor->ifa_next;
		count++;
	}
	return (count);
}