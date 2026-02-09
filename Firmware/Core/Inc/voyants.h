#ifndef VOYANTS_H
#define VOYANTS_H
#include "donnees_borne.h"
#include "boutons.h"
#include "lecteurcarte.h"
#include "generateur_save.h"
/* Contrôle des voyants (si mem == NULL utilise la memoire partagée interne) */
void voyants_set_charge();
void voyants_set_dispo_off();
void voyants_set_dispo_on();
void voyants_toggle();
void voyants_ensure_io();
void voyants_init();
void voyants_set_charge_rouge();
void voyants_set_charge_off();
void voyants_set_charge_vert();
void voyants_toggle_default();
void voyants_set_defaut_off();

#endif