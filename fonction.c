#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "prototype.h"

void enter()
{   
    int bits_reseau=0,a,b,c,d;
    char chaine[200]="",ip[100];
    printf("entrer cdir (ip/nbre de bits reseau) : ");
    scanf("%s",chaine);
    printf("data : %s\n",chaine);
    sscanf(chaine, "%[^/]/%d",ip, &bits_reseau);
    separe(ip,&bits_reseau);
    
}

void separe(char *ip,int* bits_reseau)
{   
    int i;
    int a=0,b=0,c=0,d=0;
    char ip1[8]="",ip2[8]="",ip3[8]="",ip4[8]="";
    char *token=strtok(ip,".");
    if(token != NULL)
    {
        a=atoi(token);
    }
    token = strtok(NULL,".");
    if(token != NULL)
    {
        b=atoi(token);
    }
    token = strtok(NULL,".");
    if(token != NULL)
    {
        c=atoi(token);
    }
    token = strtok(NULL,".");
    if(token != NULL)
    {
        d=atoi(token);
    }
    printf("a=%d b=%d c=%d d=%d\n",a,b,c,d);
    decimal(a,ip1);
    decimal(b,ip2);
    decimal(c,ip3);
    decimal(d,ip4);
    for(i=0 ; i<8 ; i++)
    {
       printf("ip[%d] = %d\n",i,ip1[i] - '0');
    }
    msr(bits_reseau,ip1,ip2,ip3,ip4);
}

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

void msr(int *bits_reseau,char *ip1,char *ip2,char *ip3,char *ip4)
{   
    int i ,a,b,c,d,decoup=0;
    int bits_host = 32 -(*bits_reseau); 
    char take[100];
    char msr1[8]="",msr2[8]="",msr3[8]="",msr4[8]="";
    char reseaux1[8]="",reseaux2[8]="",reseaux3[8]="",reseaux4[8]="";
    int msr11[8],msr22[8],msr33[8],msr44[8];
    char adr_reseau[21] = "";
    char adr_broadcast[21] = "";
    for(i=0 ; i<32 ; i++)
    {
        if(i< *bits_reseau)
            strcat(take,"1");   
        else
            strcat(take,"0");
        
    }

    for(i=0 ; i<8 ; i++)
    {
        msr11[i] = take[i]-'0';
        msr22[i] = take[i+8]-'0';
        msr33[i] = take[i+16]-'0';
        msr44[i] = take[i+24]-'0';
        
    }
    a=binaire(msr11);
    b=binaire(msr22);
    c=binaire(msr33);
    d=binaire(msr44);
    printf("msr : %d.%d.%d.%d\n",a,b,c,d);
    decimal(a,msr1);
    decimal(b,msr2);
    decimal(c,msr3);
    decimal(d,msr4);

    reseau_exec(bits_reseau,ip1,ip2,ip3,ip4,msr1,msr2,msr3,msr4,adr_reseau,adr_broadcast);

    boucle(bits_reseau,&decoup);
    display(adr_reseau,adr_broadcast,&bits_host,decoup);
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


void reseau_exec(int *bits_reseau,char *ip1,char *ip2,char *ip3,char *ip4,char *msr1,char *msr2,char *msr3,char *msr4,char *adr_reseau,char *adr_broadcast)
{   
    int i;
    int *reseau1 =(int *)malloc(8 * sizeof(int));
    int *reseau2 =(int *)malloc(8 * sizeof(int));
    int *reseau3 =(int *)malloc(8 * sizeof(int));
    int *reseau4 =(int *)malloc(8 * sizeof(int));

    reseau1=reseau(ip1,msr1);
    reseau2=reseau(ip2,msr2);
    reseau3=reseau(ip3,msr3);
    reseau4=reseau(ip4,msr4);

    printf("\n\t@reseau : %d.%d.%d.%d\n",binaire(reseau1),binaire(reseau2),binaire(reseau3),binaire(reseau4));
    broadcast(bits_reseau,reseau1,reseau2,reseau3,reseau4,adr_reseau,adr_broadcast);
    sprintf(adr_reseau, "%d.%d.%d.%d",binaire(reseau1),binaire(reseau2),binaire(reseau3),binaire(reseau4));
    free(reseau1);
    free(reseau2);
    free(reseau3);
    free(reseau4);
}

void binaire_reverse(int *sous_reseau,int *chaine1) 
{   
    int i; 
    for (i = 0; i < 32; i++) 
    {   
        if (i < *sous_reseau) 
        {   
            chaine1[i]=0;
        } 
        else 
        {   
            chaine1[i]=1;
        }
    }
}

void broadcast(int *bits_reseau,int *reseau1,int *reseau2,int *reseau3,int *reseau4,char *adr_reseau,char *adr_broadcast)
{   
    int i;
    int broad[100];
    int grande_chaine[32],reverse[32];
    int resultat1[8],resultat2[8],resultat3[8], resultat4[8];
    binaire_reverse(bits_reseau,reverse);
    
    for(i=0 ; i<8 ; i++)
    {
        grande_chaine[i] = reseau1[i];
        grande_chaine[i+8] = reseau2[i];
        grande_chaine[i+16] = reseau3[i];
        grande_chaine[i+24] = reseau4[i];   
    }

    for (i = 0; i < 32; i++) 
    {   
        if (i < 8)  // Premier octet (bits 0 à 7)
        {   
            if (grande_chaine[i] == 0 && reverse[i] == 0)
                resultat1[i] = 0;
            else
                resultat1[i] = 1;
        }
        else if (i >= 8 && i < 16)  // Deuxième octet (bits 8 à 15)
        {
            if (grande_chaine[i] == 0 && reverse[i] == 0)
                resultat2[i - 8] = 0;
            else
                resultat2[i - 8] = 1;
        }
        else if (i >= 16 && i < 24)  // Troisième octet (bits 16 à 23)
        {
            if (grande_chaine[i] == 0 && reverse[i] == 0)
                resultat3[i - 16] = 0;
            else
                resultat3[i - 16] = 1;
        }
        else if (i >= 24 && i < 32)  // Quatrième octet (bits 24 à 31)
        {
            if (grande_chaine[i] == 0 && reverse[i] == 0)
                resultat4[i - 24] = 0;
            else
                resultat4[i - 24] = 1;
        }
    }
    printf("\n\t@boardcast : %d.%d.%d.%d\n\n",binaire(resultat1),binaire(resultat2),binaire(resultat3),binaire(resultat4));
    sprintf(adr_broadcast, "%d.%d.%d.%d",binaire(resultat1),binaire(resultat2),binaire(resultat3),binaire(resultat4));

}

void boucle(int *bits_reseau,int *decoup)
{   
    printf("entrer le nombre  de decouppage : ");
    scanf("%d",decoup);
    int bits_host = 32 - (*bits_reseau + *decoup);
}


int nbre_machine(int bits_host)        
{
    int nbr = pow(2, bits_host) - 2;
    return nbr;
}


void display(char *adr_reseau, char *adr_broadcast,int *bits_host, int decoup)
{   
    int i , j=1;
    int *adresse_reseau;
    adresse_reseau = malloc(4*sizeof(int));

    int *adresse_broadcast;
    adresse_broadcast = malloc(4*sizeof(int));

    sscanf(adr_reseau, "%d.%d.%d.%d", &adresse_reseau[0], &adresse_reseau[1], &adresse_reseau[2], &adresse_reseau[3]);
    sscanf(adr_broadcast, "%d.%d.%d.%d", &adresse_broadcast[0], &adresse_broadcast[1], &adresse_broadcast[2], &adresse_broadcast[3]);

    int number_of_machine = nbre_machine(*bits_host-decoup);

    for (i=0; i<pow(2, decoup); i++)
    {   
        printf("\n\n==> sous reseau %d :\n",j);
        printf("@Réseau : %d.%d.%d.%d \n", adresse_reseau[0], adresse_reseau[1], adresse_reseau[2], adresse_reseau[3]);
        adresse_broadcast = get(adresse_reseau, number_of_machine+1); 
        printf("@Broadcast : %d.%d.%d.%d \n Nombre machine : %d\n\n", adresse_broadcast[0], adresse_broadcast[1], adresse_broadcast[2], adresse_broadcast[3], number_of_machine);
        adresse_reseau = get(adresse_broadcast, 1);
        j++;
    }

}


int * get(int *adresse_reseau, int number_of_machine)
{
    int *nouveau;
    nouveau = malloc(4*sizeof(int));
    nouveau[0] = adresse_reseau[0];
    nouveau[1] = adresse_reseau[1];
    nouveau[2] = adresse_reseau[2];
    nouveau[3] = adresse_reseau[3];

    while (number_of_machine > 0)
    {
        nouveau[3] += (number_of_machine > 255) ? 256 : number_of_machine;
        number_of_machine -= nouveau[3];
        while (nouveau[3] > 255)
        {
            nouveau[3] = 0;
            nouveau[2]++;
        }
        while (nouveau[2] > 255)
        {
            nouveau[2] = 0;
            nouveau[1]++;
        }
        while (nouveau[1] > 255)
        {
            nouveau[1] = 0;
            nouveau[0]++;
        }
    }

    return nouveau;
}
