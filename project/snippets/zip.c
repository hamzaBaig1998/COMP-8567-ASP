#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

// Compresses a file using zlib and writes it to a new file
void compress_file(char *source_filename, char *dest_filename)
{
    // Open the source file for reading
    FILE *source_file = fopen(source_filename, "rb");
    if (!source_file)
    {
        printf("Error: Could not open %s\n", source_filename);
        return;
    }

    // Open the destination file for writing
    FILE *dest_file = fopen(dest_filename, "wb");
    if (!dest_file)
    {
        printf("Error: Could not create %s\n", dest_filename);
        fclose(source_file);
        return;
    }

    // Initialize zlib
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    deflateInit(&stream, Z_DEFAULT_COMPRESSION);

    // Read the source file and write compressed data to the destination file
    unsigned char in[1024];
    unsigned char out[1024];
    stream.avail_in = fread(in, 1, sizeof(in), source_file);
    while (stream.avail_in != 0)
    {
        stream.next_in = in;
        do
        {
            stream.avail_out = sizeof(out);
            stream.next_out = out;
            deflate(&stream, Z_FINISH);
            printf("Compressed %ld bytes into %ld bytes\n", srcsize, destsize - stream.avail_out);
            // if (ret == Z_STREAM_ERROR)
            // {
            //     fprintf(stderr, "Error: deflate returned Z_STREAM_ERROR: %s\n", zError(ret));
            //     return 1;
            // }
            fwrite(out, 1, sizeof(out) - stream.avail_out, dest_file);
        } while (stream.avail_out == 0);
        stream.avail_in = fread(in, 1, sizeof(in), source_file);
    }
    deflateEnd(&stream);

    // Close the files
    fclose(source_file);
    fclose(dest_file);
}

// Decompresses a file using zlib and writes it to a new file
void decompress_file(char *source_filename, char *dest_filename)
{
    // Open the source file for reading
    FILE *source_file = fopen(source_filename, "rb");
    if (!source_file)
    {
        printf("Error: Could not open %s\n", source_filename);
        return;
    }

    // Open the destination file for writing
    FILE *dest_file = fopen(dest_filename, "wb");
    if (!dest_file)
    {
        printf("Error: Could not create %s\n", dest_filename);
        fclose(source_file);
        return;
    }

    // Initialize zlib
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    inflateInit(&stream);

    // Read the source file and write decompressed data to the destination file
    unsigned char in[1024];
    unsigned char out[1024];
    stream.avail_in = fread(in, 1, sizeof(in), source_file);
    while (stream.avail_in != 0)
    {
        stream.next_in = in;
        do
        {
            stream.avail_out = sizeof(out);
            stream.next_out = out;
            inflate(&stream, Z_NO_FLUSH);
            fwrite(out, 1, sizeof(out) - stream.avail_out, dest_file);
        } while (stream.avail_out == 0);
        stream.avail_in = fread(in, 1, sizeof(in), source_file);
    }
    inflateEnd(&stream);

    // Close the files
    fclose(source_file);
    fclose(dest_file);
}

int main()
{
    // Important command to run: sudo apt-get install zlib1g-dev
    // To compile: gcc zip.c -lz -o zip
    // Compress a file
    compress_file("example.txt", "example.zip");

    // Decompress a file
    // decompress_file("example.zip", "example_unzipped.txt");

    return 0;
}
