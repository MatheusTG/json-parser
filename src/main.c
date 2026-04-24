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

    JsonValue *value = jsonCreateString("Matheus");
    JsonObject *obj = jsonCreateObject(1);

    jsonObjectSetKey(obj, 0, "name");
    jsonObjectSetValue(obj, 0, value);

    JsonValue *root = jsonCreateRoot(obj);

    printf("%s\n", root->object->pairs[0].value->string);

    char *conteudo = readFile(path);

    int len = strlen(conteudo);
    
    conteudo[len] = '\0',
    
    printf("%s\n", conteudo);

    printf("\n=====================\n");
    initParser();
    JsonValue *result = parse(conteudo, 0, NULL);
    printf("Key: %s\n", result->object->pairs[0].key);
    printf("Value: %s\n", result->object->pairs[0].value->string);
    printf("Count: %d\n", result->object->count);
    printf("\n=====================\n");

    return 0;
}