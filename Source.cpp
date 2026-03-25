#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

void display_field(char** field, int rows, int cols, int count);

int count_neighbors(int x, int y, int height, int width, char** field);

bool star_detect(char** field, int rows, int cols);
///////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	system("chcp 65001 > nul");
	int rows, cols;
	std::ifstream filling("data.txt");
	filling >> rows >> cols;

	char** field = new char* [rows]; // объявление начального поля символов
	for (int i = 0; i < rows; i++)
		field[i] = new char[cols];

	char** nextfield = new char* [rows];
	for (int i = 0; i < rows; i++)
		nextfield[i] = new char[cols];

	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			field[i][j] = '-';

	int row, cell;
	bool has_alive_cells = false;

	while (filling >> row >> cell) {
		if (row >= 0 && cell >= 0) {
			field[row][cell] = '*';
			has_alive_cells = true;
		}
	}

	if (!has_alive_cells) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				std::cout << field[i][j] << " ";
			}
			std::cout << std::endl;
		}

		std::cout << "Поколение: 1. Живых клеток: 0" << std::endl;
		std::cout << "Мир умер!" << std::endl;

		for (int i = 0; i < rows; i++) {
			delete[] field[i];
			delete[] nextfield[i];
		}
		delete[] field;
		delete[] nextfield;

		return EXIT_SUCCESS;
	}

	int** roommate_for_cell = new int* [rows];// в этом массиве будет содержаться информация о  колличество соседей для каждой клетки массива field 
	for (int i = 0; i < rows; i++)
		roommate_for_cell[i] = new int[cols];

		int count = 0;

	while (true) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					roommate_for_cell[i][j] = count_neighbors(i, j, rows, cols, field);
				}
			}
			display_field(field, rows, cols, count);
		//-----------------------------------------------------------------------------------------
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
		//------------------------------------------------------------------------------------------
		bool detect = star_detect(nextfield, rows, cols);
		bool exit1 = true;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (field[i][j] != nextfield[i][j]) {
					exit1 = false;
					break;
				}
			}
			if (!exit1) { break; }
		}

		if (exit1) { 
			if (detect) {
				std::cout << "Мир умер!\n";
				break;
			}
			else
				std::cout << "Мир застыл!\n";
			break;
		}

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				field[i][j] = nextfield[i][j];
		count++;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	for (int i = 0; i < rows; i++) {
		delete[] field[i];
		delete[] nextfield[i];
		delete[] roommate_for_cell[i];
	}
	delete[] field;
	delete[] nextfield;
	delete[] roommate_for_cell;
	return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////////////////////
void display_field(char** field, int rows, int cols, int count) {
	system("chcp 65001 > nul");
	int alive_cells = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			std::cout << field[i][j] << " ";
		std::cout << std::endl;
	}
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			if (field[i][j] == '*') { alive_cells++; }
	}

	std::cout << "Поколение: " << count + 1 << " Живых клеток: " << alive_cells << std::endl;

}

int count_neighbors(int x, int y, int height, int width, char** field) {
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

bool star_detect(char** arr, int rows, int cols) {
	bool detect = 1;
	char star = '*';
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (arr[i][j] == star) {
				detect = 0;
				return detect;
			}
		}
	}

	return detect;
}
