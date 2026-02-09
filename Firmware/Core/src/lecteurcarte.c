/**
 * \file lecteurcarte.c
 * \brief Gestion du lecteur de carte utilisateur.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 25 décembre 2025
 *
 * Ce module gère :
 *  - l’initialisation du lecteur de carte
 *  - la lecture du numéro de badge utilisateur
 *  - l’attente du retrait de la carte
 *
 */

#include <unistd.h>
#include "lecteurcarte.h"
#include <stdio.h>
#include "baseclient.h"
#include "voyants.h"

volatile int numero;

/**
 * \fn void lecteurcarte_initialiser()
 * \brief Initialise le lecteur de carte.
 *
 * Prépare le lecteur de carte  pour permettre la lecture
 * des badges utilisateurs.
 */

void lecteurcarte_initialiser()
{
   initialisations_ports();

}

/**
 * \fn int lecteurcarte_lire_carte()
 * \brief Attend l'insertion d'une carte et lit le numéro de badge.
 *
 * Affiche une demande à l’utilisateur, attend l’insertion,
 * puis récupère le numéro de carte.
 *
 * \return le numéro du badge lu.
 */
int lecteurcarte_lire_carte()
{
    
    //lire carte
    printf("veuillez sil vous plait rentrez le numero: \n");
    attente_insertion_carte();
    numero = lecture_numero_carte();
    printf("le numero de badge: %d \n",numero);

    //liberez carte
    
    return numero;
    
}

/**
 * \fn int lecteurcarte_retirer_carte()
 * \brief Attend le retrait de la carte par l’utilisateur.
 *
 * Bloque l’exécution tant que la carte n’est pas retirée.
 *
 * \return 1 lorsque la carte a bien été retirée.
 */
int lecteurcarte_retirer_carte()
{
    attente_retrait_carte();
    //liberation_ports();
    return 1;

}

/*int lecteur_carte_valide_carte(){
//pas de parametres
//demande n de badge
//retourne 1 si la carte est valide 0 sinon
//appel authentifier
//appel voyants pour voir etat_dispo
    int authentification = 1;
    //authentification = base_clients_authentifier(numero);
    if (authentification == 1){
        printf ("success \n");
     
    }
    else if (authentification == 100000) //nombre aleatoire
    {
        printf ("client non trouve");
          
    }
    
     return authentification;
}*/
    


