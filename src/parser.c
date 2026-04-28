#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "json.h"

bool hasEmptyKey = false;

char *currentString = NULL;
int currentLength = 0;
bool isStringActive = false;

char *nonStringChars = NULL;
int nonStringCharsLength = 0;

char *expectedNonStringChars = NULL;

bool isNonStringCharsADigit = false;

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
  expectedNonStringChars = NULL;
  isNonStringCharsADigit = false;
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
  if (nonStringChars) cleanNonStringChars();
  if (currentString) cleanCurrentString();
  return currentJson;
}

JsonValue* testAndSetNonStringValue(JsonValue *currentJson) {
  if (strcmp(nonStringChars, "true") == 0) {
      setValueInCurrentJson(currentJson, jsonCreateBoolean(true));
  } else if (strcmp(nonStringChars, "false") == 0) {
      setValueInCurrentJson(currentJson, jsonCreateBoolean(false));
  } else {
    fprintf(stderr, "Invalid JSON syntax error\n");
    return NULL;
  }

  return currentJson;
}

JsonValue* parseNonStringValue(JsonValue *currentJson, const char character) {
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
    nonStringChars = NULL;
    return NULL;
  }
  nonStringChars = tmp;

  if (!expectedNonStringChars) {
    if (character == 't') {
      expectedNonStringChars = "true";
    }
    else if (character == 'f') {
      expectedNonStringChars = "false";
    }
    else if (isdigit(character)) {
      isNonStringCharsADigit = true;
    }
    else {
      fprintf(stderr, "Invalid JSON syntax error\n");
      return NULL;
    }
  }

  if (character != expectedNonStringChars[nonStringCharsLength]) {
    fprintf(stderr, "Invalid JSON syntax error\n");
    return NULL;
  }

  if (isNonStringCharsADigit) {
    if (!isdigit(character)) {
      fprintf(stderr, "Invalid JSON syntax error\n");
      return NULL;
    }
  }

  nonStringChars[nonStringCharsLength] = character;
  nonStringChars[nonStringCharsLength + 1] = '\0';
  nonStringCharsLength++;

  JsonValue *result = currentJson;
  if (expectedNonStringChars && strcmp(expectedNonStringChars, "true") == 0 && nonStringCharsLength == 4) {
    result = testAndSetNonStringValue(currentJson); 
  } else if (expectedNonStringChars && strcmp(expectedNonStringChars, "false") == 0 && nonStringCharsLength == 5) {
    result = testAndSetNonStringValue(currentJson); 
  }

  return result;
}

JsonValue* parseStringChar(JsonValue *currentJson, const char character) {
  if (!currentString) {
    currentString = malloc(1);
    if (!currentString) {
      fprintf(stderr, "Invalid JSON syntax error\n");
      return NULL;
    }
    currentString[0] = '\0';
  }
  
  char *tmp = realloc(currentString, currentLength + 2);
  if (!tmp) {
    fprintf(stderr, "Invalid JSON syntax error\n");
    free(currentString);
    currentString = NULL;
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
        if (nonStringChars) {
          fprintf(stderr, "Invalid JSON syntax error\n");
          return NULL;
        };
        return parseObject(currentJson, character);
      case '"':
        return parseString(currentJson);
      case ':':
        return setKeyInCurrentJson(currentJson);
      case ',':
        if (nonStringChars) {
          fprintf(stderr, "Invalid JSON syntax error\n");
          return NULL;
        };
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