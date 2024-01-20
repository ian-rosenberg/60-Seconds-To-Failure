#include "drunkard.h"
#include <algorithm>

void DrunkardsWalk::Step(int x, int y)
{
	Coord pair(x, y);
	float coverable = ((width - 1) * (height - 1));
	float coverage = std::clamp(gf2d_random(), 0.05f, 0.1f);
	float covered = visited.size() / coverable;
	
	while (covered < coverage) {

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

		InBounds(pair.X, pair.Y);

		if (!visited.contains(pair))
			visited.insert(pair);

		covered = visited.size() / coverable;
	}
}

DrunkardsWalk::DrunkardsWalk(int w, int h)
{
	width = w;
	height = h;
}

DrunkardsWalk::~DrunkardsWalk()
{
	visited.clear();
}

std::vector<Coord> DrunkardsWalk::Walk()
{	
	std::vector<Coord> walked;
	int corner = ceil((int)(gf2d_random() * 10) % 4);
	int cx, cy;
	int curWalk = 1;

	switch (corner) {
	case 1:	
		cx = rand() % (width / 4);
		cy = rand() % (height / 4);
		break;
	case 2:	
		cx = width - 2;
		cy = 2;
		break;
	case 3:	
		cx = width - 2;
		cy = height - 2;
		break;
	case 4:	
		cx = 2;
		cy = height - 2;
		break;
	default:
		cx = width / 2;
		cy = height / 2;
		break;

	}

	Step(cx, cy);

	walked.reserve(visited.size());
	for (auto it = visited.begin(); it != visited.end();)
		walked.push_back(std::move(visited.extract(it++).value()));

	return walked;
}

std::vector<Coord> DrunkardsWalk::Walk(Coord start)
{	
	std::vector<Coord> walked;

	Step(start.X, start.Y);

	walked.reserve(visited.size());
	for (auto it = visited.begin(); it != visited.end();)
		walked.push_back(std::move(visited.extract(it++).value()));

	return walked;
}

int DrunkardsWalk::InBounds(int &x, int &y)
{
	if (x > 0 && x < width-1 && y > 0 && y < height-1)
		return 0;
	else if ((x < 1 || x >= width - 1)
		&& (y < 1 || y >= height - 1)) {
		x = std::clamp(x, 1, width - 2);
		y = std::clamp(y, 1, height - 2);
		return 1;
	}
	else if (x < 1 || x >= width-1) {
		x = std::clamp(x, 1, width - 2);
		return 2;
	}
	else if (y < 1 || y >= height-1){
		y = std::clamp(y, 1, height - 2);
		return 3;
	}

}




