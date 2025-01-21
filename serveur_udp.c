#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>

#include <string.h>

/* Port local du serveur */ 
#define PORT 9600

int main(int argc, char *argv[]){
    /*
    * Variables du serveur
    *
    * Déclarer ici les variables suivantes : 
    * - sockfd le descripteur de socket
    * - structure d'adresse locale du serveur
    * - structure d'adresse du client
    * - taille de l'adresse du client
    */ 
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[256];

    /*
    * Code du serveur
    *
    * - Ouvrir le socket du serveur
    * - Remplir la structure d'adresse locale du serveur : 
    *   - la famille d'adresse
    *   - l'adresse IP 
    *   - le port 
    *   - Spécifier l'adresse locale du socket du serveur 
    */

    // Création de la socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {  // La création a échoué
        perror("Erreur lors de la création du socket");
        return 1;
    }

    // Configuration de la socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Liaison de la socket à l'adresse locale
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la liaison");  // La liaison a échoué
        return 1;
    }

    // Taille de la structure d'adresse
    clilen = sizeof(cli_addr);

    /*
    * Boucle générale du serveur (infinie)
    */ 
    while (1){
        /*
        * Code de l'intérieur de la boucle
        */

        // Réception et stockage des données
        bzero(buffer, 256);
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli_addr, &clilen) < 0) {
            perror("Erreur de réception");  // La réception a rencontré une erreur
            return 1;
        }

        // Affichage du message reçu
        printf("Message reçu : %s\n", buffer);

        // Envoie d'une réponse au client
        if (sendto(sockfd, "Message reçu par le serveur", 28, 0, (struct sockaddr *) &cli_addr, clilen) < 0) {
            perror("Erreur d'envoi de la réponse");  // L'envoi de la réponse du serveur a échoué
            return 1;
        }
    }

    close(sockfd);  // Fermeture de la socket du server

    return 0;
}
