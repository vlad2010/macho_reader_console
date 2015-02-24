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

int check_architecture(int fd);
void print_load_commands(int fd);

void print_help()
{
     printf ("Usage: %s executable file\n", EXENAME); 

}

void print_header(int fd)
{
   struct mach_header mh32;
   //struct mach_header_64 mh64;
    
   int arch = check_architecture(fd);
   lseek(fd,0,SEEK_SET);
   if((arch==ARCH386) || (arch=ARCH64))
   {
   	#ifdef DEBUG
   	   printf("32 bit arch\n");
   	#endif

   	  read(fd,&mh32, sizeof(mh32));

    printf("\nMach-o executable file header\n");
    printf("-------------------------------------------------\n");

    printf("Cpu type                    : ");
    switch(mh32.cputype)
    {
      case (CPU_TYPE_POWERPC):   printf("Power PC 32 Bit\n"); break;
      case (CPU_TYPE_POWERPC64): printf("Power PC 64 Bit\n"); break;
      case (CPU_TYPE_I386):      printf("x86 32 Bit\n"); break;
      case (CPU_TYPE_X86_64):    printf("x86 64 Bit\n"); break;
    }

    printf("Cpu subtype                 : 0x%04x\n", mh32.cpusubtype);
    printf("File type                   : ");
    switch(mh32.filetype)
    {
      case (MH_OBJECT):   printf ("Object file\n"); break;
      case (MH_EXECUTE):  printf ("Executable file\n"); break;
      case (MH_BUNDLE):   printf ("Bundle or plugin file\n"); break;
      case (MH_DYLIB):    printf ("Shared library file\n"); break;
      case (MH_PRELOAD):  printf ("Firmware executable file\n"); break;
      case (MH_CORE):     printf ("Crash dump file\n"); break;
      case (MH_DYLINKER): printf ("Dynamic linker shared library file\n"); break;
      case (MH_DSYM):     printf ("Symbolic information file\n"); break;
      case (MH_KEXT_BUNDLE):     printf ("x86_64 kexts file\n"); break;

    }
     
    printf("Number of load commands     : %d\n",mh32.ncmds);
    printf("Size of load commands       : %d\n",mh32.sizeofcmds);

    printf("Flags                       : 0x%04x\n",mh32.flags);

     
    if(mh32.flags & MH_NOUNDEFS)   printf ("0x%08x - MH_NOUNDEF: No undefined references.\n", MH_NOUNDEFS);
    if(mh32.flags & MH_INCRLINK)   printf ("0x%08x - MH_INCRLINK: It is incremental link against base file. Cannot' be linked again\n", MH_INCRLINK);
    if(mh32.flags & MH_DYLDLINK)   printf ("0x%08x - MH_DYLDLINK: The file is input for the dynamic linker and cannot be statically linked again.\n", MH_DYLDLINK);
    if(mh32.flags & MH_TWOLEVEL)   printf ("0x%08x - MH_TWOLEVEL: The image is using two-level namespace bindings.\n", MH_TWOLEVEL);
    if(mh32.flags & MH_BINDATLOAD) printf ("0x%08x - MH_BINDATLOAD: Bind undefined references at load.\n", MH_BINDATLOAD);
    if(mh32.flags & MH_PREBOUND)   printf ("0x%08x - MH_PREBOUND: The file’s undefined references are prebound.\n", MH_PREBOUND);
    if(mh32.flags & MH_PREBINDABLE)     printf ("0x%08x - MH_PREBINDABLE: This file is not prebound.\n", MH_PREBINDABLE);
    if(mh32.flags & MH_NOFIXPREBINDING) printf ("0x%08x - MH_NOFIXPREBINDING: The dynamic linker doesn’t notify the prebinding agent about this executable. \n",MH_NOFIXPREBINDING );
    if(mh32.flags & MH_ALLMODSBOUND)   printf ("0x%08x - MH_ALLMODSBOUND: This binary binds to all two-level namespace modules of its dependent libraries.\n",MH_ALLMODSBOUND );
    if(mh32.flags & MH_CANONICAL)   printf ("0x%08x - MH_CANONICAL: This file has been canonicalized by unprebinding—clearing prebinding information from the file.\n",MH_CANONICAL );
    if(mh32.flags & MH_SPLIT_SEGS)   printf ("0x%08x - MH_SPLIT_SEGS: The file has its read-only and read-write segments split.\n", MH_SPLIT_SEGS );
    if(mh32.flags & MH_FORCE_FLAT)   printf ("0x%08x - MH_FORCE_FLAT: The executable is forcing all images to use flat namespace bindings. \n",MH_FORCE_FLAT );
    if(mh32.flags & MH_SUBSECTIONS_VIA_SYMBOLS)   printf ("0x%08x - MH_SUBSECTIONS_VIA_SYMBOLS: The sections of the object file can be divided into individual blocks.\n", MH_SUBSECTIONS_VIA_SYMBOLS );
    if(mh32.flags & MH_NOMULTIDEFS)   printf ("0x%08x - MH_NOMULTIDEFS: This umbrella guarantees there are no multiple definitions of symbols in its subimages.\n", MH_NOMULTIDEFS);

    if(mh32.flags & MH_PIE)   printf ("0x%08x - MH_PIE: The OS will load the main executable at a random address.\n", MH_PIE);
    if(mh32.flags & MH_ROOT_SAFE)   printf ("0x%08x - MH_ROOT_SAFE: Binary is safe to use by root.\n", MH_ROOT_SAFE);

    if(mh32.flags & MH_ALLOW_STACK_EXECUTION)   printf ("0x%08x - MH_ALLOW_STACK_EXECUTION: Permission to execute stack.\n", MH_ALLOW_STACK_EXECUTION);
    if(mh32.flags & MH_NO_HEAP_EXECUTION)   printf ("0x%08x - MH_NO_HEAP_EXECUTION: No heap execution\n", MH_NO_HEAP_EXECUTION);

   }
   else
   {
   	 printf("Unknown architecture. Can't print header.\n");
   }
}

void print_load_commands(int fd)
{
   struct mach_header mh32;
   struct load_command lc;

   //struct mach_header_64 mh64;
    
   lseek(fd,0,SEEK_SET);
   read(fd,&mh32, sizeof(mh32));

    printf("Number of load commands     : %d\n",mh32.ncmds);
    printf("Size of load commands       : %d\n",mh32.sizeofcmds);

     for (int i=0;i<mh32.ncmds;i++)
     {
         read(fd,&lc, sizeof(lc));
         //printf ("\n%02d) Command type: 0x%08x Size:%d \n", i, lc.cmd, lc.cmdsize);

         printf("%02d) 0x%08x Size:%04d ",i+1, lc.cmd, lc.cmdsize);

         switch(lc.cmd) 
         {
             //case(): printf(": \n"); break;

             case(LC_SEGMENT): printf("LC_SEGMENT: Segment of this file to be mapped."); break;
             case(LC_SYMTAB):  printf("LC_SYMTAB: Link-edit stab symbol table info."); break;
             case(LC_SYMSEG):  printf("LC_SYMSEG: Link-edit gdb symbol table info (obsolete)."); break;

             case(LC_THREAD):      printf("LC_THREAD: Thread\n"); break;
             case(LC_UNIXTHREAD):  printf("LC_UNIXTHREAD: Unix thread (includes a stack)."); break;

             case(LC_LOADFVMLIB):  printf("LC_LOADFVMLIB: Load a specified fixed VM shared library."); break;
             case(LC_IDFVMLIB):    printf("LC_IDFVMLIB: Fixed VM shared library identification."); break;

             case(LC_IDENT):      printf("LC_IDENT: Object identification info (obsolete)."); break;
             case(LC_FVMFILE):    printf("LC_FVMFILE: Fixed VM file inclusion (internal use)."); break;

             case(LC_PREPAGE):    printf("LC_PREPAGE: Prepage command (internal use)."); break;
             case(LC_DYSYMTAB):   printf("LC_DYSYMTAB: Dynamic link-edit symbol table info."); break;

             case(LC_LOAD_DYLIB):      printf("LC_LOAD_DYLIB: Load a dynamically linked shared library."); break;
             case(LC_ID_DYLIB):        printf("LC_ID_DYLIB: Dynamically linked shared lib ident."); break;
             case(LC_LOAD_DYLINKER):   printf("LC_LOAD_DYLINKER: Load a dynamic linker."); break;

             case(LC_ID_DYLINKER):    printf("LC_ID_DYLINKER: Dynamic linker identification."); break;
             case(LC_PREBOUND_DYLIB): printf("LC_PREBOUND_DYLIB: Modules prebound for a dynamically."); break;
             case(LC_ROUTINES):       printf("LC_ROUTINES: Image routines."); break;

             case(LC_SUB_FRAMEWORK):    printf("LC_SUB_FRAMEWORK: Sub framework."); break;
             case(LC_SUB_UMBRELLA):     printf("LC_SUB_UMBRELLA: Sub framework."); break;
             case(LC_SUB_CLIENT):       printf("LC_SUB_CLIENT: Sub framework."); break;

             case(LC_SUB_LIBRARY):        printf("LC_SUB_LIBRARY: Sub library."); break;
             case(LC_TWOLEVEL_HINTS):     printf("LC_TWOLEVEL_HINTS: Two-level namespace lookup hints."); break;
             case(LC_PREBIND_CKSUM):      printf("LC_PREBIND_CKSUM: Prebind checksum."); break;

             case(LC_LOAD_WEAK_DYLIB):printf("LC_LOAD_WEAK_DYLIB: Weak load?"); break;
             case(LC_SEGMENT_64):     printf("LC_SEGMENT_64: 64-bit segment of this file to be mapped."); break;
             case(LC_ROUTINES_64):    printf("LC_ROUTINES_64: 64-bit image routines."); break;

             case(LC_UUID):            printf("LC_UUID: The UUID."); break;
             case(LC_RPATH):           printf("LC_RPATH: Runpath additions."); break;
             case(LC_CODE_SIGNATURE):  printf("LC_CODE_SIGNATURE: Local of code signature."); break;

             case(LC_SEGMENT_SPLIT_INFO): printf("LC_SEGMENT_SPLIT_INFO: Local of info to split segments."); break;
             case(LC_REEXPORT_DYLIB):     printf("LC_REEXPORT_DYLIB: Load and re-export dylib."); break;
             case(LC_LAZY_LOAD_DYLIB):    printf("LC_LAZY_LOAD_DYLIB: Delay load of dylib until first use."); break;

             case(LC_ENCRYPTION_INFO): printf("LC_ENCRYPTION_INFO: Encrypted segment information."); break;
             case(LC_DYLD_INFO):     printf("LC_DYLD_INFO: Compressed dyld information."); break;
             case(LC_DYLD_INFO_ONLY):    printf("LC_DYLD_INFO_ONLY: Compressed dyld information only."); break;

         }

         printf("\n");

         lseek(fd, lc.cmdsize-sizeof(lc), SEEK_CUR);
     }

      return;

}

int check_architecture(int fd)
{
int magic;


lseek(fd,0,SEEK_SET);
read(fd,&magic,sizeof(magic));
#ifdef DEBUG
    printf("Arch magic is: %x\n", magic);
#endif


    if(magic==MH_MAGIC)
    	return ARCH386;

    if(magic==MH_MAGIC_64)
    	return ARCH64;


return ARCHUNKNOWN;
}



int main (int argc, char *argv[])
{
    if(argc<2)
    {
    	print_help();
    	return 0;
    }

#ifdef DEBUG
    printf("File to analyse: %s\n", argv[1]);
#endif

    int fd = open(argv[1],O_RDONLY);
#ifdef DEBUG
    printf("File desc: %d\n", fd);
#endif

    if(fd==-1)
    {
        printf("Can't open file:%s\n", argv[1]); 
    	return 0;
    }
    print_header(fd); 
    print_load_commands(fd);

    close(fd);

	return 0;
}