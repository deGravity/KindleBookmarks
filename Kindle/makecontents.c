#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Functions to swap byte order from little endian to big endian. Required
 * because Kindle is big endian, but x86 is little endian.*/

#define ByteSwap16(n) \
    ( ((((unsigned int) n) << 8) & 0xFF00) | \
      ((((unsigned int) n) >> 8) & 0x00FF) )

#define ByteSwap32(n) \
    ( ((((unsigned long) n) << 24) & 0xFF000000) | \
      ((((unsigned long) n) << 8) & 0x00FF0000) | \
      ((((unsigned long) n) >> 8) & 0x0000FF00) | \
      ((((unsigned long) n) >> 24) & 0x000000FF) )

/* Stores a Kindle bookmark entry */
typedef struct bookmark
{
    int page;
    char *pageNumber;
    struct bookmark *next;
} bookmark;

/* Allocate and initialize a bookmark */
bookmark* makeBookmark(int, char*);
/* Copies linked list of bookmarks to the specified file */
int addBookmarks(bookmark*, FILE *);
/* Displays the usage string. */
int usage(void);

int main(int argc, char** argv)
{
    int bookmarkCount = 0;
    int lastPage = 1;
    FILE *input;
    FILE *output;
    bookmark *head;
    bookmark *current;
    
    if (argc != 3)
    {
        usage();
        return 0;
    }

    input = fopen(argv[1], "r");

    if (input != NULL)
    {
        char line[512];
        while (fgets(line, sizeof(line), input) != NULL)
        {
            if (line[strlen(line)-1] == '\n')
                line[strlen(line)-1] = 0;

            if (bookmarkCount++ == 0)
            {
                head = makeBookmark(bookmarkCount, line);
                current = head;
            }
            else
            {
                current->next = makeBookmark(bookmarkCount, line);
                current = current->next;
            }
            printf("%s\n", line);
        }
    }
    else
    {
        perror(argv[1]);
        return 0;
    }
   
    unsigned char header[] = {0xDE, 0xAD, 0xCA, 0xBB, 0x01};
    unsigned char spacer = 0x00;
    unsigned int lastReadPage = ByteSwap32(1);
    bookmarkCount = ByteSwap32(bookmarkCount);
    
    output = fopen(argv[2], "wb");
    fprintf(output, "%s", header);
    fwrite(&lastReadPage, 1, sizeof(lastReadPage), output);
    fwrite(&bookmarkCount, 1, sizeof(bookmarkCount), output);
    fwrite(&spacer, 1, sizeof(spacer), output);
    addBookmarks(head, output);
    fclose(output);

    return 0;
}

bookmark* makeBookmark(int page, char* pageNumber)
{
    bookmark* bMark;
    bMark = (bookmark*) malloc(sizeof(bookmark));
    bMark->page = page;
    bMark->pageNumber = (char*) malloc(strlen(pageNumber) + 1);
    strcpy(bMark->pageNumber, pageNumber);
    bMark->next = NULL;
    return bMark;
}

int addBookmarks(bookmark *head, FILE *outFile)
{
    short int numChars;
    unsigned int pageNum = ByteSwap32(head->page);
    fwrite(&pageNum, 1, sizeof(pageNum), outFile);
    numChars = strlen(head->pageNumber);
    numChars = ByteSwap16(numChars);
    fwrite(&numChars, 1, sizeof(numChars), outFile);
    fprintf(outFile, "%s", head->pageNumber);
    if (head->next != NULL)
    {
        addBookmarks(head->next, outFile);
    }
    return 1;
}

int usage(void)
{
    printf("Usage: makecontents input output\n");
    return 0;
}