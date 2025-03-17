#include <stdlib.h> /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params, char *cookie, char *jwt) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    /* builds HTTP GET line using the given parameters */
    if (query_params != NULL)
    {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    }
    else
    {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    /* adds the host */
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* adds headers and/or cookie, according to the protocol format */
    if (cookie != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    /* adds headers and/or JSON Web Token, according to the protocol format */
    if (jwt != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", jwt);
        compute_message(message, line);
    }

    /* adds final new line */
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char *content_type, char *body_data, char *cookie, char *jwt) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    /* writes the method name, URL and protocol type */
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    /* adds the host */
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* adds necessary headers in order to write Content-Length you must first compute the message size */
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(body_data));
    compute_message(message, line);

    /* adds headers and/or cookie, according to the protocol format */
    if (cookie != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    /* adds headers and/or JSON Web Token, according to the protocol format */
    if (jwt != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", jwt);
        compute_message(message, line);
    }

    /* adds new line at end of header */
    compute_message(message, "");

    /* adds the actual payload data */
    compute_message(message, body_data);

    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params, char *cookie, char *jwt) {
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    /* builds HTTP GET line using the given parameters */
    if (query_params != NULL)
    {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    }
    else
    {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    /* adds the host */
    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* adds headers and/or cookie, according to the protocol format */
    if (cookie != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Cookie: %s", cookie);
        compute_message(message, line);
    }

    /* adds headers and/or JSON Web Token, according to the protocol format */
    if (jwt != NULL) {
        memset(line, 0, LINELEN);
        sprintf(line, "Authorization: Bearer %s", jwt);
        compute_message(message, line);
    }

    /* adds final new line */
    compute_message(message, "");
    return message;
}