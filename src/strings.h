#ifndef AZILE_STRINGS_H
#define AZILE_STRINGS_H

#include <stddef.h>

#define AZ_SB_INIT_CAP 256

typedef struct
{
    char *items;
    size_t count;
    size_t capacity;
} AZ_String_Builder;

/*
 * Append a NULL-terminated string to a string builder
 */
void az_sb_append(AZ_String_Builder *sb, const char *cstr);

/*
 * Free the memory allocated by a string builder
 */
void az_sb_free(AZ_String_Builder *sb);

#endif // AZILE_STRINGS_H
