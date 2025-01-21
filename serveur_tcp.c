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
    int sockfd, sockfdClient;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    /*
    * Code du serveur
    *
    * - Ouvrir le socket du serveur
    * - Remplir la structure d'adresse locale du serveur : 
    * - la famille d'adresse
    * - l'adresse IP 
    * - le port 
    * - Spécifier l'adresse locale du socket du serveur 
    */

    // Création de la socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
        perror("Erreur lors de la liaison avec l'adresse locale");  // La liaison a échoué
        return 1;
    }

    // Mise en écoute du serveur
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);  // Taille de la structure d'adresse

    /*
    * Boucle générale du serveur (infinie)
    */ 
    while (1) {
        /*
        * Code de l'intérieur de la boucle
        */

        // Validation de la connexion en renvoyant un descripteur de socket
        sockfdClient = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (sockfdClient < 0) {  // La connexion n'a pas été acceptée
            perror("Erreur lors de l'acceptation de la connexion avec le client");
            return 1;
        }

        // Lecture des données envoyées par le client
        bzero(buffer, 256);  // On peut lire jusqu'à 255 octers
        n = read(sockfdClient, buffer, 255);
        if (n < 0) {  // Problème lors de la lecture (on a lu moins de 0 octets)
            perror("Erreur de lecture depuis le socket client");
            return 1;
        }

        // Affichage du message reçu
        printf("Message reçu : %s\n", buffer);

        // Envoie de l'accusé de réception au client
        n = write(sockfdClient, "Message reçu par le serveur", 28);  // ACK
        if (n < 0) {  // L'envoi de l'accusé de réception a rencontré un problème
            perror("Erreur d'écriture de l'ACK sur le socket client");
            return 1;
        }
        close(sockfdClient);  // Fermeture de la socket de la connexion client
    }

    close(sockfd);  // Fermeture de la socket du serveur

    return 0;
}
