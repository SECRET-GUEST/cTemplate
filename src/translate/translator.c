#include <json-c/json.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "translator.h"

#define TRANSLATION_PATH "src/translate/locale/"

const char* translate(const char* key) {
    char* lang_env = getenv("LANG"); // Retrieve system's locale.
    char lang[3];
    if (lang_env != NULL && strlen(lang_env) >= 2) {
        // Extracts the language code from LANG variable.
        strncpy(lang, lang_env, 2);
        lang[2] = '\0';
    } else {
        strcpy(lang, "en");  // Default to English if LANG is not set.
    }

    char filepath[100];
    // Constructs the filepath to the JSON language file based on the language code.
    snprintf(filepath, sizeof(filepath), TRANSLATION_PATH "%s.json", lang);

    FILE *fp = fopen(filepath, "r"); // Try opening the corresponding language file.
    if (fp == NULL) {
        if (strcmp(lang, "en") != 0) {
            // If the file can’t be opened and it's not already English, try opening the English file.
            snprintf(filepath, sizeof(filepath), TRANSLATION_PATH "en.json");
            fp = fopen(filepath, "r");
            if (fp == NULL) return key; // If English file also can't be opened, return the key.
        } else {
            // If it's English and still can't be opened, return the key.
            return key;
        }
    }

    // Finding the length of the file content.
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Reading the file content into a string.
    char* content = malloc(length + 1); // +1 for null-terminator
    if (content == NULL) {
        fclose(fp);
        return key; // Memory allocation failed, return key.
    }

    fread(content, 1, length, fp);
    content[length] = '\0'; // Ensure null-terminated string
    fclose(fp);

    // Parsing the JSON string.
    struct json_object *parsed_json = json_tokener_parse(content);
    free(content);  // Free the allocated memory for file content.

    if (parsed_json == NULL) {
        return key; // JSON parsing failed, return key.
    }

    char* section = strtok(strdup(key), "."); // "gui"
    char* txt_key = strtok(NULL, "."); // "txt1"

    struct json_object *section_obj;
    json_object_object_get_ex(parsed_json, section, &section_obj); // Retrieves the section object from the JSON

    if (section_obj == NULL) {
        json_object_put(parsed_json); // Free JSON object
        return key; // Section not found, return key.
    }

    struct json_object *text;
    json_object_object_get_ex(section_obj, txt_key, &text);  // Retrieves the translation for the provided key.

    if (text == NULL) {
        json_object_put(parsed_json); // Free JSON object
        return key; // Text key not found, return key.
    }

    const char* result = json_object_get_string(text);  // Getting the string from JSON object.
    json_object_put(parsed_json); // Free JSON object

    return result;
}
