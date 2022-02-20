//
// Created by esgi on 19/02/2022.
//
#include "memory.h"
#include "stdlib.h"
#include <stddef.h>
#include <curl/curl.h>
#include <mysql.h>
#ifndef PROJET_CURLBDD_H
#define PROJET_CURLBDD_H

struct string {
    char *ptr;
    size_t len;
};
void init_string(struct string *s);
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *destString);

/**
 * Récupère la page html de la source passée en paramètre \b URL et la range dans la string de la struct \b bufffer.\n\n
 *
 * La struct string \b bufferString ne doit pas être initialisée auparavant, juste déclarée
 *
 *
 * @param URL URL de la page à récupérer
 * @param bufferString buffer dans lequel on écrit le body
 * @return 0 en cas de bon déroulement.
 */
int recupBody(char* URL, struct string bufferString);

/**
 * Permet de vérifier si un produit est en stock sur topachat.com\n
 * @param html Page html à parser
 * @return 1 si le produit est en stock, 0 si le produit n'est pas immédiatement en stock
 */
int verifyStockFromBuffer(char* html);

/**
 * Permet de récupérer le prix d'un produit en stock sur topachat.com\n
 * @param html Page html à parser
 * @return le prix du produit
 */
double verifyPriceFromBuffer(char* html);

/**
 * Permet d'enregistrer en BDD le statut d'un produit
 * @param id id du produit dans la bdd
 * @param price son prix actuel
 * @param inStock son statut (1 pour en stock, 0 pour pas immédiatement en stock)
 * @return
 */
int logHistoryBdd(int id, double price, int inStock);

#endif //PROJET_CURLBDD_H
