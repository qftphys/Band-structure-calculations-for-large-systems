#include "../defMacro.h"
#include "../huckel/header.h"
#include "../huckel/read_input_hkl.h"




void create_stm(atom *SYSTEM, int nb_atom_tot, atom *MOL, int nb_atom_mol, atom* SURFACE, int nb_atom_surface,
		   atom *TIP, int nb_atom_tip, int *index_pos_tip, int nb_index_pos_tip, double dist_mol_surf, double dist_mol_tip,
		   int *index_elec_1, int *index_elec_2 )
{
  
  
  int i;
  int size_system;

  
  double xM = 0, yM = 0, zM = 0;
  double xS = 0, yS = 0, zS = 0;
  double xT = 0, yT = 0, zT = 0;
  double dx,dy,dz;
  
  size_system = nb_atom_mol;
  
  ///////////////////////////////////////////////////
  // copy the molecule in the sys
  ///////////////////////////////////////////////////
  for(i=0;i<nb_atom_mol;i++)
  {
      // copy the molecule in the sys_temp
      strcpy(SYSTEM[i].atomTypeChar,MOL[i].atomTypeChar);	SYSTEM[i].atomtype = MOL[i].atomtype;
      SYSTEM[i].x = MOL[i].x;		SYSTEM[i].y = MOL[i].y; 		SYSTEM[i].z = MOL[i].z;  
      
      // compute the center of the molecule
      xM += MOL[i].x; yM += MOL[i].y; zM += MOL[i].z;
      
  }
  xM /= nb_atom_mol;	yM /= nb_atom_mol; zM/= nb_atom_mol;

  /////////////////////////////////////////////////
  // compute the mean position of the surface
  ///////////////////////////////////////////////////
  for(i=0;i<nb_atom_surface;i++)
  {
      xS += SURFACE[i].x;	
      yS += SURFACE[i].y;
      
      if(i==0)
	zS = SURFACE[i].z;
      else
      {
	if(SURFACE[i].z > zS)
	  zS = SURFACE[i].z;
      }
  }
  xS /= nb_atom_surface;	yS/=nb_atom_surface;
  
  
  // translation vector for the first cluster
  dx = xM-xS;	dy = yM-yS;	dz = zM-zS;
  
  
     
  // move the surface
  for(i=0;i<nb_atom_surface;i++)
  {
    // store the cluster
    strcpy(SYSTEM[size_system].atomTypeChar,SURFACE[i].atomTypeChar);
    SYSTEM[size_system].atomtype = findIndex(SURFACE[i].atomTypeChar);
    SYSTEM[size_system].x = SURFACE[i].x + dx;
    SYSTEM[size_system].y = SURFACE[i].y + dy;
    SYSTEM[size_system].z = SURFACE[i].z + dz - dist_mol_surf;
    
    // index of the cluster atom in the SYSTEM
    index_elec_1[i] = size_system;
    
    
    // increment the system size
    size_system++;
  }

    
    
  // find the position of the terminal atom of the tip
  for(i=0;i<nb_atom_tip;i++)
  {
    if(i == 0)
    {
      xT = TIP[i].x;
      yT = TIP[i].y;
      zT = TIP[i].z;
    }
    
    else
    {
	  if(TIP[i].z < zT) 
	  {
	    xT = TIP[i].x; 
	    yT = TIP[i].y; 
	    zT = TIP[i].z; 
	  }
    }
  }

  
  ////////////////////////////////////////////////////////////////
  // find the position of the terminal atom above the molecule
  ////////////////////////////////////////////////////////////////
  
  
  // if a position was specifeid in the input file
  if(index_pos_tip[0] != -1)
  {
    xM = 0; yM = 0; zM = 0;
    for(i=0;i<nb_index_pos_tip;i++)
    {
      xM += MOL[index_pos_tip[i]].x;	
      yM += MOL[index_pos_tip[i]].y;	
      zM += MOL[index_pos_tip[i]].z;
    
    }
     xM/=nb_index_pos_tip;	 	yM/=nb_index_pos_tip;		zM/=nb_index_pos_tip;
  }
  
  
 
  ////////////////////////////////////////////////
  // translation vector for the first cluster
  ////////////////////////////////////////////////
  dx =  xM-xT;	dy = yM-yT;	dz = zM-zT;

   //////////////////////////////
   // move the tip
  ///////////////////////////////
  for(i=0;i<nb_atom_tip;i++)
  {
   
    // store the cluster
    strcpy(SYSTEM[size_system].atomTypeChar,TIP[i].atomTypeChar);
    SYSTEM[size_system].atomtype = findIndex(TIP[i].atomTypeChar);
    SYSTEM[size_system].x = TIP[i].x + dx;
    SYSTEM[size_system].y = TIP[i].y + dy;
    SYSTEM[size_system].z = TIP[i].z + dz + dist_mol_tip;
    
    // index of the cluster atom in the SYSTEM
    index_elec_2[i] = size_system;
    
    // increment the system size
    size_system++;
  }
  
}








void create_junction(atom *SYSTEM, int nb_atom_tot, atom *MOL, int nb_atom_mol,
		   atom *CLUSTER, int nb_atom_cluster, int *index_connect_cluster, int nb_contact_with_mol,   int *connect, 
		   int *index_elec_1, int *index_elec_2 )
{
  
  int i;
  //int nself_system=0;
  int size_system;

    
  //double x0 = MOL[connect[0]].x;
  //double y0 = MOL[connect[0]].y;
  double z0 = MOL[connect[0]].z;
     
  double x1 = MOL[connect[1]].x;
  double y1 = MOL[connect[1]].y;
  double z1 = MOL[connect[1]].z;
  
  double xC =0, yC=0, zC=0;
  double dx,dy,dz;
  
  // alloc memory
  size_system = nb_atom_mol;
  
 // copy the molecule in the sys_temp
  for(i=0;i<nb_atom_mol;i++)
  {
      strcpy(SYSTEM[i].atomTypeChar,MOL[i].atomTypeChar);	SYSTEM[i].atomtype = MOL[i].atomtype;
      SYSTEM[i].x = MOL[i].x;		SYSTEM[i].y = MOL[i].y; 		SYSTEM[i].z = MOL[i].z;   
  }
  
  
  // determine the point where the molecule contact the cluster
  for(i=0;i<nb_contact_with_mol;i++)
  {
    xC += CLUSTER[index_connect_cluster[i]].x;
    yC += CLUSTER[index_connect_cluster[i]].y;
    zC += CLUSTER[index_connect_cluster[i]].z;
  }
  
  xC /= nb_contact_with_mol;	yC /= nb_contact_with_mol; zC /=nb_contact_with_mol;
  
  // translation vector for the first cluster
  dx = x1-xC;	dy = y1-yC;	dz = z1-zC;
  
  // move the first cluster
  for(i=0;i<nb_atom_cluster;i++)
  {
    // store the cluster
    strcpy(SYSTEM[size_system].atomTypeChar,CLUSTER[i].atomTypeChar);
    SYSTEM[size_system].atomtype = findIndex(CLUSTER[i].atomTypeChar);
    SYSTEM[size_system].x = CLUSTER[i].x + dx;
    SYSTEM[size_system].y = CLUSTER[i].y + dy;
    SYSTEM[size_system].z = CLUSTER[i].z + dz + distSAu;
    
    // index of the cluster atom in the SYSTEM
    index_elec_1[i] = size_system;
    
    
    // increment the system size
    size_system++;
  }
  
  
  // flip the cluster
  for(i=0;i<nb_atom_cluster;i++)
      CLUSTER[i].z = -CLUSTER[i].z;
  
  // determine the point where the molecule contact the cluster
  xC = 0;	yC = 0;	zC = 0;
  for(i=0;i<nb_contact_with_mol;i++)
  {
    xC += CLUSTER[index_connect_cluster[i]].x;
    yC += CLUSTER[index_connect_cluster[i]].y;
    zC += CLUSTER[index_connect_cluster[i]].z;
  }
  
  xC /= nb_contact_with_mol;	yC /= nb_contact_with_mol; zC /=nb_contact_with_mol;
    
  
  // translation vector for the second cluster
  dz = zC-z0; 
  
  
    // move the second cluster
  for(i=0;i<nb_atom_cluster;i++)
  {
   
    // store the cluster
    strcpy(SYSTEM[size_system].atomTypeChar,CLUSTER[i].atomTypeChar);
    SYSTEM[size_system].atomtype = findIndex(CLUSTER[i].atomTypeChar);
    SYSTEM[size_system].x = CLUSTER[i].x + dx;
    SYSTEM[size_system].y = CLUSTER[i].y + dy;
    SYSTEM[size_system].z = CLUSTER[i].z - dz - distSAu;
    
    
    // index of the cluster atom in the SYSTEM
    index_elec_2[i] = size_system;
    
    // increment the system size
    size_system++;
  }
  
}



void write_system(atom *SYSTEM,int nb_atom_tot,char *file_name)
{
  int  i;
  FILE *f;

  f = fopen(file_name,"w");
  if (!f) 
  {
      printf("can't create %s \n",file_name);
      exit(1);
  } 

  fprintf(f,"   %d\n\n",nb_atom_tot);
  for(i=0;i<nb_atom_tot;i++)
    fprintf(f,"%s  %lf %lf %lf\n",SYSTEM[i].atomTypeChar,SYSTEM[i].x,SYSTEM[i].y,SYSTEM[i].z);
  
  fclose(f);
}

void write_system_continue(atom *SYSTEM,int nb_atom_tot,char *file_name,int it)
{
  int  i;
  FILE *f;

  if(it == 0)
    f = fopen(file_name,"w");
  else
    f = fopen(file_name,"r+");
  
  if (!f) 
  {
      printf("can't create %s \n",file_name);
      exit(1);
  } 
  fseek(f, 0, SEEK_END);
  fprintf(f,"   %d\n\n",nb_atom_tot);
  for(i=0;i<nb_atom_tot;i++)
    fprintf(f,"%s  %lf %lf %lf\n",SYSTEM[i].atomTypeChar,SYSTEM[i].x,SYSTEM[i].y,SYSTEM[i].z);
  
  fclose(f);
}