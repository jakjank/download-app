#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int parse_arguments(char *url, args *args){   
    // 99 like MAX_LENGTH
    return sscanf(url, "ftp://%99[^:]:%99[^@]@%99[^/]/%99[^\n]",
                  args->user, args->password, args->host, args->url_path);
}