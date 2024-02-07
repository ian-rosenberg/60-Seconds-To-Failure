#pragma once
#include "vectortypes.h"
#include <vector>
#include <SDL_stdinc.h>
#include <unordered_set>

typedef struct Coord {
	int X;
	int Y;

	auto operator<=>(const Coord&) const = default;
}Coord_S;

struct PairHash {
	inline std::size_t operator()(const Coord& v) const {
		return v.X + 1 * 17 + v.Y + 1 * 19;
	}
};

class DrunkardsWalk {
private:
	int													width;
	int													height;
	
	Coord												start;

	bool												fullMap;
	
	std::unordered_set<Coord, PairHash>  				visited;

	void												Step(std::vector<std::vector<int>>& map, int x, int y, Coord &lastStep);
public:

	DrunkardsWalk(int w, int h);

	~DrunkardsWalk();

	std::vector<Coord> Walk(int numIterations, std::vector<std::vector<int>>& map, Coord Start);

	bool InBounds(int x, int y);
};