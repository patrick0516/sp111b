#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMAX 100
#define MAX_TOKENS 1000

char tokens[MAX_TOKENS][SMAX];
int tokenIdx = 0;
int tokenTop = 0;

void tokenize(char *program) {
  char *token = strtok(program, " \t\n");
  while (token != NULL) {
    strcpy(tokens[tokenTop++], token);
    token = strtok(NULL, " \t\n");
  }
}

int E();
void STMT();
void IF();
void BLOCK();

int tempIdx = 0, labelIdx = 0;

#define nextTemp() (tempIdx++)
#define nextLabel() (labelIdx++)

int isNext(char *set) {
  char eset[SMAX], etoken[SMAX];
  sprintf(eset, " %s ", set);
  sprintf(etoken, " %s ", tokens[tokenIdx]);
  return (tokenIdx < tokenTop && strstr(eset, etoken) != NULL);
}

int isEnd() {
  return tokenIdx >= tokenTop;
}

char *next() {
  return tokens[tokenIdx++];
}

char *skip(char *set) {
  if (isNext(set)) {
    return next();
  } else {
    printf("skip(%s) got %s fail!\n", set, next());
    assert(0);
  }
}

int F() {
  int f;
  if (isNext("(")) { // '(' E ')'
    next(); // (
    f = E();
    next(); // )
  } else { // Number | Id
    f = nextTemp();
    char *item = next();
    if (isdigit(*item)) {
      printf("t%d = %s\n", f, item);
    } else {
      printf("t%d = %s\n", f, item);
    }
  }
  return f;
}

int E() {
  int i1 = F();
  while (isNext("+ - * / & | ! < > = <= >= == !=")) {
    char *op = next();
    int i2 = E();
    int i = nextTemp();
    printf("t%d = t%d %s t%d\n", i, i1, op, i2);
    i1 = i;
  }
  return i1;
}


void FOR() {
  skip("do");
  STMT();
  skip("while");
  skip("(");
  E();
  skip(")");
  skip(";");
}

void WHILE() {
  int whileBegin = nextLabel();
  int whileEnd = nextLabel();
  printf("(L%d)\n", whileBegin);
  skip("while");
  skip("(");
  E();
  printf("if not T%d goto L%d\n", whileEnd, whileEnd);
  skip(")");
  STMT();
  printf("goto L%d\n", whileBegin);
  printf("(L%d)\n", whileEnd);
}

void IF() {
  skip("if");
  skip("(");
  E();
  skip(")");
  STMT();
  if (isNext("else")) {
    skip("else");
    STMT();
  }
}

void STMT() {
  if (isNext("while")) {
    return WHILE();
  } else if (isNext("if")) {
    IF();
  } else if (isNext("{")) {
    BLOCK();
  } else if (isNext("do")) {
    FOR();
  } else {
    printf("Unknown statement\n");
  }
}

void STMTS() {
  while (!isEnd() && !isNext("}")) {
    STMT();
  }
}

void BLOCK() {
  skip("{");
  STMTS();
  skip("}");
}

void PROG() {
  STMTS();
}

void parse() {
  printf("============ parse =============\n");
  tokenIdx = 0;
  PROG();
}

int main() {
  char program[] = "do { printf(\"Hello\"); } while (i < 10);";
  tokenize(program);
  parse();
  return 0;
}
