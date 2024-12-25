#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCS 1000
#define TAILLE_BLOC 512
#define MAX_ENREGISTREMENTS 100

typedef struct {
    int libre;  // 1 si le bloc est libre, 0 sinon
} Bloc;

typedef struct {
    char nom[50];
    int taille_en_blocs;
    int taille_en_enregistrements;
    int adresse_premier_bloc;
    char mode_organisation_globale[10];
    char mode_organisation_interne[10];
} Metadonnees;

typedef struct {
    int id;
    char champs[100];
} Enregistrement;

typedef struct {
    char nom[50];
    Metadonnees metadonnees;
    Enregistrement enregistrements[MAX_ENREGISTREMENTS];
    int nb_enregistrements;
} Fichier;
Bloc disque[MAX_BLOCS];

void initialiser_disque(int nombre_blocs, int taille_bloc) {
    for (int i = 0; i < nombre_blocs; i++) {
        disque[i].libre = 1; // Tous les blocs sont libres au départ
    }
}
void creer_fichier(Fichier *fichier, char *nom, int nb_enregistrements, char *mode_globale, char *mode_interne) {
    strcpy(fichier->nom, nom);
    fichier->metadonnees.taille_en_blocs = (nb_enregistrements * sizeof(Enregistrement) + TAILLE_BLOC - 1) / TAILLE_BLOC;
    fichier->metadonnees.taille_en_enregistrements = nb_enregistrements;
    fichier->metadonnees.adresse_premier_bloc = -1; // Adresse à définir lors du chargement
    strcpy(fichier->metadonnees.mode_organisation_globale, mode_globale);
    strcpy(fichier->metadonnees.mode_organisation_interne, mode_interne);
    fichier->nb_enregistrements = 0;
}

void charger_fichier(Fichier *fichier) {
    int blocs_necessaires = fichier->metadonnees.taille_en_blocs;
    for (int i = 0; i < MAX_BLOCS && blocs_necessaires > 0; i++) {
        if (disque[i].libre) {
            disque[i].libre = 0;
            if (fichier->metadonnees.adresse_premier_bloc == -1) {
                fichier->metadonnees.adresse_premier_bloc = i;
            }
            blocs_necessaires--;
        }
    }
}
void inserer_enregistrement(Fichier *fichier, int id, char *champs) {
    if (fichier->nb_enregistrements < MAX_ENREGISTREMENTS) {
        fichier->enregistrements[fichier->nb_enregistrements].id = id;
        strcpy(fichier->enregistrements[fichier->nb_enregistrements].champs, champs);
        fichier->nb_enregistrements++;
    } else {
        printf("Le fichier est plein.\n");
    }
}
int rechercher_enregistrement(Fichier *fichier, int id) {
    for (int i = 0; i < fichier->nb_enregistrements; i++) {
        if (fichier->enregistrements[i].id == id) {
            return i; // Retourne l'indice de l'enregistrement
        }
    }
    return -1; // Enregistrement non trouvé
}
void supprimer_enregistrement(Fichier *fichier, int id) {
    int index = rechercher_enregistrement(fichier, id);
    if (index != -1) {
        for (int i = index; i < fichier->nb_enregistrements - 1; i++) {
            fichier->enregistrements[i] = fichier->enregistrements[i + 1];
        }
        fichier->nb_enregistrements--;
    } else {
        printf("Enregistrement non trouvé.\n");
    }
}
void defragmenter_fichier(Fichier *fichier) {
    // Code pour réorganiser les enregistrements et les blocs pour récupérer l'espace inutilisé
}

void compactage() {
    // Code pour juxtaposer tous les fichiers sur le disque sans laisser d'espace libre
}
void afficher_menu() {
    printf("\n--- Simulateur de Système de Gestion de Fichiers ---\n");
    printf("1. Initialiser la mémoire secondaire\n");
    printf("2. Créer un fichier\n");
    printf("3. Afficher l'état de la mémoire secondaire\n");
    printf("4. Rechercher un enregistrement\n");
    printf("5. Insérer un nouvel enregistrement\n");
    printf("6. Supprimer un enregistrement\n");
    printf("7. Défragmenter un fichier\n");
    printf("8. Supprimer un fichier\n");
    printf("9. Renommer un fichier\n");
    printf("10. Compactage de la mémoire secondaire\n");
    printf("11. Vider la mémoire secondaire\n");
    printf("12. Quitter\n");
}
int main() {
    int choix, id;
    char nom[50], mode_globale[10], mode_interne[10], champs[100];
    Fichier fichier;

    while (1) {
        afficher_menu();
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                initialiser_disque(MAX_BLOCS, TAILLE_BLOC);
                printf("Mémoire secondaire initialisée.\n");
                break;
            case 2:
                printf("Nom du fichier : ");
                scanf("%s", nom);
                printf("Nombre d'enregistrements : ");
                int nb_enregistrements;
                scanf("%d", &nb_enregistrements);
                printf("Mode d'organisation globale (contigue/chainee) : ");
                scanf("%s", mode_globale);
                printf("Mode d'organisation interne (trie/nontrie) : ");
                scanf("%s", mode_interne);
                creer_fichier(&fichier, nom, nb_enregistrements, mode_globale, mode_interne);
                charger_fichier(&fichier);
                printf("Fichier créé et chargé en mémoire.\n");
                break;
            // Ajouter des cas pour les autres options du menu
            case 12:
                printf("Au revoir !\n");
                return 0;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }
    }
}

