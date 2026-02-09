/**
 * \file generateur_save.c
 * \brief Programme pour gérer le générateur et le contacteur via la mémoire partagée.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 25 décembre 2025
 *
 * Ce fichier permet d'accéder à la mémoire partagée de la borne et de piloter :
 *  - Le générateur PWM (DC, AC 1kHz, AC CL)
 *  - Le contacteur AC
 */


#include <donnees_borne.h>
#include <memoire_borne.h>
#include "voyants.h"
#include <stdio.h>
#include "generateur_save.h"

/**
 * \var entrees* io_g
 * \brief Pointeur vers la structure des entrées partagées.
 */
entrees* io_g;
/**
 * \var int shmid_g
 * \brief Identifiant de la mémoire partagée.
 */
int shmid_g;

/**
 * \fn void generateur_save_ensure_io()
 * \brief Fonction pour s'assurer que la connexion à la mémoire partagée est établie.
 *
 * Établit l'accès à la mémoire partagée si ce n'est pas déjà fait.
 * Affiche un message d'erreur si la connexion est impossible.
 */
void generateur_save_ensure_io()
{
    if (io_g != NULL) return;
    io_g = acces_memoire(&shmid_g);
    if (io_g == NULL) {
        fprintf(stderr, "Erreur: acces memoire partagee impossible\n");
    }
}

/**
 * \fn int generateur_save_check_io()
 * \brief Vérifie si la mémoire partagée est accessible et l'établit si nécessaire.
 * \return 1 si l'accès est valide, 0 sinon.
 */
int generateur_save_check_io()
{
    if (io_g == NULL) generateur_save_ensure_io();
    return (io_g != NULL);
}

/**
 * \fn void generateur_save_generer_dc()
 * \brief genere 12v DC.
 */
void generateur_save_generer_dc(){
    if (!generateur_save_check_io()) return; 
    
   
    io_g ->gene_pwm = DC;
}

/**
 * \fn void generateur_save_ac()
 * \brief Genere AC de 1KHz.
 */

void generateur_save_ac(){
    if (!generateur_save_check_io()) return; 

    io_g ->gene_pwm = AC_1K;
}

/**
 * \fn void generateur_save_ac_cl()
 * \brief Generer PWM AC variable.
 */
void generateur_save_ac_cl(){
    if (!generateur_save_check_io()) return; 
   
    io_g ->gene_pwm = AC_CL;
}

/**
 * \fn void generateur_save_contacteur()
 * \brief Active le contacteur AC (fermeture du relais).
 */
void generateur_save_contacteur(){
    if (!generateur_save_check_io()) return; 
   
    io_g->contacteur_AC = 1;
}

/**
 * \fn void generateur_save_contacteur_fermer()
 * \brief Désactive le contacteur AC (ouverture du relais).
 */
void generateur_save_contacteur_fermer(){
    if (!generateur_save_check_io()) return; 
   
    io_g->contacteur_AC = 0;
}



