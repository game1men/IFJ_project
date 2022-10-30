#include "scanner.h"

int main() {

    if(detectProlog() !=1){
        return 1;
    }

    while (true) {
        T_token* tt = getToken();
        if (tt->type == NOT_TOKEN) {
            return 1;
        } else if (tt->type == TOKEN_EOF) {
            break;
        }
    }
    return 0;
}