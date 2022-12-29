
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <windows.h>

#include "maps.h"

using namespace sf;
using namespace std;

unsigned short phase = 0; // 0 - начальное меню  1 - подготовительная фаза  2 - бой  3 - окончание боя
int difficulty = 2; // 1 - лёгкая 2 - сложная 3 - невозможная

void menu(RenderWindow & window) {
	Image menu;
	menu.loadFromFile("textures/menubar.bmp");
	menu.createMaskFromColor(Color(255, 255, 255));

	Texture menu_background, menubar;
	menu_background.loadFromFile("textures/menubackgroundjpg.jpg");
	menubar.loadFromImage(menu);


	Sprite menu_bg, exit, start, eazy, hard, impossible;
	menu_bg.setTexture(menu_background);

	exit.setTexture(menubar);
	exit.setTextureRect(IntRect(210,180,130,50));
	exit.setPosition(100,550);
	exit.setColor(Color(0,0,255));

	start.setTexture(menubar);
	start.setTextureRect(IntRect(190, 50, 235, 60));
	start.setPosition(100,400);
	start.setColor(Color(0, 0, 255));

	eazy.setTexture(menubar);
	eazy.setTextureRect(IntRect(190, 350, 115, 40));
	eazy.setPosition(350,700);
	eazy.setColor(Color(0, 0, 255));

	hard.setTexture(menubar);
	hard.setTextureRect(IntRect(350,350,150,40));
	hard.setPosition(550,700);
	hard.setColor(Color(0, 0, 255));

	impossible.setTexture(menubar);
	impossible.setTextureRect(IntRect(550,350,250,40));
	impossible.setPosition(750,700);
	impossible.setColor(Color(0, 0, 255));

	Event event;

	while (phase == 0)
	{
		while (window.pollEvent(event)) {
			window.clear();
			start.setColor(Color::Blue);
			exit.setColor(Color::Blue);
			eazy.setColor(Color::Blue);
			hard.setColor(Color::Blue);
			impossible.setColor(Color::Blue);
			if (difficulty == 1) {
				eazy.setColor(Color::White);
			}
			else if (difficulty == 2) {
				hard.setColor(Color::White);
			}
			else{
				impossible.setColor(Color::White);
			}



			if (event.type == Event::Closed)
				window.close();

			Vector2i pixelPos = Mouse::getPosition(window);
			int difficulty_hover = 0;

			if (eazy.getGlobalBounds().contains(pixelPos.x,pixelPos.y)) { eazy.setColor(Color::White); difficulty_hover = 1; }
			if (hard.getGlobalBounds().contains(pixelPos.x, pixelPos.y)) { hard.setColor(Color::White); difficulty_hover = 2; }
			if (impossible.getGlobalBounds().contains(pixelPos.x, pixelPos.y)) { impossible.setColor(Color::White); difficulty_hover = 3; }
			if (start.getGlobalBounds().contains(pixelPos.x, pixelPos.y)) { start.setColor(Color::Red); difficulty_hover = 4; }
			if (exit.getGlobalBounds().contains(pixelPos.x, pixelPos.y)) { exit.setColor(Color::Red); difficulty_hover = 5; }

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (difficulty_hover == 4) { phase = 1; }//если нажали первую кнопку, то выходим из меню 
				if (difficulty_hover == 1) { difficulty = 1; }
				if (difficulty_hover == 2) { difficulty = 2; }
				if (difficulty_hover == 3) { difficulty = 3; }
				if (difficulty_hover == 5) { window.close(); phase = 5; }

			}

			window.draw(menu_bg);
			window.draw(start);
			window.draw(exit);
			window.draw(eazy);
			window.draw(hard);
			window.draw(impossible);


			window.display();
		}
	}

}

String check_winner(String flag, vector<vector<int>> player_pole, vector<vector<int>> enemy_pole) {
	int enemy_count = 0, player_count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (player_pole[i][j] < 0 && player_pole[i][j] > -5)
				player_count++;
			if (enemy_pole[i][j] < 0 && enemy_pole[i][j] > -5)
				enemy_count++;
		}
	}
	if (enemy_count == 20)
		return "player";
	if (player_count == 20)
		return "enemy";
	return "none";
}


bool player_turn = true;
// параметры поля игрока
int player_grid_cord[2] = { 100,150 };// X, Y
int grid_len = 400;
vector<vector<int>> player_pole{
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
};
vector<vector<int>> live_player_pole{
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
};

// параметры поля противника
int enemy_grid_cord[2] = { 900,150 };// X, Y
vector<vector<int>> visible_enemy_pole{
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
	vector<int>(10,0),
};
vector<vector<int>> real_enemy_pole;


bool is_correct(int x, int y, int len) {
	if (x >= 0 && x < 10 && y >= 0 && y < 10) {
		return true;
	}
	return false;
}

vector<vector<int>> crush_the_ship(vector<vector<int>> pole) {
	for (int i = 0; i <= 9; i++) {
		for (int j = 0; j <= 9; j++) {
			int num = abs(pole[i][j]);
			if ((pole[i][j] < 0 && pole[i][j] > -10 && is_correct(j + num - 1, i, 10) && pole[i][j + num - 1] == pole[i][j] && num < 3) || (pole[i][j] < 0 && pole[i][j] > -10 && is_correct(j + num - 1, i, 10) && pole[i][j + num - 1] == pole[i][j] && pole[i][j + num - 2] == pole[i][j] && num == 3) || (pole[i][j] < 0 && pole[i][j] > -10 && is_correct(j + num - 1, i, 10) && pole[i][j + num - 1] == pole[i][j] && pole[i][j + num - 2] == pole[i][j] && pole[i][j + num - 3] == pole[i][j] && num == 4)) {
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + num; n++) {
						if (is_correct(m, n, 10) && pole[m][n] != pole[i][j])
							pole[m][n] = -10;
					}
				}
			}
		}
	}
	for (int i = 0; i <= 9; i++) {
		for (int j = 0; j <= 9; j++) {
			int num = abs(pole[j][i]);
			if ((pole[j][i] < 0 && pole[j][i] > -10 && is_correct(i, j + num - 1, 10) && pole[j + num - 1][i] == pole[j][i] && num < 3) || (pole[j][i] < 0 && pole[j][i] > -10 && is_correct(i, j + num - 1, 10) && pole[j + num - 1][i] == pole[j][i] && pole[j + num - 2][i] == pole[j][i] && num == 3) || (pole[j][i] < 0 && pole[j][i] > -10 && is_correct(i, j + num - 1, 10) && pole[j + num - 1][i] == pole[j][i] && pole[j + num - 2][i] == pole[j][i] && pole[j + num - 1][i] == pole[j][i] && pole[j + num - 3][i] == pole[j][i] && num == 4)) {
				for (int m = i - 1; m <= i + 1; m++) {
					for (int n = j - 1; n <= j + num; n++) {
						if (is_correct(m, n, 10) && pole[n][m] != pole[j][i]) {
							pole[n][m] = -10;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << pole[i][j] << " ";
		}
		cout << endl;
	}
	return pole;
}
vector<vector<int>> delete_spaces(vector<vector<int>> pole, int num, int x, int y, float rotation) {
	if (rotation == 0) {
		for (int i = y - 1; i <= y + 1; i++) {
			for (int j = x - 1; j <= x + num; j++) {
				if (is_correct(j, i, 10))
					pole[i][j] += 10;
			}
		}
	}
	else {
		for (int i = y - 1; i <= y + num; i++) {
			for (int j = x - 1; j <= x + 1; j++) {
				if (is_correct(j, i, 10))
					pole[i][j] += 10;
			}
		}
	}
	return pole;
}

vector<vector<int>> set_spaces(vector<vector<int>> pole, int num, int x, int y, float rotation) {
	if (rotation == 0) {
		for (int i = y - 1; i <= y + 1; i++) {
			for (int j = x - 1; j <= x + num; j++) {
				if (is_correct(j, i, 10) && abs(pole[i][j]) != num)
					pole[i][j] -= 10;
			}
		}
	}
	else {
		for (int i = y - 1; i <= y + num; i++) {
			for (int j = x - 1; j <= x + 1; j++) {
				if (is_correct(j, i, 10) && abs(pole[i][j]) != num)
					pole[i][j] -= 10;
			}
		}
	}
	return pole;
}


bool pole_is_ready(vector<vector<int>> pole) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (pole[i][j] > 0)
				count++;
		}
	}
	if (count == 20)
		return true;
	return false;
}



vector<vector<int>> pole_to_battle(vector<vector<int>> pole) {
	int count = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (pole[i][j] <= -10)
				pole[i][j] = 0;
		}
	}
	return pole;
}




vector<vector<int>> hit_shot(vector<vector<int>> pole, int cords[2]) {
	int num = pole[cords[1]][cords[0]];
	num = 0 - num;
	pole[cords[1]][cords[0]] = num;
	return pole;
}

vector<vector<int>> miss_shot(vector<vector<int>> pole, int cords[2]) {
	pole[cords[1]][cords[0]] = -10;
	return pole;
}

vector<vector<int>> choose_map_to_battle(vector<vector<vector<int>>> maps) {
	srand(time(NULL));
	int x = rand() % maps.size();
	return maps[x];
}

vector<vector<int>> shot(int num_pole, vector<vector<int>> pole, int cords[2]) {
	if (pole[cords[1]][cords[0]] > 0) {
		pole = hit_shot(pole, cords);
		pole = crush_the_ship(pole);
	}
	else if (pole[cords[1]][cords[0]] == 0) {
		pole = miss_shot(pole, cords);
		(num_pole == 1) ? player_turn = true : player_turn = false;

	}
	//cout << 1;
	return pole;
}

vector<vector<int>> eazy(vector<vector<int>> pole) {
	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		if (pole[i / 10][i % 10] >= 0 && rand() % 20 == 1 || i == 99) {
			int m[2] = { i % 10,i / 10 };
			pole = shot(1, pole, m);
			break;
		}
	}

	return pole;
}

vector<vector<int>> hard(vector<vector<int>> pole) {
	srand(time(NULL));
	int m[2];
	for (int i = 0; i < 100; i++) {
		if (pole[i / 10][i % 10] < 0 && pole[i / 10][i % 10] > -5) {
			m[0] = i % 10;
			m[1] = i / 10;
			if (is_correct(i / 10, i % 10 - 1, 10) && pole[i / 10][i % 10 - 1] >= 0) {
				m[0] -= 1;
				pole = shot(1, pole, m);
				return pole;
			}
			else if (is_correct(i / 10, i % 10 + 1, 10) && pole[i / 10][i % 10 + 1] >= 0) {
				m[0] += 1;
				pole = shot(1, pole, m);
				return pole;
			}
			else if (is_correct(i / 10 - 1, i % 10, 10) && pole[i / 10 - 1][i % 10] >= 0) {
				m[1] -= 1;
				pole = shot(1, pole, m);
				return pole;
			}
			else if (is_correct(i / 10 + 1, i % 10, 10) && pole[i / 10 + 1][i % 10] >= 0) {
				m[1] += 1;
				pole = shot(1, pole, m);
				return pole;
			}
		}
	}
	for (int i = 0; i < 100; i++) {
		if (pole[i / 10][i % 10] >= 0 && rand() % 20 == 1 || i == 99) {
			m[0] = i % 10;
			m[1] = i / 10;
			pole = shot(1, pole, m);
			return pole;
		}
	}
}

vector<vector<int>> imposible(vector<vector<int>> pole) {
	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		if (pole[i / 10][i % 10] > 0) {
			if (rand() % 2 == 1) {
				int m[2] = { i % 10,i / 10 };
				pole = shot(1, pole, m);
				break;
			}
			else {
				for (int i = 0; i < 100; i++) {
					if (pole[i / 10][i % 10] >= 0 && rand() % 2 == 1) {
						int m[2] = { i % 10,i / 10 };
						pole = shot(1, pole, m);
						break;
					}
				}
			}
		}
	}

	return pole;
}

int* ship_pos(int num, int origin_x, int origin_y, float rotation, int grid_len) {
	static int cords[4];
	cords[0] = cords[1] = cords[2] = cords[3] = -1;

	if (num == 1) {
		if (origin_x > 0 && origin_x < grid_len && origin_y > 0 && origin_y < grid_len) {
			cords[0] = origin_x / 40;
			cords[2] = origin_x / 40;
			cords[1] = origin_y / 40;
			cords[3] = origin_y / 40;
		}
	}
	else if (num == 2) {
		if (rotation == 0) {
			origin_x -= 20;
			if (origin_x > 0 && origin_x < grid_len - 40 && origin_y > 0 && origin_y < grid_len) {
				cords[0] = origin_x / 40;
				cords[2] = cords[0] + 1;
				cords[1] = origin_y / 40;
				cords[3] = cords[1];
			}
		}
		else {
			origin_y -= 20;
			if (origin_x > 0 && origin_x < grid_len && origin_y > 0 && origin_y < grid_len - 40) {
				cords[0] = origin_x / 40;
				cords[2] = cords[0];
				cords[1] = origin_y / 40;
				cords[3] = cords[1] + 1;
			}
		}
	}
	else if (num == 3) {
		if (rotation == 0) {
			origin_x -= 40;
			if (origin_x > 0 && origin_x < grid_len - 40 * 2 && origin_y > 0 && origin_y < grid_len) {
				cords[0] = origin_x / 40;
				cords[2] = cords[0] + 2;
				cords[1] = origin_y / 40;
				cords[3] = cords[1];
			}
		}
		else {
			origin_y -= 40;
			if (origin_x > 0 && origin_x < grid_len && origin_y > 0 && origin_y < grid_len - 40 * 2) {
				cords[0] = origin_x / 40;
				cords[2] = cords[0];
				cords[1] = origin_y / 40;
				cords[3] = cords[1] + 2;
			}
		}
	}
	else if (num == 4) {
		if (rotation == 0) {
			origin_x -= 60;
			if (origin_x > 0 && origin_x < grid_len - 40 * 3 && origin_y > 0 && origin_y < grid_len) {
				cords[0] = origin_x / 40;
				cords[2] = cords[0] + 3;
				cords[1] = origin_y / 40;
				cords[3] = cords[1];
			}
		}
		else {
			origin_y -= 60;
			if (origin_x > 0 && origin_x < grid_len && origin_y > 0 && origin_y < grid_len - 40 * 3) {
				cords[0] = origin_x / 40;
				cords[2] = cords[0];
				cords[1] = origin_y / 40;
				cords[3] = cords[1] + 3;
			}
		}
	}

	return cords;
}





int main()
{
	RenderWindow window(VideoMode(1400, 800), "Ship Battle", Style::Close);
	if (phase == 0) {
		menu(window);
	}
	Font font;
	font.loadFromFile("fonts/ArialRegular.ttf");
	Text text;
	text.setFont(font);
	text.setCharacterSize(30);
	text.setStyle(Text::Regular);
	text.setFont(font);
	text.setFillColor(Color::Magenta);

	Text text_player;
	text_player.setFont(font);
	text_player.setCharacterSize(44);
	text_player.setStyle(Text::Bold);
	text_player.setFont(font);
	text_player.setFillColor(Color::Green);
	text_player.setString("YOU");

	Text text_enemy;
	text_enemy.setFont(font);
	text_enemy.setCharacterSize(44);
	text_enemy.setStyle(Text::Bold);
	text_enemy.setFont(font);
	text_enemy.setFillColor(Color::Red);
	text_enemy.setString("ENEMY");

	Text text_vs;
	text_vs.setFont(font);
	text_vs.setCharacterSize(48);
	text_vs.setStyle(Text::Bold);
	text_vs.setFont(font);
	text_vs.setFillColor(Color::White);
	text_vs.setString("VS");

	Text text_preparing_phase;
	text_preparing_phase.setFont(font);
	text_preparing_phase.setCharacterSize(40);
	text_preparing_phase.setStyle(Text::Bold);
	text_preparing_phase.setFont(font);
	text_preparing_phase.setFillColor(Color::Black);
	text_preparing_phase.setString("Preapring Phase");
	text_preparing_phase.setPosition(525, 210);

	Text win;
	win.setFont(font);
	win.setCharacterSize(84);
	win.setStyle(Text::Bold);
	win.setFont(font);
	win.setFillColor(Color::Green);
	win.setString("YOU WON!");
	win.setPosition(490, 250);

	Text lose;
	lose.setFont(font);
	lose.setCharacterSize(84);
	lose.setStyle(Text::Bold);
	lose.setFont(font);
	lose.setFillColor(Color::Red);
	lose.setString("DEFEAT!");
	lose.setPosition(505, 250);

	Text p_turn;
	p_turn.setFont(font);
	p_turn.setCharacterSize(76);
	p_turn.setStyle(Text::Bold);
	p_turn.setFont(font);
	p_turn.setFillColor(Color::Blue);
	p_turn.setString("YOUR TURN");
	p_turn.setPosition(480, 640);


	Text text_end_preparing_phase;
	text_end_preparing_phase.setFont(font);
	text_end_preparing_phase.setCharacterSize(40);
	text_end_preparing_phase.setStyle(Text::Bold);
	text_end_preparing_phase.setFont(font);
	text_end_preparing_phase.setFillColor(Color::Black);
	text_end_preparing_phase.setString("Press Enter to start the battle");
	text_end_preparing_phase.setPosition(400, 715);


	Texture backgound_texture;
	backgound_texture.loadFromFile("textures/background.jpg");

	Sprite backgroud;
	backgroud.setTexture(backgound_texture);

	Image ships;
	ships.loadFromFile("textures/ships.png");
	ships.createMaskFromColor(Color(255, 255, 255));

	Texture ships_texture;
	ships_texture.loadFromImage(ships);

	Sprite ship1_1;
	ship1_1.setTexture(ships_texture);
	ship1_1.setTextureRect(IntRect(110, 152, 40, 40));
	ship1_1.setOrigin(20, 20);
	ship1_1.setPosition(280, 590);

	Sprite ship1_2;
	ship1_2.setTexture(ships_texture);
	ship1_2.setTextureRect(IntRect(110, 152, 40, 40));
	ship1_2.setOrigin(20, 20);
	ship1_2.setPosition(340, 590);

	Sprite ship1_3;
	ship1_3.setTexture(ships_texture);
	ship1_3.setTextureRect(IntRect(110, 152, 40, 40));
	ship1_3.setOrigin(20, 20);
	ship1_3.setPosition(400, 590);

	Sprite ship1_4;
	ship1_4.setTexture(ships_texture);
	ship1_4.setTextureRect(IntRect(110, 152, 40, 40));
	ship1_4.setOrigin(20, 20);
	ship1_4.setPosition(460, 590);

	Sprite ship4;
	ship4.setTexture(ships_texture);
	ship4.setTextureRect(IntRect(245, 240, 160, 40));
	ship4.setOrigin(80, 20);
	ship4.setPosition(120, 590);

	Sprite ship3_1;
	ship3_1.setTexture(ships_texture);
	ship3_1.setTextureRect(IntRect(93, 240, 120, 40));
	ship3_1.setOrigin(60, 20);
	ship3_1.setPosition(100, 650);

	Sprite ship3_2;
	ship3_2.setTexture(ships_texture);
	ship3_2.setTextureRect(IntRect(93, 240, 120, 40));
	ship3_2.setOrigin(60, 20);
	ship3_2.setPosition(250, 650);

	Sprite ship2_1;
	ship2_1.setTexture(ships_texture);
	ship2_1.setTextureRect(IntRect(234, 150, 80, 40));
	ship2_1.setOrigin(40, 20);
	ship2_1.setPosition(100, 700);

	Sprite ship2_2;
	ship2_2.setTexture(ships_texture);
	ship2_2.setTextureRect(IntRect(234, 150, 80, 40));
	ship2_2.setOrigin(40, 20);
	ship2_2.setPosition(200, 700);

	Sprite ship2_3;
	ship2_3.setTexture(ships_texture);
	ship2_3.setTextureRect(IntRect(234, 150, 80, 40));
	ship2_3.setOrigin(40, 20);
	ship2_3.setPosition(300, 700);
	ship2_3.setRotation(0);

	bool is_move1_1 = false, is_move1_2 = false, is_move1_3 = false, is_move1_4 = false, is_move2_1 = false, is_move2_2 = false, is_move2_3 = false, is_move3_1 = false, is_move3_2 = false, is_move4 = false;
	float dX = 0;//корректировка нажатия по х
	float dY = 0;//по у

	

	String winner = "none";


	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sounds/shoot.ogg");
	Sound shoot(shootBuffer);

	SoundBuffer missBuffer;
	missBuffer.loadFromFile("sounds/miss.ogg");
	Sound miss(missBuffer);

	Clock shootTime;
	while (window.isOpen())
	{
		
		Vector2i pixelPos = Mouse::getPosition(window);
		//координаты корабля относительно поля игрока

		int ship1_1_cords_in_grid[2] = { ship1_1.getPosition().x - player_grid_cord[0],ship1_1.getPosition().y - player_grid_cord[1] };
		int ship1_2_cords_in_grid[2] = { ship1_2.getPosition().x - player_grid_cord[0],ship1_2.getPosition().y - player_grid_cord[1] };
		int ship1_3_cords_in_grid[2] = { ship1_3.getPosition().x - player_grid_cord[0],ship1_3.getPosition().y - player_grid_cord[1] };
		int ship1_4_cords_in_grid[2] = { ship1_4.getPosition().x - player_grid_cord[0],ship1_4.getPosition().y - player_grid_cord[1] };
		int ship2_1_cords_in_grid[2] = { ship2_1.getPosition().x - player_grid_cord[0],ship2_1.getPosition().y - player_grid_cord[1] };
		int ship2_2_cords_in_grid[2] = { ship2_2.getPosition().x - player_grid_cord[0],ship2_2.getPosition().y - player_grid_cord[1] };
		int ship2_3_cords_in_grid[2] = { ship2_3.getPosition().x - player_grid_cord[0],ship2_3.getPosition().y - player_grid_cord[1] };
		int ship3_1_cords_in_grid[2] = { ship3_1.getPosition().x - player_grid_cord[0],ship3_1.getPosition().y - player_grid_cord[1] };
		int ship3_2_cords_in_grid[2] = { ship3_2.getPosition().x - player_grid_cord[0],ship3_2.getPosition().y - player_grid_cord[1] };
		int ship4_cords_in_grid[2] = { ship4.getPosition().x - player_grid_cord[0],ship4.getPosition().y - player_grid_cord[1] };

		// координаты кораблей по клеткам
		int cords1_1[4] = { ship_pos(1, ship1_1_cords_in_grid[0], ship1_1_cords_in_grid[1], ship1_1.getRotation(), grid_len)[0],ship_pos(1,ship1_1_cords_in_grid[0],ship1_1_cords_in_grid[1],ship1_1.getRotation(),grid_len)[1],ship_pos(1,ship1_1_cords_in_grid[0],ship1_1_cords_in_grid[1],ship1_1.getRotation(),grid_len)[2],ship_pos(1,ship1_1_cords_in_grid[0],ship1_1_cords_in_grid[1],ship1_1.getRotation(),grid_len)[3] };
		int cords1_2[4] = { ship_pos(1, ship1_2_cords_in_grid[0], ship1_2_cords_in_grid[1], ship1_2.getRotation(), grid_len)[0],ship_pos(1,ship1_2_cords_in_grid[0],ship1_2_cords_in_grid[1],ship1_2.getRotation(),grid_len)[1],ship_pos(1,ship1_2_cords_in_grid[0],ship1_2_cords_in_grid[1],ship1_2.getRotation(),grid_len)[2],ship_pos(1,ship1_2_cords_in_grid[0],ship1_2_cords_in_grid[1],ship1_2.getRotation(),grid_len)[3] };
		int cords1_3[4] = { ship_pos(1, ship1_3_cords_in_grid[0], ship1_3_cords_in_grid[1], ship1_3.getRotation(), grid_len)[0],ship_pos(1,ship1_3_cords_in_grid[0],ship1_3_cords_in_grid[1],ship1_3.getRotation(),grid_len)[1],ship_pos(1,ship1_3_cords_in_grid[0],ship1_3_cords_in_grid[1],ship1_3.getRotation(),grid_len)[2],ship_pos(1,ship1_3_cords_in_grid[0],ship1_3_cords_in_grid[1],ship1_3.getRotation(),grid_len)[3] };
		int cords1_4[4] = { ship_pos(1, ship1_4_cords_in_grid[0], ship1_4_cords_in_grid[1], ship1_4.getRotation(), grid_len)[0],ship_pos(1,ship1_4_cords_in_grid[0],ship1_4_cords_in_grid[1],ship1_4.getRotation(),grid_len)[1],ship_pos(1,ship1_4_cords_in_grid[0],ship1_4_cords_in_grid[1],ship1_4.getRotation(),grid_len)[2],ship_pos(1,ship1_4_cords_in_grid[0],ship1_4_cords_in_grid[1],ship1_4.getRotation(),grid_len)[3] };
		int cords2_1[4] = { ship_pos(2, ship2_1_cords_in_grid[0], ship2_1_cords_in_grid[1], ship2_1.getRotation(), grid_len)[0],ship_pos(2,ship2_1_cords_in_grid[0],ship2_1_cords_in_grid[1],ship2_1.getRotation(),grid_len)[1],ship_pos(2,ship2_1_cords_in_grid[0],ship2_1_cords_in_grid[1],ship2_1.getRotation(),grid_len)[2],ship_pos(2,ship2_1_cords_in_grid[0],ship2_1_cords_in_grid[1],ship2_1.getRotation(),grid_len)[3] };
		int cords2_2[4] = { ship_pos(2, ship2_2_cords_in_grid[0], ship2_2_cords_in_grid[1], ship2_2.getRotation(), grid_len)[0],ship_pos(2,ship2_2_cords_in_grid[0],ship2_2_cords_in_grid[1],ship2_2.getRotation(),grid_len)[1],ship_pos(2,ship2_2_cords_in_grid[0],ship2_2_cords_in_grid[1],ship2_2.getRotation(),grid_len)[2],ship_pos(2,ship2_2_cords_in_grid[0],ship2_2_cords_in_grid[1],ship2_2.getRotation(),grid_len)[3] };
		int cords2_3[4] = { ship_pos(2, ship2_3_cords_in_grid[0], ship2_3_cords_in_grid[1], ship2_3.getRotation(), grid_len)[0],ship_pos(2,ship2_3_cords_in_grid[0],ship2_3_cords_in_grid[1],ship2_3.getRotation(),grid_len)[1],ship_pos(2,ship2_3_cords_in_grid[0],ship2_3_cords_in_grid[1],ship2_3.getRotation(),grid_len)[2],ship_pos(2,ship2_3_cords_in_grid[0],ship2_3_cords_in_grid[1],ship2_3.getRotation(),grid_len)[3] };
		int cords3_1[4] = { ship_pos(3, ship3_1_cords_in_grid[0], ship3_1_cords_in_grid[1], ship3_1.getRotation(), grid_len)[0],ship_pos(3,ship3_1_cords_in_grid[0],ship3_1_cords_in_grid[1],ship3_1.getRotation(),grid_len)[1],ship_pos(3,ship3_1_cords_in_grid[0],ship3_1_cords_in_grid[1],ship3_1.getRotation(),grid_len)[2],ship_pos(3,ship3_1_cords_in_grid[0],ship3_1_cords_in_grid[1],ship3_1.getRotation(),grid_len)[3] };
		int cords3_2[4] = { ship_pos(3, ship3_2_cords_in_grid[0], ship3_2_cords_in_grid[1], ship3_2.getRotation(), grid_len)[0],ship_pos(3,ship3_2_cords_in_grid[0],ship3_2_cords_in_grid[1],ship3_2.getRotation(),grid_len)[1],ship_pos(3,ship3_2_cords_in_grid[0],ship3_2_cords_in_grid[1],ship3_2.getRotation(),grid_len)[2],ship_pos(3,ship3_2_cords_in_grid[0],ship3_2_cords_in_grid[1],ship3_2.getRotation(),grid_len)[3] };
		int cords4[4] = { ship_pos(4, ship4_cords_in_grid[0], ship4_cords_in_grid[1], ship4.getRotation(), grid_len)[0],ship_pos(4,ship4_cords_in_grid[0],ship4_cords_in_grid[1],ship4.getRotation(),grid_len)[1],ship_pos(4,ship4_cords_in_grid[0],ship4_cords_in_grid[1],ship4.getRotation(),grid_len)[2],ship_pos(4,ship4_cords_in_grid[0],ship4_cords_in_grid[1],ship4.getRotation(),grid_len)[3] };



		Event event;
		Clock music;
		bool f = true, enemy_shooting = false;
		
		while (window.pollEvent(event))
		{

			window.clear();
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed) {   //если нажата клавиша мыши
				if (event.key.code == Mouse::Left) {
					// расставление кораблей
					if (ship1_1.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 1 1" << endl;
						is_move1_1 = true;
						if (cords1_1[0] != -1 && cords1_1[1] != -1 && cords1_1[2] != -1 && cords1_1[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 1, cords1_1[0], cords1_1[1], ship1_1.getRotation());
							player_pole[cords1_1[1]][cords1_1[0]] = 0;
						}

					}
					else if (ship1_2.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 1 2" << endl;
						is_move1_2 = true;
						if (cords1_2[0] != -1 && cords1_2[1] != -1 && cords1_2[2] != -1 && cords1_2[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 1, cords1_2[0], cords1_2[1], ship1_2.getRotation());
							player_pole[cords1_2[1]][cords1_2[0]] = 0;
						}
					}
					else if (ship1_3.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 1 3" << endl;
						is_move1_3 = true;
						if (cords1_3[0] != -1 && cords1_3[1] != -1 && cords1_3[2] != -1 && cords1_3[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 1, cords1_2[0], cords1_2[1], ship1_2.getRotation());
							player_pole[cords1_3[1]][cords1_3[0]] = 0;
						}
					}
					else if (ship1_4.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 1 4" << endl;
						is_move1_4 = true;
						if (cords1_4[0] != -1 && cords1_4[1] != -1 && cords1_4[2] != -1 && cords1_4[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 1, cords1_4[0], cords1_4[1], ship1_4.getRotation());
							player_pole[cords1_4[1]][cords1_4[0]] = 0;
						}
					}
					else if (ship2_1.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 2 1" << endl;
						is_move2_1 = true;
						if (cords2_1[0] != -1 && cords2_1[1] != -1 && cords2_1[2] != -1 && cords2_1[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 2, cords2_1[0], cords2_1[1], ship2_1.getRotation());
							if (ship2_1.getRotation() == 0) {
								player_pole[cords2_1[1]][cords2_1[0]] = 0; player_pole[cords2_1[1]][cords2_1[0] + 1] = 0;
							}
							else {
								player_pole[cords2_1[1]][cords2_1[0]] = 0; player_pole[cords2_1[1] + 1][cords2_1[0]] = 0;

							}
						}
					}
					else if (ship2_2.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 2 2" << endl;
						is_move2_2 = true;
						if (cords2_2[0] != -1 && cords2_2[1] != -1 && cords2_2[2] != -1 && cords2_2[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 2, cords2_2[0], cords2_2[1], ship2_2.getRotation());
							if (ship2_2.getRotation() == 0) {
								player_pole[cords2_2[1]][cords2_2[0]] = 0; player_pole[cords2_2[1]][cords2_2[0] + 1] = 0;
							}
							else {
								player_pole[cords2_2[1]][cords2_2[0]] = 0; player_pole[cords2_2[1] + 1][cords2_2[0]] = 0;

							}
						}
					}
					else if (ship2_3.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 2 3" << endl;
						is_move2_3 = true;
						if (cords2_3[0] != -1 && cords2_3[1] != -1 && cords2_3[2] != -1 && cords2_3[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 2, cords2_3[0], cords2_3[1], ship2_3.getRotation());
							if (ship2_3.getRotation() == 0) {
								player_pole[cords2_3[1]][cords2_3[0]] = 0; player_pole[cords2_3[1]][cords2_3[0] + 1] = 0;
							}
							else {
								player_pole[cords2_3[1]][cords2_3[0]] = 0; player_pole[cords2_3[1] + 1][cords2_3[0]] = 0;

							}
						}
					}
					else if (ship3_1.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 3 1" << endl;
						is_move3_1 = true;
						if (cords3_1[0] != -1 && cords3_1[1] != -1 && cords3_1[2] != -1 && cords3_1[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 3, cords3_1[0], cords3_1[1], ship3_1.getRotation());
							if (ship3_1.getRotation() == 0) {
								player_pole[cords3_1[1]][cords3_1[0]] = 0; player_pole[cords3_1[1]][cords3_1[0] + 1] = 0; player_pole[cords3_1[1]][cords3_1[0] + 2] = 0;
							}
							else {
								player_pole[cords3_1[1]][cords3_1[0]] = 0; player_pole[cords3_1[1] + 1][cords3_1[0]] = 0; player_pole[cords3_1[1] + 2][cords3_1[0]] = 0;
							}
						}
					}
					else if (ship3_2.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 3 2" << endl;
						is_move3_2 = true;
						if (cords3_2[0] != -1 && cords3_2[1] != -1 && cords3_2[2] != -1 && cords3_2[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 3, cords3_2[0], cords3_2[1], ship3_2.getRotation());
							if (ship3_2.getRotation() == 0) {
								player_pole[cords3_2[1]][cords3_2[0]] = 0; player_pole[cords3_2[1]][cords3_2[0] + 1] = 0; player_pole[cords3_2[1]][cords3_2[0] + 2] = 0;
							}
							else {
								player_pole[cords3_2[1]][cords3_2[0]] = 0; player_pole[cords3_2[1] + 1][cords3_2[0]] = 0; player_pole[cords3_2[1] + 2][cords3_2[0]] = 0;
							}
						}
					}
					else if (ship4.getGlobalBounds().contains(pixelPos.x, pixelPos.y) && phase == 1) {
						cout << "ship 4" << endl;
						is_move4 = true;
						cout << 555 << endl;
						if (cords4[0] != -1 && cords4[1] != -1 && cords4[2] != -1 && cords4[3] != -1) { // обнуляем предыдущую позицию корабля
							player_pole = delete_spaces(player_pole, 4, cords4[0], cords4[1], ship4.getRotation());
							if (ship4.getRotation() == 0) {
								player_pole[cords4[1]][cords4[0]] = 0; player_pole[cords4[1]][cords4[0] + 1] = 0; player_pole[cords4[1]][cords4[0] + 2] = 0; player_pole[cords4[1]][cords4[0] + 3] = 0;
							}
							else {
								player_pole[cords4[1]][cords4[0]] = 0; player_pole[cords4[1] + 1][cords4[0]] = 0; player_pole[cords4[1] + 2][cords4[0]] = 0; player_pole[cords4[1] + 3][cords4[0]] = 0;
							}
						}
					}
				}
				if (phase == 2) {
					if (player_turn) {
						int click_cords[2] = { -1,-1 };
						int mouse_cords_by_enemy_grid[2] = { pixelPos.x - enemy_grid_cord[0],pixelPos.y - enemy_grid_cord[1] };
						if (mouse_cords_by_enemy_grid[0] > 0 && mouse_cords_by_enemy_grid[1] > 0 && mouse_cords_by_enemy_grid[1] < grid_len && mouse_cords_by_enemy_grid[0] < grid_len) {
							click_cords[0] = mouse_cords_by_enemy_grid[0] / 40;
							click_cords[1] = mouse_cords_by_enemy_grid[1] / 40;
							real_enemy_pole = shot(2, real_enemy_pole, click_cords);
							shoot.play();
							shootTime.restart();
							// скрываем корабли для видимого поля противника
							for (int i = 0; i < 10; i++) {
								for (int j = 0; j < 10; j++) {
									if (real_enemy_pole[i][j] < 0)
										visible_enemy_pole[i][j] = real_enemy_pole[i][j];
								}
							}
						}
					}
				}
			}
			// ход противника
			if (phase == 2 && !player_turn && shootTime.getElapsedTime().asSeconds()>=0.02) {
				enemy_shooting = true;
			}
			if (event.type == sf::Event::KeyPressed)
			{
				// транспонирование корабля
				if (event.key.code == sf::Keyboard::T && phase == 1)
				{
					if (is_move4)
						if (ship4.getRotation() == 0)
							ship4.setRotation(90);
						else
							ship4.setRotation(0);
					else if (is_move2_1)
						if (ship2_1.getRotation() == 0)
							ship2_1.setRotation(90);
						else
							ship2_1.setRotation(0);
					else if (is_move2_2)
						if (ship2_2.getRotation() == 0)
							ship2_2.setRotation(90);
						else
							ship2_2.setRotation(0);
					else if (is_move2_3)
						if (ship2_3.getRotation() == 0)
							ship2_3.setRotation(90);
						else
							ship2_3.setRotation(0);
					else if (is_move3_1)
						if (ship3_1.getRotation() == 0)
							ship3_1.setRotation(90);
						else
							ship3_1.setRotation(0);
					else if (is_move3_2)
						if (ship3_2.getRotation() == 0)
							ship3_2.setRotation(90);
						else
							ship3_2.setRotation(0);
				}

				// отслеживание окончания подготовительной фазы
				if (event.key.code == sf::Keyboard::Enter && phase == 1 && pole_is_ready(player_pole)) {
					player_pole = pole_to_battle(player_pole);
					real_enemy_pole = choose_map_to_battle(maps);
					phase = 2;
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							cout << real_enemy_pole[i][j] << " ";
						}
						cout << endl;
					}
				}
				if (event.key.code == sf::Keyboard::R && phase == 1) {
					player_pole = {
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
						vector<int>(10,0),
					};
					ship1_1.setPosition(280, 590);
					ship1_1.setRotation(0);
					ship1_2.setPosition(340, 590);
					ship1_2.setRotation(0);
					ship1_3.setPosition(400, 590);
					ship1_3.setRotation(0);
					ship1_4.setPosition(460, 590);
					ship1_3.setRotation(0);
					ship2_1.setPosition(100, 700);
					ship2_1.setRotation(0);
					ship2_2.setPosition(200, 700);
					ship2_2.setRotation(0);
					ship2_3.setPosition(300, 700);
					ship2_3.setRotation(0);
					ship3_1.setPosition(100, 650);
					ship3_1.setRotation(0);
					ship3_2.setPosition(250, 650);
					ship3_2.setRotation(0);
					ship4.setPosition(120, 590);
					ship4.setRotation(0);
				}
			}


			// проверка координат корабля
			if (is_move1_1) {
				if (cords1_1[0] != -1 && cords1_1[1] != -1 && cords1_1[2] != -1 && cords1_1[3] != -1) {
					if (player_pole[cords1_1[1]][cords1_1[0]] == 0) {
						ship1_1.setColor(Color::Green);
					}
					else
						ship1_1.setColor(Color::Red);
				}

				else
					ship1_1.setColor(Color::Red);
				ship1_1.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move1_2) {
				if (cords1_2[0] != -1 && cords1_2[1] != -1 && cords1_2[2] != -1 && cords1_2[3] != -1) {
					if (player_pole[cords1_2[1]][cords1_2[0]] == 0) {
						ship1_2.setColor(Color::Green);
					}
					else
						ship1_2.setColor(Color::Red);
				}
				else
					ship1_2.setColor(Color::Red);
				ship1_2.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move1_3) {
				if (cords1_3[0] != -1 && cords1_3[1] != -1 && cords1_3[2] != -1 && cords1_3[3] != -1) {
					if (player_pole[cords1_3[1]][cords1_3[0]] == 0) {
						ship1_3.setColor(Color::Green);
					}
					else
						ship1_3.setColor(Color::Red);

				}
				else
					ship1_3.setColor(Color::Red);
				ship1_3.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move1_4) {
				if (cords1_4[0] != -1 && cords1_4[1] != -1 && cords1_4[2] != -1 && cords1_4[3] != -1) {
					if (player_pole[cords1_4[1]][cords1_4[0]] == 0) {
						ship1_4.setColor(Color::Green);
					}
					else
						ship1_4.setColor(Color::Red);
				}
				else
					ship1_4.setColor(Color::Red);
				ship1_4.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move2_1) {
				if (cords2_1[0] != -1 && cords2_1[1] != -1 && cords2_1[2] != -1 && cords2_1[3] != -1) {
					if (player_pole[cords2_1[1]][cords2_1[0]] == 0 && player_pole[cords2_1[3]][cords2_1[2]] == 0) {
						ship2_1.setColor(Color::Green);
					}
					else
						ship2_1.setColor(Color::Red);
				}
				else
					ship2_1.setColor(Color::Red);
				ship2_1.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move2_2) {
				if (cords2_2[0] != -1 && cords2_2[1] != -1 && cords2_2[2] != -1 && cords2_2[3] != -1) {
					if (player_pole[cords2_2[1]][cords2_2[0]] == 0 && player_pole[cords2_2[3]][cords2_2[2]] == 0) {
						ship2_2.setColor(Color::Green);
					}
					else
						ship2_2.setColor(Color::Red);
				}
				else
					ship2_2.setColor(Color::Red);
				ship2_2.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move2_3) {
				if (cords2_3[0] != -1 && cords2_3[1] != -1 && cords2_3[2] != -1 && cords2_3[3] != -1) {
					if (player_pole[cords2_3[1]][cords2_3[0]] == 0 && player_pole[cords2_3[3]][cords2_3[2]] == 0) {
						ship2_3.setColor(Color::Green);
					}
					else
						ship2_3.setColor(Color::Red);
				}
				else
					ship2_3.setColor(Color::Red);
				ship2_3.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move3_1) {
				if (cords3_1[0] != -1 && cords3_1[1] != -1 && cords3_1[2] != -1 && cords3_1[3] != -1) {
					if (player_pole[cords3_1[1]][cords3_1[0]] == 0 && player_pole[cords3_1[3]][cords3_1[2]] == 0) {
						ship3_1.setColor(Color::Green);
					}
					else
						ship3_1.setColor(Color::Red);
				}
				else
					ship3_1.setColor(Color::Red);
				ship3_1.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move3_2) {
				if (cords3_2[0] != -1 && cords3_2[1] != -1 && cords3_2[2] != -1 && cords3_2[3] != -1) {
					if (player_pole[cords3_2[1]][cords3_2[0]] == 0 && player_pole[cords3_2[3]][cords3_2[2]] == 0) {
						ship3_2.setColor(Color::Green);
					}
					else
						ship3_2.setColor(Color::Red);
				}
				else
					ship3_2.setColor(Color::Red);
				ship3_2.setPosition(pixelPos.x, pixelPos.y);
			}
			else if (is_move4) {
				if (cords4[0] != -1 && cords4[1] != -1 && cords4[2] != -1 && cords4[3] != -1) {
					if (player_pole[cords4[1]][cords4[0]] == 0 && player_pole[cords4[3]][cords4[2]] == 0) {
						ship4.setColor(Color::Green);
					}
					else
						ship4.setColor(Color::Red);
				}
				else
					ship4.setColor(Color::Red);
				ship4.setPosition(pixelPos.x, pixelPos.y);
			}
			// примагничивание корабля на позицию или возварщение его в исходное положение
			if (event.type == Event::MouseButtonReleased) {
				if (event.key.code == Mouse::Left && is_move1_1) {
					is_move1_1 = false;
					if (ship1_1.getColor() == Color::Red) {
						ship1_1.setPosition(280, 590);
					}
					else {
						ship1_1.setPosition(player_grid_cord[0] + 20 + 40 * cords1_1[0], player_grid_cord[1] + 20 + 40 * cords1_1[1]);
						player_pole[cords1_1[1]][cords1_1[0]] = 1;
						player_pole = set_spaces(player_pole, 1, cords1_1[0], cords1_1[1], ship1_1.getRotation());
					}
					ship1_1.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move1_2) {
					is_move1_2 = false;
					if (ship1_2.getColor() == Color::Red) {
						ship1_2.setPosition(340, 590);
					}
					else {
						ship1_2.setPosition(player_grid_cord[0] + 20 + 40 * cords1_2[0], player_grid_cord[1] + 20 + 40 * cords1_2[1]);
						player_pole[cords1_2[1]][cords1_2[0]] = 1;
						player_pole = set_spaces(player_pole, 1, cords1_2[0], cords1_2[1], ship1_2.getRotation());
					}
					ship1_2.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move1_3) {
					is_move1_3 = false;
					if (ship1_3.getColor() == Color::Red) {
						ship1_3.setPosition(400, 590);
					}
					else {
						ship1_3.setPosition(player_grid_cord[0] + 20 + 40 * cords1_3[0], player_grid_cord[1] + 20 + 40 * cords1_3[1]);
						player_pole[cords1_3[1]][cords1_3[0]] = 1;
						player_pole = set_spaces(player_pole, 1, cords1_3[0], cords1_3[1], ship1_3.getRotation());
					}
					ship1_3.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move1_4) {
					is_move1_4 = false;
					if (ship1_4.getColor() == Color::Red) {
						ship1_4.setPosition(460, 590);
					}
					else {
						ship1_4.setPosition(player_grid_cord[0] + 20 + 40 * cords1_4[0], player_grid_cord[1] + 20 + 40 * cords1_4[1]);
						player_pole[cords1_4[1]][cords1_4[0]] = 1;
						player_pole = set_spaces(player_pole, 1, cords1_4[0], cords1_4[1], ship1_4.getRotation());
					}
					ship1_4.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move2_1) {
					is_move2_1 = false;
					if (ship2_1.getColor() == Color::Red) {
						ship2_1.setPosition(100, 700);
						ship2_1.setRotation(0);

					}
					else {

						if (ship2_1.getRotation() == 0) {
							ship2_1.setPosition(player_grid_cord[0] + 40 + 40 * cords2_1[0], player_grid_cord[1] + 20 + 40 * cords2_1[1]);
							player_pole[cords2_1[1]][cords2_1[0]] = 2; player_pole[cords2_1[1]][cords2_1[0] + 1] = 2;
							player_pole = set_spaces(player_pole, 2, cords2_1[0], cords2_1[1], ship2_1.getRotation());
						}
						else {
							ship2_1.setPosition(player_grid_cord[0] + 20 + 40 * cords2_1[0], player_grid_cord[1] + 40 + 40 * cords2_1[1]);
							player_pole[cords2_1[1]][cords2_1[0]] = 2; player_pole[cords2_1[1] + 1][cords2_1[0]] = 2;
							player_pole = set_spaces(player_pole, 2, cords2_1[0], cords2_1[1], ship2_1.getRotation());
						}
					}
					ship2_1.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move2_2) {
					is_move2_2 = false;
					if (ship2_2.getColor() == Color::Red) {
						ship2_2.setPosition(200, 700);
						ship2_2.setRotation(0);

					}
					else {

						if (ship2_2.getRotation() == 0) {
							ship2_2.setPosition(player_grid_cord[0] + 40 + 40 * cords2_2[0], player_grid_cord[1] + 20 + 40 * cords2_2[1]);
							player_pole[cords2_2[1]][cords2_2[0]] = 2; player_pole[cords2_2[1]][cords2_2[0] + 1] = 2;
							player_pole = set_spaces(player_pole, 2, cords2_2[0], cords2_2[1], ship2_2.getRotation());
						}
						else {
							ship2_2.setPosition(player_grid_cord[0] + 20 + 40 * cords2_2[0], player_grid_cord[1] + 40 + 40 * cords2_2[1]);
							player_pole[cords2_2[1]][cords2_2[0]] = 2; player_pole[cords2_2[1] + 1][cords2_2[0]] = 2;
							player_pole = set_spaces(player_pole, 2, cords2_2[0], cords2_2[1], ship2_2.getRotation());
						}
					}
					ship2_2.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move2_3) {
					is_move2_3 = false;
					if (ship2_3.getColor() == Color::Red) {
						ship2_3.setPosition(300, 700);
						ship2_3.setRotation(0);

					}
					else {

						if (ship2_3.getRotation() == 0) {
							ship2_3.setPosition(player_grid_cord[0] + 40 + 40 * cords2_3[0], player_grid_cord[1] + 20 + 40 * cords2_3[1]);
							player_pole[cords2_3[1]][cords2_3[0]] = 2; player_pole[cords2_3[1]][cords2_3[0] + 1] = 2;
							player_pole = set_spaces(player_pole, 2, cords2_3[0], cords2_3[1], ship2_3.getRotation());
						}
						else {
							ship2_3.setPosition(player_grid_cord[0] + 20 + 40 * cords2_3[0], player_grid_cord[1] + 40 + 40 * cords2_3[1]);
							player_pole[cords2_3[1]][cords2_3[0]] = 2; player_pole[cords2_3[1] + 1][cords2_3[0]] = 2;
							player_pole = set_spaces(player_pole, 2, cords2_3[0], cords2_3[1], ship2_3.getRotation());
						}
					}
					ship2_3.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move3_1) {
					is_move3_1 = false;
					if (ship3_1.getColor() == Color::Red) {
						ship3_1.setPosition(100, 650);
						ship3_1.setRotation(0);

					}
					else {

						if (ship3_1.getRotation() == 0) {
							ship3_1.setPosition(player_grid_cord[0] + 60 + 40 * cords3_1[0], player_grid_cord[1] + 20 + 40 * cords3_1[1]);
							player_pole[cords3_1[1]][cords3_1[0]] = 3; player_pole[cords3_1[1]][cords3_1[0] + 1] = 3; player_pole[cords3_1[1]][cords3_1[0] + 2] = 3;
							player_pole = set_spaces(player_pole, 3, cords3_1[0], cords3_1[1], ship3_1.getRotation());
						}
						else {
							ship3_1.setPosition(player_grid_cord[0] + 20 + 40 * cords3_1[0], player_grid_cord[1] + 60 + 40 * cords3_1[1]);
							player_pole[cords3_1[1]][cords3_1[0]] = 3; player_pole[cords3_1[1] + 1][cords3_1[0]] = 3; player_pole[cords3_1[1] + 2][cords3_1[0]] = 3;
							player_pole = set_spaces(player_pole, 3, cords3_1[0], cords3_1[1], ship3_1.getRotation());
						}
					}
					ship3_1.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move3_2) {
					is_move3_2 = false;
					if (ship3_2.getColor() == Color::Red) {
						ship3_2.setPosition(250, 650);
						ship3_2.setRotation(0);

					}
					else {

						if (ship3_2.getRotation() == 0) {
							ship3_2.setPosition(player_grid_cord[0] + 60 + 40 * cords3_2[0], player_grid_cord[1] + 20 + 40 * cords3_2[1]);
							player_pole[cords3_2[1]][cords3_2[0]] = 3; player_pole[cords3_2[1]][cords3_2[0] + 1] = 3; player_pole[cords3_2[1]][cords3_2[0] + 2] = 3;
							player_pole = set_spaces(player_pole, 3, cords3_2[0], cords3_2[1], ship3_2.getRotation());
						}
						else {
							ship3_2.setPosition(player_grid_cord[0] + 20 + 40 * cords3_2[0], player_grid_cord[1] + 60 + 40 * cords3_2[1]);
							player_pole[cords3_2[1]][cords3_2[0]] = 3; player_pole[cords3_2[1] + 1][cords3_2[0]] = 3; player_pole[cords3_2[1] + 2][cords3_2[0]] = 3;
							player_pole = set_spaces(player_pole, 3, cords3_2[0], cords3_2[1], ship3_2.getRotation());
						}
					}
					ship3_2.setColor(Color::White);
				}
				else if (event.key.code == Mouse::Left && is_move4) {
					is_move4 = false;
					if (ship4.getColor() == Color::Red) {
						ship4.setPosition(120, 590);
						ship4.setRotation(0);

					}
					else {

						if (ship4.getRotation() == 0) {
							ship4.setPosition(player_grid_cord[0] + 80 + 40 * cords4[0], player_grid_cord[1] + 20 + 40 * cords4[1]);
							player_pole[cords4[1]][cords4[0]] = 4; player_pole[cords4[1]][cords4[0] + 1] = 4; player_pole[cords4[1]][cords4[0] + 2] = 4; player_pole[cords4[1]][cords4[0] + 3] = 4;
							player_pole = set_spaces(player_pole, 4, cords4[0], cords4[1], ship4.getRotation());
						}
						else {
							ship4.setPosition(player_grid_cord[0] + 20 + 40 * cords4[0], player_grid_cord[1] + 80 + 40 * cords4[1]);
							player_pole[cords4[1]][cords4[0]] = 4; player_pole[cords4[1] + 1][cords4[0]] = 4; player_pole[cords4[1] + 2][cords4[0]] = 4; player_pole[cords4[1] + 3][cords4[0]] = 4;
							player_pole = set_spaces(player_pole, 4, cords4[0], cords4[1], ship4.getRotation());
						}
					}
					ship4.setColor(Color::White);
				}
			}
		}
		String alph = "ABCDEFGHIJ";
		//рисуем фон

		window.draw(backgroud);

		// Рисуем поле игрока



		for (int i = 0; i < 11; i++) {
			VertexArray line(Lines, 8);
			line[0].position = Vector2f(player_grid_cord[0], player_grid_cord[1] + i * 40);
			line[1].position = Vector2f(player_grid_cord[0] + grid_len, player_grid_cord[1] + i * 40);
			line[0].color = Color::Blue;
			line[1].color = Color::Blue;

			window.draw(line);
		}
		for (int i = 0; i < 11; i++) {
			VertexArray line(Lines, 8);
			line[0].position = Vector2f(player_grid_cord[0] + i * 40, player_grid_cord[1]);
			line[1].position = Vector2f(player_grid_cord[0] + i * 40, player_grid_cord[1] + grid_len);
			line[0].color = Color::Blue;
			line[1].color = Color::Blue;

			window.draw(line);

		}
		for (int i = 0; i < 10; i++) {
			text.setString(alph[i]);
			text.setPosition(player_grid_cord[0] + 10 + i * 40, player_grid_cord[1] - 38);

			window.draw(text);
		}
		for (int i = 1; i < 11; i++) {
			text.setString(to_string(i));

			if (i == 10)
				text.setPosition(player_grid_cord[0] - 40, player_grid_cord[1] + (i - 1) * 40);
			else
			{
				text.setPosition(player_grid_cord[0] - 34, player_grid_cord[1] + (i - 1) * 40);
			}

			window.draw(text);
		}

		text_player.setPosition(player_grid_cord[0] + grid_len / 3, player_grid_cord[1] - 114);
		window.draw(text_player);



		// Рисуем поле противника


		for (int i = 0; i < 11; i++) {
			VertexArray line(Lines, 8);
			line[0].position = Vector2f(enemy_grid_cord[0], enemy_grid_cord[1] + i * 40);
			line[1].position = Vector2f(enemy_grid_cord[0] + grid_len, enemy_grid_cord[1] + i * 40);
			line[0].color = Color::Blue;
			line[1].color = Color::Blue;

			window.draw(line);

		}
		for (int i = 0; i < 11; i++) {
			VertexArray line(Lines, 16);
			line[0].position = Vector2f(enemy_grid_cord[0] + i * 40, enemy_grid_cord[1]);
			line[1].position = Vector2f(enemy_grid_cord[0] + i * 40, enemy_grid_cord[1] + grid_len);
			line[0].color = Color::Blue;
			line[1].color = Color::Blue;


			window.draw(line);

		}

		for (int i = 0; i < 10; i++) {
			text.setString(alph[i]);
			text.setPosition(enemy_grid_cord[0] + 10 + i * 40, enemy_grid_cord[1] - 38);

			window.draw(text);
		}
		for (int i = 1; i < 11; i++) {
			text.setString(to_string(i));

			if (i == 10)
				text.setPosition(enemy_grid_cord[0] - 40, enemy_grid_cord[1] + (i - 1) * 40);
			else
			{
				text.setPosition(enemy_grid_cord[0] - 34, enemy_grid_cord[1] + (i - 1) * 40);
			}

			window.draw(text);
		}


		text_enemy.setPosition(enemy_grid_cord[0] + grid_len / 3, enemy_grid_cord[1] - 114);
		window.draw(text_enemy);

		text_vs.setPosition(1400 / 2 - 40, enemy_grid_cord[1] - 114);
		window.draw(text_vs);



		// вывод надписей уведомляющих о подготовительной фазе
		if (phase == 1) {
			window.draw(text_preparing_phase);
			window.draw(text_end_preparing_phase);
		}

		// рисуем корабли
		window.draw(ship1_1);
		window.draw(ship1_2);
		window.draw(ship1_3);
		window.draw(ship1_4);
		window.draw(ship4);
		window.draw(ship3_1);
		window.draw(ship3_2);
		window.draw(ship2_1);
		window.draw(ship2_2);
		window.draw(ship2_3);
		// отрисовка видимого поля игрока
		vector<vector<int>> buffer;
		if (phase != 1) {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					int cords_of_square[2] = { player_grid_cord[0] + 20 + j * 40, player_grid_cord[1] + 20 + i * 40 };
					if (player_pole[i][j] > 0) {
						Sprite ship;
						ship.setTexture(ships_texture);
						ship.setTextureRect(IntRect(110, 152, 40, 40));
						ship.setOrigin(20, 20);
						ship.setPosition(cords_of_square[0], cords_of_square[1]);
						window.draw(ship);
					}
					else if (player_pole[i][j] < 0 && player_pole[i][j] != -10) {
						Sprite terminated_ship;
						terminated_ship.setTexture(ships_texture);
						terminated_ship.setTextureRect(IntRect(93, 308, 40, 40));
						terminated_ship.setOrigin(20, 20);
						terminated_ship.setPosition(cords_of_square[0], cords_of_square[1]);
						window.draw(terminated_ship);
					}
					else if (player_pole[i][j] == -10) {
						CircleShape miss;
						miss.setRadius(7);
						miss.setFillColor(Color::Blue);
						miss.setPosition(cords_of_square[0] - 7, cords_of_square[1] - 7);
						window.draw(miss);
					}
				}
			}
			if (enemy_shooting) {
				shootTime.restart();
				if (difficulty == 1) {
					buffer = eazy(player_pole);
					if (buffer != player_pole) {
						player_pole = buffer;
						if (music.getElapsedTime().asSeconds() > 1)
							shoot.play(); music.restart();
					}
				}
				else if (difficulty == 3) {
					buffer = imposible(player_pole);
					if (buffer != player_pole) {
						player_pole = buffer;
						if (music.getElapsedTime().asSeconds() > 1)
							shoot.play(); music.restart();
					}
				}
				else if (difficulty == 2) {
					buffer = hard(player_pole);
					if (buffer != player_pole) {
						player_pole = buffer;
						if (music.getElapsedTime().asSeconds() > 1) {
							shoot.play(); music.restart();
						}
					}

				}
				// отрисовка видимого поля игрока
				if (phase != 1) {
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							int cords_of_square[2] = { player_grid_cord[0] + 20 + j * 40, player_grid_cord[1] + 20 + i * 40 };
							if (player_pole[i][j] > 0) {
								Sprite ship;
								ship.setTexture(ships_texture);
								ship.setTextureRect(IntRect(110, 152, 40, 40));
								ship.setOrigin(20, 20);
								ship.setPosition(cords_of_square[0], cords_of_square[1]);
								window.draw(ship);
							}
							else if (player_pole[i][j] < 0 && player_pole[i][j] != -10) {
								Sprite terminated_ship;
								terminated_ship.setTexture(ships_texture);
								terminated_ship.setTextureRect(IntRect(93, 308, 40, 40));
								terminated_ship.setOrigin(20, 20);
								terminated_ship.setPosition(cords_of_square[0], cords_of_square[1]);
								window.draw(terminated_ship);
							}
							else if (player_pole[i][j] == -10) {
								CircleShape miss;
								miss.setRadius(7);
								miss.setFillColor(Color::Blue);
								miss.setPosition(cords_of_square[0] - 7, cords_of_square[1] - 7);
								window.draw(miss);
							}
						}
					}
				}
				enemy_shooting = false;
			}
			// отрисовка видимого поля противника
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					int cords_of_square[2] = { enemy_grid_cord[0] + 20 + j * 40, enemy_grid_cord[1] + 20 + i * 40 };
					if (visible_enemy_pole[i][j] > 0) {
						Sprite ship;
						ship.setTexture(ships_texture);
						ship.setTextureRect(IntRect(110, 152, 40, 40));
						ship.setOrigin(20, 20);
						ship.setPosition(cords_of_square[0], cords_of_square[1]);
						window.draw(ship);
					}
					else if (visible_enemy_pole[i][j] < 0 && visible_enemy_pole[i][j] != -10) {
						Sprite terminated_ship;
						terminated_ship.setTexture(ships_texture);
						terminated_ship.setTextureRect(IntRect(93, 308, 40, 40));
						terminated_ship.setOrigin(20, 20);
						terminated_ship.setPosition(cords_of_square[0], cords_of_square[1]);
						window.draw(terminated_ship);
					}
					else if (visible_enemy_pole[i][j] == -10) {
						CircleShape miss;
						miss.setRadius(7);
						miss.setFillColor(Color::Blue);
						miss.setPosition(cords_of_square[0] - 7, cords_of_square[1] - 7);
						window.draw(miss);
					}
				}
			}
		}
		// проверка на победу
		if (phase == 2) {
			winner = check_winner(winner, player_pole, real_enemy_pole);
			if (winner != "none")
				phase = 3;
		}

		if (phase == 2 && player_turn)
				window.draw(p_turn);

		if (phase == 3) {
			if (winner == "player") {
				window.draw(win);
			}
			else {
				window.draw(lose);
				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						int cords_of_square[2] = { enemy_grid_cord[0] + 20 + j * 40, enemy_grid_cord[1] + 20 + i * 40 };
						if (real_enemy_pole[i][j] > 0) {
							Sprite ship;
							ship.setTexture(ships_texture);
							ship.setTextureRect(IntRect(110, 152, 40, 40));
							ship.setOrigin(20, 20);
							ship.setPosition(cords_of_square[0], cords_of_square[1]);
							window.draw(ship);
						}
						else if (real_enemy_pole[i][j] < 0 && real_enemy_pole[i][j] != -10) {
							Sprite terminated_ship;
							terminated_ship.setTexture(ships_texture);
							terminated_ship.setTextureRect(IntRect(93, 308, 40, 40));
							terminated_ship.setOrigin(20, 20);
							terminated_ship.setPosition(cords_of_square[0], cords_of_square[1]);
							window.draw(terminated_ship);
						}
						else if (real_enemy_pole[i][j] == -10) {
							CircleShape miss;
							miss.setRadius(7);
							miss.setFillColor(Color::Blue);
							miss.setPosition(cords_of_square[0] - 7, cords_of_square[1] - 7);
							window.draw(miss);
						}
					}
				}
			}
		}



		window.display();
	}

	return 0;
}