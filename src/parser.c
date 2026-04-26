#include <stdio.h>
#include <stdlib.h>
#include "json.h"

JsonObject *objectOpen = NULL;

char *currentString = NULL;

int currentLength = 0;
int isStringActive = 0;
int hasEmptyKey = 0;

void cleanCurrentString() {
  free(currentString);
  currentString = malloc(1);
  currentString[0] = '\0';
  currentLength = 0;
}

JsonValue* parseObject(JsonValue *currentJson, const char character) {
  if (character == '{') {
    if (!currentJson->type) {
      currentJson->type = JSON_OBJECT;
    }
    return currentJson;
  }

  if (character == '}') {
    return currentJson;
  }

  return currentJson;
}

JsonValue* parseStringChar(JsonValue *currentJson, const char character) {
  if (isStringActive) {
    currentString = realloc(currentString, currentLength + 2);
    currentString[currentLength] = character;
    currentString[currentLength + 1] = '\0';
    currentLength++;
  }

  return currentJson;
}

JsonValue* setValueInCurrentJson(JsonValue *currentJson, JsonValue *value) {
  jsonObjectSetValue(currentJson->object, currentJson->object->count, value);
  currentJson->object->count += 1;
  hasEmptyKey = 0;
  cleanCurrentString();
  return currentJson;
}

JsonValue* setKeyInCurrentJson(JsonValue *currentJson) {
  jsonObjectSetKey(currentJson->object, currentJson->object->count, currentString);
  hasEmptyKey = 1;
  cleanCurrentString();
  return currentJson;
}

JsonValue* parseString(JsonValue *currentJson) {
  if (currentJson->type == JSON_NULL) {
    currentJson->type = JSON_STRING;
  }

  isStringActive = !isStringActive;

  if (!isStringActive) {
    switch (currentJson->type) {
      case JSON_STRING:
        currentJson->string = currentString;
        break;
      case JSON_OBJECT:
        if (hasEmptyKey) {
          setValueInCurrentJson(currentJson, jsonCreateString(currentString));
        }
        break;
      default:
        fprintf(stderr, "invalid JSON type for string value \n");
        break;
    }
  }
  return currentJson;
}

JsonValue* parseChar(JsonValue *currentJson, const char character) {
  if (character == ' ' || character == '\n' || character == '\t') return currentJson;

  switch (character) {
    case '{':
      return parseObject(currentJson, character);
    case '}':
      return parseObject(currentJson, character);
    case '"':
      return parseString(currentJson);
    case ':':
      return setKeyInCurrentJson(currentJson);
    case ',':
      return currentJson;
    default:
      return parseStringChar(currentJson, character);
  }
}

void initParser() {
  currentString = malloc(1);
  if (!currentString) {
    fprintf(stderr, "Memory allocation error\n");
    exit(1);
  }
  currentString[0] = '\0';
}

JsonValue* parse(const char *json, int begin, JsonValue *currentJson) {
  if (!currentJson) {
    currentJson = jsonCreateRoot(jsonCreateObject(0));
  }

  while (json[begin] != '\0') {
    currentJson = parseChar(currentJson, json[begin]);
    begin++;
  }

  return currentJson;
}