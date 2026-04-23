#ifndef JSON_H
#define JSON_H

#include <stddef.h>

typedef enum {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

typedef struct JsonValue JsonValue;

typedef struct {
    char *key;
    JsonValue *value;
} JsonPair;

typedef struct {
    JsonPair *pairs;
    int count;
} JsonObject;

typedef struct {
    JsonValue **items;
    int count;
} JsonArray;

struct JsonValue {
    JsonType type;

    union {
        double number;
        char *string;
        int boolean;

        JsonArray *array;
        JsonObject *object;
    };
};

// funções
JsonValue* jsonCreateString(const char *str);
JsonObject* jsonCreateObject(int amount);
void jsonObjectSetKey(JsonObject *obj, int index, char *key);
void jsonObjectSetValue(JsonObject *obj, int index, JsonValue *value);
JsonValue* jsonCreateRoot(JsonObject *obj);

#endif