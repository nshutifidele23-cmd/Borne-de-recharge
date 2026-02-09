/**
 * \file prise.c
 * \brief Gestion de la prise et de la trappe de la borne.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 25 décembre 2025
 *
 * Ce module permet de contrôler :
 *  - l'état de la trappe (ouverte / fermée)
 *  - la LED prise (prise connectée ou non)
 *
 * Les accès aux E/S se font via la mémoire partagée,
 * avec une initialisation automatique.
 */


#include <donnees_borne.h>
#include <memoire_borne.h>
#include "voyants.h"
#include <stdio.h>
#include "prise.h"

/* Pointeur vers la mémoire partagée */
entrees* io_p;
int shmid_p;

/**
 * \fn void prise_ensure_io()
 * \brief Initialise l'accès à la mémoire partagée si nécessaire.
 *
 * Fonction de sécurité permettant d'éviter les accès NULL.
 */
void prise_ensure_io()
{
    if (io_p != NULL) return;
    io_p = acces_memoire(&shmid_p);
    if (io_p == NULL) {
        fprintf(stderr, "Erreur: acces memoire partagee impossible\n");
    }
}

/**
 * \fn int prise_check_io()
 * \brief Vérifie et assure la connexion à la mémoire partagée.
 * \return 1 si io_p est valide, 0 sinon.
 */
int prise_check_io()
{
    if (io_p == NULL) prise_ensure_io();
    return (io_p != NULL);
}

/**
 * \fn void prise_deverouiller_trap()
 * \brief Déverrouille la trappe et allume l’indication correspondante.
 *        permet d'avoir acces a la checkbox pour brancher la prise
 *
 * Utilisé lorsque l'utilisateur peut brancher ou retirer le câble.
 */
void prise_deverouiller_trap(){
    
    if (io_p == NULL) prise_ensure_io();
    io_p->led_trappe = VERT;

}

/**
 * \fn void prise_verouiller_trap()
 * \brief Verrouille la trappe et éteint l’indication.
 *
 * Utilisé lorsque le câble doit rester bloqué pendant la charge.
 */
void prise_verouiller_trap(){
    
    if (io_p == NULL) prise_ensure_io();
    io_p->led_trappe = OFF;

}
/**
 * \fn void prise_led_prise_on()
 * \brief Allume la LED prise.
 */
void prise_led_prise_on(){
    
    if (io_p == NULL) prise_ensure_io();
    io_p->led_prise = VERT;

}

/**
 * \fn void prise_led_prise_off()
 * \brief Éteint la LED prise.
 */
void prise_led_prise_off(){
    
    if (io_p == NULL) prise_ensure_io();
    io_p->led_prise = OFF;

}