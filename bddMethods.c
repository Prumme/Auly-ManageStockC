#include "bddMethods.h"

void ptdrTKi(int id, char **rowCopy) {
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
    sprintf(sql, "SELECT * FROM Product WHERE id = %d", id); // On insère l'id dans la string query sql
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

    printf("\n\nNombre de colonnes : %lu\n", num_fields);

    printf("\nID | titre | description | url \n");

    // mysql_fetch_row fonctionne comme en PHP, chaque call de la fonction récupère la ligne suivante dans le résultat (result)
    while ((row = mysql_fetch_row(result))) {
        // La ligne existe sous forme de array donc on itère pour récupérer.
        for (int i = 0; i < num_fields; i++) {
            lengths = mysql_fetch_lengths(result); // donne la taille des colonnes dans un array
            rowCopy[i] = malloc(lengths[i] + 1);
            strncpy(rowCopy[i], row[i], lengths[i] + 1); // On copie la colonne dans un pointeur de l'array de strings.
        }
    }

    // J'imagine qu'on free la mémoire prise par tous les mallocs des fonctions mysql pour stocker les strings
    mysql_free_result(result);

    // On ferme la connexion au serveur
    mysql_close(con);
}

void finish_with_error(MYSQL *con) {
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}
