#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#define TAILLE_TABLE 13
//Creation des structures Produit et liste des produits et table de hachage 
typedef struct Produit {
    int id;
    char nom[30];
    float prix;
    int quantite;
    struct Produit *suivant;
    struct Produit *precedent;
    struct Produit *nextHach;
} Produit;
typedef struct Liste{
    Produit *tete;
    Produit *queue;
    int nbr_Produit;
}Liste;
typedef struct {
    Produit *table[TAILLE_TABLE];
} TableHachage;

Liste *Li=NULL; 
TableHachage *table;
//__________________________________________________________________________________________________
//Creation des structures Client et l'arbre binaire et file
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

Arbre_Client *Ar = NULL;
File *file = NULL;
//________________________________________________________________
//Creation des structures Transaction et la pile
typedef struct Transaction {
    int idClient;
    int idProduit;
    int quantite;
    float total;
    char date[20];
    struct Transaction *suivant;
} Transaction;
typedef struct {
    Transaction *tete;
    int taille_Pile;
} Pile;
Pile *p=NULL;
//_______________________________________________________________
int hachage(int id) {
    return id % TAILLE_TABLE;
}
void initialiser_table_hachage() {
    table = (TableHachage*)malloc(sizeof(TableHachage));
    if (table == NULL) {
        perror("Erreur d'allocation de la table de hachage.\n");
        return;
    }
    for (int i = 0; i < TAILLE_TABLE; i++)
        table->table[i] = NULL;
    } 
    Produit *rechercher_par_ID(int id){
    int index=hachage(id);
    if (index < 0 || index >= TAILLE_TABLE) {
        perror( "Erreur interne: Index de hachage invalide");
        return NULL;
    }
    Produit *P=table->table[index];
    while(P!=NULL){
        if(P->id==id) return P;
        P=P->nextHach;
    }
    return NULL;
}
Produit* creer_produit(){
    Produit* P= (Produit*)malloc(sizeof(Produit));//Allocation dynamique
    if(P==NULL){
        perror("Erreur d'allocation !");
        return NULL;
    }
    printf("entre l id (l id doit etre positif) : ");
    do {
        scanf("%d", &P->id);
        if(P->id <= 0) printf("l id non valide;veulliez ressayer: ");
    } while(P->id <= 0 || rechercher_par_ID(P->id)!=NULL);
    printf("Nom : ");
    scanf("%s",P->nom);
    printf(" entre le prix : ");
    do {
        scanf("%f", &P->prix);
        if(P->prix < 0) printf("Prix ne peut pas etre negatif;veulliez ressayer: ");
    } while(P->prix < 0);
    printf("entre la Quantite : ");
    do {
        scanf("%d", &P->quantite);
        if(P->quantite < 0) printf("Quantite ne peut pas etre negative;veulliez ressayer: ");
    } while(P->quantite < 0);
    P->suivant=NULL;
    P->precedent=NULL;
    P->nextHach=NULL;
    return(P);
}
Liste * CreerListe_Produits(){
    Liste *nv_liste = (Liste*) malloc(sizeof(Liste));  // Allocation dynamique
    if(nv_liste==NULL){
        perror("Erreur allocation liste");
        return NULL;
    }
    nv_liste->tete = NULL;
    nv_liste->queue = NULL;
    nv_liste->nbr_Produit = 0;
    return nv_liste;
}
void insererHachage(Produit*P){
    if (P == NULL || table==NULL) {
        perror("Erreur d'allocation de la table de hachage.\n");
        return;
    }
    int index=hachage(P->id);
    P->nextHach=table->table[index];
    table->table[index]=P;
}
void ajouter_produit_liste(Produit* P){
    if(Li == NULL || P == NULL ){
        perror("Erreur allocation du produit ou de la liste\n ");
        return ;
    }
    insererHachage(P);
    if(Li ->tete == NULL){/*cas de la liste vide*/
        Li -> tete=P;
        Li ->queue=P;
    }
    else{//cas de la liste non vide
        P->precedent = Li->queue;
        Li->queue-> suivant=P;
        Li->queue=P;
        P->suivant=NULL;
    }
    Li->nbr_Produit++;
}
Produit* rechercher_par_nom(const char* nom){
    if(Li == NULL || Li->tete == NULL || nom == NULL) {
        perror("Erreur de recherche: Liste ou nom invalide.");
        return NULL;
    }
    Produit* p = Li->tete;
    while(p != NULL){
        if(strcmp(p->nom, nom) == 0) return p;
        p = p->suivant;
    }
    printf("Produit du nom %s introuvable.\n", nom);
    return NULL;
}
void supprimerHachage(int id){
    int index=hachage(id);
    if (index < 0 || index >= TAILLE_TABLE) {
        perror("Erreur interne: Index de hachage est invalide\n");
        return ;
    }
    Produit *courant=table->table[index];
    Produit *prec=NULL;
    while(courant!=NULL){
        if(courant->id==id) {
            if(prec==NULL){
                table->table[index]=courant->nextHach;
            }
            else{
                prec->nextHach=courant->nextHach;
            }
            courant->nextHach = NULL;
            printf("suppression dans la table de hachage a reussi\n");
            return;
        }
        prec=courant;
        courant=courant->nextHach;
    }
    printf("Produit ID:%d introuvable dans la table\n", id);
}
void supprimer_produit_liste(int id){
    if(!Li ) {
        perror("Liste vide rien a supprime\n");
        return;
    }
    if(Li->nbr_Produit==0){// Vérifier si la liste est vide
        printf("liste est vide\n");
        return;
    }
    Produit *p = rechercher_par_ID(id);
    if(p!=NULL){// si on a un seul elemnet dans la liste 
        supprimerHachage(p->id);
        if(Li->nbr_Produit==1){
            Li->tete=NULL;
            Li->queue=NULL;
        }
        else if (p==Li->queue){//si l'element se trouve a la fin 
            Li->queue=p->precedent;          
            Li->queue->suivant=NULL;
        }
        else if (Li->tete==p) {//si l'element se trouve au debut
            Li->tete=p->suivant;// Nouvelle tête = suivant ancienne tête
            Li->tete->precedent=NULL;
        }
        else {// Relier le précédent et le suivant pour contourner le nœud à supprimer
            p->precedent->suivant=p->suivant;// Suivant précédent = suivant
            p->suivant->precedent=p->precedent;// Précédent suivant = précédent
        }
        free(p);
        Li->nbr_Produit--;
    }
    else{printf("ce produit est introuvable.\n");}  
}
void trier_liste_produits(){
    if (Li == NULL || Li->tete == NULL || Li->tete->suivant == NULL) {
        perror("Liste vide ou avec un seul élément - pas de tri nécessaire\n");
        return;
    }
    Produit *compt1 , *compt2;
    float tempprix;
    char tempnom[30];
    int tempquant;
    int tempid;
    for(compt1=Li->tete ; compt1!=NULL ; compt1=compt1->suivant){
        for(compt2=compt1->suivant ; compt2!=NULL ; compt2=compt2->suivant){
            if(compt1->prix>compt2->prix){
                strcpy(tempnom,compt1->nom);
                strcpy(compt1->nom,compt2->nom);
                strcpy(compt2->nom,tempnom);

                tempprix=compt1->prix;
                compt1->prix=compt2->prix;
                compt2->prix=tempprix;

                tempquant=compt1->quantite;
                compt1->quantite=compt2->quantite;
                compt2->quantite=tempquant;

                tempid=compt1->id;
                compt1->id=compt2->id;
                compt2->id=tempid;
            }
        }
    }
}
void afficher_liste_produits(){
    if (Li == NULL || Li->tete == NULL) {
        printf("Liste vide ou nulle\n");
        return;
    }
    Produit* ptr;
    ptr= Li->tete;
    while(ptr != NULL){ //condition d’arrêt
        printf("\nID : %d\t|\tNom : %s\t|\tPrix : %f\t|\tQuantite : %d \n", ptr->id,ptr->nom,ptr->prix,ptr->quantite);
        ptr=ptr->suivant;
    }
}
int modifier_produit(int id , float prix , int quantite){
    Produit *p =rechercher_par_ID(id);
    if(p==NULL){
        perror("produit introvable");
        return 0;
    }
    p->prix=prix;
    p->quantite=quantite;
    return 1;
}
void enregistrer_liste_produits(){
    if(Li==NULL) {  
        perror("Liste non initialisee\n");
        return;
    }
    FILE *fp = fopen("produits.txt","w");
    if(fp==NULL){
        perror("error d'ouverture du fichier des produits\n");
        return;
    }else{
        if(Li->tete==NULL){
            printf("la liste est vide\n");
        }else {
            Produit *p1;
            p1=Li->tete;
            while(p1!=NULL){
                fprintf(fp,"%d|%s|%f|%d\n", p1->id,p1->nom,p1->prix,p1->quantite);
                p1=p1->suivant;
            }
        }
    }
    fclose(fp); 
    printf("la liste des produits a ete enregistree\n");
}
void lire_liste_produits() {
    FILE *fp = fopen("produits.txt", "r");
    if (fp == NULL || table==NULL  || Li==NULL) {
        perror("Erreur ouverture fichier pour lecture des produits\n");
        return;
    }
    Produit *p1;
    int lecture=4;
    while (lecture == 4) {//scan tout la ligne dans lignetemp
        p1=(Produit*)malloc(sizeof(Produit));
        if(!p1) {
            perror("Erreur d'allocation de memoire pour le produit\n");
            return; // Une seule allocation dans la boucle
        }
        lecture = fscanf(fp,"%d|%29[^|]|%f|%d\n", &p1->id,p1->nom,&p1->prix,&p1->quantite);
        p1->nextHach=NULL;
        p1->precedent=NULL;
        p1->suivant=NULL;
        if (lecture != 4)  {
            free(p1);  // Libérer si lecture échoue
            continue;
        }
        if(rechercher_par_ID(p1->id)!=NULL){
            printf("produit avec ID %d deja existe, saut de l'insertion.\n", p1->id);
            free(p1);
            continue;
        }
        insererHachage(p1);
        p1->suivant = NULL;
        p1->precedent = Li->queue;
        if (Li->tete == NULL) {
            Li->tete = p1;
        } else {
            Li->queue->suivant = p1;
        }
        Li->queue = p1;
        Li->nbr_Produit++;
    }
    fclose(fp);
}
// Fonction pour libérer la mémoire
void liberer_memoire() {
    printf("\n=== LIBERATION DE LA MEMOIRE ===\n");
    if (Li != NULL) {
        int compteur = 0;
        Produit *courant = Li->tete;
        Produit *suivant=NULL;
        while (courant != NULL) {
            suivant = courant->suivant;
            free(courant);
            courant = suivant;
            compteur++;
        }
        free(Li);
        Li = NULL;
        printf("%d produits liberes\n", compteur);
    }
    if (table != NULL) {
        free(table);
        table = NULL;
        printf("Table de hachage liberee\n");
    }
}
//________________________________________________________________
void menu_afficher() {
    int choix;
    do {
        printf("\n=== AFFICHAGE ===\n");
        printf("1. Afficher la LISTE des produits non trie\n");
        printf("2. Afficher la TABLE des produits trien\n");
        printf("3. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d",&choix);
        switch(choix) {
            case 1:
                printf("\n=== LISTE DES PRODUITS ===\n");
                if (Li == NULL || Li->nbr_Produit == 0) {
                    printf("Aucun produit dans la liste\n");
                } else {
                    printf("Nombre de produits : %d\n", Li->nbr_Produit);
                    afficher_liste_produits();
                }
                break;
            case 2:
                printf("liste des produites tries\n");
                trier_liste_produits();
                afficher_liste_produits();
                break;
            case 3:
                break;
            default:
                printf("Choix invalide! ");
                break;
        }
    } while(choix != 3);
}
void menu_rechercher() {
    int choix, id;
    char nom[30];
    Produit *resultat = NULL;
    do {
        printf("\n=== RECHERCHE D'UN PRODUIT ===\n");
        printf("1. Rechercher par ID\n");
        printf("2. Rechercher par NOM\n");
        printf("3. Retour au menu principal\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        switch(choix) {
            case 1:
                do {
                    printf("Entrez l'ID du produit : ");
                    scanf("%d", &id);
                    if (id <= 0) {
                        printf("ID doit etre positif! ");
                    }
                } while(id <= 0 );
                resultat = rechercher_par_ID(id);
                break;
            case 2:
                printf("Entrez le NOM du produit : ");
                scanf("%s", nom);
                resultat = rechercher_par_nom(nom);
                break;
            case 3:
                break;
            default:
                printf("Choix invalide! ");
        }
        if (choix == 1 || choix == 2) {
            if (resultat != NULL) {
                printf("\n=== DETAILS PRODUIT ===\n");
                printf("ID : %d | Nom : %s | Prix : %.2f | Quantite : %d \n", resultat->id,resultat->nom,resultat->prix,resultat->quantite);
            } else {
                printf("Produit non trouve\n");
            }
        }
    } while(choix != 3);
}
void menu_modifier() {
    int id, quantite;
    float prix;
    printf("\n=== MODIFICATION D'UN PRODUIT ===\n");
    printf("Entrez l'ID du produit a modifier : ");
    do {
        scanf("%d", &id);
        if (id <= 0) {
            printf("ID doit etre positif! veuillez ressayez: ");
        }
    } while(id <= 0);
    Produit *p = rechercher_par_ID(id);
    if (p == NULL) {
        printf("Produit avec ID %d introuvable\n", id);
        return;
    }
    printf("\nValeurs actuelles :\n");
    printf("ID : %d | Nom : %s | Prix : %.2f | Quantite : %d \n",p->id,p->nom,p->prix,p->quantite);
    printf("\nNouvelles valeurs :\n");
    printf("Nouveau prix (actuel: %.2f) : ", p->prix);
    do {
        scanf("%f", &prix);
        if (prix < 0) {
            printf("Prix ne peut pas etre negatif! veuillez ressayez: ");
        }
    } while(prix < 0);
    printf("Nouvelle quantite (actuel: %d) : ", p->quantite);
    do {
        scanf("%d", &quantite);
        if (quantite < 0) {
            printf("Quantite ne peut pas etre negative! veuillez ressayez: ");
        }
    } while(quantite < 0);
    if (modifier_produit(id, prix, quantite)) {
        printf("Produit modifie avec succes\n");
    } else {
        printf("Echec de la modification\n");
    }
}
void menu_produit(){
    int choix;
    do {
        printf("\n\n=== MENU PRINCIPAL - GESTION DE PRODUITS ===\n");
        printf("1. AJOUTER un produit\n");
        printf("2. AFFICHER un produit\n");
        printf("3. SUPPRIMER un produit\n");
        printf("4. RECHERCHER un produit\n");
        printf("5. MODIFIER un produit\n");
        printf("6. ENREGISTRER la liste dans un fichier\n");
        printf("7. CHARGER la liste depuis un fichier\n");
        printf("0. RETOUR au menu principale\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        switch(choix) {
            case 1:{
                Produit *nouveau = creer_produit();
                if (nouveau == NULL) {
                    printf("Echec de creation du produit\n");
                    break;
                }
                ajouter_produit_liste(nouveau);
                break;}
            case 2:
                menu_afficher();
                break;
            case 3:{
                int choix, id;
                if (Li == NULL || Li->nbr_Produit == 0) {
                    printf("La liste est vide, suppression impossible\n");
                    return ;
                }
                printf("\n=== SUPPRESSION D'UN PRODUIT ===\n");
                printf("id a supprimer  : ");
                do{
                    scanf("%d", &id);
                    if (id<=0) {
                        printf("id invalide! vieullez ressayer: ");
                    }
                }while(id<=0);
                supprimer_produit_liste(id);
                break;}
            case 4:
                menu_rechercher();
                break;
            case 5:
                menu_modifier();
                break;
            case 6:
                printf("\n=== ENREGISTREMENT DANS FICHIER ===\n");
                enregistrer_liste_produits();
                break;
            case 7:
                printf("\n=== CHARGEMENT DEPUIS FICHIER ===\n");
                lire_liste_produits();
                break;
            case 0:
                break;
            default:
                printf("\nChoix invalide ! Veuillez choisir entre 1 et 7: \n");
        }
    } while(choix != 0);
}                                                                                                              
//_____________________________________________________________________________________________
//gestion des clients
//fonction initialiser une file
File *initialiserFile(){
    File *nv_file = (File*)malloc(sizeof(File));
    if(!nv_file){
        printf("erreur d'allocation de la memoire pour la file.\n");
        return NULL;
    }
    nv_file->tete = NULL;
    nv_file->queue = NULL;
    nv_file->taille = 0;
    return nv_file;
}
// fct verifier si la file est vide
int estVideFile(){
    if(file->taille > 0)
        return file->taille;
    return 0;
}
//fct enfiler un client dans la file
void enfiler(Client *C){
    if( file == NULL || C == NULL){
        printf("erreur, la file est null ou client est null.\n");
        return;
    }
    ElementClient *ptr = (ElementClient*)malloc(sizeof(ElementClient));
    if( ptr == NULL ){
        printf("echec d'allocation\n");
        return;}
    ptr->El_Client = C;
    ptr->suivant = NULL;
    if(estVideFile(file) == 0){//la liste est vide
        file->tete = ptr;
        file->queue = ptr;
    }else{
        file->queue->suivant = ptr;
        file->queue = ptr;
    }
    printf("ajout du client au file avec succes.\n");
    file->taille++;
}
//fct affichage des client en file d'attente
void afficherFile(){
    if( file == NULL || estVideFile() == 0){
        printf("erreur, la file est null ou vide.\n");
        return;
    }
    ElementClient *ptr = (ElementClient*)malloc(sizeof(ElementClient));
    if( ptr == NULL ){
        printf("echec d'allocation\n");
        return;
    }
    ptr = file->tete ;
    do{
        printf("client %d : %s || %.2f.\n",ptr->El_Client->id,ptr->El_Client->nom,ptr->El_Client->totalDepense);
        ptr = ptr->suivant;
    } while (ptr != NULL);
}
//fct defiler un client de la file
Client *defiler(){
    if( file == NULL || estVideFile() == 0){
        printf("erreur, la file est null ou vide.\n");
        return NULL;
    }
    ElementClient *temp = file->tete;
    Client *Cl = temp->El_Client;
    file->tete = temp->suivant;
    file->taille--;
    if(file->tete == NULL){//traiter le cas si on defile la seule element dans la file
        file->queue = NULL;
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
//fonction creation d'un nouveau Client
Client *Creer_Client(){
    Client *Cl = (Client*)calloc(1,sizeof(Client));
    if(!Cl){
        printf("echec d'allocation de la memoire pour le Client.\n");
        return NULL;
    }
    printf("entrer l'identificateur du client: ");
    do{
        scanf("%d",&Cl->id);
        if(Cl->id <= 0){
            printf("l'id doit etre positive;vieullez ressayez: \n");
        }
    }while(Cl->id <= 0);
    printf("entrer le nom du client: ");
    scanf("%s",Cl->nom);
    Cl->totalDepense = 0;
    Cl->gauche = NULL;
    Cl->droite = NULL;
    return Cl;
}
//ajouter un client
Client *Ajouter_Client(Client *racine, Client *Cl){
    if(racine == NULL){
        return Cl;
    }
    else if(strcmp( Cl->nom, racine->nom) < 0){
        racine->gauche = Ajouter_Client(racine->gauche, Cl);
    }else if(strcmp( Cl->nom, racine->nom) > 0){
        racine->droite = Ajouter_Client(racine->droite, Cl);
    }
    return racine;
}
//fonction recherche a un client par son identificateur
Client *Rechercher_Id_Client(Client *racine, int Id){///// a expliquer ----------------------------------
    Client *resultat = NULL;
    if( racine == NULL ){
        return NULL;
    }
    if( Id == racine->id ){
        return racine;
    }
    resultat = Rechercher_Id_Client(racine->gauche, Id);
    if ( resultat != NULL) {
        return resultat; //Trouvé dans la branche gauche
    }
    return Rechercher_Id_Client(racine->droite, Id);
}
//fonction recherche a un client par son nom
Client *Rechercher_Nom_Client(Client *racine, char nom[30]){
    if( racine == NULL ){
        return NULL;
    }
    if(strcmp(nom,racine->nom) == 0 ){
        return racine;
    }else if(strcmp(nom,racine->nom) < 0){
        return Rechercher_Nom_Client(racine->gauche,nom);
    }else if(strcmp(nom,racine->nom) > 0){
        return Rechercher_Nom_Client(racine->droite,nom);
    }
}
// fonction qui calcule le min
Client *min(Client *racine){
    while(racine->gauche!=NULL){
        racine=racine->gauche;//parcouru jusqu'au trouver le plus petit element
    }
    return racine;
}
//fonction pour supprimer un client
Client *supprimer_client(Client *racine, char nom[30]){
    if(racine==NULL){
        return NULL;
    }//on recherche client a supprimer
    int val = strcmp( nom, racine->nom);
    if( val < 0){
        racine->gauche=supprimer_client(racine->gauche,nom);
    }else if ( val > 0 ){
        racine->droite=supprimer_client(racine->droite,nom);
        //return racine;
    }else{ //si l'arbre contient juste la racine
        if(racine->gauche==NULL && racine->droite==NULL){
            free(racine);//on supprime la racine
            return NULL;
        }//si on a un seul client fils
        if(racine->gauche==NULL){
            Client *tmp=racine->droite;
            free(racine);
            return tmp;//on retourne le racine
        }else if(racine->droite==NULL){
            Client *tmp=racine->gauche;
            free(racine);
            return tmp;
        }//si le le client a supprimer a deux enfant
        else{
            Client *succ=min(racine->droite);
            strcpy(racine->nom,succ->nom);//on copier la valeur de succ dans racine
            racine->totalDepense = succ->totalDepense;
            racine->id = succ->id;//on copier la valeur de succ dans la racine
            //supprime le succ dans le sous arbre droite
            racine->droite=supprimer_client(racine->droite,succ->nom);
        }
    }
    return racine;//on retourne la racine de nouveau arbre
}
//focntion affichage de l'arbre des clients
void Affichage_GRD(Client *racine){
    if(racine == NULL)
        return;
    Affichage_GRD(racine->gauche);
    printf("[%d]-> %s || %.2f \n",racine->id,racine->nom,racine->totalDepense);
    Affichage_GRD(racine->droite);
}
//fct recursif enregistre l'arbre dans un fichier texte
void Enregistre_Arbre_Client(FILE *pfile, Client *racine){
    if(racine == NULL)
        return;
    Enregistre_Arbre_Client(pfile, racine->gauche);
    if(fprintf(pfile,"%d|%s|%f\n",racine->id,racine->nom,racine->totalDepense) < 0 ){
        perror("erreur lors enregitrement dans le fichier.\n");
        fclose(pfile);
        return;
    }
    Enregistre_Arbre_Client(pfile, racine->droite);
}
//fonction d'enregistrement du l'arbre dans le fichier texte
void Enregistre_Client_fichier(){
    FILE *pfile;
    pfile = fopen("Clients.txt","a");
    if(!pfile){
        perror("echec d'ouverture du fichier");
        return;
    }
    if(Ar->racine == NULL){
        perror("erreur,l'arbre est vide.\n");
        return;
    }
    Enregistre_Arbre_Client(pfile,Ar->racine);
    printf("les clients Sont enregistraient dans le fichier clients\n");
    fclose(pfile);
}
//fct extraire l'arbre depuis un fichier texte
void Extraire_Arbre_Client(){
    FILE *pfile;
    pfile = fopen("Clients.txt","r");
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
        if(fscanf(pfile,"%d|%29[^|]|%f\n",&Cl->id,Cl->nom,&Cl->totalDepense) != 3){
            printf("erreur, lors du lecture du fichier\n");
            free(Cl);
            fclose(pfile);
            return ;
        }else if( Rechercher_Id_Client(Ar->racine,Cl->id) == NULL){
            Ar->racine = Ajouter_Client(Ar->racine,Cl);
            if(feof(pfile)){ return;}
        }
    }
    printf("les clients sont extraient dans le fichier clients\n");
    fclose(pfile);
}
// fonction libirer Arbre
void libirer_arbre(Client *racine) {
    if (racine == NULL) {
        return;
    }
    libirer_arbre(racine->gauche);// libiration du sous-arbre gauche
    libirer_arbre(racine->droite);// libiration du sous-arbre droite
    free(racine);//liberation du noueud actuel
}
void menu_client(){
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
                    if(Rechercher_Id_Client(Ar->racine, C->id) != NULL){
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
                    Client *ptr1 = Rechercher_Id_Client(Ar->racine, id_rech);
                    if( ptr1 != NULL){
                        printf("le client d'id %d existe dans l'arbre des clients:\n",id_rech);
                        printf("[%d]-> %s || %.2f \n",ptr1->id,ptr1->nom,ptr1->totalDepense);
                    }else{
                        printf("le client d'id %d n'existe pas dans l'arbre des clients.\n",id_rech);
                    }
                }
                else if(reponse == 'n' || reponse == 'N'){
                    char nom_rech[30];
                    printf("entrer le nom du client rechercher: ");
                    scanf("%s",nom_rech);
                    Client *ptr2 = Rechercher_Nom_Client(Ar->racine, nom_rech);
                    if( ptr2 != NULL){
                        printf("le client du nom %s existe dans l'arbre des clients\n",nom_rech);
                        printf("[%d]-> %s || %.2f \n",ptr2->id,ptr2->nom,ptr2->totalDepense);
                    }else{
                        printf("le client du nom %s n'existe pas dans l'arbre des clients.\n",nom_rech);
                    }
                }
                else{
                    printf("reponse invalide.\n");
                }
                break;
            }
            case 3:{
                char nom_supp[30];
                printf("entrer le nom du client a supprimer: ");
                scanf(" %s",nom_supp);
                Client *Cl_supp = Rechercher_Nom_Client(Ar->racine, nom_supp);
                if(Cl_supp == NULL){
                    printf("le client du nom %s n'existe pas dans la table.\n",nom_supp);
                    break;
                }
                printf("voici les informations du client supprimee:\n");
                printf("[%d]-> %s || %.2f \n",Cl_supp->id,Cl_supp->nom,Cl_supp->totalDepense);
                Ar->racine = supprimer_client(Ar->racine, nom_supp);
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
                Enregistre_Client_fichier();
                break;
            }
            case 6:{
                Extraire_Arbre_Client();
                break;
            }
            case 7:{
                int id_client;
                printf("entrer l'id du client: ");
                scanf("%d",&id_client);
                Client *Cl = Rechercher_Id_Client(Ar->racine, id_client);
                if (Cl != NULL) { //verification si le client existe dans l'arbre
                    enfiler(Cl);
                }else{//le cas d'un nouveau client
                    ElementClient *ptr = (ElementClient*)malloc(sizeof(ElementClient));
                    Client *Cl = (Client*)malloc(sizeof(Client));
                    if(ptr==NULL || Cl==NULL){
                        printf("erreur d'allocation\n");
                        break;
                    }
                    Cl->id = id_client;
                    printf("entrer le nom du clients: ");
                    scanf("%s",Cl->nom);
                    Cl->totalDepense = 0;
                    ptr->El_Client = Cl;
                    ptr->suivant = NULL;
                    if(file->taille == 0){
                        file->queue = ptr;
                        file->tete = ptr;
                    }else{
                        ptr->suivant = file->tete;
                        file->tete = ptr;
                    }
                    file->taille++;
                }
                printf("voici les client en file d'attente:\n");
                afficherFile();
                break;
                }
            case 0:
                break;
            default :
                printf("choix invalide veuillez ressayer.\n");
        }
    }while(choix != 0);
}
//_____________________________________________________________________________________________
//---------------------------------------------------
void obtenir_date_actuelle(char *date){
    time_t t=time(NULL);
    struct tm *tmp=localtime(&t);
    strftime(date, 20, "%Y-%m-%d %H:%M:%S", tmp);
}
Pile *initialiser_pile(){
    Pile *p = (Pile*) malloc(sizeof(Pile));
    if(p==NULL){
        printf("echec d'alocation:\n");
    }
    p->tete = NULL;
    p->taille_Pile = 0;
    return p;
}
int estVidepile() {
    return (p->tete == NULL);
}
Transaction *creer_transaction(){
    Transaction *T=(Transaction*)calloc(1,sizeof(Transaction));
    if(!T) exit(1);
    T->suivant=NULL;
    return T;
}
void empiler(Transaction *T){//empiler dans la pile
    T->suivant = p->tete;
    p->tete = T;//la tete de la pile contient recente historique
    p->taille_Pile++;
}
Transaction *depiler() {
    if (estVidepile()) return NULL;
    Transaction *T = p->tete;
    p->tete = T->suivant;
    p->taille_Pile--;
    return T; // on retourne directement
}
void afficher_ticket(int idC){
    char nomfich[50];
    sprintf(nomfich,"ticket_client_%d.text",idC);
    FILE *f = fopen(nomfich, "r");
    if(f==NULL) {
        perror("Echec d'ouverture");
        return;
    }
    int c;
    while( (c = fgetc(f) ) != EOF){
        putchar(c);
    }
    fclose(f);
    return;
}
char ajouter_panier(char rep){
    printf("voulez vous ajouter un autre produit (o/O) ou (n/N): ");
    do{
        scanf(" %c",&rep);
        if(!(rep=='n'|| rep=='N' || rep=='o'|| rep=='O'))
            printf("reponse invalide, vieullez ressayer :\n");
    }while(!(rep=='n'|| rep=='N' || rep=='o'|| rep=='O'));
    return rep;
}

void servir_client(){
    Client *Cl = defiler(file);
    if (Cl==NULL) {
        printf("aucun client en file d'attente: \n");
        return;
    }
    Client *C = Rechercher_Id_Client(Ar->racine, Cl->id);
    char nomfich[50];
    sprintf(nomfich,"ticket_client_%d.text",C->id);
    FILE *teckit=fopen(nomfich,"w");
    if(!teckit){
        printf("echec d'ouverture du fichier.\n");
        return ;
    }
    char DATE[20];
    obtenir_date_actuelle(DATE);
    fprintf(teckit,"\n====TICKIT DU CLIENT ID: %d ==== DATE : %s \n",C->id,DATE);
    fprintf(teckit,"ID_PRODUIT | Quantite | TOTAL \n");
    char rep = 'o';
    Produit *prd = NULL;
    Produit *prd_liste=NULL;
    while(rep=='o'|| rep=='O'){
        Transaction *T= creer_transaction();
        T->idClient=C->id;
        printf("ID du produit(ID>0) :");
        do{
            scanf("%d",&T->idProduit);
            prd= rechercher_par_ID(T->idProduit);
            if(T->idProduit<=0){
                printf("ID doit etre strictement positif! veuillez ressayer:\n");
            }
            else if(prd == NULL){
                printf("le produit d ID:%d est est introvable, veuillez ressayer: \n",T->idProduit);
            }
        } while (T->idProduit<=0 || prd == NULL);
        printf("QUANTITE :");
        do{
           scanf("%d",&T->quantite);
            if(T->quantite<=0){
                printf("QUANTITE doit etre strictement positive! veuillez ressayer: \n");
            }
        } while (T->quantite<=0);
        prd_liste =rechercher_par_nom(prd->nom);
        if(prd->quantite >= T->quantite){//premier test
            T->total = T->quantite * prd->prix;
            C->totalDepense+=T->total;
            prd->quantite-=T->quantite;
            prd_liste->quantite-=T->quantite;
            obtenir_date_actuelle(T->date);
            fprintf(teckit,"%d\t|\t%d\t|\t%.2f\n",T->idProduit,T->quantite,T->total);
            empiler(T);
            rep = ajouter_panier(rep);
        }else if(prd->quantite == 0){
            printf("le produit d ID:%d est terminer\n",prd->id);
            rep = ajouter_panier(rep);
        }else{
            printf("la quantite disponible est %d ,voulez vous la prendre ?(o/O) ou (n/N):",prd->quantite);
            scanf(" %c",&rep);
            if( rep == 'o'|| rep == 'O'){
                T->quantite=prd->quantite;
                prd_liste->quantite-=T->quantite;
                T->total = T->quantite * prd->prix;
                C->totalDepense+=T->total;
                prd->quantite=0;
                obtenir_date_actuelle(T->date);
                fprintf(teckit,"%d\t|\t%d\t|\t%.2f\n",T->idProduit,T->quantite,T->total);
                empiler(T);
                rep = ajouter_panier(rep);
            }else{rep = ajouter_panier(rep);}
        }
    }
    fprintf(teckit,"TOTAL CLIENT : %.2f\n",C->totalDepense);
    fclose(teckit);
    afficher_ticket(C->id);
}
void afficher_historique() {
    Transaction *T=(Transaction*)malloc(sizeof(Transaction));
    if(T==NULL){
        printf("echec d'allocation:\n");
        return;
    }
    T = p->tete;
    if(estVidepile()){
        printf("l'historique est vide.\n");
        return;
    } 
    while(T!=NULL){
        Client *Cl= Rechercher_Id_Client(Ar->racine, T->idClient);
        printf("Id_client: %d |Nom_client: %s |Totale: %.2f |Date: %s \n",T->idClient,Cl->nom,T->total,T->date);
        T=T->suivant;
    }
    return;
}
void annulation_transaction(){
    Transaction *annuler =depiler();
    if(annuler == NULL){
        printf("Aucune transaction a annuler.\n");
        return;
    }
    Produit *p=rechercher_par_ID(annuler->idProduit);
    Client *c=Rechercher_Id_Client(Ar->racine,annuler->idClient);
    p->quantite+=annuler->quantite;
    c->totalDepense-=annuler->total;
    printf("la transaction est annuler avec succes.\n");
    free(annuler);
}
//fct enregistre transaction dans le fichier
void enregistrer_historique(){
    if(p->taille_Pile == 0){
        printf("aucune transaction a enregistrer.\n");
        return;
    }
    FILE *f;
    f=fopen("historique.txt","w");
    Transaction *T= (Transaction*)malloc(sizeof(Transaction));
    if(f==NULL || T==NULL){
        perror("echec d'ouverture:\n");
        exit(1);
    }
    T =p->tete;
    while(T!=NULL){
        Client *Cl= Rechercher_Id_Client(Ar->racine, T->idClient);
        fprintf(f,"Id_client: %d |Nom_client: %s |Totale: %.2f |Date: %s \n",T->idClient,Cl->nom,T->total,T->date);
        if(ferror(f)){
            perror("erreur\n");
            break;
        }
        T=T->suivant;
    }
    fclose(f);
}
void menu_passage_en_caise(){
    int choix;
    do{
        printf("\n----passage en caisse----:\n");
        printf("1.servir le prochaine client:\n");
        printf("2.afficher historique:\n");
        printf("3.annulation de la derniere transaction:\n");
        printf("4.enregistrer l'historique:\n");
        printf("0.retour menu principal:\n");
        printf("choix:  ");
        scanf("%d",&choix);
        switch(choix){
            case 1: servir_client(); break;
            case 2: afficher_historique(); break;
            case 3: {
                int val;
                printf("voulez vous annuler la dernier transaction (si oui : 1 , si non :0)\n");
                scanf("%d",&val);
                if(val==1){
                    annulation_transaction();
                }
                break;
            }
            case 4: enregistrer_historique(); break;
            case 0: break;
            default: printf("votre choix est invalide, veuillez ressayer: \n");
        }
    }while(choix!=0);
}
int main(){
    Ar = Creer_Arbre();
    file=initialiserFile();
    Li = CreerListe_Produits();
    p=initialiser_pile();
    if(Li==NULL || Ar==NULL || file==NULL || p==NULL){
        printf("Echec d'allocation\n");
        return 1;
    }
    initialiser_table_hachage();
    int choix;
    do{
        printf("\n---- MENUE PRINCIPALE----- \n");
        printf("1.MENUE GESTION DES CLIENTS :\n");
        printf("2.MENUE GESTION DES PRODUITS :\n");
        printf("3.MENUE GESTION DE LA CAISSE :\n");
        printf("0.QUITTER LE PROGRAMME :\n");
        printf("votre choix: ");
        scanf("%d",&choix);
        switch(choix){
            case 1: menu_client(); break;
            case 2: menu_produit(); break;
            case 3: menu_passage_en_caise(); break;
            case 0: printf("\n=== FIN DU PROGRAMME ===\n"); break;
            default:printf("choix invalide veuillez ressayer.\n");
        }
    }while(choix != 0);
    liberer_memoire();
    libirer_arbre(Ar->racine);
    return 0;
}