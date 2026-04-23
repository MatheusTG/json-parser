#include <stdio.h>
#include "json.h"
#include "io.h"

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
    printf("%s\n", conteudo);

    return 0;
}