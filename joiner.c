#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <io.h>

int main(int argc, char *argv[])
{
    const char about[] = "Usage: joiner out.bin file1 file2...\n";
    FILE *fp;
    char *filename;
    int i;
    char cfiles;
    char **data;
    int *size;
    int files = argc - 2;

    if (argc < 2)
    {
        printf(about);
        return 1;
    }

    data = (char **)malloc(files * sizeof(char *));
    size = (int *)malloc(files * sizeof(int));

    for (i = 0; i < files; i++)
    {
        filename = argv[i + 2];
        if (!(fp = fopen(filename, "rb")))
        {
            printf("Cannot open %s\n", filename);
            return 1;
        }

        size[i] = (int)_filelength(_fileno(fp));
        data[i] = (char *)malloc(size[i]);

        printf("Reading file %s, filesize %d\n", filename, size[i]);

        fread(data[i], size[i], 1, fp);
        fclose(fp);

        fclose(fp);
    }

    filename = argv[1];

    printf("Opening %s for writing\n", filename);

    if (!(fp = fopen(filename, "wb")))
    {
        printf("Cannot open %s\n", filename);
        return 1;
    }

    printf("Writing count (byte, %d)\n", files);

    cfiles = files;
    fwrite(&cfiles, sizeof(char), 1, fp);

    for (i = 0; i < files; i++)
    {
        int s = size[i];
        int b1 = s & 0xff;
        int b2 = (s >> 8) & 0xff;
        int b3 = (s >> 16) & 0xff;
        int b4 = (s >> 24) & 0xff;
        int be = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;

        printf("Writing length (be-int, %d)\n", size[i]);
        fwrite(&be, sizeof(int), 1, fp);
    }

    for (i = 0; i < files; i++)
    {

        printf("Writing file %s\n", argv[i + 2]);
        fwrite(data[i], size[i], 1, fp);
        free(data[i]);
    }

    free(data);
    free(size);
    fclose(fp);

    printf("All OK\n");

    return 0;
}
