#ifndef LECTEURCARTE_H
#define LECTEURCARTE_H
#include <lcarte.h>

void lecteurcarte_initialiser();
int lecteurcarte_lire_carte();
int lecteur_carte_valide_carte();
void lecteur_carte_demande_creation_client();//ne prends rien en parametre, appel la fonction creer client et lui passe le nom et prenom
void lecteur_carte_demande_modifier_client();//ne prends rien en parametre, appel la fonction modifier client et lui passe le numero de badge
void lecteur_carte_demande_supprimer_client();//ne prends rien en parametre, appel la fonction supprimer client et lui passe le numero de badge
int lecteurcarte_retirer_carte();
#endif // LECTEURCARTE_H
