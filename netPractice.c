#include "libft/libft.h"
#include <stdio.h>

int	*int_array(char **char_array)
{
	int	*ret;

	ret = malloc(4 * sizeof(int));
	for (int i = 0; i < 4; i++)
	{
		ret[i] = ft_atoi(char_array[i]);
		free(char_array[i]);
	}
	free(char_array);
	return ret;
}

int	*get_nwaddr(int *ip, int *mask)
{
	int	*nwaddr;

	nwaddr = malloc(4 * sizeof(int));
	for (int i = 0; i < 4; i++)
		nwaddr[i] = ip[i] & mask[i];
	return nwaddr;
}

void	print_octet_bin(int octet, int len)
{
	if (octet > 1 || len < 8)
		print_octet_bin(octet / 2, len + 1);
	printf("%d", octet % 2);
}

void	print_binary(int *ip)
{
	for (int i = 0; i < 4; ++i)
	{
		print_octet_bin(ip[i], 1);
		if (i != 3)
			printf(".");
		else
			printf("\n");
	}
}

void	print_ip(char *name, int *ip)
{
	printf("%s ", name);
	for (int i = 0; i < 4; i++)
	{
		printf("%d", ip[i]);
		if (i != 3)
			printf(".");
		else
			printf("\n");
	}
	print_binary(ip);
}

int	get_cidr(int *mask)
{
	int	cidr;
	int	octet;
	int	bit;

	cidr = 32;
	for (int i = 3; i >= 0; i--)
	{
		octet = mask[i];
		bit = 7;
		while ((octet & 1) == 0 && bit >= 0)
		{
			bit--;
			octet = octet >> 1;
			cidr--;
		}
	}
	return cidr;
}

int	*get_bcaddr(int *ip, int cidr)
{
	int	*bcaddr;

	bcaddr = malloc(4 * sizeof(int));
	for (int i = 0; i < 4; i++)
	{
		bcaddr[i] = ip[i];
		for (int bit = 7; bit >= 0; bit--)
		{
			if (cidr <= 0)
				bcaddr[i] |= (1 << bit);
			cidr--;
		}
	}
	return bcaddr;
}

int	main(int argc, char **argv)
{
	int	*ip;
	int	*mask;
	int	*network_address;
	int	*broadcast_address;

	if (argc != 3)
	{
		printf("Give ip and mask\n");
		return (1);
	}
	ip = int_array(ft_split(argv[1], '.'));
	mask = int_array(ft_split(argv[2], '.'));
	network_address = get_nwaddr(ip, mask);
	broadcast_address = get_bcaddr(ip, get_cidr(mask));
	print_ip("ip address:       ", ip);
	print_ip("mask:             ", mask);
	printf("/%d\n", get_cidr(mask));
	print_ip("network address:  ", network_address);
	print_ip("broadcast address:", broadcast_address);
	network_address[3]++;
	print_ip("host min:         ", network_address);
	network_address[3]--;
	broadcast_address[3]--;
	print_ip("host max:         ", broadcast_address);
	broadcast_address[3]++;
	free(mask);
	free(ip);
	free(network_address);
	free(broadcast_address);
	return 0;
}
