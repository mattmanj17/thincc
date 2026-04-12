
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define CONSTRAINT(predicate, constraint)   \
do {                                        \
    if (predicate)                          \
        break;                              \
                                            \
    fprintf(                                \
        stderr,                             \
        "FATAL ERROR %s:%d: !(%s)\n",       \
        __FILE__,                           \
        __LINE__,                           \
        #predicate                          \
    );                                      \
                                            \
    if (constraint) {                       \
        fprintf(                            \
            stderr,                         \
            "This violates a constraint:\n" \
            "%s\n",                         \
            constraint                      \
        );                                  \
    }                                       \
                                            \
    exit(EXIT_FAILURE);                     \
} while (0)

#define REQUIRE(predicate) \
CONSTRAINT(predicate, NULL)

typedef unsigned char Byte;
Byte ReadByteOrExitIfEOF(
    FILE* file, 
    int exit_eof
) {
    REQUIRE(file);
    REQUIRE(
        exit_eof == EXIT_SUCCESS || 
        exit_eof == EXIT_FAILURE
    );

    int c = fgetc(file);
    if (c != EOF) {
        REQUIRE(c >= 0);
        REQUIRE(c <= 255);
        return (Byte)c;
    }
    
    CONSTRAINT(
        !ferror(file), 
        "The only fgetc error handled by "
        "ReadByteOrExitIfEOF is EOF."
    );

    CONSTRAINT(
        feof(file),
        "If fgetc(file) returns EOF "
        "without setting ferror(file), "
        "it must set feof(file)."
    );

    if (exit_eof == EXIT_FAILURE) {
        fprintf(stderr, "Unexpected EOF!\n");
    }
        
    exit(exit_eof);
}

Byte ReadByte(FILE* file) {
    return ReadByteOrExitIfEOF(
        file, 
        EXIT_FAILURE
    );
}

void WriteByte(
    Byte byte,
    FILE* file
) {
    REQUIRE(file);

    int c = fputc(byte, file);
    
    CONSTRAINT(
        (c == EOF) == (ferror(file) != 0),
        "On failure, fputc(ch, file) must return EOF,\n"
        "and set ferror(file)"
    );

    CONSTRAINT(
        !ferror(file),
        "WriteByte does not handle fputc errors."
    );

    CONSTRAINT(
        c == byte,
        "On success, fputc must return the written character."
    );
}

void ReadPastLineBreak(FILE* file) {
    REQUIRE(file);
    for (;;) {
        Byte byte = ReadByte(file);
        if (byte == '\n')
            return;
    }
}

void ReadString(
    FILE* file, 
    Byte** cursor,
    Byte* end
) {
    REQUIRE(file);
    REQUIRE(cursor);
    REQUIRE(end);
    
    Byte* it = *cursor;
    REQUIRE(it);
    REQUIRE(end >= it);

    for (;;) {
        Byte byte = ReadByte(file);
        if (byte == '"')
            break;

        CONSTRAINT(
            byte == ' ' ||
            byte == '\t' ||
            byte == '\n' ||
            (byte >= '!' &&
            byte <= '~'),
            "ReadString only supports a subset of ascii."
        );

        CONSTRAINT(
            it < end,
            "ReadString does not handle buffer exhaustion."
        );

        *it = byte;
        ++it;
    }

    *cursor = it;
}

typedef struct
{
    Byte* begin;
    Byte* end;
} Token;

enum { 
    token_buffer_size = 1024 * 4,

    // tokens are null terminated
    max_token_length = token_buffer_size - 1
};
Token AllocateAndReadToken(
    FILE* file,
    int* command_done
) {
    REQUIRE(file);
    REQUIRE(command_done);

    Byte* token_buffer = (Byte*)calloc(token_buffer_size, 1);
    REQUIRE(token_buffer);

    Byte* it = token_buffer;
    Byte* end = token_buffer + max_token_length;

    for (;;) {
        Byte byte = ReadByteOrExitIfEOF(file, EXIT_SUCCESS);
        
        CONSTRAINT(
            byte != '\r', 
            "AllocateAndReadToken does not support '\\r'."
        );

        if (byte == ' ' || byte == '\t')
            break;

        if (byte == '\n') {
            *command_done = 1;
            break;
        }

        if (byte == '"') {
            ReadString(file, &it, end);

            // tokens may contain strings.
            // the quotes are not included.
            continue;
        }

        if (byte == '#') {
            ReadPastLineBreak(file);
            *command_done = 1;
            break;
        }

        if (byte == '\\') {
            byte = ReadByte(file);
            CONSTRAINT(
                byte == '\n',
                "'\\n' must follow '\\'."
            );
            break;
        }

        CONSTRAINT(
            byte >= '!' &&
            byte <= '~',
            "AllocateAndReadToken only handles printable ascii."
        );

        CONSTRAINT(
            it < end,
            "AllocateAndReadToken does not handle "
            "buffer exhaustion."
        );

        *it = byte;
        ++it;
    }

    // return NULL if token is zero length
    Token token = { NULL, NULL };
    if (it == token_buffer) {
        free(token_buffer);
        return token;
    }

    token.begin = token_buffer;
    token.end = it;
    return token;
}

void PrintToken(Token token) {
    Byte* it = token.begin;
    Byte* end = token.end;

    REQUIRE(it);
    REQUIRE(end > it);

    for (;;) {
        if (it >= end)
            break;

        WriteByte(*it, stdout);
        ++it;
    }
}

typedef struct {
    Token* begin;
    Token* end;
} Argv;

void EchoArgv(Argv argv) {
    Token* begin = argv.begin;
    Token* end = argv.end;
    
    REQUIRE(begin);
    REQUIRE(end > begin);

    WriteByte(' ', stdout);
    WriteByte('#', stdout);
    WriteByte(' ', stdout);

    for (Token* it = begin; it < end; ++it) {
        PrintToken(*it);
        WriteByte(' ', stdout);
    }

    WriteByte('\n', stdout);
}

void CopyToken(
    Token token, 
    char** cursor
) {
    char* it = *cursor;
    for (;;) {
        if (token.begin >= token.end)
            break;

        char ch = (char)*token.begin;

        REQUIRE(ch != '\\');
        REQUIRE(ch != '"');

        *it = ch;
        
        ++token.begin;
        ++it;
    }
    *cursor = it;
}

enum { max_argument_count = 256 };
char* AllocateCommand(Argv argv) {
    Token* begin = argv.begin;
    Token* end = argv.end;

    REQUIRE(begin);
    REQUIRE(end > begin);

    enum { 
        // +3 for the quotes and space
        max_argument_length = max_token_length + 3,

        max_command_length = (
            max_argument_count * max_argument_length
        ),
    };

    size_t command_length = 0;
    for (Token* it = begin; it < end; ++it) {
        REQUIRE(it->begin);
        REQUIRE(it->end > it->begin);

        ptrdiff_t token_length = it->end - it->begin;
        REQUIRE(token_length > 0);
        REQUIRE(token_length <= max_token_length);
        
        size_t arg_length = (size_t)token_length + 3;
        REQUIRE(
            command_length <= (max_command_length - arg_length)
        );
        
        command_length += arg_length;
        
        REQUIRE(command_length <= max_command_length);
    }

    // command is null terminated
    char* command = calloc(command_length + 1, 1);
    REQUIRE(command);

    char* it = command;
    for (Token* token = begin; token < end; ++token) {
        // leading '"'
        *it = '"';
        ++it;

        // token
        CopyToken(*token, &it);

        // trailing '" '
        it[0] = '"';
        it[1] = ' ';
        it += 2;
    }

    REQUIRE(it > command);
    REQUIRE((size_t)(it - command) == command_length);

    return command;
}

void RunChildProcess(
    Token arg0,
    char* command
) {
    REQUIRE(arg0.begin);
    REQUIRE(command);

    PROCESS_INFORMATION pi = {0};
    STARTUPINFOA si = {0};
    {
        si.cb = sizeof(si);
        
        si.dwFlags |= STARTF_USESTDHANDLES;
        
        si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
        REQUIRE(si.hStdInput);
        REQUIRE(si.hStdInput != INVALID_HANDLE_VALUE);
        
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        REQUIRE(si.hStdOutput);
        REQUIRE(si.hStdOutput != INVALID_HANDLE_VALUE);
        
        si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
        REQUIRE(si.hStdError);
        REQUIRE(si.hStdError != INVALID_HANDLE_VALUE);
    }
    
    char * exe = (char *)arg0.begin;
    REQUIRE(
        CreateProcessA(
            exe,        // lpApplicationName
            command,    // lpCommandLine
            NULL,       // lpProcessAttributes
            NULL,       // lpThreadAttributes
            TRUE,       // bInheritHandles
            0,          // dwCreationFlags
            NULL,       // lpEnvironment
            NULL,       // lpCurrentDirectory
            &si,        // lpStartupInfo
            &pi         // lpProcessInformation
        )
    );
    
    REQUIRE(
        WaitForSingleObject(pi.hProcess, INFINITE)  
        == WAIT_OBJECT_0
    );
    
    DWORD rc = 0;
    REQUIRE(GetExitCodeProcess(pi.hProcess, &rc));
    
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    REQUIRE(rc == 0);
}

void RunNextCommand(FILE* file) {
    REQUIRE(file);

    // length of arguments is max_argument_count + 1.
    // this makes tokens[max_argument_count] a valid address.
    // this prevents confusing semantics if argv.end
    // points to exactly arguments[max_argument_count]
    Token arguments[max_argument_count + 1] = {0};

    size_t argument_count = 0;
    int command_done = 0;
    while (!command_done) {
        Token token = AllocateAndReadToken(
            file, 
            &command_done
        );
        if (token.begin) {
            REQUIRE(argument_count < max_argument_count);
            arguments[argument_count] = token;
            ++argument_count;
        }
    }

    if (argument_count == 0)
        return;

    Argv argv;
    argv.begin = &arguments[0];
    argv.end = &arguments[argument_count];

    EchoArgv(argv);

    char* command = AllocateCommand(argv);
    RunChildProcess(*argv.begin, command);
    free(command);

    for (Token* it = argv.begin; it < argv.end; ++it) {
        free(it->begin);
    }
}

int main(int argc, char** argv) {
    REQUIRE(argc == 2);
    REQUIRE(argv);
    REQUIRE(argv[0]);
    REQUIRE(argv[1]);

    FILE* file = fopen(argv[1], "rb");
    REQUIRE(file);

    for (;;) {
        RunNextCommand(file);
    }
}
