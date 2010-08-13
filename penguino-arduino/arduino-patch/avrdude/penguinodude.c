#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define startswith(haystack,needle) (strncmp( haystack, needle, strlen(needle) ) == 0)

#define FLASH_PREFIX "-Uflash:w:"

int main( int argc, char *argv[] ) {
   char *args[argc+1];
   
   int i;
   for ( i = 0; i < argc; i++ ) {
      args[i] = argv[i];
   }
   args[argc] = NULL;
   
   char real_avrdude[1024];
   sprintf( real_avrdude, "%s-real", argv[0] );
   
   args[0] = real_avrdude;
   
   // -Uflash:w:/var/folders/l2/l2TvkW2TFSCxAVMckCdo9++++TM/-Tmp-/build2190858350308842553.tmp/Blink.cpp.hex:i
   
   bool isPenguino = false;
   char hexFile[1024];
   for ( i = 0; i < argc; i++ ) {
      if ( strcmp( args[i], "-cpenguino" ) == 0 ) {
         isPenguino = true;
      } else if ( startswith( args[i], FLASH_PREFIX ) ) {
         strcpy( hexFile, &args[i][strlen(FLASH_PREFIX)] );
         hexFile[strlen(hexFile)-2] = '\0';
      }
   }
   
   int result;
   
   if ( isPenguino ) {
      sprintf( real_avrdude, "flip '%s'", hexFile );
      result = system( real_avrdude );
      
      printf( "\n\n" );
      printf( "Thanks for choosing Icy Labs Penguino AVR!\n" );
   } else {
      result = execv( real_avrdude, args );
   }
   
   return result;
}
