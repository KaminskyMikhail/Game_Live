#include <iostream>i
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <process.h>
#include <thread>
#include <chrono>
void display_field(std::vector<std::vector<char>>&, int rows, int cols, int count);
void task_for_game();
int count_neighbors(int x, int y, int height, int width, std::vector<std::vector<char>>& field);
bool star_detect(std::vector<std::vector<char>>&, int rows, int cols);
///////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	int rows, cols;
	task_for_game();
	std::ifstream filling("data.txt");
	filling >> rows >> cols;
	std::vector<std::vector<char>> field(rows, std::vector<char>(cols, '-'));
	std::vector<std::vector<char>> nextfield(rows, std::vector<char>(cols));
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			int row, cell;
			filling >> row >> cell;
			field[row][cell] = '*';
		}

	//display_field(field, rows, cols, 0);

	int total_cells = rows * cols;
	
	int** roommate_for_cell = new int*[rows];// в этоv массиве будет содержаться информация о  колличество соседей для каждой клетки массива field 
	for (int i = 0; i < rows; i++)           
		roommate_for_cell[i] = new int[cols];
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			roommate_for_cell[i][j] = count_neighbors(i, j, rows, cols, field);// тепер ячейка массива содержит информацию о количестве живых клеток, которые ее окружают
		}
	}

	std::cout << std::endl;
	
	int count = 1;

	while (true) {
		//----------------------------------------------------------------------------------------
		if (count == 1) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					roommate_for_cell[i][j] = count_neighbors(i, j, rows, cols, field);
				}
			}
			display_field(field, rows, cols, count);
		}																								// в этом блоке производится перенос информации о соседях 
		else if(count > 1) {																			// в массив для первой генерации или для последующих
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					roommate_for_cell[i][j] = count_neighbors(i, j, rows, cols, nextfield);
				}
			}
		}
		//-----------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				if (field[i][j] == '*') {
					nextfield[i][j] = (roommate_for_cell[i][j] < 2 || roommate_for_cell[i][j] > 3) ? '-' : '*';				//в этом блоке производится реализация правил игры,
				}																											//связанных с зарождением клетки, ее жизнью и смертью
				else if (field[i][j] == '*') {
					nextfield[i][j] = (roommate_for_cell[i][j] == 2 || roommate_for_cell[i][j] == 3) ? '*' : '-';
				}
				else if (field[i][j] == '-') {
					nextfield[i][j] = (roommate_for_cell[i][j] == 3) ? '*' : '-';
				}
		//------------------------------------------------------------------------------------------------------
		//system("cls");
		if (count > 1) {
			display_field(nextfield, rows, cols, count);
		}
		if (field == nextfield) {
			bool detect = star_detect(field, rows, cols);
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					if (detect) {
						std::cout << "All cells are dead. Game over." << std::endl;
						delete roommate_for_cell[i];
						delete[] roommate_for_cell;
						exit(0);
					}
					else {
						std::cout << "The world has stagnated. Game over." << std::endl;
						delete roommate_for_cell[i];
						delete[] roommate_for_cell;
						exit(0);
					}

				}
			}
		}
			field = nextfield;
			count++;

		//field = nextfield;
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
	return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void task_for_game()
{
	system("chcp 65001");
	std::ofstream task("data.txt");
	std::string line;
	std::getline(std::cin, line);                             //создаем файл для вводных данных
	std::stringstream ss(line);
	int rows, cols, count{}, maxCount;
	ss >> rows >> cols;
	task << rows << " " << cols << std::endl;
	maxCount = rows;
	for (int i = 0; i < rows; i++) {
		std::getline(std::cin, line);
		int sym, bol;
		std::stringstream ss(line);
		ss >> sym >> bol;
		if (sym >= rows || bol >= cols) {
			std::cout << "Значение вне диапазона игрового поля! Введите корректное значение."
				<< std::endl;;
			--i;
			continue;
		}
		task << sym << " " << bol << std::endl;;
	}
	task.close();
}

void display_field(std::vector<std::vector<char>>& field, int rows, int cols, int count) {
	int alive_cells{};
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			std::cout << field[i][j] << " ";
		std::cout << std::endl;
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			if (field[i][j] == '*') { alive_cells++; }
	}
	
		std::cout << "Generation: " << count << " Alive cells: " << alive_cells << std::endl;
	
}

int count_neighbors(int x, int y, int height, int width, std::vector<std::vector<char>>& field) {
	int count = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) { continue; }
			int nx = i + x;
			int ny = j + y;
			if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
				if (field[nx][ny] == '*')
					count++;
			}
		}
	}
	return count;
}
bool star_detect(std::vector<std::vector<char>>& arr, int rows, int cols) {
	bool detect = 1;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr[i][j] = '*') {
				detect = 0;
				return detect;
			}
		}
	}
	
	return detect;
}
