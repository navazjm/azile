#ifndef AZILE_STRINGS_H
#define AZILE_STRINGS_H

#include <stddef.h>

#define AZ_SB_INIT_CAP 256

typedef struct {
    char *items;
    size_t count;
    size_t capacity;
} AZ_String_Builder;

void az_sb_append_cstr(AZ_String_Builder *sb, const char *cstr);
void az_sb_free(AZ_String_Builder *sb);

#endif // AZILE_STRINGS_H
