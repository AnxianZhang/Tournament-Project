/****************************************
 Nom---------: Sprint1.c
 Role--------: Aplication qui organise 
               les tournois
 Auteur------: Anxian Zhang, Wang Florent
 Version-----: V3 du 06/11/2021
****************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#pragma warning(disable:4996)

// definition des constantes imposer: 
#define max_tournois 10                  // Nombre max de tounois
#define nb_matchs 127                    // Nombre de matchs par tournois
#define nb_joueuse 128                   // Nombre de joueuses par tournoi
#define lgmot 30                         // chaine de caractere max
    
// définitions des finales en fonction du nombre de matchs
#define finale_64 64
#define finale_32 96
#define finale_16 112
#define finale_8  120
#define finale_4  124
#define finale_2  126

// -----------------------------------------------definition des types-----------------------------------

typedef struct Joueuse {
    char nom_joueuse[lgmot + 1];
    unsigned int points;
}Joueuse;

typedef struct Match {
    unsigned int idx_gagnante;
    unsigned int idx_perdante;
}Match;

typedef struct Tournoi {
    char date_tournoi[5];
    char nom_tournoi[lgmot + 1];
    Match data_matchs[nb_matchs];
}Tournoi;

typedef struct TournoiWTA {
    unsigned int nb_tournois;
    unsigned int num_tournois;
    Tournoi dataTournois[max_tournois];
    Joueuse dataJoueuse[max_tournois * nb_joueuse];
}TournoiWTA;

/*------------------------------------------declaration des fonction-----------------------------------*/

/*
* recupere le numero du tournois actuelle, et verrifis si le tournois existe
* int_Tournoi_WTA [in-out] type TournoiWTA
* nom [in] type char
* anne [in] type annee
* t [return] le numéros du tournois actuele
* -1 [return] si un tournois est pas présent
*/
unsigned int recup_num_tournois(TournoiWTA *int_Tournoi_WTA, char nom[], char annee[])
{
    for(int t = 0; t< int_Tournoi_WTA->nb_tournois; t++)
    {
        if(strcmp(int_Tournoi_WTA->dataTournois[t].nom_tournoi, nom) == 0 && 
           strcmp(int_Tournoi_WTA->dataTournois[t].date_tournoi , annee) == 0)
        {
            return t;
        }
    }
    return -1;
}

/*
* fournit l'index du joueur en fonction de son nom 
* joueuse [in] type char
* int_Tournoi_WTA [in-out] type TournoiWtA
* j [return] type int, l'index de la joueuse
*/
unsigned int recup_idx_joueuse(TournoiWTA *int_Tournoi_WTA, char joueuse[])
{
    for(int j = 0; j<nb_joueuse*max_tournois; j++)
    {
        if(strcmp(int_Tournoi_WTA->dataJoueuse[j].nom_joueuse, joueuse) == 0)
        {
            return j;
        }
    }
}

/*
* Recupere les 128 joueuses de chaque tournois, et attribut les index respecive en fonction de la joueuse d'un mtach
* num_tournois [in] type unseigned int
* startin_num_joueuse [in] type int
* startin_match [in] type int
* int_Tournoi_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void recup_joueuse(TournoiWTA *int_Tournoi_WTA, int startin_match, int startin_num_joueuse, unsigned int num_tournois)
{
    char winner[lgmot+1], loser[lgmot+1];
    
    if(startin_match == 0) // 0 équiveaux au premier match d'un tournois
    {
       for (int i = 0, j = startin_match, k=startin_num_joueuse; i < nb_joueuse; i += 2, j++, k+=2) // enregistrement des 128 joueuses
        {
            scanf("%s %s", winner, loser);
            strcpy(int_Tournoi_WTA->dataJoueuse[k].nom_joueuse, winner); 
            strcpy(int_Tournoi_WTA->dataJoueuse[k+1].nom_joueuse, loser);
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_gagnante = k; // mise en place des gagnates dans l'idx
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_perdante = k + 1; //mise en place des perdantes dans l'idx
        } 
    }
    else if(startin_match != 0)
    {
        for (int i = 0, j = startin_match; i <nb_joueuse-startin_match; i += 2, j++) 
        {
            scanf("%s %s", winner, loser);
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_gagnante = recup_idx_joueuse(int_Tournoi_WTA, winner);
            int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[j].idx_perdante = recup_idx_joueuse(int_Tournoi_WTA, loser);
        } 
    }
}

/*
* Enregistre le nombre de tournois a creer
* int_Tournoi_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void definir_nombre_tournois(TournoiWTA* int_Tournoi_WTA) 
{
    scanf("%d", &(int_Tournoi_WTA->nb_tournois));
    assert(int_Tournoi_WTA->nb_tournois > 0 && int_Tournoi_WTA->nb_tournois <= max_tournois);
}

/*
* Enregistre le(s) tournoi(s)
* int_Tournois_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void enregistrement_tournoi(TournoiWTA *int_Tournoi_WTA)
{
    unsigned int num_tournois = int_Tournoi_WTA->num_tournois;
    unsigned int start_rang_joueuse = num_tournois*nb_joueuse; // le rang ou on commance l'enregistrement d'une joueuse

    // enregistrement du nom et de la date
    char nom[lgmot + 1], annee[5];
    scanf("%s %s", nom, annee);
    strcpy(int_Tournoi_WTA->dataTournois[num_tournois].nom_tournoi, nom);
    strcpy(int_Tournoi_WTA->dataTournois[num_tournois].date_tournoi, annee);

    recup_joueuse(int_Tournoi_WTA, 0, start_rang_joueuse,num_tournois);
    recup_joueuse(int_Tournoi_WTA, finale_64, start_rang_joueuse, num_tournois);
    recup_joueuse(int_Tournoi_WTA, finale_32, start_rang_joueuse, num_tournois);
    recup_joueuse(int_Tournoi_WTA, finale_16, start_rang_joueuse, num_tournois);
    recup_joueuse(int_Tournoi_WTA, finale_8, start_rang_joueuse, num_tournois);
    recup_joueuse(int_Tournoi_WTA, finale_4, start_rang_joueuse, num_tournois);
    recup_joueuse(int_Tournoi_WTA, finale_2, start_rang_joueuse, num_tournois);

    int_Tournoi_WTA->num_tournois++;
}

/*
* affiche le ieme de final selon le numeros de match
* match [in] type int
* aucune valeur de retour
*/
void affiche_ieme_final(int match)
{
    if (match == 0)
        {
            printf("64emes de finale\n");
        }
        else if(match == finale_64)
        {
            printf("32emes de finale\n");
        }
        else if(match == finale_32)
        {
            printf("16emes de finale\n");
        }
        else if(match == finale_16)
        {
            printf("8emes de finale\n");
        }
        else if(match == finale_8)
        {
            printf("quarts de finale\n");
        }
        else if(match == finale_4)
        {
            printf("demi-finales\n");
        }
        else if(match == finale_2)
        {
            printf("finale\n");
        }
}

/*
* affiche les matchs d'un tournoi
* int_Tournoi_WTA [in-out] type TournoiWTA
* pas de valeur de retour
*/
void affichage_matchs_tournoi(TournoiWTA *int_Tournoi_WTA)
{   
    char nom[lgmot+1], annee[5];
    scanf("%s %s", nom, annee);

    unsigned int num_tournois = recup_num_tournois(int_Tournoi_WTA, nom, annee);
    (num_tournois == -1) ? printf("Tournoi inconnue\n"): printf("%s %s\n", nom, annee);

    for(int m = 0; m<nb_matchs; m++)
    {
        affiche_ieme_final(m);

        Match num_match = int_Tournoi_WTA->dataTournois[num_tournois].data_matchs[m];
        printf("%s %s\n", int_Tournoi_WTA->dataJoueuse[num_match.idx_gagnante].nom_joueuse, 
               int_Tournoi_WTA->dataJoueuse[num_match.idx_perdante].nom_joueuse);
    }
}

/*-------------------------------------------main---------------------------------------------*/

/*
* Il les commande du fichier et execute les commandes
* le but est d'afficher les tournois et leurs match
*/
int main(void)
{   
    char mot[lgmot + 1];
    
    //initialisation du tournois
    TournoiWTA int_Tournoi_WTA;
    int_Tournoi_WTA.nb_tournois = 0;
    int_Tournoi_WTA.num_tournois = 0;

    while (1)
    {
        scanf("%s", mot);

        if (strcmp(mot, "definir_nombre_tournois") == 0)
        {
            definir_nombre_tournois(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "enregistrement_tournoi") == 0)
        {
            enregistrement_tournoi(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "affichage_matchs_tournoi") == 0)
        {
            affichage_matchs_tournoi(&int_Tournoi_WTA);
        }
        else if (strcmp(mot, "exit") == 0)
        {
            exit(0);
        }
    }

    system("pause");
    return 0;
}