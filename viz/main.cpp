#include "viz.hpp"

using namespace std;

typedef int64_t i64;
typedef uint64_t u64;
#define	u64_MAX		UINT64_MAX
#define	u64_MIN		0

#define SIZE 1000
#define MARGIN 50
#define SPEED 2.0

string basedir = "./";

sf::Texture loadTexture(const string &path) {
	sf::Texture tex;
	if (!tex.loadFromFile(path.c_str())) {
		die("failed to load texture");
	}
	return tex;
}

struct Remaper {
	u64	minX;
	u64	maxX;
	u64	minY;
	u64	maxY;
	u64 diffX;
	u64 diffY;

	Remaper(const map<string, anthill> &antsHills) {
		this->minX = u64_MAX;
		this->minY = u64_MAX;
		this->maxX = u64_MIN;
		this->maxY = u64_MIN;

		for (auto &antHill : antsHills) {
			this->minX = min(this->minX, (u64)antHill.second.pos.first);
			this->minY = min(this->minY, (u64)antHill.second.pos.second);
			this->maxX = max(this->maxX, (u64)antHill.second.pos.first);
			this->maxY = max(this->maxY, (u64)antHill.second.pos.second);
		}

		this->diffX = this->maxX - this->minX;
		if (this->diffX == 0)
			this->diffX = 1;
		this->diffY = this->maxY - this->minY;
		if (this->diffY == 0)
			this->diffY = 1;
	}

	pair<u64, u64> remap(const pair<u64, u64> &pos) {
		return make_pair(
			(pos.first - this->minX) * (SIZE - 2 * MARGIN) / this->diffX + MARGIN,
			(pos.second - this->minY) * (SIZE - 2 * MARGIN) / this->diffY + MARGIN
		);
	}
};

struct Ant {
	sf::Sprite sprite;
	vector<pair<u64, u64>>	sequence;

	Ant(vector<pair<u64, u64>> seq) {
		static sf::Texture texture = loadTexture(basedir +"assets/ant.png");

		this->sprite = sf::Sprite(texture);
		this->sprite.setOrigin(SIZE / 2, SIZE / 2);
		this->sprite.setScale(0.05, 0.05);
		this->sequence = seq;
	}
	void draw(sf::RenderWindow &window, float time) {
		if (time >= this->sequence.size() - 1) {
			time = this->sequence.size() - 1.001;
		}
		if (time < 0) {
			time = 0;
		}
		
		// Do a interpolation/lerp between the two points
		// to get the position of the ant at the given time
		// and draw it
		pair<u64, u64> prev = this->sequence[(int)time];
		pair<u64, u64> next = this->sequence[(int)time + 1];
		float t = time - (int)time;

		float x = prev.first * (1 - t) + next.first * t;
		float y = prev.second * (1 - t) + next.second * t;

		this->sprite.setPosition(x, y);
		window.draw(this->sprite);
	}
};

struct AntHill {
	sf::Sprite sprite;

	AntHill(pair<u64, u64> pos) {
		static sf::Texture texture = loadTexture(basedir + "assets/anthill.png");

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
		static sf::Texture texture = loadTexture(basedir + "assets/path.png");

		this->sprite = sf::Sprite(texture);
		this->sprite.setOrigin(0, SIZE / 2);

		const i64 dx = (i64)to.first - (i64)from.first;
		const i64 dy = (i64)to.second - (i64)from.second;

		this->sprite.setScale(sqrt(pow(dx, 2) + pow(dy, 2)) / SIZE, .02);
		this->sprite.setPosition(from.first, from.second);
		this->sprite.setRotation(atan2(dy, dx) * 180 / M_PI);
	}
	void draw(sf::RenderWindow &window) {
		window.draw(this->sprite);
	}
};

struct Timer {
	sf::Clock clock;
	float runTime;
	bool paused;

	Timer() {
		paused = false;
		runTime = 0;
		clock.restart();
	}

	void reset() {
		clock.restart();
		runTime = 0;
	}

	void start() {
		if (paused) {
			clock.restart();
		}
		paused = false;
	}

	void pause() {
		if (!paused) {
			runTime += clock.getElapsedTime().asSeconds();
		}
		paused = true;
	}

	void toggle() {
		if (paused) {
			start();
		} else {
			pause();
		}
	}

	float getElapsedSeconds() {
		if (!paused) {
			return runTime + clock.getElapsedTime().asSeconds();
		}
		return runTime;
	}
};

int main(int ac, char **av) {
	if (av) {
		basedir = string(dirname(av[0])) + "/";
	}

	map<string, anthill> graph;
	vector<vector<string>> solution;
	string start = "", end = "";
	parse(graph, solution, start, end);

	if (solution.size() == 0) {
		die("no ants");
	}

	dbg(start);
	dbg(end);

	sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "lem-in");

	int n = solution.size(); // number of ants
	int m = graph.size(); // number of nodes

	Remaper remaper(graph);

	vector<AntHill> antHills;
	vector<Path> paths;

	for (auto &it : graph) {
		antHills.push_back(AntHill(remaper.remap(it.second.pos)));
		for (int j = 0; j < it.second.out.size(); j++) {
			if (it.second.out[j] < it.first)
				continue;
			paths.push_back(Path(
				remaper.remap(it.second.pos),
				remaper.remap(graph[it.second.out[j]].pos)
			));
		}
	}

	vector<Ant> ants;
	for (int i = 0; i < n; i++) {
		vector<pair<u64, u64>> seq;

		for (int j = 0; j < solution[i].size(); j++) {
			seq.push_back(remaper.remap(graph[solution[i][j]].pos));
		}

		ants.push_back(Ant(seq));
	}

	sf::Texture backgroundTexture = loadTexture(basedir + "assets/background.png");
	sf::Sprite background(backgroundTexture);

	Timer timer;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					case sf::Keyboard::Space:
						timer.toggle();
						break;
					case sf::Keyboard::Right:
						timer.runTime += 0.2;
						break;
					case sf::Keyboard::Left:
						timer.runTime = fmax(fmin(timer.getElapsedSeconds(), (solution[0].size() - 1) * SPEED) - 0.2, 0)
							- (timer.paused ? 0 : timer.clock.getElapsedTime().asSeconds());
						break;
					case sf::Keyboard::R:
						timer.reset();
						break;
				}
			}
		}

		window.clear(sf::Color(0x937675ff));

		window.draw(background);
		for (auto &path: paths)
			path.draw(window);
		for (auto &antHill : antHills)
			antHill.draw(window);
		for (auto &ant : ants)
			ant.draw(window, timer.getElapsedSeconds() / SPEED);

		window.display();
	}

	return 0;
}