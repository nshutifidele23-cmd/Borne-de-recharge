

/**
 * \file baseclient.c
 * \brief programmes pour gerer le client.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 22 decembre 2025
 * 
 * programmes pour gerer le client avec une structure client et des fonctions d'authenitifiaction, creation, modification et supression
 * 
 */

#include <stdio.h>
#include <string.h>
#include "baseclient.h"
#include <stdlib.h>

  /**
     * \struct Client
     * Client contient deux champs; un entier pour le numero de badge et une chaine de charactere pour le nom 
    */
typedef struct {
  
    int badge;
    char nom[50];   
} Client;

\def badge
volatile int badge; 

/**
 * \fn int base_client_authentification(void)
 * \brief fonction d'authentification du client
 * 
 * \return 1 si le client existe et 0 en cas d'echec
 */

int base_client_authentification(){

    Client nouveau_client;
    FILE *fichier_clients;
    lecteurcarte_initialiser();
    badge = lecteurcarte_lire_carte();

    nouveau_client.badge = badge;
    
    int i;
    fichier_clients = fopen("clients_liste.txt", "r");
   
    if (fichier_clients == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 0;
    }
    while (fscanf(fichier_clients, "%d %s", &i, nouveau_client.nom) != EOF) {
        if(i == nouveau_client.badge){
            printf("Authentification reussie, bienvenue %s \n",nouveau_client.nom);
            fclose(fichier_clients);
            return 1;
        }
        
    }

  return 0 ;
}

/**
 * \fn void base_client_creer_client(void)
 * \brief fonction de creation du nouveau client en cas d'echec d'authentification
 * 
 */
void base_client_creer_client(){
    Client nouveau_client;
    FILE *fichier_clients;

    nouveau_client.badge = badge;

    fichier_clients = fopen("clients_liste.txt", "a");

    printf("veuillezs'il vous plait renseigner votre nom svp: \n");
    fgets(nouveau_client.nom,sizeof(nouveau_client.nom),stdin);
    nouveau_client.nom[strcspn(nouveau_client.nom, "\n")] = 0; //pas de newline
    
    if (fichier_clients == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }
    fprintf(fichier_clients, "%d %s\n", nouveau_client.badge, nouveau_client.nom);
    printf("client creer avec numero de badge %d \n",nouveau_client.badge);
    fclose(fichier_clients);
}


/**
 * \fn void base_client_modifier_client(void)
 * \brief fonction de modification du nom d'un client existant
 * 
 */


void base_client_modifier_client(){
    Client client_a_modifier;
    FILE *fichier_clients;
    FILE *fichier_temp;

    fichier_clients = fopen("clients_liste.txt", "r");
    fichier_temp = fopen("temp.txt", "w");

    if (fichier_clients == NULL || fichier_temp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    while (fscanf(fichier_clients, "%d %49s", &client_a_modifier.badge, client_a_modifier.nom) == 2) {
        if (client_a_modifier.badge == badge) {
            printf("Entrez le nouveau nom: ");
            fgets(client_a_modifier.nom, sizeof(client_a_modifier.nom), stdin);
            client_a_modifier.nom[strcspn(client_a_modifier.nom, "\n")] = 0; //pas de newline
        }
        fprintf(fichier_temp, "%d %s\n", client_a_modifier.badge, client_a_modifier.nom);
    }

    fclose(fichier_clients);
    fclose(fichier_temp);

    remove("clients_liste.txt");
    rename("temp.txt", "clients_liste.txt");

    printf("Client modifié.\n");
}

/**
 * \fn void base_client_supprimer_client(void)
 * \brief fonction de supression du nom et du numero de badge d'un client existant
 * 
 */
void base_client_supprimer_client(){
    int badge_locale;
    Client client_a_supprimer;
    FILE *fin = fopen("clients_liste.txt", "r");
    FILE *fout = fopen("temp.txt", "w");

    if (!fin || !fout) {
        printf("Erreur ouverture fichier.\n");
        return;
    }


    while (fscanf(fin, "%d %49s", &badge_locale, client_a_supprimer.nom) == 2) {
        if (badge_locale == badge) {
            // on le prends pas
            continue;
        }
        fprintf(fout, "%d %s\n", badge_locale, client_a_supprimer.nom);
    }

    fclose(fin);
    fclose(fout);

    remove("clients_liste.txt");
    rename("temp.txt", "clients_liste.txt");

    printf("Client supprimé.\n");
}

