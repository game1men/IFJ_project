#include "parser.h"
#include "generator.h"

int main() {
    AST* tree = PROLOG();
    startGenerate(tree);
    return 0;
}