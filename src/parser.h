#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "json.h"

/* =========================
   Global variables
   ========================= */
extern JsonObject *objectOpen;

extern char *currentString;
extern int currentLength;
extern bool isStringActive;
extern bool hasEmptyKey;

extern char *nonStringChars;
extern int nonStringCharsLength;

/* =========================
   Utility functions
   ========================= */
void cleanCurrentString();
void cleanNonStringChars();

/* =========================
   Parsing helpers
   ========================= */
JsonValue* setValueInCurrentJson(JsonValue *currentJson, JsonValue *value);
JsonValue* setKeyInCurrentJson(JsonValue *currentJson);
JsonValue* testAndSetNonStringValue(JsonValue *currentJson);

/* =========================
   Parsing functions
   ========================= */
JsonValue* parseObject(JsonValue *currentJson, const char character);
JsonValue* parseStringChar(JsonValue *currentJson, const char character);
JsonValue* parseString(JsonValue *currentJson);
JsonValue* parseNonStringValue(JsonValue *currentJson, const char character);
JsonValue* parseChar(JsonValue *currentJson, const char character);
JsonValue* parse(const char *json, int begin, JsonValue *currentJson);

#endif /* PARSER_H */