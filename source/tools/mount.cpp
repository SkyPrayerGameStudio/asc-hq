/*
    This file is part of Advanced Strategic Command; http://www.asc-hq.de
    Copyright (C) 1994-1999  Martin Bickel  and  Marc Schellenberger

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; see the file COPYING. If not, write to the 
    Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
    Boston, MA  02111-1307  USA
*/

#include <errno.h>
#include <string.h>
#include "../global.h"
#include "../basestrm.h"
#include "../misc.h"
#include "../strtmesg.h"

#ifdef _DOS_
#  include <direct.h> 
#else
#  include <dirent.h>
#  include <sys/stat.h>
// #  include <unistd.h>
#endif


#ifndef MAX
 #define MAX(a,b)		((a) >= (b) ? (a) : (b))
#endif
#ifndef MIN
 #define MIN(a,b)		((a) <= (b) ? (a) : (b))
#endif
#define MAX3(a,b,c)		((a) >= (b) ? MAX(a,c) : MAX(b,c))
#define MIN3(a,b,c)		((a) <= (b) ? MIN(a,c) : MIN(b,c))



FILE* out;
int verbose = 1;

/*--- rle1.c ------------------------------ Listing 9-1 ---------
 *  Run-Length Encoding for files of all types
 *
 *-------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#define Sentinel    0xF0   /* the sentinel flag */
#define BUFFER_SIZE 30000

#define WriteCode(a,b,c) \
            fprintf( outfile, "%c%c%c", a, b, c )

FILE *infile,
     *outfile;

int rlemain ( char* argv1, char* argv2  )
{
    unsigned char *buffer;
    unsigned char prev_char;
    int  bytes_read,
         count,
         eof,
         first_time,
         i;

    if (( infile = fopen ( argv1, "rb" )) == NULL )
    {
        fprintf ( stderr, "Error opening %s\n", argv1 );
        return ( EXIT_FAILURE );
    }

    if (( outfile = fopen ( argv2, "wb" )) == NULL )
    {
        fprintf ( stderr, "Error opening %s\n", argv2 );
        return ( EXIT_FAILURE );
    }

    /* write the header to the output file */

    fprintf ( outfile, "%c%c%c%c%c%c%c%c%c%c", 0, 'M', 'B', 'R', 'L', 'E', '1', 0, 0, Sentinel );

    /* initialize the necessary variables */

    eof        = 0;
    first_time = 1;

    buffer = (unsigned char *) malloc ( BUFFER_SIZE );
    if ( buffer == NULL )
    {
        fprintf ( stderr, "Unable to allocate %d bytes\n",
                    BUFFER_SIZE );
        return ( EXIT_FAILURE );
    }

    /* process the input file */

    while ( ! eof )
    {
        bytes_read = fread ( buffer, 1, BUFFER_SIZE, infile );
        if ( bytes_read == 0 )
        {
            eof = 1;
            break;
        }

        for ( i = 0; i < bytes_read; i++ )
        {
            /* first time through is a special case */

            if ( first_time )
            {
                prev_char = buffer[i];
                count = 1;
                first_time = 0;
                i++;
            }

            if ( buffer[i] == prev_char )   /* repeated char */
            {
                count += 1;
                if ( count == 255 )
                {
                    WriteCode ( Sentinel, count, prev_char );
                    first_time = 1;
                }
                continue;
            }
            else    /* a new char, so write out all known data */
            {
                if ( count < 3 )
                {
                    if ( prev_char == Sentinel )
                    {
                        fprintf ( outfile, "%c%c",
                                        Sentinel, count );
                    }
                    else
                        do
                        {
                            fputc ( prev_char, outfile );
                        }
                           while ( --count );
                }
                else
                    WriteCode ( Sentinel, count, prev_char );

                prev_char = buffer[i];
                count = 1;
            }
        }

        /* we're at end of bytes_read, is it EOF? */

        if ( bytes_read < BUFFER_SIZE )
            eof = 1;
    }

    /* at EOF, so flush out any remaining bytes to be written */

    if ( count < 3 )
    {
        if ( prev_char == Sentinel )
        {
            fprintf ( outfile, "%c%c",
                            Sentinel, count );
        }
        else
            do
            {
                fputc ( prev_char, outfile );
            }
                while ( --count );
    }
    else
        WriteCode ( Sentinel, count, prev_char );

    fclose ( infile );
    fclose ( outfile );

    return ( EXIT_SUCCESS );
}



int
bzmain( char *argv1, char* argv2  )
{
    int size = filesize ( argv1 );
    void* buf = malloc ( size );

    FILE* fp = fopen ( argv1, filereadmode );
    fread ( buf, size, 1, fp );
    fclose ( fp );

    // tnfilestream instream ( argv1, 1 );
    tnfilestream outstream ( argv2, tnstream::writing );
    // instream.readdata ( buf, size, 0 );
    outstream.writedata ( buf, size );

    free ( buf );
    return 0;
}




dynamic_array<tcontainerindex> nindex;
int num = 0;
int pos = 0;

int uncompsize = 0;
int   compsize = 0;

#define NOLZW


void copyfile ( const char* name, const char* orgname, int size )
{

   char* buf = new char[size+100];

   FILE* in = fopen ( name, "rb" );
   if ( fread ( buf, 1, size, in ) != size ) {
      fprintf( stderr, "error reading file %s \n", name );
      exit(1);
   }
   fclose ( in );

   int s = fwrite ( buf, 1, size, out );
   if ( s != size ) {
      fprintf( stderr, "error writing file %s \n", name );
      exit(1);
   }
   nindex[num].name = strdup ( orgname );
   nindex[num].start = pos;
   pos += s;
   nindex[num].end = pos-1;
   num++;
   if ( verbose )
      printf ( " ; written \n" );
   compsize += size;

/*
   int r = spawnl ( P_WAIT, "cmd.exe", "cmd.exe", "/C", "del", name,  NULL );
   if ( r ) {
      printf ( "\n\n error delete file %s \n", name );
      exit ( r );
   }
   r = spawnl ( P_WAIT, "cmd.exe", "cmd.exe", "/C", "ren", orgname, name,  NULL );
   if ( r ) {
      printf ( "\n\n error renaming file %s to %s \n", name, orgname );
      exit ( r );
   }
*/
}


void testcompress ( char* name, int size )
{
   char newname[1000];
   char* orgname = name;
   if ( patimat ( "*.pcx", name ) ) {

      FILE* infile = fopen ( name, filereadmode );


      if ( getenv ( "temp" ))
         strcpy ( newname, getenv ( "temp" ));
      else
         if ( getenv ( "tmp" ))
            strcpy ( newname, getenv ( "tmp" ));
         else {
            strcpy ( newname, pathdelimitterstring );
            strcat ( newname, "tmp" );
         }
      if ( strlen ( newname ) && newname[strlen ( newname ) -1] != pathdelimitter )
         strcat ( newname, pathdelimitterstring );

      strcat ( newname, name );

      FILE* outfile = fopen ( newname, filewritemode );
      if ( !outfile )
         fatalError( "Unable to open " + ASCString(newname) + " for writing." );

      int size = filesize ( name );
      char* pc = (char*) malloc ( size );
      fread ( pc, size, 1, infile );
      fclose ( infile );

      int* ip = (int*) (pc + 124);
      *ip = size;

      int w = fwrite ( pc, 1, size, outfile );
      fclose ( outfile );
      if ( w != size )
         fatalError( "Could not write file " + ASCString(newname) );
         
      name = newname;
   }

   uncompsize += size;

   int comp = 0;
   {
      char buf[100];
      FILE* in = fopen ( name, "rb" );
      if ( !in )
         fatalError("cannot access file " + ASCString(name) );

      if ( fread ( buf, 1, 10, in ) == 10 ) {
         if ( strncmp ( &buf[1], "MBLZW16", 7) == 0 )
            comp = 1;
         if ( strncmp ( &buf[1], "MBRLE1", 6) == 0 )
            comp = 1;
         if ( strncmp ( &buf[0], "MBZLB2X!", 8) == 0 )
            comp = 1;
      }
      fclose ( in );
   }
   if ( comp ) {
      if ( verbose )
         printf ( "is already compressed ! " );
      copyfile ( name, name, filesize ( name ) );
      return;
   }

   int r;
   fflush ( stdout );

   r = rlemain( name, "temp.rle");
   if ( r ) {
      fprintf ( stderr, "\n\n error executing rle1.exe; errno is %d \n", errno );
      exit ( r );
   }
   int rl = filesize ( "temp.rle" );

  #ifndef NOLZW

   fflush ( stdout );
   r = spawnl ( P_WAIT, "lzwcomp.exe", "lzwcomp.exe", name, "temp.lzw", NULL );
   if ( r ) {
      fprintf ( stderr, "\n\n error executing lzwcomp.exe; errno is %d \n", errno);
      exit ( r );
   }
   int lz = filesize ( "temp.lzw" );

  #endif

   fflush ( stdout );
//   r = spawnl ( P_WAIT, "mbzip.exe", "mbzip.exe", name, "temp.mzl",  NULL );
   r = bzmain ( name, "temp.mzl" );
   if ( r ) {
      fprintf ( stderr, "\n\n error executing mbzip.exe; errno is %d \n", errno);
      exit ( r );
   }
   int mz = filesize ( "temp.mzl" );



  #ifdef NOLZW
   if ( verbose )
      printf ( ";  rle: %3d%%; mzl: %3d%% ; ", 100 * rl / size, 100 * mz / size );
   int compr = MIN ( rl, mz );
  #else
   if ( verbose )
      printf ( "; lzw: %3d%%; rle: %3d%%; mzl: %3d%%", 100 * lz / size, 100 * rl / size, 100 * mz / size );
   int compr = MIN3 ( rl, lz, mz );
  #endif

   if ( compr * 120 / 100 > size ) {
      if ( verbose )
         printf ( "not compressed" );
      copyfile ( name, name, size );
   } else
 #ifndef NOLZW
   if ( compr == lz ) {
      if ( verbose )
         printf ( "lzw compressed" );
      copyfile ( "temp.lzw", orgname, compr );
   } else
 #endif
   if ( compr == rl ) {
      if ( verbose )
         printf ( "rle compressed" );
      copyfile ( "temp.rle", orgname, compr );
   } else
   if ( compr == mz ) {
      if ( verbose )
         printf ( "mzl compressed" );
      copyfile ( "temp.mzl", orgname, compr );
   }


   if ( name == newname )
      remove ( newname );

}


// including the command line parser, which is generated by genparse
#include "../clparser/mount.cpp"

int main(int argc, char *argv[] )
{
   Cmdline cl ( argc, argv );

   if ( cl.v() ) {
      cout << argv[0] << " " << getVersionString() << endl;
      exit(0);
   }

   if ( argc - cl.next_param() < 2 ) {
      cl.usage();
      exit(1);
   }

   if (cl.q())
      verbose = 0;

   addSearchPath(".");

   out = fopen ( argv[argc-1], filewritemode );
   if ( !out ) {
      fprintf(stderr, "cannot open file %s for writing \n", argv[argc-1] );
      exit(1);
   }

   int i = 0;
   pos += fwrite ( containermagic, 1, 4, out );
   pos += fwrite ( &i, 1, 4, out );

   for ( int df = cl.next_param(); df < argc-1; df++ ) {
         int compress = 1;

         DIR *dirp;
         struct dirent *direntp;

         dirp = opendir( "." );
         if( dirp != NULL ) {
           for(;;) {
             direntp = readdir( dirp );
             if ( direntp == NULL )
                break;

             if ( patimat ( argv[df] , direntp->d_name ) &&
                  strcmp ( direntp->d_name, "." ) != 0 &&
                  strcmp ( direntp->d_name, ".." ) != 0 ) {
                int fnd = 0;

                for ( int j = 0; j < num; j++ )
                   if ( strcmpi ( nindex[j].name, direntp->d_name ) == 0 )
                      fnd = 1;

                if ( !fnd ) {
                   if ( verbose )
                      printf( direntp->d_name );
                   if ( compress )
                      testcompress ( direntp->d_name,  filesize(direntp->d_name) );
                   else {
                      if ( verbose )
                         printf ( " is not compressed, " );
                      copyfile ( direntp->d_name, direntp->d_name,  filesize(direntp->d_name)  );
                   }
                }
             }

           }
           closedir( dirp );
         }

   } /* endwhile */

   if ( verbose )
      printf ( "ftell: %d ; pos : %d ; num : %d \n ", int( ftell ( out )), pos, num );

   fwrite ( &num, 4, 1, out );

   for ( i = 0; i < num; i++) {
      fwrite ( &nindex[i], sizeof ( tcontainerindex ), 1, out );
      if ( nindex[i].name )
         fwrite ( nindex[i].name, 1, strlen ( nindex[i].name ) + 1, out );

   } /* endfor */

   fseek ( out, 4, SEEK_SET );
   fwrite ( &pos, 4, 1, out );
   fclose ( out );

   if ( verbose ) {
     // printf(" average compression is %d%% \noverhead is %d byte \n", 100 * compsize / uncompsize, filesize ( argv[2] ) - compsize );

      printf ( "completed \n" );
   }
   remove ( "temp.rle" );
   remove ( "temp.mzl" );

  #ifndef NOLZW
   remove ( "temp.lzw" );
  #endif

   return 0;
}

