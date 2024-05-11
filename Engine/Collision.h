#pragma once
#include "vector.h"

namespace Collision
{
	struct Point final
	{
		vector<2> Location;
	};

	struct Circle final
	{
		float     Diameter;
		vector<2> Location;
	};

	struct RectAngle final
	{
		vector<2> Length;
		float Angle;
		vector<2> Location;
	};

	bool Collide(Point const& LHS, Point const& RHS);
	bool Collide(Point const& LHS, Circle const& RHS);
	bool Collide(Point const& LHS, RectAngle const& RHS);
	bool Collide(Circle const& LHS, Point const& RHS);
	bool Collide(Circle const& LHS, Circle const& RHS);
	bool Collide(Circle const& LHS, RectAngle const& RHS);
	bool Collide(RectAngle const& LHS, Point const& RHS);
	bool Collide(RectAngle const& LHS, Circle const& RHS);
	bool Collide(RectAngle const& LHS, RectAngle const& RHS);
}