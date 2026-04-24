#ifndef PARSER_H
#define PARSER_H

#include "json.h"

/* =========================
   Variáveis globais
   ========================= */
extern JsonObject *objectOpen;

extern char *currentString;

extern int currentLength;
extern int isStringActive;
extern int hasEmptyKey;

/* =========================
   Inicialização
   ========================= */
void initParser();
void cleanCurrentString();

/* =========================
   Funções de parsing
   ========================= */
JsonValue* parseObject(JsonValue *currentJson, const char character);
JsonValue* parseStringChar(JsonValue *currentJson, const char character);
JsonValue* parseString(JsonValue *currentJson);
JsonValue* parseChar(JsonValue *currentJson, const char character);
JsonValue* parse(const char *json, int begin, JsonValue *currentJson);

#endif /* PARSER_H */