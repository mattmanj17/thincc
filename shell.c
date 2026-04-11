
// includes
#include <stdio.h>
#include <stdlib.h>

// assertions
#define REQUIRE(x, msg)                         \
    do {                                        \
        if (x)                                  \
            break;                              \
                                                \
        fprintf(                                \
            stderr,                             \
            "ERROR %s:%d: !(%s)\n",             \
            __FILE__,                           \
            __LINE__,                           \
            #x                                  \
        );                                      \
                                                \
        if (msg) {                              \
            fprintf(                            \
                stderr,                         \
                "This violates a constraint:\n" \
                "%s\n",                         \
                msg                             \
            );                                  \
        }                                       \
                                                \
        exit(EXIT_FAILURE);                     \
    } while (0)
#define ASSERT(x) REQUIRE(x, NULL)

// tag = byte
typedef unsigned char Byte;
#define BYTE_MAX 0xFFu 

// roughly :fgetc from the hex
Byte ByteReadOrExit(
    FILE* pFile, 
    int nExitCodeEOF
) {
    ASSERT(pFile);
    ASSERT(
        nExitCodeEOF == EXIT_SUCCESS || 
        nExitCodeEOF == EXIT_FAILURE
    );

    int c = fgetc(pFile);
    if (c != EOF) {
        ASSERT(c >= 0);
        ASSERT(c <= BYTE_MAX);
        return (Byte)c;
    }
    
    REQUIRE(
        !ferror(pFile), 
        "The only fgetc error handled by ByteReadOrExit is EOF."
    );

    REQUIRE(
        feof(pFile),
        "If fgetc(pFile) returns EOF without setting ferror(pFile), "
        "it must set feof(pFile)."
    );

    if (nExitCodeEOF == EXIT_FAILURE) {
        fprintf(stderr, "Unexpected EOF!\n");
    }
        
    exit(nExitCodeEOF);
}

// roughly :fputc in the hex
void WriteByteOrAbort(
    Byte byte,
    FILE* pFile
) {
    ASSERT(pFile);

    int c = fputc(byte, pFile);
    
    REQUIRE(
        (c == EOF) == (ferror(pFile) != 0),
        "On failure, fputc(ch, pFile) must return EOF, \n"
        "and set ferror(pFile)"
    );

    REQUIRE(
        !ferror(pFile),
        "WriteByteOrAbort does not handle fputc errors."
    );

    REQUIRE(
        c == byte,
        "On success, fputc must return the written character."
    );
}

// roughly :collect_comment from the hex
void DropLine(FILE* pFile) {
    ASSERT(pFile);
    for (;;) {
        Byte byte = ByteReadOrExit(pFile, EXIT_FAILURE);
        if (byte == '\n')
            return;
    }
}

// roughly :collect_string from the hex
void ReadString(
    FILE* pFile, 
    Byte** ppByteWrite,
    const Byte* pByteWriteEnd
) {
    ASSERT(pFile);
    ASSERT(ppByteWrite);
    ASSERT(pByteWriteEnd);
    
    Byte* pByteWrite = *ppByteWrite;
    ASSERT(pByteWrite);

    for (;;) {
        Byte byte = ByteReadOrExit(pFile, EXIT_FAILURE);
        if (byte == '"')
            break;

        REQUIRE(
            pByteWrite < pByteWriteEnd,
            "ReadString does not handle buffer exhaustion."
        );

        *pByteWrite = byte;
        ++pByteWrite;
    }

    *ppByteWrite = pByteWrite;
}

// tag = rspan
typedef struct SReadSpan
{
    const Byte *    m_pByteBegin;
    const Byte *    m_pByteEnd;
} SReadSpan;

// roughly :collect_token from the hex
SReadSpan ReadToken(
    FILE* pFile, 
    int* pCommandDone
) {
    ASSERT(pFile);
    ASSERT(pCommandDone);

    // surely 4k is enough for anyone... :)
    enum { cByteTokenBufferSize = 1024 * 4 };
    Byte* pTokenBuffer = (Byte*)calloc(cByteTokenBufferSize, 1);
    ASSERT(pTokenBuffer);

    // NOTE -1 on pByteWriteEnd so it points to legit mem.
    //  Put another way, even though we are doing things in
    //  terms of spans, we want to have at least one 
    //  null byte at the end, for sanity.
    Byte* pByteWrite = pTokenBuffer;
    const Byte* pByteWriteEnd = pTokenBuffer + cByteTokenBufferSize - 1;

    for (;;) {

        // NOTE we just exit(EXIT_SUCCESS) if we hit EOF here.
        Byte byte = ByteReadOrExit(pFile, EXIT_SUCCESS);
        
        // no support for CR or CRLF yet
        REQUIRE(
            byte != '\r', 
            "ReadToken does not support carriage returns."
        );

        // blanks separate tokens
        if (byte == ' ' || byte == '\t')
            break;

        // skip line breaks (which separate tokens).
        //  line breaks signal the end of a command.
        if (byte == '\n') {
            *pCommandDone = 1;
            break;
        }

        // strings
        //  may contain whitespace or be multi line
        //  does not include the open/close '"'
        // XXX '""' (the empty string) will be treated as a space...
        if (byte == '"') {
            ReadString(pFile, &pByteWrite, pByteWriteEnd);

            // XXX this may act strangely with input like
            // 'foo"bar baz"bing'. I think it would tokenize it as
            //  'foobar baz' and 'bing'. The 'right' behavior
            //  in this case is open to interpretation.
            //
            // ... maybe this should be 'continue' instead of 'break'
            break;
        }

        // comments skip the rest of the line,
        //  separate tokens,
        //  and signal the end of a command
        // NOTE DropLine consumes the line break,
        //  and aborts the program if it can not find one.
        if (byte == '#') {
            DropLine(pFile);
            *pCommandDone = 1;
            break;
        }

        // skip \\\n
        // NOTE this still separates tokens.
        //  the point is writing long commands that
        //  span many lines, not tokens that span many lines
        if (byte == '\\') {
            byte = ByteReadOrExit(pFile, EXIT_FAILURE);
            REQUIRE(
                byte == '\n',
                "'\\n' must follow '\\'."
            );
            break;
        }

        // ran out of space?
        REQUIRE(
            pByteWrite < pByteWriteEnd,
            "ReadToken does not handle buffer exhaustion."
        );

        // default case, extend current token and move on
        *pByteWrite = byte;
        ++pByteWrite;
    }

    // { NULL, NULL } indicates 'no token'
    SReadSpan rspan;
    rspan.m_pByteBegin = NULL;
    rspan.m_pByteEnd = NULL;

    // Nothing read? no token
    if (pByteWrite == pTokenBuffer) {
        free(pTokenBuffer);
        return rspan;
    }

    // actualy read something, ship it out!
    rspan.m_pByteBegin = pTokenBuffer;
    rspan.m_pByteEnd = pByteWrite;
    return rspan;
}

// roughly :File_Print in the hex
void PrintRspan(const SReadSpan* pRspan) {
    if (pRspan == NULL)
        return;

    const Byte * pByteCursor = pRspan->m_pByteBegin;
    const Byte * pByteEnd = pRspan->m_pByteEnd;

    for (;;) {
        if (pByteCursor >= pByteEnd)
            break;

        WriteByteOrAbort(*pByteCursor, stdout);
        ++pByteCursor;
    }
}

// helper for Echo
void PrintRspans(
    const SReadSpan * pRspanBegin, 
    const SReadSpan * pRspanEnd,
    const SReadSpan * pRspanSep
) {
    if (!pRspanBegin)
    {
        ASSERT(!pRspanEnd);
        return;
    }
    ASSERT(pRspanEnd);

    for (
        const SReadSpan * pRspan = pRspanBegin; 
        pRspan < pRspanEnd; 
        ++pRspan
    ) {        
        if (pRspanSep && pRspan != pRspanBegin)
        {
            PrintRspan(pRspanSep);
        }
        
        PrintRspan(pRspan);
    }
}

// roughly :print_command from the hex
void Echo(
    const SReadSpan * pRspanBegin, 
    const SReadSpan * pRspanEnd
) {
    WriteByteOrAbort(' ', stdout);
    WriteByteOrAbort('#', stdout);
    WriteByteOrAbort(' ', stdout);

    Byte space[] = " ";
    SReadSpan rspanSpace;
    rspanSpace.m_pByteBegin = space;
    rspanSpace.m_pByteEnd = space + 1;

    PrintRspans(pRspanBegin, pRspanEnd, &rspanSpace);

    WriteByteOrAbort('\n', stdout);
}
