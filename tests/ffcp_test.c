#include <stdio.h>
#include <string.h>

typedef struct huffman_decode_node
{
    unsigned int fstate;
    unsigned int flags;
    int sym;
}huffman_decode_node;

#include "huffman_table.h"

int
qpack_huffman_decode(char* dest, const char* src, size_t srclen, int fin) {
    char* p = dest;
    const char* end = src + srclen;
    huffman_decode_node node = { 0, 0, 0 };
    const huffman_decode_node* t = &node;
    unsigned char c;

    for (; src != end;) {
        c = *src++;
        t = &huffman_decode_table[t->fstate & 0x1ff][c >> 4];
        if (t->flags) {
            *p++ = t->sym;
        }

        t = &huffman_decode_table[t->fstate & 0x1ff][c & 0xf];
        if (t->flags) {
            *p++ = t->sym;
        }
    }

    return p - dest;
}

struct Test
{
    char* source;
    int srcLen;
    char* encoded;
    int encLen;
} tests[] = {
    { "111111111111", 12, "\x08\x42\x10\x84\x21\x08\x42\x1f", 8 }
};

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif // !_countof

int main(int argc, char** argv)
{
    char dest[1000];
    for (int i = 0; i < _countof(tests); i++)
    {
        int len = qpack_huffman_decode(dest, tests[i].encoded, tests[i].encLen, 1);
        if (len != tests[i].srcLen
            || memcmp(dest, tests[i].source, tests[i].srcLen) != 0) {
            return 1;
        }
    }
    return 0;
}
