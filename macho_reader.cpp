#include "macho_reader.h"

void print_help()
{
     printf ("Usage: %s executable file\n", EXENAME); 
}

void print_header(int fd)
{
    struct mach_header_64 mh;
   
    int arch = check_architecture(fd);
    if( arch != ARCH64 )
    {
        printf("Architecture is not arch64. Can't print header.\n");
        return;
    } 

   	#ifdef DEBUG
   	   printf("64 bit arch\n");
   	#endif
   
    lseek(fd,0,SEEK_SET);
   	read(fd,&mh, sizeof(mh));

    printf("\nMach-o executable file header\n");
    printf("-------------------------------------------------\n");

    printf("Cpu type                    : ");
    switch(mh.cputype)
    {
      case (CPU_TYPE_POWERPC):   printf("Power PC 32 Bit\n"); break;
      case (CPU_TYPE_POWERPC64): printf("Power PC 64 Bit\n"); break;
      case (CPU_TYPE_I386):      printf("x86 32 Bit\n"); break;
      case (CPU_TYPE_X86_64):    printf("x86 64 Bit\n"); break;
      case(CPU_TYPE_ARM): printf("ARM \n"); break;
      case(CPU_TYPE_ARM64): printf("ARM64 \n"); break;
      case(CPU_TYPE_ARM64_32): printf("ARM64 32\n"); break; 
    }

    printf("Cpu subtype                 : 0x%04x\n", mh.cpusubtype);
    printf("File type                   : ");
    switch(mh.filetype)
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
     
    printf("Number of load commands     : %d\n",mh.ncmds);
    printf("Size of load commands       : %d\n",mh.sizeofcmds);
    printf("Flags                       : 0x%04x\n",mh.flags);
     
    if(mh.flags & MH_NOUNDEFS)   printf ("0x%08x - MH_NOUNDEF: No undefined references.\n", MH_NOUNDEFS);
    if(mh.flags & MH_INCRLINK)   printf ("0x%08x - MH_INCRLINK: It is incremental link against base file. Cannot' be linked again\n", MH_INCRLINK);
    if(mh.flags & MH_DYLDLINK)   printf ("0x%08x - MH_DYLDLINK: The file is input for the dynamic linker and cannot be statically linked again.\n", MH_DYLDLINK);
    if(mh.flags & MH_BINDATLOAD) printf ("0x%08x - MH_BINDATLOAD: Bind undefined references at load.\n", MH_BINDATLOAD);
    if(mh.flags & MH_PREBOUND)   printf ("0x%08x - MH_PREBOUND: The file’s undefined references are prebound.\n", MH_PREBOUND);
    if(mh.flags & MH_SPLIT_SEGS)   printf ("0x%08x - MH_SPLIT_SEGS: The file has its read-only and read-write segments split.\n", MH_SPLIT_SEGS );
    if(mh.flags & MH_LAZY_INIT)   printf ("0x%08x - MH_LAZY_INIT: The shared library init routine is to be run lazily via catching memory faults to its writeable segments.\n", MH_LAZY_INIT );
    if(mh.flags & MH_TWOLEVEL)   printf ("0x%08x - MH_TWOLEVEL: The image is using two-level namespace bindings.\n", MH_TWOLEVEL);
    if(mh.flags & MH_FORCE_FLAT)   printf ("0x%08x - MH_FORCE_FLAT: The executable is forcing all images to use flat namespace bindings. \n",MH_FORCE_FLAT );
    if(mh.flags & MH_NOMULTIDEFS)   printf ("0x%08x - MH_NOMULTIDEFS: This umbrella guarantees there are no multiple definitions of symbols in its subimages.\n", MH_NOMULTIDEFS);
    if(mh.flags & MH_NOFIXPREBINDING) printf ("0x%08x - MH_NOFIXPREBINDING: The dynamic linker doesn’t notify the prebinding agent about this executable. \n",MH_NOFIXPREBINDING );
    if(mh.flags & MH_PREBINDABLE)     printf ("0x%08x - MH_PREBINDABLE: This file is not prebound.\n", MH_PREBINDABLE);
    if(mh.flags & MH_ALLMODSBOUND)   printf ("0x%08x - MH_ALLMODSBOUND: This binary binds to all two-level namespace modules of its dependent libraries.\n",MH_ALLMODSBOUND );
    if(mh.flags & MH_SUBSECTIONS_VIA_SYMBOLS)   printf ("0x%08x - MH_SUBSECTIONS_VIA_SYMBOLS: The sections of the object file can be divided into individual blocks.\n", MH_SUBSECTIONS_VIA_SYMBOLS );
    if(mh.flags & MH_CANONICAL)   printf ("0x%08x - MH_CANONICAL: This file has been canonicalized by unprebinding—clearing prebinding information from the file.\n",MH_CANONICAL );
    if(mh.flags & MH_WEAK_DEFINES)   printf ("0x%08x - MH_WEAK_DEFINES: The final linked image contains external weak symbols.\n",MH_WEAK_DEFINES );
    if(mh.flags & MH_BINDS_TO_WEAK)   printf ("0x%08x - MH_BINDS_TO_WEAK: The final linked image uses weak symbols.\n",MH_BINDS_TO_WEAK );
    if(mh.flags & MH_ALLOW_STACK_EXECUTION)   printf ("0x%08x - MH_ALLOW_STACK_EXECUTION: Permission to execute stack.\n", MH_ALLOW_STACK_EXECUTION);
    if(mh.flags & MH_ROOT_SAFE)   printf ("0x%08x - MH_ROOT_SAFE: Binary is safe to use by root.\n", MH_ROOT_SAFE);
    if(mh.flags & MH_SETUID_SAFE)   printf ("0x%08x - MH_SETUID_SAFE: It is safe for use in processes when issetugid() is true.\n", MH_SETUID_SAFE);
    if(mh.flags & MH_NO_REEXPORTED_DYLIBS)   printf ("0x%08x - MH_NO_REEXPORTED_DYLIBS: The static linker does not need to examine dependent dylibs to see if any are re-exported.\n", MH_NO_REEXPORTED_DYLIBS);
    if(mh.flags & MH_PIE)   printf ("0x%08x - MH_PIE: The OS will load the main executable at a random address.\n", MH_PIE);
    if(mh.flags & MH_DEAD_STRIPPABLE_DYLIB)   printf ("0x%08x - MH_DEAD_STRIPPABLE_DYLIB: Do not create a LC_LOAD_DYLIB load command to the dylib if no symbols are being referenced from the dylib.\n", MH_DEAD_STRIPPABLE_DYLIB);
    if(mh.flags & MH_HAS_TLV_DESCRIPTORS)   printf ("0x%08x - MH_HAS_TLV_DESCRIPTORS: Contains a section of type S_THREAD_LOCAL_VARIABLES.\n", MH_HAS_TLV_DESCRIPTORS);
    if(mh.flags & MH_NO_HEAP_EXECUTION)   printf ("0x%08x - MH_NO_HEAP_EXECUTION: No heap execution\n", MH_NO_HEAP_EXECUTION);
}

void print_load_commands(int fd)
{
   struct mach_header_64 mh;
   struct load_command lc;

   lseek(fd, 0, SEEK_SET);
   read(fd, &mh, sizeof(mh));

    printf("Number of load commands     : %d\n",mh.ncmds);
    printf("Size of load commands       : %d\n",mh.sizeofcmds);

     for (int i=0; i < mh.ncmds; i++)
     {
         read(fd, &lc, sizeof(lc));
         //printf ("\n%02d) Command type: 0x%08x Size:%d \n", i, lc.cmd, lc.cmdsize);
         printf("%02d) 0x%08x Size:%04d ",i+1, lc.cmd, lc.cmdsize);

         switch(lc.cmd) 
         {
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
             case(LC_LOAD_UPWARD_DYLIB): printf("LC_LOAD_UPWARD_DYLIB: Load upward dylib."); break;
             case(LC_VERSION_MIN_MACOSX):     printf("LC_VERSION_MIN_MACOSX: Minimum Mac OS X version."); break;
             case(LC_BUILD_VERSION):  printf("LC_BUILD_VERSION: Minimum Mac OS X version."); break;
             case(LC_VERSION_MIN_IPHONEOS):    printf("LC_VERSION_MIN_IPHONEOS: Minimum iOS version."); break;
             case(LC_FUNCTION_STARTS): printf("LC_FUNCTION_STARTS: Compressed table of function start addresses."); break;
             case(LC_DYLD_ENVIRONMENT):     printf("LC_DYLD_ENVIRONMENT: String for dyld to treat like environment variable."); break;
             case(LC_MAIN):    printf("LC_MAIN: Replacement for LC_UNIXTHREAD."); break;
             case(LC_DATA_IN_CODE):        printf("LC_DATA_IN_CODE: Table of non-instructions in __text."); break;
             case(LC_SOURCE_VERSION):      printf("LC_SOURCE_VERSION: Source version used to build binary."); break;
             case(LC_DYLIB_CODE_SIGN_DRS): printf("LC_DYLIB_CODE_SIGN_DRS: Code signing DRs copied from linked dylibs."); break;
             case(LC_ENCRYPTION_INFO_64):  printf("LC_ENCRYPTION_INFO_64: 64-bit encrypted segment information."); break;
             case(LC_LINKER_OPTION):       printf("LC_LINKER_OPTION: Linker options in MH_OBJECT files."); break;
             case(LC_DYLD_EXPORTS_TRIE): printf("LC_DYLD_EXPORTS_TRIE: used with linkedit_data_command, payload is trie."); break;
             case(LC_DYLD_CHAINED_FIXUPS): printf("LC_DYLD_CHAINED_FIXUPS: used with linkedit_data_command."); break;
         }

         printf("\n");
         // go to the next command
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