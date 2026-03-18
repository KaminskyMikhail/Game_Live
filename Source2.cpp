#include <iostream>i
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <process.h>

void displayField(std::vector<std::vector<char>>&, int rows, int cols);
void task_for_game();
int count_Neighbors(int x, int y, int height, int width, std::vector<std::vector<char>>& field);
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
	nextfield = field;
	displayField(field, rows, cols);
	int nei{};
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++) {
			int nei = count_Neighbors(i, j, rows, cols, field);
			std::cout << nei;
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

void displayField(std::vector<std::vector<char>>& field, int rows, int cols) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++)
			std::cout << field[i][j] << " ";
		std::cout << std::endl;
	}
}

int count_Neighbors(int x, int y, int height, int width, std::vector<std::vector<char>>& field) {
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