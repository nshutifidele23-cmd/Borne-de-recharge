/**
 * \file borne.c
 * \brief Programme principal de gestion de la borne de recharge.
 * \author (Nshuti Nkurikiyinka Fidele, Mapako Eddy)
 * \version 1.0
 * \date 25 décembre 2025
 *
 * Ce programme assure la gestion complète d'une session de recharge :
 *  - Authentification du client via sa carte
 *  - creation du client
 *  - Choix des actions (charger, modifier compte, supprimer compte)
 *  - Gestion des voyants et de l'affichage utilisateur
 *  - Détection et gestion du bouton CHARGE
 *  - Séquence complète de charge (DC → AC → AC_CL → fin de charge)
 *  - Gestion sécurisée du STOP avec authentification
 *  - Manipulation de la trappe et du contacteur
 *  - Validation de la carte pour débuter et terminer la charge
 */


#include <stdio.h>
#include <memoire_borne.h>
#include <donnees_borne.h>
#include "lecteurcarte.h"
#include "voyants.h"
#include "boutons.h"
#include <unistd.h>
#include "lecteurcarte.h"
#include "generateur_save.h"
#include "prise.h"
#include "baseclient.h"



int main()
{
    /* Variables liées aux états utilisateur et matériel */
    int carte;

    int charge;  //pour mettre le bouton charge a 1
   //mettre a zero le stop
    int depart_timer;
    int retire_carte;
    int stop;
    int client_charge;
    int carte_2=0;
    int carte_stop;
    volatile int  choix_stop = 0;
     
    
    /* Accès mémoire partagée */
    entrees* io = NULL;
    int shmid ;

    if (io != NULL) return 0;
    io = acces_memoire(&shmid);
    if (io == NULL) {
        fprintf(stderr, "Erreur: acces memoire partagee impossible\n");
    }


    /* Authentification du client */
    carte = base_client_authentification();
     
  
    //validation = lecteur_carte_valide_carte();
    
    voyants_init();
    voyants_ensure_io();
    /**
     * ============================
     *   CLIENT RECONNU
     * ============================
     */
   
        if(carte == 1){
            char choix;
            printf("Appuyer [C] pour charger le vehicule, [M] pour modifier votre compte, [S] pour supprimer votre compte\n");
            scanf(" %c",&choix);
            getchar(); //pour enlever le newline du buffer
            switch (choix){
                case 'M':
                case 'm':
                    base_client_modifier_client();
                    printf("Au revoir.\n");
                     return 0;
                    break;
                case 'S':
                case 's':
                    base_client_supprimer_client();
                    printf("Au revoir.\n");
                    return 0;
                    break;
                case 'C':
                case 'c':
                    printf("Appuyer sur charge et Relaxez vous pendant que le vehicule se charge...\n");
                    client_charge = 1;
                    break;
                default:
                    printf("Choix invalide au revoir.\n");
                     return 0;
                    break;
            }
            
        /**
         * Attente du bouton CHARGE pendant 8 secondes
         * avec clignotement des voyants
         */ 
            depart_timer = io->timer_sec;
            while(io->timer_sec - depart_timer != 8){

                while(1){
                    voyants_toggle();//faire clignoter ici
                    charge= bouton_appuyer_pour_charger(); //le changement d'etat ne s'opere que q l'execution suivqnte
                    if(charge == 1 || (io->timer_sec - depart_timer) >= 8){
                        break;
                    }
                }   
                break;

            }

         /**
         *   DÉBUT PROCÉDURE DE CHARGE
         */
        if (client_charge == 1){
            printf("veuillez s'ils vous plait retirer la carte...\n");

               /**
                 * Boucle de surveillance tension + bouton STOP
                 */
        while(1){//pour stop 
        /* Validation retrait carte */       
        if (charge == 1 ){
            bouton_relacher_pour_charger();
            voyants_set_charge_vert();// ici
            voyants_set_dispo_off();
            retire_carte = lecteurcarte_retirer_carte();
            while (retire_carte != 1);
            voyants_set_charge_rouge(); //rouge ici
            prise_deverouiller_trap();
        
            generateur_save_generer_dc();
            sleep(1);
            while (io->gene_u != 9){
                sleep(1);
                stop = bouton_appuyer_pour_stop();
                if(io->gene_u == 9){
                    prise_led_prise_on();
                    prise_verouiller_trap();
                    generateur_save_ac();
                    sleep(1);
                    break;
                }
                else if(stop == 1){
                    carte_stop = base_client_authentification();
                    if (carte_stop == 1){
                        
                        choix_stop = 1;
                        sleep(0.5);
                        break;
                    }
                  
            
           }
           if(stop == 1 ){
                    
                    
                    if (choix_stop == 1){
                        //sleep(1);
                        break;
                    }
                    else if (choix_stop != 1) {
                        carte_stop = base_client_authentification();
                        if (carte_stop == 1){
                            sleep(1);
                            choix_stop = 1;
                            break;
                    }
                
                    }
            
           }
        }   
        stop = bouton_appuyer_pour_stop();
        /**
          * Gestion AC_CL + Contacteur
          */
        if(io->gene_u == 6 && stop == 0){
            generateur_save_ac_cl();
            generateur_save_contacteur();
            sleep(0.5);
        }
        else if (stop == 1 ){
          if (choix_stop == 1){
                        //sleep(1);
                        break;
                    }
                    else if (choix_stop != 1) {
                        carte_stop = base_client_authentification();
                        if (carte_stop == 1){
                            sleep(1);
                            choix_stop = 1;
                            break;
                    }
                      
                    }
        }
        /**
          * Phase retour DC + ouverture trappe
          */
        while(1){
            //sleep(1);
            stop = bouton_appuyer_pour_stop();
            if (io->gene_u == 9 && stop == 0){
                sleep(0.5);
                
                generateur_save_contacteur_fermer();
                   // }
                generateur_save_generer_dc();
                //prise_deverouiller_trap();
               voyants_set_charge_vert();// vert ici
               
                break;
            }
            sleep(0.5);
           if (stop == 1){
                if (choix_stop == 1){
                        //sleep(1);
                       // break;
                       continue;
                    }
                else if (choix_stop != 1) {
                        carte_stop = base_client_authentification();
                        if (carte_stop == 1){
                            sleep(1);
                          choix_stop = 1;
                            //break;
                    }
                     
                      break;
                    }
                    break;
                  
        }                 

        }
        /**
            * Fin de charge : Vérification carte client
            */
        sleep(1);
       if(choix_stop != 1){
            carte_2 = base_client_authentification();
       }
        while (1){
            
                while (io->gene_u != 12 && stop == 0){
                    sleep(1);
                    stop = bouton_appuyer_pour_stop();
                    if(carte_2 == 1){
                        prise_deverouiller_trap();
                        sleep(0.5);
                    }
                    if(io->gene_u == 12 && stop == 0 && carte_2 == 1){
                        sleep(0.5);
                        voyants_set_charge_off();//OFF ici
                        prise_led_prise_off();
                        prise_verouiller_trap();
                
                   
                    break;
                    }
                    else if(carte_2 != 1){
                        printf("veuillez s'il vous plait utilisez la bonne carte\n");
                        sleep(1);
                       carte_2 = base_client_authentification();
                    }
                    else if (stop == 1){
          
                          if (choix_stop == 1){
                        //sleep(1);
                        break;
                    }
                    else if (choix_stop != 1) {
                        carte_stop = base_client_authentification();
                        
                        if (carte_stop == 1){
                            sleep(1);
                        choix_stop = 1;
                            break;
                    }
                    else{
                        continue;
                    }
                     
                    }
                    }
                  
                }
              break;  
            }
         

       // break;
     
           //printf("je suis ici 2\n");

    }
    else if(io->timer_sec - depart_timer >= 8){
        bouton_relacher_pour_charger();
        voyants_set_charge_off();// off ici
        voyants_set_dispo_on();
        break;

    }
    if (choix_stop == 1){
        prise_deverouiller_trap();
        while (io->gene_u == 0 || io->gene_u == 6|| io->gene_u == 9 || io->gene_u == 12){
            sleep(1);
           // if(io->gene_u == 12){
                        //sleep(1);
                        //printf("je suis ici 1\n");
                        voyants_set_charge_off();//OFF ici
                        prise_led_prise_off();
                        generateur_save_contacteur_fermer();
                        //prise_verouiller_trap();
                        sleep(5);
                        choix_stop = 0;
                        bouton_relacher_stop();
                       
                        
                    break;
                   // }

    }
    break;
    }
    else{
        break;
    }
}
   }
   else if (client_charge != 1) {
         printf("vous n'avez pas appuyer, veuillez relancer la borne\n");
          voyants_init();
        bouton_relacher_stop();
        bouton_relacher_pour_charger();
        generateur_save_contacteur_fermer();
    }

    }
     /**
     *   CLIENT NON RECONNU
     */
    else {
        char choix_creation;
        depart_timer = io->timer_sec;
            printf("veuillez attendre la fin du clignotement\n");
            while(io->timer_sec - depart_timer != 8){

                while(1){
                    voyants_toggle_default();//faire clignoter ici
                    if((io->timer_sec - depart_timer) >= 8){
                        break;
                    }
                }   
                break;

            }
        printf("Client non reconnu, voulez vous creez un comptez [Y] [N]...\n");
        scanf(" %c",&choix_creation);
        
        getchar(); //pour enlever le newline du buffer
        if(choix_creation == 'Y' || choix_creation == 'y'){
            base_client_creer_client();
        }
        else{
            printf("Au revoir !\n");
        }
    }
    if (choix_stop == 1){
        prise_deverouiller_trap();
        while (io->gene_u == 0 || io->gene_u == 6|| io->gene_u == 9 || io->gene_u == 12){
            //sleep(1);
           // if(io->gene_u == 12){
                        sleep(1);
                        //printf("je suis ici 1\n");
                        voyants_set_charge_off();//OFF ici
                        prise_led_prise_off();
                        generateur_save_contacteur_fermer();
                        //prise_verouiller_trap();
                        sleep(5);
                        
                    break;
                   // }

    }
}

    /**
     *   REMISE À ZÉRO SÉCURISÉE
     */
    //generateur_save_contacteur_fermer();
    prise_verouiller_trap();
    voyants_init();
    bouton_relacher_stop();
    bouton_relacher_pour_charger();
    generateur_save_contacteur_fermer();
    prise_verouiller_trap();
    
    
   

    return 0;

}

