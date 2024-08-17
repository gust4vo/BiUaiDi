#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_STRING_LENGTH 256

typedef enum { false, true } bool;

typedef struct {
    char idend[MAX_STRING_LENGTH];
    long id_logrado;
    char sigla_tipo[MAX_STRING_LENGTH];
    char nome_logra[MAX_STRING_LENGTH];
    int numero_imo;
    char nome_bairr[MAX_STRING_LENGTH];
    char nome_regio[MAX_STRING_LENGTH];
    int cep;
    double x;
    double y;
    bool isOn;
} address_t;

typedef struct knn {
    double dist;
    int id;
} knn_t, *ptr_knn_t;

int cmpknn(const void* a, const void * b) {
    ptr_knn_t k1 = (ptr_knn_t) a;
    ptr_knn_t k2 = (ptr_knn_t) b;
    if (k1->dist > k2->dist) return 1;
    else if (k1->dist < k2->dist) return -1;
    else return 0;
}

double dist(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0); 
}

void parseLine(const char* line, address_t* address) {
    char* actualWord;
    char lineCopy[MAX_STRING_LENGTH]; 
    strcpy(lineCopy, line);
    actualWord = strtok(lineCopy, ";");
    int count = 0;

    while (actualWord != NULL) {
        switch (count) {
            case 0: strcpy(address->idend, actualWord); break;
            case 1: address->id_logrado = strtol(actualWord, NULL, 10); break;
            case 2: strcpy(address->sigla_tipo, actualWord); break;
            case 3: strcpy(address->nome_logra, actualWord); break;
            case 4: address->numero_imo = strtol(actualWord, NULL, 10); break;
            case 5: strcpy(address->nome_bairr, actualWord); break;
            case 6: strcpy(address->nome_regio, actualWord); break;
            case 7: address->cep = strtol(actualWord, NULL, 10); break;
            case 8: address->x = strtod(actualWord, NULL); break;
            case 9: address->y = strtod(actualWord, NULL); break;
            default: break;
        }
        actualWord = strtok(NULL, ";");
        ++count;
    }

    address->isOn = true;
}

int main() {
    FILE* pointsFile = fopen("geracarga.base", "r");
    if (pointsFile == NULL) {
        fprintf(stderr, "There's no such file 'geracarga.base'.\n");
        return -1;
    }

    char firstLine[MAX_STRING_LENGTH];
    fgets(firstLine, sizeof(firstLine), pointsFile);

    int numAddresses = atoi(firstLine);

    address_t* addresses = (address_t*) malloc(sizeof(address_t) * numAddresses);

    char line[MAX_STRING_LENGTH];
    int linesRead = 0;
    while (fgets(line, sizeof(line), pointsFile) != NULL && linesRead < numAddresses) {
        address_t address;
        parseLine(line, &address);
        addresses[linesRead] = address;
        linesRead++;
    }

    fclose(pointsFile);

    FILE* commandsFile = fopen("geracarga.ev", "r");
    if (commandsFile == NULL) {
        fprintf(stderr, "There's no such file 'geracarga.ev'.\n");
        free(addresses);
        return -1;
    }

    fgets(firstLine, sizeof(firstLine), commandsFile);  // Read the first line

    int numberOfEvents = atoi(firstLine);  // Get the number of events from the first line

    char type;
    double x, y;
    int numberOfNearestPoints;
    char id[MAX_STRING_LENGTH];

    clock_t start = clock();
    while (fscanf(commandsFile, "%c", &type) != EOF) {
        if (type == 'C') {
            fscanf(commandsFile, "%lf %lf %d", &x, &y, &numberOfNearestPoints);
            printf("%c %lf %lf %d\n", type, x, y, numberOfNearestPoints);

            ptr_knn_t kvet = (ptr_knn_t) malloc(numAddresses * sizeof(knn_t));
            for (int i = 0; i < numAddresses; i++) {
                kvet[i].id = i;
                kvet[i].dist = (addresses[i].isOn) ? dist(x, y, addresses[i].x, addresses[i].y) : -1;
            }

            qsort(kvet, numAddresses, sizeof(knn_t), cmpknn);

            int found = 0;
            int i = 0;
            while (found < numberOfNearestPoints && i < numAddresses) {
                if (kvet[i].dist != -1) {
                    printf("%s %s, %d, %s, %s, %d", addresses[i].sigla_tipo,
                        addresses[i].nome_logra, addresses[i].numero_imo,
                        addresses[i].nome_bairr, addresses[i].nome_regio,
                        addresses[i].cep);
                    printf(" (%.3f)\n", kvet[i].dist);
                    found++;
                }
                i++;
            }

            free(kvet);
        } else if (type == 'D' || type == 'A') {
            fscanf(commandsFile, "%s", id);
            printf("%c %s\n", type, id);

            bool alreadySet = false;

            for (int i = 0; i < numAddresses; i++) {
                if (strcmp(addresses[i].idend, id) == 0) {
                    alreadySet = (type == 'D' && addresses[i].isOn == false) || (type == 'A' && addresses[i].isOn == true);
                    addresses[i].isOn = (type == 'D') ? false : true;
                    break;
                }
            }

            if (alreadySet) {
                printf("Ponto de recarga %s já estava %s.\n", id, (type == 'D') ? "desativado" : "ativo");
            } else {
                printf("Ponto de recarga %s %s.\n", id, (type == 'D') ? "desativado" : "ativado");
            }
        }
    }

    fclose(commandsFile);

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    // Print execution time to console
    //printf("Tempo de execução: %.3f segundos\n", elapsed);

    // Write the number of events and execution time to 'tempos2.txt'
    FILE* timesFile = fopen("tempos2.txt", "a");
    if (timesFile == NULL) {
        fprintf(stderr, "Unable to open 'tempos2.txt' for writing.\n");
        free(addresses);
        return -1;
    }

    fprintf(timesFile, "(%d, %.4f),\n", numberOfEvents, elapsed);
    fclose(timesFile);

    free(addresses);
    return 0;
}
