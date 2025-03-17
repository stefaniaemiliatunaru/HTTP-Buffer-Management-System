#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"     /* JSON library */
#include <ctype.h>

#define HOST "34.246.184.49"
#define PORT 8080
#define LEN 1000
#define PAYLOAD "application/json"
#define REGISTER "/api/v1/tema/auth/register"
#define LOGIN "/api/v1/tema/auth/login"
#define ACCESS "/api/v1/tema/library/access"
#define BOOKS "/api/v1/tema/library/books"
#define LOGOUT "/api/v1/tema/auth/logout"

/* function that removes newline from the end of a string */
void remove_newline(char string[LEN])
{
    if (string[strlen(string) - 1] == '\n') {
        string[strlen(string) - 1] = '\0';
    }
}

/* function that processes the register command */
char *register_function(int sockfd) {
    /* reads the username and password */
    char line[LEN], username[LEN], password[LEN];
    char *message, *response;

    /* reads user's information */
    /* reads username */
    memset(username, 0, LEN);
    printf("username=");
    fgets(line, LEN, stdin);
    strncpy(username, line, LEN);
    remove_newline(username);

    /* reads password */
    memset(password, 0, LEN);
    printf("password=");
    fgets(line, LEN, stdin);
    strncpy(password, line, LEN);
    remove_newline(password);

    for (int i = 0; i < strlen(password); i++) {
        if (password[i] == ' ') {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "credentials error");
            return response_buffer;
        }
    }

    for (int i = 0; i < strlen(username); i++) {
        if (username[i] == ' ') {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "credentials");
            return response_buffer;
        }
    }

    /* adds the introduced information to a JSON object */
    JSON_Value *json_value = json_value_init_object();
    JSON_Object *json_object = json_value_get_object(json_value);
    json_object_set_string(json_object, "username", username);
    json_object_set_string(json_object, "password", password);
    char *serialized_string = json_serialize_to_string(json_value);

    /* computes post request */
    message = compute_post_request(HOST, "/api/v1/tema/auth/register", "application/json", serialized_string, NULL, NULL);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    json_value_free(json_value);
    json_free_serialized_string(serialized_string);
    
    return response;
}

/* function that processes the login command */
char *login_function(int sockfd) {
    /* reads the username and password */
    char line[LEN], username[LEN], password[LEN];
    char *message, *response;

    /* reads user's information */
    /* reads username */
    memset(username, 0, LEN);
    printf("username=");
    fgets(line, LEN, stdin);
    strncpy(username, line, LEN);
    remove_newline(username);

    /* reads password */
    memset(password, 0, LEN);
    printf("password=");
    fgets(line, LEN, stdin);
    strncpy(password, line, LEN);
    remove_newline(password);

    for (int i = 0; i < strlen(username); i++) {
        if (username[i] == ' ') {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "credentials");
            return response_buffer;
        }
    }

    for (int i = 0; i < strlen(password); i++) {
        if (password[i] == ' ') {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "credentials error");
            return response_buffer;
        }
    }

    /* adds the introduced information to a JSON object */
    JSON_Value *json_value = json_value_init_object();
    JSON_Object *json_object = json_value_get_object(json_value);
    json_object_set_string(json_object, "username", username);
    json_object_set_string(json_object, "password", password);
    char *serialized_string = json_serialize_to_string(json_value);

    /* computes post request */
    message = compute_post_request(HOST, "/api/v1/tema/auth/login", "application/json", serialized_string, NULL, NULL);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    json_value_free(json_value);
    json_free_serialized_string(serialized_string);
    
    return response;
}

/* function that processes the enter_library command */
char *enter_library_function(int sockfd, char cookie[LEN]) {
    char *message, *response;

    /* computes get request */
    message = compute_get_request(HOST, "/api/v1/tema/library/access", NULL, cookie, NULL);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    return response;
}

/* function that processes the get_books command */
char *get_books_function(int sockfd, char cookie[LEN], char jwt[LEN]) {
    char *message, *response;

    /* computes get request */
    message = compute_get_request(HOST, "/api/v1/tema/library/books", NULL, cookie, jwt);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    return response;
}

/* function that processes the get_books command */
char *get_book_function(int sockfd, char cookie[LEN], char jwt[LEN]) {
    char line[LEN], id[LEN];
    char *message, *response;

    /* checks if the introduced id is a number */
    memset(id, 0, LEN);
    printf("id=");
    fgets(line, LEN, stdin);
    strncpy(id, line, LEN);
    remove_newline(id);
    for (int i = 0; i < strlen(id); i++) {
        if (isdigit(id[i]) == 0) {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "id error");
            return response_buffer;
        }
    }

    char buffer[LEN] = "/api/v1/tema/library/books/";
    strcat(buffer, id);

    /* computes get request */
    message = compute_get_request(HOST, buffer, NULL, cookie, jwt);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    return response;
}

/* function that processes the add_book command */
char *add_book_function(int sockfd, char cookie[LEN], char jwt[LEN]) {
    /* reads the username and password */
    char line[LEN], title[LEN], author[LEN], genre[LEN], page_count[LEN], publisher[LEN];
    char *message, *response;

    /* reads book's information */
    /* reads book's title */
    memset(title, 0, LEN);
    printf("title=");
    fgets(line, LEN, stdin);
    strncpy(title, line, LEN);
    remove_newline(title);

    /* reads book's author */
    memset(author, 0, LEN);
    printf("author=");
    fgets(line, LEN, stdin);
    strncpy(author, line, LEN);
    remove_newline(author);

    /* reads book's genre */
    memset(genre, 0, LEN);
    printf("genre=");
    fgets(line, LEN, stdin);
    strncpy(genre, line, LEN);
    remove_newline(genre);

    /* reads book's page count */
    memset(page_count, 0, LEN);
    printf("page_count=");
    fgets(line, LEN, stdin);
    strncpy(page_count, line, LEN);
    remove_newline(page_count);

    /* reads book's publishser */
    memset(publisher, 0, LEN);
    printf("publisher=");
    fgets(line, LEN, stdin);
    strncpy(publisher, line, LEN);
    remove_newline(publisher);

    /* checks if page count is a number */
    for (int i = 0; i < strlen(page_count); i++) {
        if (isdigit(page_count[i]) == 0) {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "information error");
            return response_buffer;
        }
    }

    /* checks if title and author are valid */
    if (title[0] == 0 || author[0] == 0) {
        char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "information error");
            return response_buffer;
    }

    /* adds the introduced information to a JSON object */
    JSON_Value *json_value = json_value_init_object();
    JSON_Object *json_object = json_value_get_object(json_value);
    json_object_set_string(json_object, "title", title);
    json_object_set_string(json_object, "author", author);
    json_object_set_string(json_object, "genre", genre);
    json_object_set_string(json_object, "page_count", page_count);
    json_object_set_string(json_object, "publisher", publisher);
    char *serialized_string = json_serialize_to_string(json_value);

    /* computes post request */
    message = compute_post_request(HOST, "/api/v1/tema/library/books", "application/json", serialized_string, cookie, jwt);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    json_value_free(json_value);
    json_free_serialized_string(serialized_string);
    
    return response;
}

/* function that processes the delete_books command */
char *delete_book_function(int sockfd, char cookie[LEN], char jwt[LEN]) {
    char line[LEN], id[LEN];
    char *message, *response;

    /* checks if the introduced id is a number */
    memset(id, 0, LEN);
    printf("id=");
    fgets(line, LEN, stdin);
    strncpy(id, line, LEN);
    remove_newline(id);
    for (int i = 0; i < strlen(id); i++) {
        if (isdigit(id[i]) == 0) {
            char *response_buffer = calloc(LEN, sizeof(char));
            strcpy(response_buffer, "id error");
            return response_buffer;
        }
    }

    char buffer[LEN] = "/api/v1/tema/library/books/";
    strcat(buffer, id);

    /* computes delete request */
    message = compute_delete_request(HOST, buffer, NULL, cookie, jwt);
    
    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    return response;
}

/* function that processes the logout command */
char *logout_function(int sockfd, char cookie[LEN], char jwt[LEN]) {
    char *message, *response;

    /* computes get request */
    message = compute_get_request(HOST, "/api/v1/tema/auth/logout", NULL, cookie, jwt);

    /* sends the message to server and receives a response */
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    
    return response;
}

int main(int argc, char *argv[])
{
    char command[LEN], cookie[LEN], jwt[LEN];
    memset(cookie, 0, LEN);
    memset(jwt, 0, LEN);
    char *response = NULL;
    int sockfd;

    /* loop for reading commands */
    while (1) {
        fgets(command, LEN, stdin);
        remove_newline(command);
        sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

        /* check if command is of "register" type */
        if (strcmp(command, "register") == 0) {
            /* performs register action */
            response = register_function(sockfd);

            /* checks if username is already used */
            if (strstr(response, "credentials") != 0) {
                puts("ERROR! Invalid credentials!");
            } else if (strstr(response, "error") != 0) {
                puts("ERROR! The username you introduced is already in use!");
            } else {
                puts("SUCCESS! User registered successfully!");
            }
            response = NULL;

        /* check if command is of "login" type */
        } else if (strcmp(command, "login") == 0) {
            /* performs login action */
            response = login_function(sockfd);

            /* checks if credentials match */
            if (strstr(response, "credentials") != 0) {
                puts("ERROR! Invalid credentials!");
            } else if (strstr(response, "error") != 0) {
                puts("ERROR! The username and password do not match!");
            } else {
                char *cookie_start = strstr(response, "connect.sid=");
                char *cookie_finish = strstr(cookie_start, ";");
                strncpy(cookie, cookie_start, cookie_finish - cookie_start);
                puts("SUCCESS! User logged in successfully!");
            }
            response = NULL;
        
        /* checks if command is of "enter_library" type */
        } else if (strcmp(command, "enter_library") == 0) {
            /* performs enter_library action */
            response = enter_library_function(sockfd, cookie);
            
            /* checks if credentials match */
            if (strstr(response, "error") != 0) {
                puts("ERROR! No user logged in, please log in to use the library!");
            } else {
                char *jwt_start = strstr(response, "{") + 10;
                char *jwt_finish = strstr(jwt_start, "}") - 1;
                strncpy(jwt, jwt_start, jwt_finish - jwt_start);
                puts("SUCCESS! You can now access the library!");
            }
            response = NULL;

        /* checks if command is of "get_books" type */
        } else if (strcmp(command, "get_books") == 0) {
            /* performs get_books command */
            response = get_books_function(sockfd, cookie, jwt);

            /* checks if user has access to the library */
            if (strstr(response, "error") != 0) {
                puts("ERROR! User doesn't have acces to the library!");
            } else {
                char *list = strstr(response, "[");
                puts(list);
            }
            response = NULL;
        
        /* checks if command is of "get_book" type */
        } else if (strcmp(command, "get_book") == 0) {
            /* performs get_book command */
            response = get_book_function(sockfd, cookie, jwt);

            /* checks if id is valid */
            if (strstr(response, "id error") != 0) {
                puts("ERROR! Invalid id!");
            /* checks if user has access to the library */
            } else if (strstr(response, "error") != 0 || strstr(response, "404 Not Found") != 0) {
                puts("ERROR! User doesn't have access to the library!");
            } else {
                char *list = basic_extract_json_response(response);
                puts(list);
            }
            response = NULL;
        
        } else if (strcmp(command, "add_book") == 0) {
            /* performs add_book command */
            response = add_book_function(sockfd, cookie, jwt);

            /* checks if information is complete */
            if (strstr(response, "information error") != 0) {
                puts("ERROR! Information not complete!");
            /* checks if user has access to the library */
            } else if (strstr(response, "library error") != 0) {
                puts("ERROR! User doesn't have access to the library!");
            } else {
                puts("SUCCESS! Book added successfully!");
            }
            response = NULL;

        } else if (strcmp(command, "delete_book") == 0) {
            /* performs delete_book command */
            response = delete_book_function(sockfd, cookie, jwt);

            /* checks if id is valid */
            if (strstr(response, "id error") != 0) {
                puts("ERROR! Invalid id!");
            } else if (strstr(response, "error") != 0) {
                puts("ERROR! User doesn't have access to the library!");
            } else {
                puts("SUCCESS! Book deleted successfully!");
            }
            response = NULL;
        
        /* checks if request is of "logout type" */
        } else if (strcmp(command, "logout") == 0) {
            /* performs logout command */
            response = logout_function(sockfd, cookie, jwt);

            /* checks if user is logged in */
            if (strstr(response, "error") != 0) {
                puts("ERROR! No user is logged in!");
            } else {
                puts("SUCCESS! User successfully logged out!");
            }
            memset(cookie, 0, LEN);
            memset(jwt, 0, LEN);
            response = NULL;

        /* checks if request is of "exit" type */
        } else if (strcmp(command, "exit") == 0) {
            close_connection(sockfd);
            break;
        } else {
            puts("Unknown command! Please type in a valid command!");
        }
        close_connection(sockfd);
    }

    return 0;
}

