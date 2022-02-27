#include "memory.h"
#include "stdlib.h"
#include <string.h>
#include <stddef.h>
#include <curl/curl.h>
#include <mysql.h>

/**
 * Renvoie dans \b productArray les 4 colonnes de la bdd concernant l'objet désigné par sa clé primaire \b id \n\n
 * Il faut malloc 4 sizeof(*char) dans productArray avant de le passer dans la fonction et ne pas oublier de free(productArray) quand vous avez finis vos traitements. Exemple : \n\n
 *
 * char **loli;\n
 * loli = malloc(4 * sizeof(char*));\n
 * ptdrTKi(1, loli);\n
 * for (int i = 0; i < 4; ++i) {\n
 *      printf("\\n%s",loli[i]);\n
 *      free(loli[i]);\n
 * }\n
 * free(loli);\n\n
 * @param id id de l'objet dans la BDD (clé primaire)
 * @param productArray array de *char (donc un **char)
 */
void retrieveOneProductInfo(int id, char **productArray);

void freeRetrieveOneProductInfo(char **productArray);

/**
 * Renvoie dans \b historyArray jusqu'à 5 pointeurs qui eux pointent vers un array de string (une colonne mysql).\n\n
 * Il faut intialiser un pointeur de pointeur de char (char***) et lui malloc 5 sizeof(char**)\n
 * Il faut aussi initiliaser un unsigned long à 0 pour compter le nombre de lignes que l'on retourne (utile pour free ensuite les pointeurs.\n Exemple: \n\n
 * char ***historyArray;\n
    historyArray = malloc(5 * sizeof(char *));\n
    unsigned long rowCount = 0;\n
    retrieveProductHistory(2, historyArray, &rowCount);\n
    freeProductHistory(historyArray, &rowCount);\n
 * \n\n
 * @param int id
 * @param char** historyArray
 * @param unsigned_long rowCount
 */
void retrieveProductHistory(int id, char ***historyArray, unsigned long *rowCount);

void freeProductHistory(char ***historyArray, unsigned long *rowCount);

/**
 * Renvoie dans \b productList \b (nombre=rowCount) pointeurs qui eux pointent vers un array de string (une colonne mysql).\n\n
 * Il faut intialiser un pointeur de pointeur de char (char***) et lui malloc 1. On realloc dans la fonction\n
 * Il faut aussi initiliaser un unsigned long à 0 pour compter le nombre de lignes que l'on retourne (utile pour free ensuite les pointeurs.\n Exemple: \n\n
 * char ***productList;\n
    productList = malloc(1);\n
    unsigned long rowCount = 0;\n
    retrieveProducts(productList, &rowCount);\n
    freeProductList(productList, &rowCount);\n
 * \n\n
 * @param char** historyArray
 * @param unsigned_long rowCount
 */
void retrieveProducts(char ***productList, unsigned long *rowCount);

void freeProductList(char ***productList, unsigned long *rowCount);


void finish_with_error(MYSQL *con);
