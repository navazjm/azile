#include "strings.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define AZ_SB_INIT_CAP 256

// Append a NULL-terminated string to a string builder
void az_sb_append_cstr(AZ_String_Builder *sb, const char *cstr) {
    size_t n = strlen(cstr);
    if (sb->count + n > sb->capacity) {
        if (sb->capacity == 0) {
            sb->capacity = AZ_SB_INIT_CAP;
        }
        while (sb->count + n > sb->capacity) {
            sb->capacity *= 2;
        }
        sb->items = realloc(sb->items, sb->capacity * sizeof(sb->items));
        assert(sb->items != NULL && "Buy more RAM lol");
    }
    memcpy(sb->items + sb->count, cstr, n * sizeof(sb->items));
    sb->count += n;
}

// Free the memory allocated by a string builder
void az_sb_free(AZ_String_Builder *sb) {
    if (sb->items)
        free(sb->items);
}
