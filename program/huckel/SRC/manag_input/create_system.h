/////////////////////////////
// plug cluster and system
////////////////////////////
#include "../huckel/header.h"

#ifndef _create_system_H
#define _create_system_H

void create_stm(atom *SYSTEM, int nb_atom_tot, atom *MOL, int nb_atom_mol, atom* SURFACE, int nb_atom_surface,
		   atom *TIP, int nb_atom_tip, int *index_pos_tip, int nb_index_pos_tip, double dist_mol_surf, double dist_mol_tip,
		   int *index_elec_1, int *index_elec_2 );

void create_junction(atom *SYSTEM, int nb_atom_tot, atom *MOL, int nb_atom_mol, 
		   atom *CLUSTER, int nb_atom_cluster, int *index_connect_cluster, int nb_contact_with_mol,   int *connect, 
		   int *index_elec_1, int *index_elec_2 );

void write_system(atom *SYSTEM,int nb_atom_tot,char *file_name);
void write_system_continue(atom *SYSTEM,int nb_atom_tot,char *file_name,int it);

#endif