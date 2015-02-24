#include <stdio.h>
#include <usr/include/mach-o/loader.h>
#include <usr/include/mach/machine.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define EXENAME "macho_reader"
#define DEBUG 1 

enum arch_type
{
   ARCH64 = 64,
   ARCH386 = 386,
   ARCHUNKNOWN = 999
};

//function proto
void print_help();
void print_header(int fd);
void print_load_commands(int fd);
int  check_architecture(int fd);


