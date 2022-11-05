#include "viz.hpp"

using namespace std;

typedef uint64_t u64;
#define	u64_MAX		UINT64_MAX
#define	u64_MIN		0

#define SIZE 1000
#define MARGIN 50

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
		this->minX = u64_MAX;
		this->minY = u64_MAX;
		this->maxX = u64_MIN;
		this->maxY = u64_MIN;

		for (auto &antHill : antHills) {
			this->minX = min(this->minX, antHill.first);
			this->minY = min(this->minY, antHill.second);
			this->maxX = max(this->maxX, antHill.first);
			this->maxY = max(this->maxY, antHill.second);
		}
	}

	pair<u64, u64> remap(u64 x, u64 y) {
		return make_pair(
			(x - this->minX + MARGIN) * SIZE / (this->maxX - this->minX - 2 * MARGIN),
			(y - this->minY + MARGIN) * SIZE / (this->maxY - this->minY - 2 * MARGIN)
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

int main()
{
	map<string, anthill> graph;
	vector<vector<string>> paths;
	string start = "", end = "";
	parse(graph, paths, start, end);

	int n = paths.size(); // number of ants
	int m = graph.size(); // number of nodes

	cout << "GRAPH : " << endl;
	for (auto &it : graph) {
		cout << it.first << " " << it.second.pos << ": \n ";
		for (int j = 0; j < it.second.out.size(); j++) {
			cout << it.second.out[j] << " ";
		}
		cout << endl;
	}

	cout << "PATHS : " << endl;
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			cout << paths[i][j] << " ";
		}
		cout << endl;
	}

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

	Timer timer;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				cout << event.key.code << endl;
				switch (event.key.code) {
					case sf::Keyboard::Escape:
						window.close();
						break;
					case sf::Keyboard::Space:
						timer.toggle();
						break;
					case sf::Keyboard::R:
						timer.reset();
						break;
				}
			}
		}

		window.clear(sf::Color(0x937675ff));

		path.draw(window);
		for (auto &antHill : antHills)
			antHill.draw(window);
		ant.draw(window, timer.getElapsedSeconds() / 2);

		window.display();
	}

	return 0;
}