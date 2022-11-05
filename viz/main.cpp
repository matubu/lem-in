#include "viz.hpp"

using namespace std;

typedef uint64_t u64;

#define SIZE 1000

void die(const char *msg) {
	std::cerr << "\x1b[91merror\x1b[0m" << msg << std::endl;
	exit(1);
}

sf::Texture loadTexture(const char *path) {
	sf::Texture tex;
	if (!tex.loadFromFile(path))
		die("failed to load texture");
	return tex;
}

struct Remaper {
	u64	minX;
	u64	maxX;
	u64	minY;
	u64	maxY;

	Remaper(vector<pair<u64, u64>> antHills) {
		// this->minX = ;
		// this->minY = ;
		// this->maxX = ;
		// this->maxY = ;
	}

	pair<u64, u64> remap(u64 x, u64 y) {
		return make_pair(
			(x - this->minX) * SIZE / (this->maxX - this->minX),
			(y - this->minY) * SIZE / (this->maxY - this->minY)
		);
	}
};

struct Ant {
	sf::Sprite sprite;
	vector<pair<u64, u64>>	sequence;

	Ant(vector<pair<u64, u64>> seq) {
		static sf::Texture texture = loadTexture("assets/ant.png");

		this->sprite = sf::Sprite(texture);
		this->sprite.setOrigin(SIZE / 2, SIZE / 2);
		this->sprite.setScale(0.05, 0.05);
		this->sequence = seq;
	}
	void draw(sf::RenderWindow &window, double time) {
		if (time >= this->sequence.size() || time < 0)
			return;
		
		// Do a interpolation/lerp between the two points
		// to get the position of the ant at the given time
		// and draw it
		pair<u64, u64> prev = this->sequence[(int)time];
		pair<u64, u64> next = this->sequence[(int)time + 1];
		double t = time - (int)time;

		double x = prev.first * (1 - t) + next.first * t;
		double y = prev.second * (1 - t) + next.second * t;

		this->sprite.setPosition(x, y);
		window.draw(this->sprite);
	}
};

struct AntHill {
	sf::Sprite sprite;

	AntHill(pair<u64, u64> pos) {
		static sf::Texture texture = loadTexture("assets/anthill.png");

		this->sprite = sf::Sprite(texture);
		this->sprite.setOrigin(SIZE / 2, SIZE / 2);
		this->sprite.setPosition(pos.first, pos.second);
		this->sprite.setScale(0.05, 0.05);
	}
	void draw(sf::RenderWindow &window) {
		window.draw(this->sprite);
	}
};

struct Path {
	sf::Sprite sprite;

	Path(pair<u64, u64> from, pair<u64, u64> to) {
		static sf::Texture texture = loadTexture("assets/path.png");

		this->sprite = sf::Sprite(texture);
		this->sprite.setOrigin(0, SIZE / 2);
		this->sprite.setScale(sqrt(pow(to.first - from.first, 2) + pow(to.second - from.second, 2)) / SIZE, .02);
		this->sprite.setPosition(from.first, from.second);
		this->sprite.setRotation(atan2(to.second - from.second, to.first - from.first) * 180 / M_PI);
	}
	void draw(sf::RenderWindow &window) {
		window.draw(this->sprite);
	}
};

int main()
{
	map<string, anthill> graph;
	vector<vector<string>> paths;
	string start, end;
	parse(graph, paths, start, end);

	sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "lem-in");

	vector<AntHill> antHills = {
		AntHill(make_pair(200, 200)),
		AntHill(make_pair(300, 350))
	};
	Path path(make_pair(200, 200), make_pair(300, 350));
	Ant ant(vector<pair<u64, u64>>{
		make_pair(200, 200),
		make_pair(300, 350)
	});

	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(0x937675ff));

		path.draw(window);
		for (auto &antHill : antHills)
			antHill.draw(window);
		ant.draw(window, clock.getElapsedTime().asSeconds() / 2);

		window.display();
	}

	return 0;
}