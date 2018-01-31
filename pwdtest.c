#include "types.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    char path[256];
    pwd(path, 256);
    printf(0, "%s\n", path);
    exit();
}