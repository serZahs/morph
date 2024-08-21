typedef struct
{
    int LineNumber;
    char *Line;
    char *Filename;
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
FindInFile(char *Directory, char *Filename, char *StartingDirectory, char *Pattern, int *MatchCount)
{
    // TODO: Leak
    char *Path = StringConcat(Directory, Filename);
    FILE *File = fopen(Path, "r");
    if (!File)
    {
        printf("Error: Could not open file %s\n", Path);
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
            Instance.Filename = StringConcat(StartingDirectory, Filename);
            //printf("Found match in %s:%d\n", Instance.Filename, Instance.LineNumber);
            
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

int
Find(find_results *ResultList, const char *FilePattern, const char *Pattern)
{
    return FindEx(ResultList, FilePattern, NULL, Pattern, true);
}

int
FindEx(find_results *ResultList, const char *FilePattern, char *StartingDirectory, const char *Pattern, int FreePool)
{
    if (FreePool)
    {
        PoolFree(&ResultsPool);
        PoolFree(&ResultsDataPool);
    }
    
    WIN32_FIND_DATAA FindFileData;
    HANDLE FindHandle = FindFirstFileA(FilePattern, &FindFileData);
    if (FindHandle == INVALID_HANDLE_VALUE)
    {
        printf("Error: Got an invalid handle for the first file %s\n", FilePattern);
        return false;
    }
    
    while(1)
    {
        char *Filename = CopyString(FindFileData.cFileName); // TODO: Leak!
        if (StartingDirectory) printf("Starting Directory: %s\n", StartingDirectory);
        printf("For %s, found %s\n", FilePattern, Filename);
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (!StringsMatch(Filename, ".") && !StringsMatch(Filename, ".."))
            {
                char *Directory = Substring((char*)FilePattern, 0, StringFindLast((char*)FilePattern, '*')); // TODO: Leak!
                char *Suffix = Substring((char*)FilePattern, StringFindLast((char*)FilePattern, '*')-1, strlen(FilePattern)); // TODO: Leak!
                char *P = StringConcat(Directory, Filename);
                char *Path = StringConcat(P, Suffix);
                char *InStartingDir;
                if (!StartingDirectory)
                {
                    InStartingDir = StringConcat(Filename, "\\");
                }
                else
                {
                    InStartingDir = StringConcat(StringConcat(StartingDirectory, Filename), "\\"); 
                }
                FindEx(ResultList, Path, InStartingDir, Pattern, false);
            }
        }
        else
        {
            char *Directory = Substring((char*)FilePattern, 0, StringFindLast((char*)FilePattern, '*')); // TODO: Leak!
            int MatchCount;
            if (FindInFile(Directory, Filename, StartingDirectory, (char*)Pattern, &MatchCount))
            {
                ResultList->Count += MatchCount;
            }
        }
        if (!FindNextFileA(FindHandle, &FindFileData))
        {
            DWORD Error = GetLastError();
            if (Error == ERROR_NO_MORE_FILES)
                break;
            else
            {
                printf("Error: Could not find next file: %d\n", Error);
                return false;
            }
        }
    }
    FindClose(FindHandle);
    ResultList->Results = (result_instance*)ResultsPool.Data;
    return true;
}
