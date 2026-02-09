/**
 * \file boutons.c
 * \brief programmes pour gerer les boutons sur la borne.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 22 decembre 2025
 * 
 * programmes pour gerer l'appuie et le relachement du bouton charge et stop
 * 
 */

#include <donnees_borne.h>
#include <memoire_borne.h>
#include "voyants.h"
#include <stdio.h>
#include "boutons.h"


/**
 * \var entrees* io_g
 * \brief Pointeur vers la structure des entrées partagées.
 */
entrees* io_b;

/**
 * \var int shmid_b
 * \brief Identifiant de la mémoire partagée.
 */
int shmid_b;

/**
 * \fn void bouton_ensure_io()
 * \brief fonction pour s'assurer que la connexion a la memoire partagee est etabli.
 * \return si la connexion est deja etabli prealablement
 * 
 */
void bouton_ensure_io()
{
    if (io_b != NULL) return;
    io_b = acces_memoire(&shmid_b);
    if (io_b == NULL) {
        fprintf(stderr, "Erreur: acces memoire partagee impossible\n");
    }
}

/**
 * \fn static int bouton_check_io()
 * \brief fonction pour verifier et etablir la connexion si necessaire.
 * \return True si la variable io_b n'est pas NULL et 0 si non
 * 
 */
static int bouton_check_io()
{
    if (io_b == NULL) bouton_ensure_io();
    return (io_b != NULL);
}

/**
 * \fn int bouton_appuyer_pour_charger()
 * \brief fonction pour mettre a 1 le bouton charge.
 * \return 1 quand le bouton charge est appuye
 * 
 */
int bouton_appuyer_pour_charger(){
    int en_charge;
    if (!bouton_check_io()) return 0; //erreur de segma
    //io_b->bouton_charge;
    en_charge = io_b ->bouton_charge;
    return en_charge;
}

/**
 * \fn int bouton_relacher_pour_charger()
 * \brief fonction pour mettre a 0 le bouton charge.
 * \return 0 quand la foction est appele
 * 
 */
int bouton_relacher_pour_charger(){
    int charge;
    if (!bouton_check_io()) return 0; //erreur de segma
    io_b->bouton_charge = 0;
    charge = io_b ->bouton_charge;
    //printf("%d \n",charge);
    
    return charge;
    
    }

/**
 * \fn int bouton_appuyer_pour_stop()
 * \brief fonction pour mettre a 1 le bouton stop.
 * \return 1 quand le bouton stop est appuye
 * 
 */
int bouton_appuyer_pour_stop(){
    int stop;
    if (!bouton_check_io()) return 0; //erreur de segma
    //io_b->bouton_stop;
    stop = io_b ->bouton_stop;
    return stop;

    }

/**
 * \fn int bouton_relacher_stop()
 * \brief fonction pour mettre a 0 le bouton stop.
 * \return 0 quand la foction est appele
 * 
 */
int bouton_relacher_stop(){
    int stop;
    if (!bouton_check_io()) return 0; //erreur de segma
    io_b->bouton_stop = 0;
    stop = io_b ->bouton_stop;
    return stop;

    }