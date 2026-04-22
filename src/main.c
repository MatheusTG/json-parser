#include <stdio.h>
#include "json.h"

int main() {
    JsonValue *value = jsonCreateString("Matheus");
    JsonObject *obj = jsonCreateObject(1);

    jsonObjectSet(obj, 0, "name", value);

    JsonValue *root = jsonCreateRoot(obj);

    printf("%s\n", root->object->pairs[0].value->string);

    return 0;
}