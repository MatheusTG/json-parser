#include <stdio.h>
#include <string.h>
#include "json.h"
#include "io.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <json_file>\n", argv[0]);
        return 1;
    }

    char *path = argv[1];

    char *conteudo = readFile(path);

    int len = strlen(conteudo);
    
    conteudo[len] = '\0';
    
    JsonValue *result = parse(conteudo, 0, NULL);
    
    jsonPrint(result);

    return 0;
}