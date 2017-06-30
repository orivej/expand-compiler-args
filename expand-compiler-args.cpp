#include <ctype.h>
#include <stdio.h>

#include <string>

using namespace std;

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

void expandArg(string arg);

void expandFile(FILE *f) {
    string arg;
    State cur = outside;
    for (int c = fgetc(f); c != EOF; c = fgetc(f)) {
        int cls = c == '\\' ? backslash :
                  c == '\'' ? apostrophe :
                  c == '"' ? quotation_mark :
                  isspace(c) ? space : other;
        State next = State(transitions[cur][cls]);
        if (pushChar(cur, next)) {
            arg.push_back(c);
        }
        if (pushArg(cur, next)) {
            expandArg(arg);
            arg = "";
        }
        cur = next;
    }
    if (cur != outside) {
        expandArg(arg);
    }
}

void expandArg(string arg) {
    FILE *f;
    if (arg[0] == '@' && (f = fopen(&arg[1], "r"))) {
        expandFile(f);
        fclose(f);
    } else {
        fwrite(arg.c_str(), 1, arg.length() + 1, stdout);
    }
}

int main(int argc, char **argv) {
    while (*++argv) {
        expandArg(*argv);
    }
    return 0;
}
