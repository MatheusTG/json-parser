#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "json.h"

bool hasEmptyKey = false;

char *currentString = NULL;
int currentLength = 0;
bool isStringActive = false;


char *nonStringChars = NULL;
int nonStringCharsLength = 0;


void cleanCurrentString() {
  free(currentString);
  currentString = malloc(1);
  currentString[0] = '\0';
  currentLength = 0;
}

void cleanNonStringChars() {
  free(nonStringChars);
  nonStringChars = NULL;
  nonStringCharsLength = 0;
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

JsonValue* setValueInCurrentJson(JsonValue *currentJson, JsonValue *value) {
  jsonObjectSetValue(currentJson->object, currentJson->object->count, value);
  currentJson->object->count += 1;
  hasEmptyKey = false;
  cleanCurrentString();
  return currentJson;
}

JsonValue* testAndSetNonStringValue(JsonValue *currentJson) {
    if (strcmp(nonStringChars, "true") == 0) {
      setValueInCurrentJson(currentJson, jsonCreateBoolean(true));
    } else if (strcmp(nonStringChars, "false") == 0) {
      setValueInCurrentJson(currentJson, jsonCreateBoolean(false));
    } else{
      fprintf(stderr, "Invalid JSON syntax error\n");
    }
    cleanNonStringChars();
    return currentJson;
}

JsonValue* parseBoolean(JsonValue *currentJson, const char character) {
  nonStringChars = realloc(nonStringChars, nonStringCharsLength + 2);
  nonStringChars[nonStringCharsLength] = character;
  nonStringChars[nonStringCharsLength + 1] = '\0';
  nonStringCharsLength++;   

  return currentJson;
}

JsonValue* parseStringChar(JsonValue *currentJson, const char character) {
  currentString = realloc(currentString, currentLength + 2);
  currentString[currentLength] = character;
  currentString[currentLength + 1] = '\0';
  currentLength++;

  return currentJson;
}

JsonValue* setKeyInCurrentJson(JsonValue *currentJson) {
  jsonObjectSetKey(currentJson->object, currentJson->object->count, currentString);
  hasEmptyKey = true;
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
    }
  }
  return currentJson;
}

JsonValue* parseChar(JsonValue *currentJson, const char character) {
  if (!isStringActive) {
    if (character == ' ' || character == '\n' || character == '\t') return currentJson;
    
    switch (character) {
      case '{':
        return parseObject(currentJson, character);
      case '}':
        if (nonStringChars) testAndSetNonStringValue(currentJson); 
        return parseObject(currentJson, character);
      case '"':
        return parseString(currentJson);
      case ':':
        return setKeyInCurrentJson(currentJson);
      case ',':
        if (nonStringChars) testAndSetNonStringValue(currentJson); 
        return currentJson;
      default:
        if (hasEmptyKey) {
          parseBoolean(currentJson, character);
          return currentJson;
        }
        fprintf(stderr, "Invalid JSON syntax error\n");
    }
  } else {
    if (character == '"') {
      return parseString(currentJson);
    }
    return parseStringChar(currentJson, character);
  }

  return currentJson;
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