#include "json.h"
#include <stdio.h>
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
    obj->count = 0;
    obj->capacity = amount;
    obj->pairs = malloc(sizeof(JsonPair) * amount);
    return obj;
}

static void jsonObjectEnsureCapacity(JsonObject *obj, int index) {
    if (index >= obj->capacity) {
        obj->pairs = realloc(obj->pairs, sizeof(JsonPair) * (index + 1));

        for (int i = obj->count; i <= index; i++) {
            obj->pairs[i].key = NULL;
            obj->pairs[i].value = NULL;
        }

        obj->capacity = index + 1;
    }
}

void jsonObjectSetKey(JsonObject *obj, int index, char *key) {
    jsonObjectEnsureCapacity(obj, index);
    obj->pairs[index].key = key;
}

void jsonObjectSetValue(JsonObject *obj, int index, JsonValue *value) {
    jsonObjectEnsureCapacity(obj, index);
    obj->pairs[index].value = value;
}

JsonValue* jsonCreateRoot(JsonObject *obj) {
    JsonValue *root = malloc(sizeof(JsonValue));
    root->type = JSON_OBJECT;
    root->object = obj;
    return root;
}

void jsonPrintObject(JsonObject *obj) {
    if (!obj) {
        printf("null\n");
        return;
    }

    printf("{\n");

    for (int i = 0; i < obj->count; i++) {
        JsonPair pair = obj->pairs[i];

        printf("  ");

        if (pair.key) {
            printf("\"%s\"", pair.key);
        } else {
            printf("null");
        }

        printf(": ");

        if (pair.value) {
            if (pair.value->type == JSON_STRING) {
                printf("\"%s\"", pair.value->string);
            } else if (pair.value->type == JSON_OBJECT) {
                jsonPrintObject(pair.value->object);
            } else {
                printf("null");
            }
        } else {
            printf("null");
        }

        if (i < obj->count - 1) {
            printf(",");
        }

        printf("\n");
    }

    printf("}\n");
}

void jsonPrint(JsonValue *value) {
    if (!value) {
        printf("null\n");
        return;
    }

    if (value->type == JSON_OBJECT) {
        jsonPrintObject(value->object);
    } else if (value->type == JSON_STRING) {
        printf("\"%s\"\n", value->string);
    } else {
        printf("null\n");
    }
}