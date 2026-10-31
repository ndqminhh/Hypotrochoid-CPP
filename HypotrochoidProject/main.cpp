
#include "raylib.h"
#include "Circle.h"
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;

int DrawSlider(int x, int y, int width, const char* label, int value, int minValue, int maxValue)
{
    DrawText(label, x, y - 25, 16, DARKGRAY);
    DrawText(TextFormat("%d", value), x + width - 35, y - 25, 16, BLACK);

    Rectangle area = { (float)x - 5, (float)y - 10,(float)width + 10, 32.0f };

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, area) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        float fraction = (mouse.x - x) / width;
        fraction = std::clamp(fraction, 0.0f, 1.0f);

        value = minValue + (int)(fraction * (maxValue - minValue) + 0.5f);
    }

    float fraction = (float)(value - minValue) / (maxValue - minValue);

    DrawRectangle(x, y, width, 10, LIGHTGRAY);
    DrawRectangle(x, y, (int)(width * fraction), 10, SKYBLUE);

    DrawCircle(x + (int)(width * fraction), y + 5, 8, BLUE);

    return value;
}

int main()
{
    InitWindow(800, 500, "Hypotrochoid Drawing Tool");
    SetTargetFPS(60);

    Circle circle;

    circle.SetBigRadius(120);
    circle.SetSmallRadius(55);
    circle.SetPenDistance(80);

    Color colors[5] = { RED, BLUE, GREEN, PURPLE, BLACK };

    int colorIndex = 0;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_C))
            circle.Clear();

        if (IsKeyPressed(KEY_SPACE))
            circle.TogglePause();

        if (IsKeyPressed(KEY_Q))
            circle.SetBigRadius(circle.GetBigRadius() + 5);

        if (IsKeyPressed(KEY_A))
            circle.SetBigRadius(circle.GetBigRadius() - 5);

        if (IsKeyPressed(KEY_W))
            circle.SetSmallRadius(circle.GetSmallRadius() + 5);

        if (IsKeyPressed(KEY_S))
            circle.SetSmallRadius(circle.GetSmallRadius() - 5);

        if (IsKeyPressed(KEY_E))
            circle.SetPenDistance(circle.GetPenDistance() + 5);

        if (IsKeyPressed(KEY_D))
            circle.SetPenDistance(circle.GetPenDistance() - 5);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, 590, 500, Color{ 235, 235, 235, 255 });

        DrawRectangle(590, 0, 210, 500, Color{ 245, 245, 245, 255 });

        DrawLine(590, 0, 590, 500, GRAY);

        DrawText("Hypotrochoid", 610, 20, 20, BLACK);

        int R = DrawSlider(610, 90, 165, "Big Circle", circle.GetBigRadius(), 80, 190);

        circle.SetBigRadius(R);

        int r = DrawSlider(610, 160, 165, "Small Circle", circle.GetSmallRadius(), 10, circle.GetBigRadius() - 5);

        circle.SetSmallRadius(r);

        int d = DrawSlider(610, 230, 165, "Pen Distance", circle.GetPenDistance(), 0, 150);

        circle.SetPenDistance(d);

        int speed = DrawSlider(610, 300, 165, "Drawing Speed", (int)(circle.GetDrawingSpeed() * 100.0f + 0.5f), 1, 200);

        circle.SetDrawingSpeed(speed / 100.0f);

        DrawText("Drawing Color", 610, 350, 16, DARKGRAY);

        for (int i = 0; i < 5; i++)
        {
            int x = 624 + i * 34;
            int y = 390;

            DrawCircle(x, y, 12, colors[i]);

            if (i == colorIndex)
                DrawCircleLines(x, y, 15, BLACK);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointCircle(GetMousePosition(), { (float)x, (float)y }, 14))
            {
                colorIndex = i;
            }
        }

        circle.SetColor(colors[colorIndex]);

        circle.Update();

        circle.Draw();
        circle.DrawGuides();

        DrawText("C - Clear | SPACE - Pause", 610, 430, 13, BLACK);
        DrawText("Q/A - Big | W/S - Small", 610, 450, 13, DARKGRAY);
        DrawText("E/D - Pen distance", 610, 470, 13, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
