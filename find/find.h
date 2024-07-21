typedef struct
{
    int LineNumber;
    char *Line;
    //char *Filename;
} result_instance;

typedef struct
{
    int Count;
    result_instance *Results;
} find_results;

int
FindInString(char *Buffer, char *Pattern, int *Index)
{
    int BufferSize = strlen(Buffer);
    int PatternSize = strlen(Pattern);
    int Match = false;
    for (int BufferIndex = 0; BufferIndex < BufferSize; BufferIndex++)
    {
        int Pointer = 0;
        while((BufferIndex + Pointer < BufferSize) && (Pointer < PatternSize))
        {
            char A = Buffer[BufferIndex + Pointer];
            char B = Pattern[Pointer];
            if (A != B)
                break;
            Pointer++;
        }
        if (Pointer == PatternSize)
        {
            Match = true;
            break;
        }
    }
    return Match;
}

int
FindInFile(char *Filename, char *Pattern, int *MatchCount)
{
    FILE *File = fopen(Filename, "r");
    if (!File)
    {
        printf("Error: Could not open file %s\n", Filename);
        return false;
    }
    char Buffer[200];
    int LineCount = 1;
    *MatchCount = 0;
    while (fgets(Buffer, 200, File))
    {
        int Index = -1;
        if (FindInString(Buffer, Pattern, &Index))
        {
            int MatchSize = strlen(Buffer) + 1;
            char *Match = PoolAllocate(&ResultsDataPool, MatchSize);
            memcpy(Match, Buffer, MatchSize);
            
            result_instance Instance;
            Instance.LineNumber = LineCount;
            Instance.Line = Match;
            
            char *Location = PoolAllocate(&ResultsPool, sizeof(result_instance));
            *((result_instance*)Location) = Instance;
            
            *MatchCount += 1;
        }
        LineCount++;
    }
    fclose(File);
    if (*MatchCount > 0)
        return true;
    return false;
}

void
Find(find_results *ResultList, char *Filename, char *Pattern)
{
    PoolFree(&ResultsPool);
    PoolFree(&ResultsDataPool);
    int MatchCount;
    if (FindInFile(Filename, Pattern, &MatchCount))
    {
        ResultList->Count = MatchCount;
        ResultList->Results = (result_instance*)ResultsPool.Data;
    }
    else
    {
        ResultList->Count = 0;
    }
}