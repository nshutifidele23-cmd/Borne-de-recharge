/**
 * \file voyants.c
 * \brief Gestion des voyants de la borne de recharge.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 25 décembre 2025
 *
 * Ce module permet de contrôler les voyants de la borne :
 *  - LED CHARGE  (OFF / VERT / ROUGE)
 *  - LED DISPO   (OFF / VERT)
 *  - LED DEFAUT  (OFF / ROUGE)
 *  - LED PRISE
 *  - LED TRAPPE
 *
 * Fonctionnalités :
 *  - Connexion automatique à la mémoire partagée
 *  - Mise à jour  des voyants
 *  - Fonctions utilitaires d'initialisation et clignotement
 */



#include <donnees_borne.h>
#include <memoire_borne.h>
#include "voyants.h"
#include <stdio.h>
#include "boutons.h"
#include <unistd.h>
#include "lecteurcarte.h"
#include "generateur_save.h"
#include "prise.h"

/* Pointeur vers la mémoire partagée */
entrees* io_l;
int shmid_l ;


/**
 * \fn void voyants_ensure_io()
 * \brief Initialise l'accès à la mémoire partagée si nécessaire.
 *
 */
void voyants_ensure_io()
{
    if (io_l != NULL) return;
    io_l = acces_memoire(&shmid_l);
    if (io_l == NULL) {
        fprintf(stderr, "Erreur: acces memoire partagee impossible\n");
    }
}

/**
 * \fn static int voyants_check_io()
 * \brief Vérifie et assure la connexion à la mémoire partagée.
 * \return 1 si io_l est valide, 0 sinon.
 */
static int voyants_check_io()
{
    if (io_l == NULL) voyants_ensure_io();
    return (io_l != NULL);
}
/**
 * \fn void voyants_set_charge_vert()
 * \brief Active la LED charge en VERT 
 */
void voyants_set_charge_vert(){
    
    if (!voyants_check_io()) return; //erreur de segma
    
        io_l->led_charge = VERT;
      
}

/**
 * \fn void voyants_set_charge_rouge()
 * \brief Active la LED charge en ROUGE .
 */
void voyants_set_charge_rouge(){
    
    if (!voyants_check_io()) return; 
    io_l->led_charge = ROUGE;
}

/**
 * \fn void voyants_set_charge_off()
 * \brief Éteint la LED de charge.
 */
void voyants_set_charge_off(){
    
    if (!voyants_check_io()) return;
    io_l->led_prise = OFF; 
}  

/**
 * \fn void voyants_set_dispo_off()
 * \brief Éteint la LED dispo.
 */
void voyants_set_dispo_off(){
    if (!voyants_check_io()) return;
    io_l->led_dispo = OFF;
}
/**
 * \fn void voyants_set_dispo_on()
 * \brief Allume la LED dispo.
 */
void voyants_set_dispo_on(){
    if (!voyants_check_io()) return;
    io_l->led_dispo = VERT;
}


/**
 * \fn void voyants_init()
 * \brief Initialise l'état par défaut des voyants.
 *
 * - Charge  : OFF  
 * - Dispo   : VERT  
 * - Défaut  : OFF  
 * - Prise   : OFF  
 * - Trappe  : OFF
 */
void voyants_init(){
    if (!voyants_check_io()) return;
    io_l->led_charge = OFF;
    io_l->led_dispo  = VERT;
    io_l->led_defaut = OFF;
    io_l->led_prise  = OFF;
    io_l->led_trappe = OFF;
}

/**
 * \fn void voyants_toggle()
 * \brief Fait clignoter la LED charge.
 */
void voyants_toggle(){
    io_l->led_charge = VERT;
    sleep (1);
    io_l->led_charge = OFF;
    sleep (1);
}

/**
 * \fn void voyants_toggle_default()
 * \brief Fait clignoter la LED défaut en ROUGE.
 */
void voyants_toggle_default(){
    io_l->led_defaut = ROUGE;
    sleep (1);
    io_l->led_defaut = OFF;
    sleep (1);
}

/**
 * \fn void voyants_set_defaut_off()
 * \brief Allumer la LED defaut.
 *
 */
void voyants_set_defaut_off(){
    if (!voyants_check_io()) return;
    io_l->led_defaut = VERT;
}