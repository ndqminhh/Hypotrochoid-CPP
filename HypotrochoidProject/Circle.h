#pragma once

#include "raylib.h"
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

class Circle
{
private:
	struct LineSegment
	{
		Vector2 start;
		Vector2 end;
	};

	int bigRadius = 150;
	int smallRadius = 55;
	int penDistance = 70;

	Color drawColor = RED;

	float angle = 0.0f;
	float angleStep = 0.012f;
	float drawingSpeed = 0.1f;
	float stepAccumulator = 0.0f;

	bool paused = false;
	bool finished = false;
	bool hasPreviousPoint = false;

	Vector2 previousPoint = { 0, 0 };
	vector<LineSegment> lines;

	static constexpr float PI_VALUE = 3.14159265358979323846f;

	static constexpr float CENTER_X = 295.0f;
	static constexpr float CENTER_Y = 250.0f;

	float GetScale() const
	{
		float extent = static_cast<float> (bigRadius - smallRadius + penDistance);

		return 220.0f / max(extent, 1.0f);
	}

	Vector2 CalculatePoint(float t) const
	{
		float difference = static_cast<float>(bigRadius - smallRadius);
		float ratio = difference / smallRadius;
		float scale = GetScale();

		float x = difference * cosf(t) + penDistance * cosf(ratio * t);

		float y = difference * sinf(t) - penDistance * sinf(ratio * t);

		return { CENTER_X + x * scale,CENTER_Y + y * scale };
	}

public:
	void SetBigRadius(int radius)
	{
		radius = clamp(radius, 30, 190);

		if (bigRadius != radius)
		{
			bigRadius = radius;

			if (smallRadius >= bigRadius)
				smallRadius = bigRadius - 5;

			Clear();
		}
	}

	void SetSmallRadius(int radius)
	{
		radius = clamp(radius, 5, bigRadius - 5);

		if (smallRadius != radius)
		{
			smallRadius = radius;
			Clear();
		}
	}

	void SetPenDistance(int distance)
	{
		distance = clamp(distance, 0, 150);

		if (penDistance != distance)
		{
			penDistance = distance;
			Clear();
		}
	}

	void SetColor(Color color)
	{
		if (drawColor.r != color.r || drawColor.g != color.g || drawColor.b != color.b || drawColor.a != color.a)
		{
			drawColor = color;
			Clear();
		}
	}

	int GetBigRadius() const { return bigRadius; }
	int GetSmallRadius() const { return smallRadius; }
	int GetPenDistance() const { return penDistance; }

	bool IsPaused() const { return paused; }
	bool IsFinished() const { return finished; }

	void TogglePause()
	{
		paused = !paused;
	}

	void Clear()
	{
		lines.clear();
		angle = 0.0f;
		hasPreviousPoint = false;
		finished = false;
		paused = false;
		stepAccumulator = 0.0f;
	}

	void Update()
	{
		if (paused || finished)
			return;

		int divisor = gcd(bigRadius, smallRadius);

		float maxAngle = 2.0f * PI_VALUE * smallRadius / divisor;

		stepAccumulator += 100.0f * drawingSpeed;

		int stepsPerFrame = (int)stepAccumulator;
		stepAccumulator -= stepsPerFrame;

		for (int i = 0; i < stepsPerFrame; i++)
		{
			float nextAngle = min(angle + angleStep, maxAngle);

			Vector2 currentPoint = CalculatePoint(nextAngle);

			if (hasPreviousPoint)
			{
				lines.push_back({ previousPoint,currentPoint });
			}

			previousPoint = currentPoint;
			hasPreviousPoint = true;
			angle = nextAngle;

			if (angle >= maxAngle)
			{
				finished = true;
				break;
			}
		}
	}

	void Draw() const
	{
		for (const LineSegment& line : lines)
		{
			DrawLineEx(line.start, line.end, 2.0f, drawColor);
		}
	}


	void DrawGuides() const
	{
		float scale = GetScale();

		Vector2 bigCenter = { CENTER_X,CENTER_Y };

		float distance = bigRadius - smallRadius;

		Vector2 smallCenter = { CENTER_X + distance * cosf(angle) * scale,CENTER_Y + distance * sinf(angle) * scale };

		Vector2 penPoint = CalculatePoint(angle);

		DrawCircleLines((int)bigCenter.x, (int)bigCenter.y, bigRadius * scale, BLUE);

		DrawCircleLines((int)smallCenter.x, (int)smallCenter.y, smallRadius * scale, BLACK);

		DrawCircleV(smallCenter, 4, BLACK);

		DrawLineEx(smallCenter, penPoint, 2, BLACK);

		DrawCircleV(penPoint, 5, RED);
	}

	void SetDrawingSpeed(float speed)
	{
		drawingSpeed = clamp(speed, 0.01f, 2.0f);
	}

	float GetDrawingSpeed() const
	{
		return drawingSpeed;
	}
};