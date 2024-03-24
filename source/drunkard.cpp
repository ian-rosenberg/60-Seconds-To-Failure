#include "drunkard.h"
#include <algorithm>

#define	MIN_COVERAGE_PERCENT 0.15f
#define MAX_COVERAGE_PERCENT 0.25f

void DrunkardsWalk::Step(std::vector<std::vector<int>>& map, int x, int y, Coord& lastStep)
{
	Coord prevPair(x, y);
	Coord pair(x, y);
	float coverable = ((width - 1) * (height - 1));
	float coverage = std::clamp(gf2d_random(), MIN_COVERAGE_PERCENT, MAX_COVERAGE_PERCENT);
	float covered = visited.size() / coverable;
	
	while (covered < coverage) {
		pair = prevPair;

		do{
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

			pair.X = std::clamp(pair.X, 1, (int)map[y].size() - 2);
			pair.Y = std::clamp(pair.Y, 1, (int)map.size() - 2);

		}while (!InBounds(pair.X, pair.Y));

		if (!visited.contains(pair)) {
			visited.insert(pair);
			prevPair = pair;
			lastStep = pair;
		}
		else {
			int diffX, diffY;
			diffX = pair.X - prevPair.X;
			diffY = pair.Y - prevPair.Y;

			prevPair = pair;

			pair = { prevPair.X + diffX * -1, prevPair.Y + diffY * -1 };
		}

		covered = visited.size() / coverable;
	}
}

DrunkardsWalk::DrunkardsWalk(int w, int h)
{
	width = w;
	height = h;
	fullMap = false;

	start = { 0,0 };
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
	int cx = 0;
	int cy = 0;
	int curWalk = 1;
	int randSwitch = rand() % 4;

	visited.clear();

	fullMap = start.X == -1 && start.Y == -1;

	if (!fullMap) {
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

		for (int i = start.Y; i < start.Y + height && i < map.size(); i++)
			for (int j = start.X; j < start.X + width && j < map[i].size(); j++)
				map[i][j] = 1;

		cx = std::clamp(cx, 1, width);
		cy = std::clamp(cy, 1, height);
	}
	else{
		switch (randSwitch) {
		case 0:
			cx = 1;
			cy = 1;
			break;
		case 1:
			cx = map[0].size() - 2;
			cy = 1;
			break;
		case 2:
			cx = map[0].size() - 2;
			cy = map.size() - 2;
			break;
		case 3:
			cx = 1;
			cy = map.size() - 2;
			break;
		default: break;
		}	

		cx = std::clamp(cx, 1, (int)map[0].size() - 2);
		cy = std::clamp(cy, 1, (int)map.size() - 2);
	}


	for (int i = 0; i < numIterations; i++) {
		Step(map, cx, cy, lastStep);

		walked.reserve(visited.size());
		for (auto it = visited.begin(); it != visited.end();)
			walked.push_back(std::move(visited.extract(it++).value()));

		cx = lastStep.X;
		cy = lastStep.Y;
	}

	return walked;
}

bool DrunkardsWalk::InBounds(int x, int y)
{
	if(fullMap)
		return x >= 1 && x < width - 2  && y >= 1 && y < height - 2;
	else 
		return x >= 0 && x < width && y >= 0 && y < height;
}




