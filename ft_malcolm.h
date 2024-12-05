#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // delete

#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>

#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>


#include <net/if_arp.h>
#include <net/if.h>

//#include <linux/if_arp.h>
//#include <linux/if.h>

#include <linux/if_packet.h>

#include <ifaddrs.h>

#define SIZE_IP 16
#define SIZE_MAC_ADDRESS 18

struct machine {

    char *ip;
    char *mac;
};

struct header {

    struct ether_header ethernet;
    struct arphdr arp;
};

struct malcolm {

    char    **interfaces;
    struct machine source;
    struct machine target;

    struct header  header;
    struct sockaddr_ll socket_address;
    struct ifreq    ifr;
};

//void    parse_arg(char **argv, struct malcolm *arp);

/*
    Associer l'adresse IP de la victime à l'adresse MAC de l'attaquant

    - Cette structure représente un paquet ARP. Elle contient les 
    champs spécifiés dans le format ARP pour les requêtes et les réponses.
    
    struct arphdr
      {
        unsigned short int ar_hrd;                /* Format of hardware address. 
        unsigned short int ar_pro;                /* Format of protocol address. 
        unsigned char ar_hln;                /* Length of hardware address. 
        unsigned char ar_pln;                /* Length of protocol address. 
        unsigned short int ar_op;                /* ARP opcode (command). 
    #if 0
        /* Ethernet looks like this : This bit is variable sized
           however...  
        unsigned char __ar_sha[ETH_ALEN];        /* Sender hardware address. 
        unsigned char __ar_sip[4];                /* Sender IP address. 
        unsigned char __ar_tha[ETH_ALEN];        /* Target hardware address. 
        unsigned char __ar_tip[4];                /* Target IP address.  
    #endif
      };

    struct ether_header {
        u_char  ether_dhost[6];    // Adresse MAC de destination
        u_char  ether_shost[6];    // Adresse MAC source
        u_short ether_type;        // Type de protocole (0x0806 pour ARP)
    };

    - Cette structure est utilisée pour la communication au niveau de la couche de liaison de
    données (niveau 2) avec des sockets raw (bruts), comme pour envoyer des paquets ARP.
    
    struct sockaddr_ll {
        unsigned short sll_family;   /* Always AF_PACKET 
        unsigned short sll_protocol; /* Physical-layer protocol 
        int            sll_ifindex;  /* Interface number 
        unsigned short sll_hatype;   /* ARP hardware type 
        unsigned char  sll_pkttype;  /* Packet type 
        unsigned char  sll_halen;    /* Length of address 
        unsigned char  sll_addr[8];  /* Physical-layer address 
    };

    - Elle est utilisée pour obtenir des informations sur une interface réseau, 
    comme l'adresse MAC de l'interface qui enverra la requête ARP
    
    struct ifreq {
        char ifr_name[IFNAMSIZ]; /* nom interface 
        union {
            struct sockaddr ifr_addr;
            struct sockaddr ifr_dstaddr;
            struct sockaddr ifr_broadaddr;
            struct sockaddr ifr_netmask;
            struct sockaddr ifr_hwaddr;
            short           ifr_flags;
            int             ifr_ifindex;
            int             ifr_metric;
            int             ifr_mtu;
            struct ifmap    ifr_map;
            char            ifr_slave[IFNAMSIZ];
            char            ifr_newname[IFNAMSIZ];
            char           *ifr_data;
        };
    };

    struct sockaddr {
        sa_family_t     sa_family;      /* Address family
        char            sa_data[];      /* Socket address 
    };

    ///// For ARP request
    
    struct	ether_header {
	    struct	ether_addr ether_dhost;
	    struct	ether_addr ether_shost;
	    u_short	ether_type;
    };

    struct	ether_arp {
    	struct	arphdr ea_hdr;		/* fixed-size header 
    	struct	ether_addr arp_sha;	/* sender hardware address 
    	u_char	arp_spa[4];		/* sender protocol address 
    	struct	ether_addr arp_tha;	/* target hardware address 
    	u_char	arp_tpa[4];		/* target protocol address 
    };

- inet_pton: Créer une structure d'adresse réseau
- inet_ntop: Convertir des adresses IPv4 et IPv6 sous forme binaire en texte
- if_nametoindex: Renvoie l'indice de l'interface réseau correspondant au parametre
- inet_addr: 
- gethostbyname:
- getaddrinfo:  Renvoie une ou plusieurs structures addrinfo
- freeaddrinfo:
- getifaddrs: Crée une liste chaînée de structures décrivant les interfaces réseau
- freeifaddrs:
- htons: Convertit un entier 16 bits du format de l'hôte vers le format réseau (big-endian).
- ntohs: Convertit un entier 16 bits du format réseau (big-endian) vers le format de l'hôte.

*/

#endif