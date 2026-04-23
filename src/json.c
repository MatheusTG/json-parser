#include "json.h"
#include <stdlib.h>

static JsonValue* initJsonValue() {
    JsonValue *value = malloc(sizeof(JsonValue));
    value->type = JSON_NULL;
    return value;
}

static JsonObject* initJsonObject() {
    JsonObject *obj = malloc(sizeof(JsonObject));
    obj->count = 0;
    obj->pairs = NULL;
    return obj;
}

JsonValue* jsonCreateString(const char *str) {
    JsonValue *value = initJsonValue();
    value->type = JSON_STRING;
    value->string = (char *)str;
    return value;
}

JsonObject* jsonCreateObject(int amount) {
    JsonObject *obj = initJsonObject();
    obj->count = amount;
    obj->pairs = malloc(sizeof(JsonPair) * amount);
    return obj;
}

void jsonObjectSetKey(JsonObject *obj, int index, char *key) {
    obj->pairs[index].key = key;
}

void jsonObjectSetValue(JsonObject *obj, int index, JsonValue *value) {
    obj->pairs[index].value = value;
}

JsonValue* jsonCreateRoot(JsonObject *obj) {
    JsonValue *root = malloc(sizeof(JsonValue));
    root->type = JSON_OBJECT;
    root->object = obj;
    return root;
}