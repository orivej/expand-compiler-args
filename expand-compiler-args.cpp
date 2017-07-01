#include <ctype.h>
#include <stdio.h>

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

CharClass charClass(int c) {
    return c == '\\' ? backslash : c == '\'' ? apostrophe : c == '"' ? quotation_mark :
            isspace(c) ? space : other;
}

void expandArg(std::string &arg) {
    FILE *f;
    if (arg[0] != '@' || !(f = fopen(&arg[1], "r"))) {
        fwrite(arg.c_str(), 1, arg.length() + 1, stdout);
        return;
    }

    arg.clear();
    State cur = outside;
    int c;
    do {
        c = fgetc(f);
        State next = State(transitions[cur][charClass(c)]);
        if ((cur == unq && next == outside) || (cur != outside && c == EOF)) {
            expandArg(arg);
            arg.clear();
        } else if (cur == unq_esc || cur == sq_esc || cur == dq_esc ||
                   cur == outside ? next == unq : cur == next) {
            arg.push_back(c);
        }
        cur = next;
    } while (c != EOF);

    fclose(f);
}

int main(int argc, char **argv) {
    std::string arg;
    while (*++argv) {
        arg.assign(*argv);
        expandArg(arg);
    }
    return 0;
}
