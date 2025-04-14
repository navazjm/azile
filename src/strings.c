#include "strings.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void az_sb_append(AZ_String_Builder *sb, const char *cstr)
{
    size_t n = strlen(cstr);
    if (sb->count + n > sb->capacity)
    {
        if (sb->capacity == 0)
            sb->capacity = AZ_SB_INIT_CAP;

        while (sb->count + n + 1 > sb->capacity)
        {
            sb->capacity *= 2;
        }
        sb->items = realloc(sb->items, sb->capacity);
        assert(sb->items != NULL && "Buy more RAM lol");
    }
    memcpy(sb->items + sb->count, cstr, n);
    sb->count += n;
    sb->items[sb->count] = '\0';
}

void az_sb_free(AZ_String_Builder *sb)
{
    if (sb->items)
        free(sb->items);
    sb->items = NULL;
    sb->count = 0;
    sb->capacity = 0;
}
