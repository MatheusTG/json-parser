#ifndef JSON_H
#define JSON_H 

#include <stdbool.h>

/* =========================
   Tipos de JSON
   ========================= */
typedef enum {
    JSON_NULL,
    JSON_BOOLEAN,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

/* Forward declaration */
typedef struct JsonValue JsonValue;

/* =========================
   Estruturas
   ========================= */
typedef struct {
    char *key;
    JsonValue *value;
} JsonPair;

typedef struct {
    int count;
    int capacity;
    JsonPair *pairs;
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
        bool boolean;

        JsonArray *array;
        JsonObject *object;
    };
};

/* =========================
   Funções
   ========================= */
JsonValue* jsonCreateString(const char *str);
JsonValue* jsonCreateBoolean(bool val);
JsonObject* jsonCreateObject(int amount);

void jsonObjectSetKey(JsonObject *obj, int index, char *key);
void jsonObjectSetValue(JsonObject *obj, int index, JsonValue *value);

JsonValue* jsonCreateRoot(JsonObject *obj);


void jsonPrintObject(JsonObject *obj);
void jsonPrint(JsonValue *value) ;

#endif /* JSON_H */