#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "prototype.h"

void execution_total()
{   
    int a=0,b=0,c=0,d=0,i,sous_reseau=0;
    char chaine[100]="",chaine1[100]="";
    printf("Entrer IP et le nombre de sous-réseaux (CIDR  : adresse ip/nbre de sous reseau) : \n");
    printf("exemple : 192.168.10.50/21\n");
    scanf("%s", chaine);
    sscanf(chaine, "%[^/]/%d", chaine1, &sous_reseau);
    printf("\nIP : %s\n", chaine1);
    printf("Nombre de sous-réseaux : %d\n\n", sous_reseau);
    char *token=strtok(chaine1,".");
    if(token != NULL)
        a=atoi(token);
    token=strtok(NULL,".");
    if(token != NULL){
        b=atoi(token);
    }
     
    token=strtok(NULL,".");
    if(token != NULL){
        c=atoi(token);
    }
     
    token=strtok(NULL,".");
    if(token != NULL){
        d=atoi(token);
    }
    if(a<0 || a>255 || b<0 || b>255 || c<0 || c>255 || d<0 || d>255 || sous_reseau<0)
    {
        printf("==> invalide \n");
        exit(1);
    }
   
    else
    {
        sous_reseau_binaire(sous_reseau,chaine1);
        execution_boucle(chaine);
    }
}


void sous_reseau_binaire(int sous_reseau,char *chaine1) 
{   
    char chaine[200] = "";          
    int i; 
    for (i = 0; i < 32; i++) {   
        if (i < sous_reseau) {
            strcat(chaine, "1");
        } else {   
            strcat(chaine, "0");
        }
        if (i % 8 == 7) {
            strcat(chaine, "|");   
        }
    }
    separer(chaine,chaine1,sous_reseau);
}

void separer(char *chaine,char *chaine_ip,int sous_reseau) 
{  
    int a=0, b=0, c=0, d=0;
    char chaine_ip1[8]="",chaine_ip2[8]="",chaine_ip3[8]="",chaine_ip4[8]="";
    char chaine_msr1[8]="",chaine_msr2[8]="",chaine_msr3[8]="",chaine_msr4[8]="",reverse[40]="";
    int tab1[8], tab2[8], tab3[8], tab4[8];
    char grande_chaine[33]="";
    char tabl[33]="";
    int resultat1[8],resultat2[8],resultat3[8], resultat4[8];
    //variable pour stocker chaque nombre pour l'adresse reseau
    int *chaine1 =(int *)malloc(8 * sizeof(int));
    int *chaine2 =(int *)malloc(8 * sizeof(int));
    int *chaine3 =(int *)malloc(8 * sizeof(int));
    int *chaine4 =(int *)malloc(8 * sizeof(int));

    //variable pour stocker chaque nombre pour l'adresse de diffusion

    int i,s1=0,s2=0,s3=0,s4=0,j = 0;
    char *token = strtok(chaine_ip, ".");
    if (token != NULL) {
        a = atoi(token);
    }
    token = strtok(NULL, ".");
    if (token != NULL) {
        b = atoi(token);
    }
    token = strtok(NULL, ".");
    if (token != NULL) {
        c = atoi(token);
    }
    token = strtok(NULL, ".");
    if (token != NULL) {
        d = atoi(token);
    }
    decimal(a,chaine_ip1);
    decimal(b,chaine_ip2);
    decimal(c,chaine_ip3);
    decimal(d,chaine_ip4);
    token = strtok(chaine, "|");
    while (token != NULL) {   
        for (i = 0; i < 8; i++) 
        {
            if (j == 0)
                tab1[i] = token[i] - '0';
            else if (j == 1)
                tab2[i] = token[i] - '0';
            else if (j == 2)
                tab3[i] = token[i] - '0';
            else if (j == 3)
                tab4[i] = token[i] - '0';
        }
        token = strtok(NULL, "|");
        j++;
    }
    s1=binaire(tab1);
    s2=binaire(tab2);
    s3=binaire(tab3);
    s4=binaire(tab4);
    printf("\n\t==> msr : %d.%d.%d.%d\n",s1,s2,s3,s4);
    //eto reverse anilay nombre de sous reseau
    binaire_reverse(sous_reseau,reverse);
    for(i=0 ; i<8 ; i++)
    {   
        grande_chaine[i]=chaine_ip1[i]-'0';
        grande_chaine[i+8]=chaine_ip2[i]-'0';
        grande_chaine[i+16]=chaine_ip3[i]-'0';
        grande_chaine[i+24]=chaine_ip4[i]-'0';
       
       
    }
    for (i = 0; i < 32; i++) 
        {   
        if (i < 8)  // Premier octet (bits 0 à 7)
        {   
            if (grande_chaine[i] == 0 && tabl[i] == 0)
                resultat1[i] = 0;
            else
                resultat1[i] = 1;
        }
        else if (i >= 8 && i < 16)  // Deuxième octet (bits 8 à 15)
        {
            if (grande_chaine[i] == 0 && tabl[i] == 0)
                resultat2[i - 8] = 0;
            else
                resultat2[i - 8] = 1;
        }
        else if (i >= 16 && i < 24)  // Troisième octet (bits 16 à 23)
        {
            if (grande_chaine[i] == 0 && tabl[i] == 0)
                resultat3[i - 16] = 0;
            else
                resultat3[i - 16] = 1;
        }
        else if (i >= 24 && i < 32)  // Quatrième octet (bits 24 à 31)
        {
            if (grande_chaine[i] == 0 && tabl[i] == 0)
                resultat4[i - 24] = 0;
            else
                resultat4[i - 24] = 1;
        }
    }
    printf("\n\t@boardcast : %d.%d.%d.%d\n",binaire(resultat1),binaire(resultat2),binaire(resultat3),binaire(resultat4));
   
    decimal(s1,chaine_msr1);
    decimal(s2,chaine_msr2);
    decimal(s3,chaine_msr3);
    decimal(s4,chaine_msr4);

    //reseau
    chaine1=reseau(chaine_ip1,chaine_msr1);
    chaine2=reseau(chaine_ip2,chaine_msr2);
    chaine3=reseau(chaine_ip3,chaine_msr3);
    chaine4=reseau(chaine_ip4,chaine_msr4);
    printf("\n\t@reseau : %d.%d.%d.%d\n\n", binaire(chaine1), binaire(chaine2), binaire(chaine3), binaire(chaine4));
    free(chaine1);
    free(chaine2);
    free(chaine3);
    free(chaine4);
    
}

//binaire en decimal
int binaire(int *tab)
{
    int i,a=0,s=0,j=0;
    for(i=7 ; i>=0 ; i--)
    {
        a=pow(2,i);
        s=s+(tab[j]*a);
        j++;
    }
    return s;
}

//decimal en binaire
void decimal(int x,char *chaine)
{
    int i,a=0,b=0;
    for(i=7 ; i>=0 ; i--)
    {
        a=pow(2,i);
        if(x>=a)
        {   
            b=x-a;
            x=b;
            strcat(chaine,"1");
        }
        else 
        {
            strcat(chaine,"0");
        }
    }
}

int* reseau(char *chaine1, char *chaine2)
{
    int *tab = (int *)malloc(8 * sizeof(int));
    int i;
    for (i = 0; i < 8; i++) 
    {  
        tab[i] = (chaine1[i] - '0') * (chaine2[i] - '0'); 
    }
    return tab;
}

//boardcast
void binaire_reverse(int sous_reseau,char *chaine1) 
{   
    int i; 
    for (i = 0; i < 32; i++) 
    {   
        if (i < sous_reseau) {
            strcat(chaine1, "0");
        } else {   
            strcat(chaine1, "1");
        }
    }
}


void afficherAdresse(int *octets) {
    printf("%d.%d.%d.%d\n", octets[0], octets[1], octets[2], octets[3]);
}

void calculerSousReseaux(int *ip, int masque, int nombreSousReseaux) 
{
    int bitsSousReseaux = (int)log2(nombreSousReseaux); 
    int nouveauMasque = masque + bitsSousReseaux;
    int i,nombreMachines=0,hoteBits =0,tailleSousReseau =0 ,sousReseauActuel ;
    if (nouveauMasque > 32) 
    {
        printf("Le masque de sous-réseau est trop grand.\n");
        return;
    }

    hoteBits = 32 - nouveauMasque;
    tailleSousReseau = pow(2, hoteBits);
    sousReseauActuel = 0;
    printf("Nombre de machine : %d\n",tailleSousReseau-2);
    printf("\n\t==> Adresses réseau et broadcast pour chaque sous-réseau :\n");

    for (i = 0; i < nombreSousReseaux; i++) {
        int adresseReseau[4];
        int adresseBroadcast[4];

        // Calculer l'adresse réseau
        adresseReseau[0] = ip[0];
        adresseReseau[1] = ip[1];
        adresseReseau[2] = (sousReseauActuel / 256) % 256;
        adresseReseau[3] = sousReseauActuel % 256;

        // Calculer l'adresse broadcast
        int broadcast = sousReseauActuel + tailleSousReseau - 1;
        adresseBroadcast[0] = ip[0];
        adresseBroadcast[1] = ip[1];
        adresseBroadcast[2] = (broadcast / 256) % 256;
        adresseBroadcast[3] = broadcast % 256;

        printf("\nSous-réseau %d:\n", i + 1);
        printf("Adresse réseau : ");
        afficherAdresse(adresseReseau);

        printf("Adresse broadcast : ");
        afficherAdresse(adresseBroadcast);
        sousReseauActuel += tailleSousReseau;
    }
}

void execution_boucle(char *chaine) {
    int nombreSousReseaux = 0, decoupages = 0, masque = 0;
    int ip[4];
    sscanf(chaine, "%d.%d.%d.%d/%d", &ip[0], &ip[1], &ip[2], &ip[3], &masque);
    
    printf("Entrer le nombre de découpage désiré : ");
    scanf("%d", &decoupages);
    
    nombreSousReseaux = pow(2, decoupages);
    calculerSousReseaux(ip, masque, nombreSousReseaux);
}

