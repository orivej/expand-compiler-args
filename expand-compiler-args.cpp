#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <string>

enum CharClass { space = 0, other = 1, backslash = 2, apostrophe = 3, quotation_mark = 4 };
enum State { outside = 0, unq = 1, unq_esc = 2, sq = 3, sq_esc = 4, dq = 5, dq_esc = 6 };

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

bool pushArg(State cur, State next) {
    return cur == unq && next == outside;
}

bool pushChar(State cur, State next) {
    return cur == unq_esc || cur == sq_esc || cur == dq_esc ||
            cur == outside ? next == unq : cur == next;
}

CharClass charClass(char c) {
    return c == '\\' ? backslash : c == '\'' ? apostrophe : c == '"' ? quotation_mark :
            isspace(c) ? space : other;
}

void expandArg(const char *arg);

void expandFile(FILE *f) {
    std::string arg;
    State cur = outside;
    for (int c = fgetc(f); c != EOF; c = fgetc(f)) {
        State next = State(transitions[cur][charClass(c)]);
        if (pushChar(cur, next)) {
            arg.push_back(c);
        }
        if (pushArg(cur, next)) {
            expandArg(arg.c_str());
            arg.clear();
        }
        cur = next;
    }
    if (cur != outside) {
        expandArg(arg.c_str());
    }
}

void expandArg(const char *arg) {
    FILE *f;
    if (arg[0] == '@' && (f = fopen(&arg[1], "r"))) {
        expandFile(f);
        fclose(f);
    } else {
        fwrite(arg, 1, strlen(arg) + 1, stdout);
    }
}

int main(int argc, char **argv) {
    while (*++argv) {
        expandArg(*argv);
    }
    return 0;
}
