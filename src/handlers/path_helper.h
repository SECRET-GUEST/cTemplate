#ifndef PATH_HELPER_H
#define PATH_HELPER_H

char* resource_path(const char* relative_path);
void use_resource_path(const char* relative_path, void (*use_path)(const char*));

#endif // PATH_HELPER_H
