#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char *data; size_t len, cap; } String;

void resize(String *s, size_t len) {
    s->len = len;
    if (s->cap < s->len) {
        s->cap = s->len * 2;
        s->data = (char *)realloc(s->data, s->cap);
        assert(s->data);
    }
}

void append(String *s, char c) {
    resize(s, s->len + 1);
    s->data[s->len - 1] = c;
}

void assign(String *s, void *cs, size_t len) {
    resize(s, len);
    memcpy(s->data, cs, len);
}

typedef enum { space = 0, other = 1, backslash = 2, apostrophe = 3, quotation_mark = 4 } CharClass;
typedef enum { outside = 0, unq = 1, unq_esc = 2, sq = 3, sq_esc = 4, dq = 5, dq_esc = 6 } State;

// current State -> CharClass -> next State
const char *transitions[] = {
    "\0\1\2\3\5",
    "\0\1\2\3\5",
    "\1\1\1\1\1",
    "\3\3\4\1\3",
    "\3\3\3\3\3",
    "\5\5\6\5\1",
    "\5\5\5\5\5",
};

CharClass charClass(int c) {
    return c == '\\' ? backslash : c == '\'' ? apostrophe : c == '"' ? quotation_mark :
            isspace(c) ? space : other;
}

void expandArg(String *arg) {
    FILE *f;
    if (arg->data[0] != '@' || !(f = fopen(&arg->data[1], "r"))) {
        fwrite(arg->data, 1, arg->len, stdout);
        return;
    }

    resize(arg, 0);
    State cur = outside;
    int c;
    do {
        c = fgetc(f);
        State next = transitions[cur][charClass(c)];
        if ((cur == unq && next == outside) || (cur != outside && c == EOF)) {
            append(arg, '\0');
            expandArg(arg);
            resize(arg, 0);
        } else if (cur == unq_esc || cur == sq_esc || cur == dq_esc ||
                   cur == outside ? next == unq : cur == next) {
            append(arg, c);
        }
        cur = next;
    } while (c != EOF);

    fclose(f);
}

int main(int argc, char **argv) {
    String arg = { 0 };
    while (*++argv) {
        assign(&arg, *argv, strlen(*argv) + 1);
        expandArg(&arg);
    }
    free(arg.data);
    return 0;
}
