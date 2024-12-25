#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 1000
#define BLOCK_SIZE 64
#define MAX_FILES 100
#define MAX_FILENAME 20
#define MAX_RECORDS_PER_BLOCK (BLOCK_SIZE / sizeof(Record))

typedef struct {
    int id;
    char data[32];
} Record;

typedef struct {
    char name[MAX_FILENAME];
    int size_in_blocks;
    int size_in_records;
    int first_block;
    char org_global[10]; // "contigue" ou "chainee"
    char org_internal[10]; // "triee" ou "non-triee"
} Metadata;

typedef struct {
    Metadata metadata;
    int *block_pointers; // Pointeurs vers les blocs pour les fichiers chaînés
} File;

int disk[MAX_BLOCKS]; // 0: libre, 1: occupé
Record records[MAX_BLOCKS][MAX_RECORDS_PER_BLOCK];
File files[MAX_FILES];
int file_count = 0;

void initialize_disk(int total_blocks) {
    for (int i = 0; i < total_blocks; i++) {
        disk[i] = 0; // Marquer tous les blocs comme libres
    }
    file_count = 0;
    printf("Disque initialisé avec %d blocs.\n", total_blocks);
}

int allocate_blocks(int num_blocks) {
    int allocated = 0, start = -1;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (disk[i] == 0) {
            if (start == -1) start = i;
            allocated++;
        } else {
            allocated = 0;
            start = -1;
        }
        if (allocated == num_blocks) {
            for (int j = start; j < start + num_blocks; j++) {
                disk[j] = 1; // Marquer les blocs comme occupés
            }
            return start;
        }
    }
    return -1; // Espace insuffisant
}

void create_file() {
    if (file_count >= MAX_FILES) {
        printf("Nombre maximal de fichiers atteint.\n");
        return;
    }

    File *new_file = &files[file_count];

    printf("Nom du fichier : ");
    scanf("%s", new_file->metadata.name);

    printf("Nombre d'enregistrements : ");
    int num_records;
    scanf("%d", &num_records);

    printf("Organisation globale (contigue/chainee) : ");
    scanf("%s", new_file->metadata.org_global);

    printf("Organisation interne (triee/non-triee) : ");
    scanf("%s", new_file->metadata.org_internal);

    int num_blocks = (num_records + MAX_RECORDS_PER_BLOCK - 1) / MAX_RECORDS_PER_BLOCK;
    new_file->metadata.size_in_blocks = num_blocks;
    new_file->metadata.size_in_records = num_records;

    if (strcmp(new_file->metadata.org_global, "contigue") == 0) {
        int start_block = allocate_blocks(num_blocks);
        if (start_block == -1) {
            printf("Espace insuffisant pour créer le fichier.\n");
            return;
        }
        new_file->metadata.first_block = start_block;
    } else if (strcmp(new_file->metadata.org_global, "chainee") == 0) {
        new_file->block_pointers = (int *)malloc(num_blocks * sizeof(int));
        for (int i = 0; i < num_blocks; i++) {
            int block = allocate_blocks(1);
            if (block == -1) {
                printf("Espace insuffisant pour créer le fichier.\n");
                return;
            }
            new_file->block_pointers[i] = block;
        }
    } else {
        printf("Mode d'organisation non reconnu.\n");
        return;
    }

    file_count++;
    printf("Fichier '%s' créé avec succès.\n", new_file->metadata.name);
}

void display_disk() {
    printf("\nEtat du disque:\n");
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (disk[i] == 0) {
            printf("Bloc %d : Libre\n", i);
        } else {
            for (int j = 0; j < file_count; j++) {
                if (strcmp(files[j].metadata.org_global, "contigue") == 0 &&
                    files[j].metadata.first_block <= i &&
                    i < files[j].metadata.first_block + files[j].metadata.size_in_blocks) {
                    printf("Bloc %d : %s\n", i, files[j].metadata.name);
                    break;
                } else if (strcmp(files[j].metadata.org_global, "chainee") == 0) {
                    for (int k = 0; k < files[j].metadata.size_in_blocks; k++) {
                        if (files[j].block_pointers[k] == i) {
                            printf("Bloc %d : %s\n", i, files[j].metadata.name);
                            break;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int choice, total_blocks = 100;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Initialiser la mémoire secondaire\n");
        printf("2. Créer un fichier\n");
        printf("3. Afficher l'état de la mémoire\n");
        printf("4. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Nombre total de blocs : ");
                scanf("%d", &total_blocks);
                initialize_disk(total_blocks);
                break;
            case 2:
                create_file();
                break;
            case 3:
                display_disk();
                break;
            case 4:
                exit(0);
            default:
                printf("Choix invalide.\n");
        }
    }
    return 0;
}

