#pragma once
#include "vectortypes.h"
#include <vector>
#include <SDL_stdinc.h>
#include <unordered_set>

struct PairHash {
	inline std::size_t operator()(const std::pair<int, int>& v) const {
		return v.first + 1 * 17 + v.second + 1 * 19;
	}
};

class DrunkardsWalk {
private:
	int													width;
	int													height;

	std::unordered_set<std::pair<int,int>, PairHash>  	visited;

	void												Step(int x, int y);

public:

	DrunkardsWalk(int w, int h);

	~DrunkardsWalk();

	std::vector<std::pair<int, int>> Walk();

	int InBounds(int &x, int &y);
};