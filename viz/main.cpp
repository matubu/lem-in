#include "viz.hpp"

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

int main() {

	map<string, anthill> graph;
	vector<vector<string>> paths;
	string start, end;
	parse(graph, paths, start, end);



	sf::RenderWindow window(sf::VideoMode(800, 800), "lem-in");

	sf::Texture antTex = loadTexture("assets/ant.png");
	sf::Sprite ant;
	ant.setTexture(antTex);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(0x937675ff));

		ant.setScale(0.1, 0.1);
		ant.setPosition(sf::Vector2f(400, 400));
		window.draw(ant);

		window.display();
	}

	return 0;
}