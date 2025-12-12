#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Creation des structures Client et l'arbre binaire
typedef struct Client {
 int id;
 char nom[30];
 float totalDepense;
 struct Client *gauche;
 struct Client *droite;
}Client;
typedef struct ElementClient{
    Client *El_Client;
    struct ElementClient *suivant;
}ElementClient;
typedef struct{
    ElementClient *tete;
    ElementClient *queue;
    int taille;
}File;
typedef struct Arbre_Client{
    Client *racine;
}Arbre_Client;
//-------------------------------------
//gestion du file
//fonction initialiser une file
void initialiserFile(File *files){
    files->tete = NULL;
    files->queue = NULL;
    files->taille = 0;
}
// fct verifier si la file est vide
int estVideFile(File *files){
    if(files->taille > 0)
        return files->taille;
    return 0;
}
//fct enfiler un client dans la file 
void enfiler(File *files, Client *Cl){
    if( files == NULL || Cl == NULL){
        printf("erreur, la file est null ou client est null.\n");
        return;
    }
    ElementClient *ptr = (ElementClient*)malloc(sizeof(ElementClient));
    if( ptr == NULL )
        return;
    ptr->El_Client = Cl;
    ptr->suivant = NULL;
    if(estVideFile(files) == 0){//la liste est vide
        files->tete = ptr;
        files->queue = ptr;
    }
    else{
        files->queue->suivant = ptr;
        files->queue = ptr;
    }
    printf("ajout du client au file avec succes.\n");
    files->taille++;
}
//fct affichage des client en file d'attente 
void afficherFile(File *file){
    if( file == NULL || estVideFile(file) == 0){
        printf("erreur, la file est null ou vide.\n");
        return;
    }
    ElementClient *ptr = file->tete ;
    do{
        printf("client %d : %s || %d.\n",ptr->El_Client->id,ptr->El_Client->nom,ptr->El_Client->totalDepense);
        ptr = ptr->suivant;
    } while (ptr != NULL);
}
//fct defiler un client de la file 
Client *defiler(File *files){
    if( files == NULL || estVideFile(files) == 0){
        printf("erreur, la file est null ou vide.\n");
        return NULL;
    }
    ElementClient *temp = files->tete;
    Client *Cl = temp->El_Client;
    files->tete = temp->suivant;
    files->taille--;
    if(files->tete == NULL){//traiter le xas si on defile la seule element dans la file
        files->queue = NULL;
    }
    free(temp);
    return Cl;
}
//---------------------------------------------
//fonction creation d'un arbre VIDE
Arbre_Client *Creer_Arbre(){
    Arbre_Client *Ar = (Arbre_Client*)calloc(1,sizeof(Arbre_Client));
    if(!Ar){
        printf("echec d'allocation de la memoire pour l'arbre Client.\n");
        return NULL;
    }
    Ar->racine = NULL;
    return Ar;
}
//---------------------------------------------
//fonction creation d'un nouveau Client
Client *Creer_Client(){
    Client *Cl = (Client*)calloc(1,sizeof(Client));
    if(!Cl){
        printf("echec d'allocation de la memoire pour le Client.\n");
        return NULL;
    }
    printf("entrer l'identificateur du client: ");
    scanf("%d",&Cl->id);
    printf("entrer le nom du client: ");
    scanf("%s",Cl->nom);
    printf("entrer le totale depense du client: ");
    scanf("%f",&Cl->totalDepense);
    Cl->gauche = NULL;
    Cl->droite = NULL;
    return Cl;
}
//---------------------------------------------
//ajouter un client

Client *Ajouter_Client(Client *racine,Client *C){
    if(racine == NULL){
        return C;
    }
    else if(strcmp( C->nom, racine->nom) < 0){
        racine->gauche = Ajouter_Client(racine->gauche,C);
    }else if(strcmp( C->nom, racine->nom) > 0){
        racine->droite = Ajouter_Client(racine->droite,C);
    }
    return racine;
}
//---------------------------------------------
//fonction recherche a un client par son identificateur
int Rechercher_Id_Client(Client *racine, int Id){
    if( racine == NULL ){
        return 0;
    }
    if( Id == racine->id ){
        return 1;
    }else if( Id < racine->id){
        return Rechercher_Id_Client(racine->gauche ,Id);
    }else if(Id > racine->id){
        return Rechercher_Id_Client(racine->droite ,Id);
    }
}
//---------------------------------------------
//fonction recherche a un client par son nom
int Rechercher_Nom_Client(Client *racine, char nom[30]){
    if( racine == NULL ){
        return 0;
    }
    int val = strcmp( nom,racine->nom);
    if(val == 0 ){
        return 1;
    }else if(val < 0){
        return Rechercher_Nom_Client(racine->gauche ,nom);
    }else if(val > 0){
        return Rechercher_Nom_Client(racine->droite ,nom);
    }
}
//---------------------------------------------
// fonction qui calcule le min
Client *min(Client *racine){
    while(racine->gauche!=NULL){
        racine=racine->gauche;//parcouru jusqu'au trouver le plus petit element
    }
    return racine;
}
//fonction pour supprimer un client
Client *supprimer_employe(Client *racine,int id){
    if(racine==NULL){
        return NULL;
    }
    //on recherche client a supprimer
    if(racine->id > id){
        racine->gauche=supprimer_employe(racine->gauche,id);
    }else if (id > racine->id){
        racine->droite=supprimer_employe(racine->droite,id);
        //return racine;
    }
    else{
    //si l'arbre contient juste la racine
    if(racine->gauche==NULL && racine->droite==NULL){
        free(racine);//on supprime la racine
        return NULL;
    }
    //si on a un seul client fils
    if(racine->gauche==NULL){
        Client *tmp=racine->droite;
        free(racine);
        return tmp;//on retourne le racine
    }
    else if(racine->droite==NULL){
        Client *tmp=racine->gauche;
        free(racine);
        return tmp;
    }
    //si le le client a supprimer a deux enfant
    else{
        Client *succ=min(racine->droite);
        strcpy(racine->nom,succ->nom);//on copier la valeur de succ dans racine
        racine->totalDepense = succ->totalDepense;
        racine->id = succ->id;//on copier la valeur de succ dans la racine
        //supprime le succ dans le sous arbre droite
        racine->droite=supprimer_employe(racine->droite,succ->id);
    }
    }
    return racine;//on retourne la racine de nouveau arbre
}
//---------------------------------------------
//focntion affichage de l'arbre des clients
void Affichage_GRD(Client *racine){
    if(racine == NULL)
        return;
    Affichage_GRD(racine->gauche);
    printf("[%d]-> %s || %.2f \n",racine->id,racine->nom,racine->totalDepense);
    Affichage_GRD(racine->droite);
}
//---------------------------------------------
//fct recursif enregistre l'arbre dans un fichier texte
void Enregistre_Arbre_Client(FILE *pfile, Client *racine){
    if(racine == NULL)
        return;
    Enregistre_Arbre_Client(pfile, racine->gauche);
    Enregistre_Arbre_Client(pfile, racine->droite);
    if(fprintf(pfile,"%d %s %.2f \n",racine->id,racine->nom,racine->totalDepense) < 0 ){
        perror("erreur lors enregitrement dans le fichier.\n");
        fclose(pfile);
        return;
    }
}
//---------------------------------------------
//fonction d'enregistrement du l'arbre dans le fichier texte
void Enregistre_Client_file(Client *racine){
    FILE *pfile;
    pfile = fopen("C:\\Users\\hp\\Desktop\\Language C\\projet\\project\\Arbre_Clients.txt","w");
    if(!pfile){
        perror("echec d'ouverture du fichier");
        return;
    }
    if(racine == NULL){
        perror("erreur,l'arbre est vide.\n");
        return;
    }
    Enregistre_Arbre_Client(pfile,racine);
    fclose(pfile);
}
//---------------------------------------------
//fct extraire l'arbre depuis un fichier texte
void Extraire_Arbre_Client(Arbre_Client *Ar){
    FILE *pfile;
    pfile = fopen("C:\\Users\\hp\\Desktop\\Language C\\projet\\project\\Arbre_Clients.txt","r");
    if(!pfile){
        printf("echec d'ouverture du fichier");
        return ;
    }
    while(1){
        Client *Cl = (Client*)calloc(1,sizeof(Client));
        if(!Cl){
            printf("echec d'allocation de la memoire pour le Client.\n");
            return ;
        }
        if(fscanf(pfile,"%d %s %f \n",&Cl->id,Cl->nom,&Cl->totalDepense) != 3){
            free(Cl);
            fclose(pfile);
            return ;
        }else if( Rechercher_Id_Client(Ar->racine,Cl->id) != 1){
            Ar->racine = Ajouter_Client(Ar->racine,Cl);
        }
    }
    fclose(pfile);
}
//---------------------------------------------
void menu_client(){
    Arbre_Client *Ar = Creer_Arbre();
    File *file = (File*)malloc(sizeof(File));
    if(file == NULL){
        printf("echec d'allocation de la memoire pour la file.\n");
        return;
    }
    initialiserFile(file);
    int choix;
    do{
        printf("\n---- le menu ----- \n");
        printf("1.ajouter un client:\n");
        printf("2.rechercher un client :\n");
        printf("3.supprimer un client:\n");
        printf("4.afficher la liste des clients:\n");
        printf("5.enregistrer dans un fichier:\n");
        printf("6.extraire depuis le fichier:\n");
        printf("7.ajouter un client en file d'attente:\n");
        printf("0.retour au menu principale.\n");
        printf("votre choix:  ");
        scanf("%d",&choix);
        switch(choix){
            case 1:{
                Client *C = Creer_Client();
                if( C != NULL){
                    if(Rechercher_Id_Client(Ar->racine,C->id) == 1){
                        printf("un Client d'id %d existe deja dans l'arbre.\n",C->id);
                        break;
                    }
                    Ar->racine = Ajouter_Client(Ar->racine,C);
                    printf("ajout de client %d avec succes.\n",C->id);
                }else{
                    printf("erreur, lors de creation du client.\n");
                }
                break;
            }
            case 2:{
                if( Ar->racine == NULL ){
                    printf("echec, l'arbre est vide.\n");
                    break;
                }
                char reponse;
                printf("recherche par id ou nom: (i/I) ou (n/N): ");
                scanf(" %c",&reponse);
                if( reponse == 'i' || reponse == 'I'){
                    int id_rech;
                    printf("entrer l'id du client rechercher: ");
                    scanf("%d",&id_rech);
                    int tst = Rechercher_Id_Client(Ar->racine,id_rech);
                    if( tst == 1 )
                        printf("le client d'id %d existe dans l'arbre des clients.\n",id_rech);
                    else
                        printf("le client d'id %d n'existe pas dans l'arbre des clients.\n",id_rech);
                }
                else if(reponse == 'n' || reponse == 'N'){
                    char nom_rech[30];
                    printf("entrer le nom du client rechercher: ");
                    scanf("%s",nom_rech);
                    int tst = Rechercher_Nom_Client(Ar->racine,nom_rech);
                    if( tst == 1 )
                        printf("le client du nom %s existe dans l'arbre des clients.\n",nom_rech);
                    else
                        printf("le client du nom %s n'existe pas dans l'arbre des clients.\n",nom_rech);
                }
                else{
                    printf("reponse invalide.\n");
                }
                break;
            }
            case 3:{
                int id_supp;
                printf("entrer l'id du client a supprimer: ");
                scanf("%d",&id_supp);
                Ar->racine = supprimer_employe(Ar->racine,id_supp);
                break;
            }
            case 4:{
                if(Ar->racine == NULL ){
                    printf("echec, l'arbre est vide ou nulle.\n");
                    break;
                }
                printf("voici les clients en ordre alphabetique : \n");
                Affichage_GRD(Ar->racine);
                break;
            }
            case 5:{
                Enregistre_Client_file(Ar->racine);
                printf("les clients Sont enregistraient dans le fichier clients.txt\n");
                break;
            }
            case 6:{
                Extraire_Arbre_Client(Ar);
                printf("les clients sont extraient dans le fichier clients.txt\n");
                break;
            }
            case 7:{
                Client *Cl = Creer_Client();
                if (Cl != NULL) { // Verification si la creation a reussi
                    enfiler(file, Cl);
                } else {
                    printf("echec lors Ajout du client au file d'attente.\n");
                }
                printf("voici les client en file d'attente:\n");
                afficherFile(file);
            break;
            }
            case 0:{
                break;
            }
            default :{
                printf("choix invalide veuillez ressayer.\n");
            }
        }
    }while(choix != 0);
}
int main(){
    menu_client();
    return 0;
}
