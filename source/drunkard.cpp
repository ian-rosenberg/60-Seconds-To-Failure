#include "drunkard.h"
#include <algorithm>

void DrunkardsWalk::Step(int x, int y)
{
	std::pair<int, int> pair(x, y);
	int chg;
	float coverable = ((width - 1) * (height - 1));
	float coverage = std::clamp(gf2d_random(), 0.001f, .0625f);
	float covered = visited.size() / coverable;
	
	while (covered < coverage) {

		switch ((int)floor(gf2d_random() * 4)) {
		case 0: 
			pair.first += 1;
			break;
		case 1:
			pair.first -= 1;
			break;
		case 2: 
			if (gf2d_random() > 0.5f)
				pair.second += 1;
			else
				pair.first += (int)gf2d_crandom();
			break;
		case 3:
			if (gf2d_random() > 0.5f)
				pair.second -= 1;
			else
				pair.first += (int)gf2d_crandom();
			break;
		default:break;
		}

		InBounds(pair.first, pair.second);

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

std::vector<std::pair<int, int>> DrunkardsWalk::Walk()
{	
	std::vector<std::pair<int, int>> walked;
	int corner = ceil(gf2d_random() * 4);
	int cx, cy;
	int curWalk = 1;

	switch (corner) {
	case 1:	
		cx = 1;
		cy = 1;
		break;
	case 2:	
		cx = width - 2;
		cy = 1;
		break;
	case 3:	
		cx = width - 2;
		cy = height - 2;
		break;
	case 4:	
		cx = 1;
		cy = height - 2;
		break;
	default:
		cx = width / 2;
		cy = height / 2;
		break;

	}

	Step(ceil(gf2d_random() * width / 4)-1, ceil(gf2d_random() * height)-1);

	walked.reserve(visited.size());
	for (auto it = visited.begin(); it != visited.end();)
		walked.push_back(std::move(visited.extract(it++).value()));

	return walked;
}

int DrunkardsWalk::InBounds(int &x, int &y)
{
	if (x > 0 && x < width && y > 0 && y < height)
		return 0;
	else if ((x < 1 || x >= width - 1)
		&& (y < 1 || y >= height - 1)) {
		x = std::clamp(x, 1, width - 1);
		y = std::clamp(y, 1, height - 1);
		return 1;
	}
	else if (x < 1 || x >= width-1) {
		x = std::clamp(x, 1, width - 1);
		return 2;
	}
	else if (y < 1 || y >= height-1){
		y = std::clamp(y, 1, height - 1);
		return 3;
	}

}




