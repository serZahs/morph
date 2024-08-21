#include <string.h>

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

double BackspaceTimer;
#define NOGDI
#define NOUSER
#include <windows.h>

#include "util.h"
#include "ui.h"
#include "find.h"

ui_widget Widgets[10];

int main()
{
    InitPool(&ResultsPool, 1*1024*1024);
    InitPool(&ResultsDataPool, 1*1024*1024);
    find_results ResultList;
    ResultList.Count = 0;
    ResultList.Results = NULL;
    
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    int WindowWidth = 1500;
    int WindowHeight = 1000;
    InitWindow(WindowWidth, WindowHeight, "Find!");
    
    ui_font DefaultFont;
    DefaultFont.Size = 30;
    DefaultFont.Font = LoadFontEx("fonts/segoeui.ttf", DefaultFont.Size, NULL, 0);
    ui_font FontSmall;
    FontSmall.Size = 20;
    FontSmall.Font = LoadFontEx("fonts/consola.ttf", FontSmall.Size, NULL, 0);
    
    // Styling
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, RGBAToHex(20,20,20,255));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xffffffff);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x444444ff);
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 0);
    Color TextColor = {255, 255, 255, 255};
    
    Vector2 Scroll = {0,0};
    BackspaceTimer = GetTime();
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        Color ClearColor = {40, 40, 40, 255};
        ClearBackground(ClearColor);
        
        char Filename[100];
        char Value[100];
        float MarginX = 20.0f;
        float MarginY = 20.0f;
        float RowHeight = 50.0f;
        {
            float Line0Y = 20.0f;
            const char *FilesText = "Files:";
            SetFont(DefaultFont);
            Vector2 Position = {MarginX, Line0Y + ((RowHeight-DefaultFont.Size)/2.0f)};
            Vector2 TextDimensions = MeasureTextEx(DefaultFont.Font, FilesText, (float)DefaultFont.Size, 0.0f);
            DrawTextEx(DefaultFont.Font, FilesText, Position, (float)DefaultFont.Size, 0.0f, TextColor); 
            Rectangle Bounds = {TextDimensions.x + MarginX*2.0f, Line0Y, 300.0f, RowHeight};
            DrawTextbox(0, Bounds, (Color){20, 20, 20, 255}, (Color){255, 255, 255, 255}, Filename, 100, DefaultFont);
        }
        float Line1Y = 100.0f;
        {
            const char *PatternText = "Pattern:";
            SetFont(DefaultFont);
            Vector2 Position = {MarginX, Line1Y + ((RowHeight-DefaultFont.Size)/2.0f)};
            Vector2 TextDimensions = MeasureTextEx(DefaultFont.Font, PatternText, (float)DefaultFont.Size, 0.0f);
            DrawTextEx(DefaultFont.Font, PatternText, Position, (float)DefaultFont.Size, 0.0f, TextColor); 
            Rectangle Bounds = {TextDimensions.x + MarginX*2.0f, Line1Y, 300.0f, RowHeight};
            DrawTextbox(1, Bounds, (Color){20, 20, 20, 255}, (Color){255, 255, 255, 255}, Value, 100, DefaultFont);
        }
        if (GuiButton((Rectangle){500.0f, Line1Y, 80, RowHeight}, "Find!"))
        {
            // Reset the ResultList before calling Find.
            ResultList.Count = 0;
            ResultList.Results = NULL;
            Find(&ResultList, Filename, Value);
        }
        
        if (ResultList.Count > 0)
        {
            float Line2Y = Line1Y + 50.0f + MarginY;
            float ItemHeight = 40.0f;
            Rectangle ScrollView = {0};
            Rectangle PanelRect = {MarginX, Line2Y, WindowWidth - MarginX*2.0f, 400.0f};
            Rectangle PanelContentRect = {PanelRect.x + MarginX, Line2Y, PanelRect.width - MarginX*2.0f, ItemHeight*ResultList.Count*2.0f};
            GuiScrollPanel(PanelRect, NULL, PanelContentRect, &Scroll, &ScrollView);
            
            BeginScissorMode((int)ScrollView.x, (int)ScrollView.y, (int)ScrollView.width, (int)ScrollView.height);
            SetFont(FontSmall);
            float Roundness = 0.8f;
            float Thickness = 3.0f;
            float Space = 8.0f;
            for (int ResultIndex = 0; ResultIndex < ResultList.Count; ResultIndex++)
            {
                float MetadataWidth = 400.0f;
                
                result_instance Result = ResultList.Results[ResultIndex];
                Rectangle ItemRect = {
                    PanelContentRect.x + MarginX*2.0f + (MetadataWidth + 10.0f), Line2Y + Space + ResultIndex * (ItemHeight + Space) + Scroll.y, PanelContentRect.width-MarginX*4.0f - (MetadataWidth + 10.0f),
                    ItemHeight};
                DrawRectangleRounded(ItemRect, Roundness, 10, (Color){20,20,20,255});
                DrawRectangleRoundedLines(ItemRect, Roundness, 10, Thickness, (Color){40,40,40,255});
                {
                    Vector2 Padding = {20.0f, (ItemRect.height - FontSmall.Size)/2.0f};
                    Vector2 TextPosition = {ItemRect.x + Padding.x, ItemRect.y + Padding.y};
                    DrawTextEx(FontSmall.Font, Result.Line, TextPosition, (float)FontSmall.Size, 0.0f, TextColor);
                }
                
                Rectangle MetadataRect = {PanelContentRect.x + MarginX*2.0f, Line2Y + Space + ResultIndex * (ItemHeight + Space) + Scroll.y, MetadataWidth, ItemHeight};
                DrawRectangleRounded(MetadataRect, Roundness, 10, (Color){20,20,20,255});
                DrawRectangleRoundedLines(MetadataRect, Roundness, 10, Thickness, (Color){40,40,40,255});
                {
                    Vector2 Padding = {20.0f, (MetadataRect.height - FontSmall.Size)/2.0f};
                    Vector2 TextPosition = {MetadataRect.x + Padding.x, MetadataRect.y + Padding.y};
                    char Buffer[200];
                    sprintf_s(Buffer, 200, "%s:%d", Result.Filename, Result.LineNumber);
                    DrawTextEx(FontSmall.Font, Buffer, TextPosition, (float)FontSmall.Size, 0.0f, TextColor);
                }
            }
            EndScissorMode();
        }
        EndDrawing();
    }
    CloseWindow();
}