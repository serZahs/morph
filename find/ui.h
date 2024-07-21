typedef struct
{
    int Id;
    int Focused;
    int Pressed;
    int Hover;
} ui_widget;

typedef struct
{
    int Size;
    Font Font;
} ui_font;

extern ui_widget Widgets[];

void SetFont(ui_font Font)
{
    GuiSetFont(Font.Font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, Font.Size);
}

DrawTextbox(int Id, Rectangle Bounds, Color Background, Color TextColor, char *Text, int BufferSize, ui_font Font)
{
    int MouseX = GetMouseX();
    int MouseY = GetMouseY();
    
    if (MouseX >= Bounds.x && MouseX <= (Bounds.x + Bounds.width) && MouseY >= Bounds.y && MouseY <= (Bounds.y + Bounds.height))
        Widgets[Id].Hover = true;
    else
        Widgets[Id].Hover = false;
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && Widgets[Id].Hover)
        Widgets[Id].Focused = true;
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !Widgets[Id].Hover)
        Widgets[Id].Focused = false;
    
    if (Widgets[Id].Focused)
        DrawRectangleRec(Bounds, (Color){77, 3, 97, 255});
    else if (Widgets[Id].Hover)
        DrawRectangleRec(Bounds, (Color){3, 55, 97, 255});
    else
        DrawRectangleRec(Bounds, Background);
    
    DrawTextEx(Font.Font, Text, (Vector2){Bounds.x, Bounds.y + (Bounds.height - Font.Size)/2.0f}, (float)Font.Size, 0.0f, TextColor);
    
    if (Widgets[Id].Focused)
    {
        int Key = GetKeyPressed();
        if (Key >= KEY_APOSTROPHE && Key <= KEY_GRAVE)
        {
            if (Key >= KEY_A && Key <= KEY_Z)
            {
                if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
                    AppendChar(Text, BufferSize, Key);
                else
                    AppendChar(Text, BufferSize, Key+32);
            }
            else
            {
                AppendChar(Text, BufferSize, Key);
            }
        }
        if (IsKeyDown(KEY_BACKSPACE))
        {
            double CurrentTime = GetTime();
            if (CurrentTime - BackspaceTimer >= 0.07)
            {
                BackspaceTimer = CurrentTime;
                RemoveChar(Text);
            }
        }
        Vector2 TextDim = MeasureTextEx(Font.Font, Text, (float)Font.Size, 0.0f);
        float CursorPadding = 4.0f;
        float CursorThickness = 4.0f;
        Rectangle CursorRect = {Bounds.x + TextDim.x, Bounds.y + (Bounds.height - Font.Size)/2.0f, CursorThickness, (float)Font.Size};
        DrawRectangleRec(CursorRect, (Color){102, 102, 102, 255});
    }
}