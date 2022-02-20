//
// Created by esgi on 19/02/2022.
//

#include "curlBdd.h"


void init_string(struct string *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() a echoue \n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *destString) {
    size_t new_len = destString->len + size * nmemb;
    destString->ptr = realloc(destString->ptr, new_len + 1);
    if (destString->ptr == NULL) {
        fprintf(stderr, "realloc() a echoue\n");
        exit(EXIT_FAILURE);
    }
    memcpy(destString->ptr + destString->len, ptr, size * nmemb);
    destString->ptr[new_len] = '\0';
    destString->len = new_len;

    return size * nmemb;
}

int recupBody(char *URL, struct string bufferString) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        init_string(&bufferString);

        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // On suit en cas de redirection de la page (voir doc CURL)
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         writefunc); // On passe une fonction en callback qui va récupérer la réponse de la requête
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bufferString);
        res = curl_easy_perform(curl);

        int stock = verifyStockFromBuffer(bufferString.ptr);
        double price = verifyPriceFromBuffer(bufferString.ptr);
        logHistoryBdd(1, price, stock);

        free(bufferString.ptr); // On libère la mémoire accordée à la string

        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return 0;
}

int verifyStockFromBuffer(char *html) {

    int num = 0;
    char stock[] = "class=\"cart-box en-stock\"";
    if (strstr(html, stock) != 0) {//if match found
        num++;
    }
    printf("we found the word %s in the file %d times\n", stock, num);
    if (num > 0) {
        printf("\nIl y avait une occurence !");
        return 1;
    } else {
        printf("\nIl n'y avait aucune occurence !");
        return 0;
    }
}

double verifyPriceFromBuffer(char *html) {
    char price[25] = "\0";
    char *ptr, *ptr1, *ptr2, *ptr3;
    ptr = strstr(html, "itemprop=\"price\"");
    if (ptr) {
        ptr1 = strstr(ptr, "content=\"");
        if (ptr1) {
            ptr2 = strstr(ptr1, "\"");
            // printf("\n\n %s\n\n", ptr2);
            if (ptr2) {
                ptr3 = strstr(ptr2 + 1, "\"");
                // printf("\n\n %s\n\n", ptr3);
                ptrdiff_t bytes = ((char *) ptr3) - ((char *) ptr2 + 1);
                printf("\n coucou c %td", bytes);
                memcpy(price, ptr2 + 1, bytes);
            }
        }

        int done = 0;
    }

    printf("\n\n %s\n\n", price);
    double realPrice = strtod(price, NULL);
    if (realPrice) {
        return realPrice;
    } else {
        return 0.0;
    }
}

int logHistoryBdd(int id, double price, int inStock) {
    MYSQL *con = mysql_init(NULL);
    char *ptr = (char *) malloc(255 * 10 * 4 * sizeof(char));

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

    // Exécution d'une requête SQL. Exit avec erreur sinon.
    char query[2000];
    sprintf(query, "INSERT INTO History(id_product, price, status) VALUES (%d, %f, %d)", id, price, inStock);
    if (mysql_query(con, query)) {
        exit(-10);
    }

    // On ferme la connexion au serveur
    mysql_close(con);
    //  return ptr;
}
