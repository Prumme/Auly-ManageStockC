#include "memory.h"
#include "stdlib.h"
#include <string.h>
#include <stddef.h>
#include <curl/curl.h>
#include <mysql.h>

/**
 * Renvoie dans \b rowCopy les 4 colonnes de la bdd concernant l'objet désigné par sa clé primaire \b id \n\n
 * Il faut malloc 4 sizeof(*char) dans rowCopy avant de le passer dans la fonction et ne pas oublier de free(rowCopy) quand vous avez finis vos traitements. Exemple : \n\n
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
 * @param rowCopy array de *char (donc un **char)
 */
void retrieveProductInfo(int id, char **rowCopy);

void freeRetrieveProductInfo(char **rowCopy);

void finish_with_error(MYSQL *con);
