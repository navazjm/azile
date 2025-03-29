#ifndef AZILE_COMMON_H
#define AZILE_COMMON_H

#define az_return_defer(value)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        result = (value);                                                                                              \
        goto defer;                                                                                                    \
    } while (0)

#endif // AZILE_COMMON_H
