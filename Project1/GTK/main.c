#include <stdio.h>
#include "GUI/gtk.h"

int main(int argc, char **argv)
{
    if (!gtk_init_window(argc, argv))
        return 0;

    return 1;
}
