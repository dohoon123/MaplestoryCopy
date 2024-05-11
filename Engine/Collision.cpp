#define _USE_MATH_DEFINES

#include "Collision.h"

namespace Collision
{

	inline matrix<2, 2> Rotation(float const& angle)
	{
		float constexpr radian = static_cast<float>(M_PI) / 180.0f;

		return matrix<2, 2>
		{
			+cos(angle * radian), +sin(angle * radian),
			-sin(angle * radian), +cos(angle * radian)
		};
	}

	bool Collide(Point const& LHS, Point const& RHS)
	{ return (length(LHS.Location - RHS.Location) == 0); }

	bool Collide(Point const& LHS, Circle const& RHS)
	{ return length(LHS.Location - RHS.Location) <= RHS.Diameter / 2; }
	
	bool Collide(Circle const& LHS, Point const& RHS)
	{ return Collide(RHS, LHS); }
	
	bool Collide(Point const& LHS, RectAngle const& RHS)
	{
		if (RHS.Angle == 0)
		{
			vector<2> const min = RHS.Location - RHS.Length / 2;
			vector<2> const max = RHS.Location + RHS.Length / 2;

			return
				(
					min[0] <= LHS.Location[0] and LHS.Location[0] <= max[0] and
					min[1] <= LHS.Location[1] and LHS.Location[1] <= max[1]
				);
		}
		else
		{
			return Collide
			(
				Point    {                LHS.Location * Rotation(-RHS.Angle) },
				RectAngle{ RHS.Length, 0, RHS.Location * Rotation(-RHS.Angle) }
			);
		}
	}
	
	bool Collide(RectAngle const& LHS, Point const& RHS)
	{ return Collide(RHS, LHS); }
	
	bool Collide(Circle const& LHS, Circle const& RHS)
	{ return length(LHS.Location - RHS.Location) <= (LHS.Diameter + RHS.Diameter) / 2; }
	
	bool Collide(Circle const& LHS, RectAngle const& RHS)
	{
		if (RHS.Angle == 0)
		{
			vector<2> const min = RHS.Location - RHS.Length / 2;
			vector<2> const max = RHS.Location + RHS.Length / 2;

			if (
				(min[0] <= LHS.Location[0] and LHS.Location[0] <= max[0]) or
				(min[1] <= LHS.Location[1] and LHS.Location[1] <= max[1])
			   )
			{
				return Collide
				(
					Point    {												   LHS.Location },
					RectAngle{ RHS.Length + vector<2>(1, 1) * LHS.Diameter, 0, RHS.Location }
				);
			}
			else
			{
				Point target = Point();
				     if (LHS.Location[0] >= RHS.Location[0] and LHS.Location[1] >= RHS.Location[1]) target.Location = { max[0], max[1] };
				else if (LHS.Location[0] <= RHS.Location[0] and LHS.Location[1] >= RHS.Location[1]) target.Location = { min[0], max[1] };
				else if (LHS.Location[0] <= RHS.Location[0] and LHS.Location[1] <= RHS.Location[1]) target.Location = { min[0], min[1] };
				else if (LHS.Location[0] >= RHS.Location[0] and LHS.Location[1] <= RHS.Location[1]) target.Location = { max[0], min[1] };

				return Collide(LHS, target);
			}
		}
		else
		{
			return Collide
			(
				Circle    { LHS.Diameter,  LHS.Location * Rotation(-RHS.Angle) },
				RectAngle { RHS.Length, 0, RHS.Location * Rotation(-RHS.Angle) }
			);
		}
	}

	bool Collide(RectAngle const& LHS, Circle const& RHS)
	{ return Collide(RHS, LHS); }
	
	bool Collide(RectAngle const& LHS, RectAngle const& RHS)
	{
		if (LHS.Angle == 0.0f and RHS.Angle == 0.0f)
		{
			vector<2> const min[2]
			{
				LHS.Location - LHS.Length / 2,
				RHS.Location - RHS.Length / 2
			};

			vector<2> const max[2]
			{
				LHS.Location + LHS.Length / 2,
				RHS.Location + RHS.Length / 2
			};

			return
				(
					min[0][0] <= max[1][0] and min[1][0] <= max[0][0] and
					min[0][1] <= max[1][1] and min[1][1] <= max[0][1]

				);
		}
		else if (abs(LHS.Angle - RHS.Angle) < 0.1f)
		{
			return Collide(
				RectAngle{ LHS.Length, 0, LHS.Location * Rotation(-RHS.Angle) },
				RectAngle{ RHS.Length, 0, RHS.Location * Rotation(-RHS.Angle) }
			);
		}
		else 
		{
			vector<2> const distance = LHS.Location - RHS.Location;

			if (length((LHS.Length) + length(RHS.Length)) / 2 < length(distance)) { return false; }

			vector<2> const axes[4]
			{
				vector<2>(1, 0) * Rotation(LHS.Angle),
				vector<2>(0, 1) * Rotation(LHS.Angle),
				vector<2>(1, 0) * Rotation(RHS.Angle),
				vector<2>(0, 1) * Rotation(RHS.Angle)
			};

			vector<2> const vectors[4]
			{
				axes[0] * (LHS.Length[0] / 2),
				axes[1] * (LHS.Length[1] / 2),
				axes[2] * (RHS.Length[0] / 2),
				axes[3] * (RHS.Length[1] / 2)
			};

			for (unsigned i = 0; i < 4; ++i)
			{
				float sum = 0;

				for (unsigned j = 0; j < 4; ++j)
				{
					sum += abs(dot(axes[i], vectors[j]));
				}

				if (sum < abs(dot(axes[i], distance))) {
					return false;
				}

			}

			return true;
		}
	}
}
