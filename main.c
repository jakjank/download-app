#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("usage: %s %s \n", argv[0], "ftp://[<user>:<password>@]<host>/<url-path>");
        exit(-1);
    }

    args args;
    if(parse_arguments(argv[1], &args) != 4){
        printf("unable to parse 4 arguments \n");
        exit(-1);
    }

    printf("%s\n%s\n%s\n", args.user, args.password, args.host);

    return 0;
}