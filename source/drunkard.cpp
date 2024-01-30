#include "drunkard.h"
#include <algorithm>

void DrunkardsWalk::Step(int x, int y, Coord& lastStep)
{
	Coord prevPair(x, y);
	Coord pair(x, y);
	float coverable = ((width - 1) * (height - 1));
	float coverage = std::clamp(gf2d_random(), 0.05f, 0.2f);
	float covered = visited.size() / coverable;
	
	while (covered < coverage) {
		do {
			switch ((int)(gf2d_random() * 10) % 4) {
			case 0:
				pair.X += 1;
				break;
			case 1:
				pair.X -= 1;
				break;
			case 2:
				if (gf2d_random() > 0.5f)
					pair.Y += 1;
				else
					pair.X += (int)gf2d_crandom();
				break;
			case 3:
				if (gf2d_random() > 0.5f)
					pair.Y -= 1;
				else
					pair.X += (int)gf2d_crandom();
				break;
			default:break;
			}

			pair.X = std::clamp(pair.X, 1, width - 1);
			pair.Y = std::clamp(pair.Y, 1, height - 1);

		} while (!InBounds(pair.X, pair.Y));

		if (!visited.contains(pair)) {
			visited.insert(pair);
			prevPair = pair;
			lastStep = pair;
		}
		else {
			int diffX, diffY;
			diffX = pair.X - prevPair.X;
			diffY = pair.Y - prevPair.Y;

			prevPair = { prevPair.X + diffX * -1, prevPair.Y + diffY * -1 };
		}

		covered = visited.size() / coverable;
	}
}

DrunkardsWalk::DrunkardsWalk(int w, int h)
{
	width = w;
	height = h;
	fullMap = false;
}

DrunkardsWalk::~DrunkardsWalk()
{
	visited.clear();
}

std::vector<Coord> DrunkardsWalk::Walk(int numIterations, std::vector<std::vector<int>>& map, Coord start)
{	
	std::vector<Coord> walked;
	Coord lastStep(0, 0);
	int corner = ceil((int)(gf2d_random() * 10) % 4);
	int cx = start.X < 0 ? rand() % width : start.X;
	int cy = start.Y < 0 ? rand() % height : start.Y;
	int curWalk = 1;
	int randSwitch = rand() % 4;

	fullMap = start.X == -1 && start.Y == -1;

	if (fullMap) {
		switch (randSwitch) {
		case 0:
			cx = start.X;
			cy = start.Y;
			break;
		case 1:
			cx = start.X + width - 1;
			cy = start.Y;
			break;
		case 2:
			cx = start.X + width - 1;
			cy = start.Y + height - 1;
			break;
		case 3:
			cx = start.X;
			cy = start.Y + height - 1;
			break;
		default: break;
		}
	}

	for (int i = 0; i < numIterations; i++) {
		Step(cx, cy, lastStep);

		walked.reserve(visited.size());
		for (auto it = visited.begin(); it != visited.end();)
			walked.push_back(std::move(visited.extract(it++).value()));

		cx = lastStep.X;
		cy = lastStep.Y;

	}

	if(!map.empty())
		for (Coord c : walked)
			map[c.Y][c.X] = 0;

	return walked;
}

bool DrunkardsWalk::InBounds(int x, int y)
{
	if(fullMap)
		return x >= 1 && x < width - 1  && y >= 1 && y < height - 1;
	else 
		return x >= 0 && x < width && y >= 0 && y < height;
}




