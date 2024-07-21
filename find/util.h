void
Panic(char *FormatString) {
    char Buffer[200];
    sprintf_s(Buffer, 200, "Panic: %s", FormatString);
    printf("%s", Buffer);
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
        Panic("Pool size exceeded\n");
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