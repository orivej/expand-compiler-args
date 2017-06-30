#include <ctype.h>
#include <stdio.h>

#include <string>
#include <vector>

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

vector<string> expandFile(FILE *f) {
    vector<string> args;
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
            args.push_back(arg);
            arg = "";
        }
        cur = next;
    }
    if (cur != outside) {
        args.push_back(arg);
    }
    return args;
}

void expandArgs(vector<string> args) {
    FILE *f;
    for (string arg : args) {
        if (arg[0] == '@' && (f = fopen(&arg[1], "r"))) {
            expandArgs(expandFile(f));
            fclose(f);
        } else {
            fwrite(arg.c_str(), 1, arg.length() + 1, stdout);
        }
    }
}

int main(int argc, char **argv) {
    expandArgs(vector<string>(argv + 1, argv + argc));
    return 0;
}
