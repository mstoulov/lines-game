#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

const int cell_number = 9;
const int pixels_in_cell = 100;
const int drop_number = 5;
const int start_drop_number = 4;
const int destruction_number = 4;
const int colors_number = 6;
const int window_size = cell_number * pixels_in_cell + 2;
const int circle_thickness = 2;
int score = 0;

sf::Font font;
sf::Text game_over_text("", font, 100);
sf::Text score_text("", font, 100);

struct cell {
	bool filled;
	sf::Color color;
};


bool Game_Over(cell* field) {
	int sum = 0;
	for (int ii = 0; ii < cell_number; ++ii) {
		for (int iii = 0; iii < cell_number; ++iii) {
			if (!(field + ii * cell_number + iii)->filled) ++sum;
		}
	}
	if (sum >= drop_number) return 0;
	else return 1;
}

bool Potential_Destruction(cell* field, int xx, int yy, sf::Color color) {
	int lines_number = 0, line_length;
	int xx_tmp = xx - 1, yy_tmp = yy;
	sf::Color ball_color = color;

	line_length = 1;
	while (xx_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		--xx_tmp;
	}
	xx_tmp = xx + 1;
	while (xx_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		++xx_tmp;
	}
	if (line_length >= destruction_number) {
		++lines_number;
	}
	line_length = 1;
	xx_tmp = xx;
	yy_tmp = yy - 1;
	while (yy_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		--yy_tmp;
	}
	yy_tmp = yy + 1;
	while (yy_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		++yy_tmp;
	}
	if (line_length >= destruction_number) {
		++lines_number;
	}
	line_length = 1;
	xx_tmp = xx - 1;
	yy_tmp = yy - 1;
	while (xx_tmp >= 0 && yy_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		--xx_tmp;
		--yy_tmp;
	}
	xx_tmp = xx + 1;
	yy_tmp = yy + 1;
	while (yy_tmp < cell_number && xx_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		++yy_tmp;
		++xx_tmp;
	}
	if (line_length >= destruction_number) {
		++lines_number;
	}
	line_length = 1;
	xx_tmp = xx + 1;
	yy_tmp = yy - 1;
	while (xx_tmp < cell_number && yy_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		++xx_tmp;
		--yy_tmp;
	}
	xx_tmp = xx - 1;
	yy_tmp = yy + 1;
	while (xx_tmp >= 0 && yy_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) ++line_length;
		else break;
		--xx_tmp;
		++yy_tmp;
	}
	if (line_length >= destruction_number) {
		++lines_number;
	}
	if (lines_number > 0) return 1;
	else return 0;
}

bool Real_Destruction(cell* field, int xx, int yy) {
	int lines_number = 0, balls_cnt = 0, line_length;
	int xx_tmp = xx - 1, yy_tmp = yy;
	sf::Color ball_color = (field + xx * cell_number + yy)->color;

	line_length = 1;
	while (xx_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "kekhor " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		--xx_tmp;
	}
	xx_tmp = xx + 1;
	while (xx_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "kekhor " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		++xx_tmp;
	}
	if (line_length >= destruction_number) {
		--xx_tmp;
		for (int ii = 0; ii < line_length; ++ii) {
			if (xx_tmp == xx && yy_tmp == yy) {
				//continue;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			} else {
				(field + xx_tmp * cell_number + yy_tmp)->filled = 0;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			}
			--xx_tmp;
		}
		++lines_number;
		balls_cnt += line_length;
	}

	line_length = 1;
	xx_tmp = xx;
	yy_tmp = yy - 1;
	while (yy_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "kekver " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		--yy_tmp;
	}
	yy_tmp = yy + 1;
	while (yy_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "kekver " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		++yy_tmp;
	}
	if (line_length >= destruction_number) {
		--yy_tmp;
		for (int ii = 0; ii < line_length; ++ii) {
			if (xx_tmp == xx && yy_tmp == yy) {
				//continue;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			} else {
				(field + xx_tmp * cell_number + yy_tmp)->filled = 0;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			}
			--yy_tmp;
		}
		++lines_number;
		balls_cnt += line_length;
	}

	line_length = 1;
	xx_tmp = xx - 1;
	yy_tmp = yy - 1;
	while (xx_tmp >= 0 && yy_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "keklv " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		--xx_tmp;
		--yy_tmp;
	}
	xx_tmp = xx + 1;
	yy_tmp = yy + 1;
	while (yy_tmp < cell_number && xx_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			//cout << "keklv " << xx_tmp << " " << yy_tmp << "\n";
			++line_length;
		} else {
			break;
		}
		++yy_tmp;
		++xx_tmp;
	}
	if (line_length >= destruction_number) {
		--xx_tmp;
		--yy_tmp;
		for (int ii = 0; ii < line_length; ++ii) {
			if (xx_tmp == xx && yy_tmp == yy) {
				//continue;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			} else {
				(field + xx_tmp * cell_number + yy_tmp)->filled = 0;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			}
			--yy_tmp;
			--xx_tmp;
		}
		++lines_number;
		balls_cnt += line_length;
	}

	line_length = 1;
	xx_tmp = xx + 1;
	yy_tmp = yy - 1;
	while (xx_tmp < cell_number && yy_tmp >= 0) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "kekrv " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		++xx_tmp;
		--yy_tmp;
	}
	xx_tmp = xx - 1;
	yy_tmp = yy + 1;
	while (xx_tmp >= 0 && yy_tmp < cell_number) {
		if ((field + xx_tmp * cell_number + yy_tmp)->filled && (field + xx_tmp * cell_number + yy_tmp)->color == ball_color) {
			++line_length;
			//cout << "kekrv " << xx_tmp << " " << yy_tmp << "\n";
		}
		else break;
		--xx_tmp;
		++yy_tmp;
	}
	if (line_length >= destruction_number) {
		++xx_tmp;
		--yy_tmp;
		for (int ii = 0; ii < line_length; ++ii) {
			if (yy_tmp == yy && xx_tmp == xx) {
				//continue;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			} else {
				(field + xx_tmp * cell_number + yy_tmp)->filled = 0;
				//cout << xx_tmp << " " << yy_tmp << "\n";
			}
			--yy_tmp;
			++xx_tmp;
		}
		++lines_number;
		balls_cnt += line_length;
	}
	if (lines_number > 0) {
		(field + xx * cell_number + yy)->filled = 0;
	}
	//cout << "eeeeee" << balls_cnt * lines_number << "\n";
	score += balls_cnt * lines_number;
	if (lines_number > 0) return 1;
	else return 0;
}

void Draw(cell* field, sf::RenderWindow* window, bool already_selected, int xx_selected, int yy_selected) {
	window->clear();
	sf::RectangleShape background(sf::Vector2f(window_size, window_size));
	background.setFillColor(sf::Color(250, 250, 250));
	sf::RectangleShape line(sf::Vector2f(window_size, 2));
	line.setFillColor(sf::Color(75, 75, 75));
	sf::CircleShape circle(pixels_in_cell / 2 - (circle_thickness + 1));
	window->draw(background);
	line.setPosition(0, 0);
	line.setRotation(0);
	for (int ii = 0; ii <= window_size; ii += pixels_in_cell) {
		window->draw(line);
		line.move(0, pixels_in_cell);
	}
	line.setRotation(90);
	line.setPosition(2, 0);
	for (int ii = 0; ii <= window_size; ii += pixels_in_cell) {
		window->draw(line);
		line.move(pixels_in_cell, 0);
	}
	for (int xx = 0; xx < cell_number; ++xx) {
		for (int yy = 0; yy < cell_number; ++yy) {
			if ((field + xx * cell_number + yy)->filled) {
				circle.setFillColor((field + xx * cell_number + yy)->color);
				circle.setPosition(4 + xx * pixels_in_cell, 4 + yy * pixels_in_cell);
				if (already_selected && xx == xx_selected && yy == yy_selected) {
					circle.setOutlineThickness(circle_thickness);
					circle.setOutlineColor(sf::Color::Black);
				}
				window->draw(circle);
				circle.setOutlineThickness(0);
			}
		}
	}
	window->draw(game_over_text);
	window->draw(score_text);
	window->display();
	window->setTitle("Lines                      " + to_string(score));
}

void Drop(cell* field, sf::Color* color_set, int drop_number) {
	int xx, yy;
	sf::Color color;
	for (int ii = 0; ii < drop_number; ++ii) {
		xx = rand() % cell_number;
		yy = rand() % cell_number;
		color = color_set[rand() % colors_number];
		if (!((field + xx * cell_number + yy)->filled) &&
			!Potential_Destruction(field, xx, yy, color)) {
			(field + xx * cell_number + yy)->filled = 1;
			(field + xx * cell_number + yy)->color = color;
			//cout << xx << " " << yy << "\n";
			//cout << ((field + xx * yy + yy)->color);
		} else {
			--ii;
		}
	}
}

bool Aimed_Circle(int xx, int yy) {
	if (std::pow(xx % pixels_in_cell - pixels_in_cell / 2, 2) + std::pow(yy % pixels_in_cell - pixels_in_cell / 2, 2) <=
		std::pow(pixels_in_cell / 2 - circle_thickness, 2)) return 1;
	else return 0;
}

bool Make_Way(cell* field, int *way_field, int xx, int yy, int xx_dest, int yy_dest, sf::RenderWindow* window) {
	vector<pair<int, int>> way;
	if (*(way_field + xx_dest * cell_number + yy_dest) >= 10000) {
		//cout << "no path\n";
		return 0;
	} else {
		while (xx != xx_dest || yy != yy_dest) {
			way.push_back(make_pair(xx_dest, yy_dest));
			if (xx_dest > 0) {
				if (*(way_field + (xx_dest - 1) * cell_number + yy_dest) == *(way_field + xx_dest * cell_number + yy_dest) - 1) {
					--xx_dest;
					continue;
				}
			} 
			if (xx_dest < cell_number - 1) {
				if (*(way_field + (xx_dest + 1) * cell_number + yy_dest) == *(way_field + xx_dest * cell_number + yy_dest) - 1) {
					++xx_dest;
					continue;
				}
			}
			if (yy_dest > 0) {
				if (*(way_field + xx_dest * cell_number + yy_dest - 1) == *(way_field + xx_dest * cell_number + yy_dest) - 1) {
					--yy_dest;
					continue;
				}
			}
			if (yy_dest < cell_number - 1) {
				if (*(way_field + xx_dest * cell_number + yy_dest + 1) == *(way_field + xx_dest * cell_number + yy_dest) - 1) {
					++yy_dest;
					continue;
				}
			}
		}
		way.push_back(make_pair(xx_dest, yy_dest));
		for (int ii = way.size() - 2; ii >= 0; --ii) {
			(*(field + way[ii].first * cell_number + way[ii].second)).filled = 1;
			(*(field + way[ii].first * cell_number + way[ii].second)).color = 
				(*(field + way[ii + 1].first * cell_number + way[ii + 1].second)).color;
			(*(field + way[ii + 1].first * cell_number + way[ii + 1].second)).filled = 0;
			Draw(field, window, 0, 0, 0);
		}
	}
	return 1;
}

void Make_Way_Field(int xx, int yy, cell* field, int* wayfield, int xx_dest, int yy_dest, int depth) {
	if (xx < 0 || yy < 0 || xx >= cell_number || yy >= cell_number) {
		return;
	}
	if (depth < *(wayfield + xx * cell_number + yy)) {
		*(wayfield + xx * cell_number + yy) = depth;
	}
	if (xx == xx_dest && yy == yy_dest) {
		return;
	}
	if (yy - 1 >= 0) {
		if (!((field + xx * cell_number + yy - 1)->filled) && *(wayfield + xx * cell_number + yy - 1) > depth) {
		    Make_Way_Field(xx, yy - 1, field, wayfield, xx_dest, yy_dest, depth + 1);
	    }
    }
	if (yy + 1 < cell_number) {
		if (!((field + xx * cell_number + yy + 1)->filled) && *(wayfield + xx * cell_number + yy + 1) > depth) {
			Make_Way_Field(xx, yy + 1, field, wayfield, xx_dest, yy_dest, depth + 1);
		}
	}
	if (xx - 1 >= 0) {
		if (!((field + (xx - 1) * cell_number + yy)->filled) && *(wayfield + (xx - 1) * cell_number + yy) > depth) {
			Make_Way_Field(xx - 1, yy, field, wayfield, xx_dest, yy_dest, depth + 1);
		}
	}
	if (xx + 1 < cell_number) {
		if (!((field + (xx + 1) * cell_number + yy)->filled) && *(wayfield + (xx + 1) * cell_number + yy) > depth) {
			Make_Way_Field(xx + 1, yy, field, wayfield, xx_dest, yy_dest, depth + 1);
		}
	}
	return;
}


int main() {
	int xx, xx_pixel, yy_pixel, yy, selected_xx = 0, selected_yy = 0;
	bool already_selected = 0;
	int wayfield[cell_number][cell_number];
	sf::Color color_set[colors_number];
	cell field[cell_number][cell_number];
	for (int ii = 0; ii < cell_number; ++ii) {
		for (int iii = 0; iii < cell_number; ++iii) {
			field[ii][iii].filled = 0;
		}
	}
	srand(static_cast<unsigned int>(time(0)));
	for (int ii = 0; ii < colors_number; ++ii) {
		color_set[ii] = sf::Color(rand() % 256, rand() % 256, rand() % 256);
	}
	color_set[0] = sf::Color::Blue;
	color_set[1] = sf::Color::Green;
	color_set[2] = sf::Color::Cyan;
	color_set[3] = sf::Color::Magenta;
	color_set[4] = sf::Color::Red;
	color_set[5] = sf::Color::Yellow;
	//color_set[6] = sf::Color::Yellow;

	font.loadFromFile("font.ttf");
	game_over_text.setPosition(100, 100);
	game_over_text.setFillColor(sf::Color::Black);
	score_text.setPosition(100, 300);
	score_text.setFillColor(sf::Color::Black);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(window_size, window_size), "Lines",sf::Style::Default, settings);
	window.setFramerateLimit(10);
	window.setKeyRepeatEnabled(0);
	sf::RectangleShape background(sf::Vector2f(window_size, window_size));
	background.setFillColor(sf::Color(240, 240, 240));
	sf::RectangleShape line(sf::Vector2f(window_size, 2));
	line.setFillColor(sf::Color(75, 75, 75));
	sf::CircleShape circle(pixels_in_cell / 2 - (circle_thickness + 1));
	circle.setOutlineThickness(3);
	circle.setOutlineColor(sf::Color::Black);
	
	Drop(&field[0][0], &color_set[0], start_drop_number);
	Draw(&field[0][0], &window, already_selected, selected_xx, selected_yy);
	//system("pause");
	
	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				xx_pixel = event.mouseButton.x;
				yy_pixel = event.mouseButton.y;
				xx = xx_pixel / pixels_in_cell;
				yy = yy_pixel / pixels_in_cell;
				if (field[xx][yy].filled) {
					if (Aimed_Circle(xx_pixel, yy_pixel)) {
						already_selected = 1;
						selected_xx = xx;
						selected_yy = yy;
						//cout << "yes\n";
					}
				} else if (already_selected) {
					for (int ii = 0; ii < cell_number; ++ii) {
						for (int iii = 0; iii < cell_number; ++iii) {
							wayfield[ii][iii] = 10000;
						}
					}
					Make_Way_Field(selected_xx, selected_yy, &field[0][0], wayfield[0], xx, yy, 0);
					if (Make_Way(&field[0][0], wayfield[0], selected_xx, selected_yy, xx, yy, &window)) {
						if (!Real_Destruction(&field[0][0], xx, yy)) {
							Drop(&field[0][0], &color_set[0], drop_number);
						}
					}
					already_selected = 0;
				}
			}
			if (Game_Over(&field[0][0])) {
				game_over_text.setString("Game Over!");
				score_text.setString("score:  " + to_string(score));
			}
			Draw(&field[0][0], &window, already_selected, selected_xx, selected_yy);
		}
	}
	return 0;
}


