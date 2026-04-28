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
    if (nonStringChars && strcmp(nonStringChars, "true") == 0) {
      setValueInCurrentJson(currentJson, jsonCreateBoolean(true));
    } else if (nonStringChars && strcmp(nonStringChars, "false") == 0) {
      setValueInCurrentJson(currentJson, jsonCreateBoolean(false));
    } else{
      fprintf(stderr, "Invalid JSON syntax error\n");
      cleanNonStringChars();
      return NULL;
    }
    cleanNonStringChars();
    return currentJson;
}

JsonValue* parseNonStringValue(JsonValue *currentJson, const char character) {
  if (nonStringCharsLength + 1 >= 6) {
    fprintf(stderr, "Invalid JSON syntax error\n");
    return NULL;
  }

  if (!nonStringChars) {
    nonStringChars = malloc(1);
    if (!nonStringChars) {
      fprintf(stderr, "Memory allocation error\n");
      return NULL;
    }
    nonStringChars[0] = '\0';
  }

  char *tmp = realloc(nonStringChars, nonStringCharsLength + 2);
  if (!tmp) {
    fprintf(stderr, "Memory allocation failed\n");
    free(nonStringChars);
    return NULL;
  }
  nonStringChars = tmp;


  nonStringChars[nonStringCharsLength] = character;
  nonStringChars[nonStringCharsLength + 1] = '\0';
  nonStringCharsLength++;   

  return currentJson;
}

JsonValue* parseStringChar(JsonValue *currentJson, const char character) {
  if (!currentString) {
    currentString = malloc(1);
    if (!currentString) {
      fprintf(stderr, "Memory allocation error\n");
      return NULL;
    }
    currentString[0] = '\0';
  }
  
  char *tmp = realloc(currentString, currentLength + 2);
  if (!tmp) {
    fprintf(stderr, "Memory allocation failed\n");
    free(currentString);
    return NULL;
  }
  currentString = tmp;
  
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
  isStringActive = !isStringActive;

  if (!isStringActive) {
    switch (currentJson->type) {
      case JSON_STRING:
        currentJson->string = strdup(currentString);
        break;
      case JSON_OBJECT:
        if (hasEmptyKey) {
          setValueInCurrentJson(currentJson, jsonCreateString(currentString));
        }
        break;
      default:
        fprintf(stderr, "invalid JSON type for string value \n");
        return NULL;
    }
  }
  return currentJson;
}

JsonValue* parseChar(JsonValue *currentJson, const char character) {
  if (currentJson->type == JSON_NULL) {
    if (character == '{') {
      currentJson->type = JSON_OBJECT;
    } else if (character == '"') {
      currentJson->type = JSON_STRING;
    } else if (character == 't' || character == 'f') {
      currentJson->type = JSON_BOOLEAN;
    }
  }

  if (!isStringActive) {
    if (character == ' ' || character == '\n' || character == '\t') return currentJson;
    
    switch (character) {
      case '{':
        return parseObject(currentJson, character);
      case '}':
        if (nonStringCharsLength > 0) return testAndSetNonStringValue(currentJson);
        return parseObject(currentJson, character);
      case '"':
        return parseString(currentJson);
      case ':':
        return setKeyInCurrentJson(currentJson);
      case ',':
        if (nonStringCharsLength > 0) return testAndSetNonStringValue(currentJson);
        return currentJson;
      default:
        if (hasEmptyKey) {
          return parseNonStringValue(currentJson, character);
        }
        fprintf(stderr, "Invalid JSON syntax error\n");
        return NULL;
    }
  } else {
    if (character == '"') {
      return parseString(currentJson);
    }
    return parseStringChar(currentJson, character);
  }

  return currentJson;
}

JsonValue* parse(const char *json, int begin, JsonValue *currentJson) {
  if (!currentJson) {
    currentJson = jsonCreateRoot(jsonCreateObject(0));
  }

  while (json[begin] != '\0') {
    currentJson = parseChar(currentJson, json[begin]);
    begin++;
    if (!currentJson) break;
  }

  return currentJson;
}