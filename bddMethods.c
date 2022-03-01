#include "bddMethods.h"

void retrieveOneProductInfo(int id, char **productArray) {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    // Connexion à la BDD. Variable con (init), host, username, mdp, nom de la bdd, port, jsp et jsp mdrrr
    if (mysql_real_connect(con, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    char sql[50];
    sprintf(sql, "SELECT titre, description, url FROM Product WHERE id = %d", id); // On insère l'id dans la string query sql
    // Exécution d'une requête SQL. Exit avec erreur sinon.
    if (mysql_query(con, sql)) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    // Renvoie le nombre de colonnes de la row (pas le nombre de lignes)
    unsigned long num_fields = mysql_num_fields(result);
    unsigned long *lengths;

    MYSQL_ROW row;

    // mysql_fetch_row fonctionne comme en PHP, chaque call de la fonction récupère la ligne suivante dans le résultat (result)
    while ((row = mysql_fetch_row(result))) {
        // La ligne existe sous forme de array donc on itère pour récupérer.
        for (int i = 0; i < num_fields; i++) {
            lengths = mysql_fetch_lengths(result); // donne la taille des colonnes dans un array
            productArray[i] = malloc(lengths[i] + 1);
            strncpy(productArray[i], row[i], lengths[i] + 1); // On copie la colonne dans un pointeur de l'array de strings.
        }
    }

    // J'imagine qu'on free la mémoire prise par tous les mallocs des fonctions mysql pour stocker les strings
    mysql_free_result(result);

    // On ferme la connexion au serveur
    mysql_close(con);
}

void freeRetrieveOneProductInfo(char **productArray) {
    for (int i = 0; i < 4; ++i) {
        free(productArray[i]);
    }
    free(productArray);
}

void retrieveProductHistory(int id, char ***historyArray,unsigned long* rowCount) {
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    char sql[80];
    sprintf(sql, "SELECT * FROM History where id_product = %d ORDER BY timestamp DESC LIMIT 5;", id);
    if (mysql_query(con, sql)) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    unsigned long num_fields = mysql_num_fields(result);
    *rowCount = mysql_num_rows(result);
    unsigned long *lengths;

    MYSQL_ROW row;

    for (unsigned long j = 0; j < *rowCount; ++j) {
        historyArray[j] = malloc(sizeof(char *) * 5);
        row = mysql_fetch_row(result);

        for (int i = 0; i < num_fields; i++) {
            lengths = mysql_fetch_lengths(result); // donne la taille des colonnes dans un array
            historyArray[j][i] = malloc(lengths[i] + 1);
            strncpy(historyArray[j][i], row[i],
                    lengths[i] + 1); // On copie la colonne dans un pointeur de l'array de strings.
        }

    }

    mysql_free_result(result);
    mysql_close(con);
}

void freeProductHistory(char ***historyArray, unsigned long* rowCount) {
    for (unsigned long k = 0; k < *rowCount; ++k) {
        for (int l = 0; l < 5; ++l) {
            free(historyArray[k][l]);
        }
    }
    free(historyArray);
}

void retrieveProducts(char ***productList,unsigned long* rowCount) {

    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    char sql[301];
    // Renvoie sous la forme id, titre, timestamp, prix, status (boolean)
    sprintf(sql, "\n"
                 "SELECT Product.id, Product.titre, H.timestamp, H.price, H.status FROM Product INNER JOIN History H ON Product.id = H.id_product INNER JOIN ( SELECT id, id_product, price, status, MAX(timestamp) Maxdate FROM History GROUP BY id_product ) q on q.id_product = H.id_product AND q.Maxdate = H.timestamp;");
    if (mysql_query(con, sql)) {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        finish_with_error(con);
    }

    unsigned long num_fields = mysql_num_fields(result);
    *rowCount = mysql_num_rows(result);

    unsigned long *lengths;

    MYSQL_ROW row;

    realloc(productList, *rowCount);

    for (unsigned long j = 0; j < *rowCount; ++j) {
        productList[j] = malloc(sizeof(char *) * 5);
        row = mysql_fetch_row(result);

        for (int i = 0; i < num_fields; i++) {
            lengths = mysql_fetch_lengths(result); // donne la taille des colonnes dans un array
            productList[j][i] = malloc(lengths[i] + 1);
            strncpy(productList[j][i], row[i],
                    lengths[i] + 1); // On copie la colonne dans un pointeur de l'array de strings.
        }

    }

    mysql_free_result(result);
    mysql_close(con);
}

void freeProductList(char ***productList, unsigned long* rowCount) {
    for (unsigned long k = 0; k < *rowCount; ++k) {
        for (int l = 0; l < 4; ++l) {
            free(productList[k][l]);
        }
    }
    free(productList);
}


void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}