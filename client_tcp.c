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
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    // Demande de connexion avec le serveur
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la connexion");  // La connexion au serveur a échoué
        return 1;
    }

    // Ecriture du message à envoyer par le client
    printf("Veuillez entrer un message : ");
    bzero(buffer, SIZE);
    fgets(buffer, SIZE-1, stdin);

    // Ecriture des données sur le socket du client
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {  // L'écriture a échoué
        perror("Erreur d'écriture sur le socket");
        return 1;
    }

    // Lecture de la réponse du serveur sur le socket du client
    bzero(buffer, SIZE);
    n = read(sockfd, buffer, SIZE-1);
    if (n < 0) {  // La lecture a échoué
        perror("Erreur de lecture depuis le socket");
        return 1;
    }

    // Affichage de la réponse du serveur
    printf("Réponse du serveur : %s\n", buffer);

    close(sockfd);  // Fermeture de la socket du client

    return 0;
}
