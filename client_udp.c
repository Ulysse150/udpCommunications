#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE 100
#define PORT 9600

int main(int argc, char *argv[]) {
    /*
    * Variables du client
    * Déclarer ici les variables suivantes :
    * - sockfd le descripteur de socket
    * - structure d'adresse du serveur
    * - pointeur vers la structure descriptive de machine (hostent)
    * - zone de mémoire destinée à accueillir la chaine
    * - taille de la chaine à envoyer
    */ 
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[SIZE];

    /*
    * Code du client
    *
    * - Ouvrir le socket du client
    * - Récupérer l'adresse IP du serveur à partir de son nom
    *   donné en ligne de commande
    * - Remplir la structure d'adresse du serveur
    * - Lire une ligne de l'entrée standard
    * - Envoyer la chaine lue au serveur
    */

    // Création de la socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {  // La création a échoué
        perror("Erreur lors de la création du socket");
        return 1;
    }

    // Résolution du nom d'hôte passé en argument
    server = gethostbyname(argv[1]);
    if (server == NULL) {  // La résolution a échoué
        perror("Erreur, hôte non trouvé");
        return 1;
    }

    // Préparation de la structure de connexion avec le serveur
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    // Ecriture du message à envoyer
    printf("Veuillez entrer un message : ");
    bzero(buffer, SIZE);
    fgets(buffer, SIZE-1, stdin);

    // Envoie des données au serveur
    n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (n < 0) {  // L'envoie des données a échoué
        perror("Erreur d'envoi de la requête");
        return 1;
    }

    // Réception des données renvoyées par le serveur
    bzero(buffer, SIZE);
    socklen_t serv_len = sizeof(serv_addr);
    n = recvfrom(sockfd, buffer, SIZE-1, 0, (struct sockaddr *) &serv_addr, &serv_len);
    if (n < 0) {  // Echéc de la réception des données
        perror("Erreur de réception");
        return 1;
    }

    // Affichage de la réponse du serveur
    printf("Réponse du serveur : %s\n", buffer);

    close(sockfd);  // Fermeture de la socket du client

    return 0;
}
