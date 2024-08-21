void
Panic(char *FormatString) {
    char Buffer[200];
    sprintf_s(Buffer, 200, "Panic: %s", FormatString);
    printf("%s", Buffer);
    
    //DebugBreak();
    exit(1);
}

typedef struct
{
    int Used;
    int Size;
    char *Data;
} pool;

// TODO: Pools could be combined since they share the same lifetime (they are reset each time we do a find).
pool ResultsPool;
pool ResultsDataPool;

void
InitPool(pool *Pool, int StartingSize)
{
    Pool->Used = 0;
    Pool->Size = StartingSize;
    Pool->Data = malloc(Pool->Size);
}

char*
PoolAllocate(pool *Pool, int Size)
{
    if ((Pool->Used + Size) > Pool->Size)
    {
        Panic("A pool ran out of memory\n");
    }
    char *Result = Pool->Data + Pool->Used;
    Pool->Used += Size;
    return Result;
}

void
PoolFree(pool *Pool)
{
    Pool->Used = 0;
}

int
RGBAToHex(int r, int g, int b, int a)
{
    return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
        + (a & 0xff);
}

void
AppendChar(char *Text, int BufferSize, char End)
{
    int Length = strlen(Text);
    if (Length < BufferSize-1)
    {
        Text[Length] = End;
        Text[Length+1] = '\0';
    }
}

void
RemoveChar(char *Text)
{
    int Length = strlen(Text);
    if (Length > 0)
    {
        Text[Length-1] = '\0';
    }
}

char* CopyString(char *Source)
{
    if (!Source) return NULL;
    int Size = (int)strlen(Source);
    char* Result = (char*)malloc(Size + 1);
    memcpy(Result, Source, Size);
    Result[Size] = '\0';
    return Result;
}

int
StringFindLast(char *Buffer, char Target)
{
    int BufferSize = (int)strlen(Buffer);
    int CharIndex = BufferSize - 1;
    while (CharIndex >= 0)
    {
        if (Buffer[CharIndex--] == Target)
            return CharIndex + 1;
    }
    return -1;
}

char*
Substring(char *Buffer, int Start, int End)
{
    int BufferSize = (int)strlen(Buffer);
    if (Start < 0 || End > BufferSize || Start >= End || BufferSize <= 0)
        return NULL;
    char *Result = (char*)malloc(End - Start + 1);
    memcpy(Result, Buffer + Start, End - Start);
    Result[End - Start] = '\0';
    return Result;
}

char*
StringConcat(char *First, char *Second)
{
    if (!First) return Second;
    if (!Second) return First;
    int FirstSize = (int)strlen(First);
    int SecondSize = (int)strlen(Second);
    int BufferSize = (FirstSize + SecondSize) + 1;
    char *Result = (char*)malloc(BufferSize);
    memcpy(Result, First, FirstSize);
    memcpy(Result + FirstSize, Second, SecondSize);
    Result[BufferSize - 1] = '\0';
    return Result;
}

bool
StringsMatch(char *First, char *Second)
{
    int FirstSize = (int)strlen(First);
    int SecondSize = (int)strlen(Second);
    if (FirstSize != SecondSize)
        return false;
    int Index = -1;
    while (++Index < FirstSize)
    {
        if (First[Index] != Second[Index])
            return false;
    }
    return true;
}
