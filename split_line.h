/*
 * split_line taken from:
 * https://brennan.io/2015/01/16/write-a-shell-in-c/
 */

#ifndef KAPISH_SPLIT_LINE_H
#define KAPISH_SPLIT_LINE_H

char** split_line(char* line) {
    int pos = 0;
    int max_size = 64;
    char** tokens = malloc(max_size * sizeof(char*));
    char* token;

    if(!tokens) {
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while(token != NULL) {
        tokens[pos] = token;
        pos++;
        if(pos >= max_size) {
            max_size = max_size + max_size;
            tokens = realloc(tokens, max_size * sizeof(char*));
            if(!tokens){
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n\a");
    }

    tokens[pos] = NULL;
    return tokens;
}

#endif //KAPISH_SPLIT_LINE_H