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

int isInStock(int id, char *URL) {
    CURL *curl;
    CURLcode res;
    struct string bufferString;
    curl = curl_easy_init();
    if (curl) {
        init_string(&bufferString);

        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // On suit en cas de redirection de la page (voir doc CURL)
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         writefunc); // On passe une fonction en callback qui va récupérer la réponse de la requête
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bufferString);
        res = curl_easy_perform(curl);

        if (res != 0) {
            printf("\nerreur de curl\n");
            return EXIT_FAILURE;
        }

        int stock;
        double price;
        if (strstr(bufferString.ptr, "topachat.com") != 0) {//if match found
            stock = verifyStockFromBufferTopachat(bufferString.ptr);
            price = verifyPriceFromBuffer(bufferString.ptr);
            logHistoryBdd(id, price, stock);
        }


        free(bufferString.ptr); // On libère la mémoire accordée à la string

        /* On "nettoie" (des free) */
        curl_easy_cleanup(curl);
    }
    return EXIT_SUCCESS;
}

int verifyStockFromBufferTopachat(char *curledHtml) {

    int occurrence = 0;
    char stock[] = "class=\"cart-box en-stock\"";
    char stock2[] = "class=\"cart-box en-stock-limite\"";
    if (strstr(curledHtml, stock) != 0) {//if match found
        occurrence++;
    } else {
        if (strstr(curledHtml, stock2) != 0) {//if match found
            occurrence++;
        }
    }
    if (occurrence > 0) {
        return 1;
    } else {
        return 0;
    }
}

double verifyPriceFromBuffer(char *curledHtml) {
    char price[25] = "\0";
    char *ptr, *ptr1, *ptr2, *ptr3;
    ptr = strstr(curledHtml, "itemprop=\"price\"");
    if (ptr) {
        ptr1 = strstr(ptr, "content=\"");
        if (ptr1) {
            ptr2 = strstr(ptr1, "\"");
            if (ptr2) {
                ptr3 = strstr(ptr2 + 1, "\"");
                ptrdiff_t bytes = ((char *) ptr3) - ((char *) ptr2 + 1);
                memcpy(price, ptr2 + 1, bytes);
            }
        }
    }

    double realPrice = strtod(price, NULL);
    if (realPrice) {
        return realPrice;
    } else {
        return 0.0;
    }
}

int logHistoryBdd(int id, double price, int inStock) {
    MYSQL *con = mysql_init(NULL);

    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        return EXIT_FAILURE;
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

    return EXIT_SUCCESS;
}

int refreshLog(){
    MYSQL *con = mysql_init(NULL);
    if (con == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(EXIT_FAILURE);
    }

    if (mysql_real_connect(con, "localhost", "esgi", "esgi",
                           "test", 0, NULL, 0) == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    if (mysql_query(con, "SELECT id, url FROM Product")) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(EXIT_FAILURE);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(EXIT_FAILURE);
    }

    MYSQL_ROW row;

    unsigned long num_fields = mysql_num_fields(result);
    unsigned long *lengths;

    struct string buffer;

    while ((row = mysql_fetch_row(result))) {
        // La ligne existe sous forme de array donc on itère pour récupérer.
        if(isInStock(atoi(row[0]), row[1]) == EXIT_SUCCESS){
            printf("\n Mise à jour du statut de l'objet id:%d réussie", atoi(row[0]));
        } else{
            printf("\n Erreur dans la mise à jour du statut de l'objet id:%d", atoi(row[0]));
        };
    }

    mysql_free_result(result);
    mysql_close(con);
    return EXIT_SUCCESS;
}
