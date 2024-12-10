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
	free(arp->source.ip);
	free(arp->source.mac);
	free(arp->target.ip);
	free(arp->target.mac);
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

static long	ft_compt(long nb)
{
	long	size;

	size = 0;
	if (nb < 0)
		size++;
	else if (nb == 0)
		size++;
	while (nb != 0)
	{
		size++;
		nb /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	int		size;
	long	nb;
	char	*s;

	nb = n;
	size = ft_compt(nb);
	s = malloc(sizeof(char) * (size + 1));
	if (!s)
		return (NULL);
	if (nb < 0)
	{
		s[0] = '-';
		nb *= -1;
	}
	else if (nb == 0)
		s[0] = '0';
	s[size] = '\0';
	while (nb > 0)
	{
		s[size - 1] = (nb % 10) + 48;
		nb /= 10;
		size--;
	}
	return (s);
}
