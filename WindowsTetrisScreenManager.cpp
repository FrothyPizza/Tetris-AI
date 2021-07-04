#include "WindowsTetrisScreenManager.h"

int ts::WindowsTetrisScreenManager::matrixX = 0;
int ts::WindowsTetrisScreenManager::matrixY = 0;
int ts::WindowsTetrisScreenManager::matrixW = 0;
int ts::WindowsTetrisScreenManager::matrixH = 0;
int ts::WindowsTetrisScreenManager::incX = 0;
int ts::WindowsTetrisScreenManager::incY = 0;
ts::Point ts::WindowsTetrisScreenManager::nextListPositions[5] = { {0,0},{0,0},{0,0},{0,0},{0,0} };

//std::unordered_map<int, ts::WindowsTetrisScreenManager::RGB> ts::WindowsTetrisScreenManager::tetrominoColors;

std::vector<ts::WindowsTetrisScreenManager::RGB> ts::WindowsTetrisScreenManager::tetrominoColors;

ts::WindowsTetrisScreenManager::Game ts::WindowsTetrisScreenManager::playingGame = ts::WindowsTetrisScreenManager::Game::TETRIO;

void ts::WindowsTetrisScreenManager::askForMatrixLocation() {

	//testing
	//std::cout << ColorDistance({ 14, 5, 2 }, { 0,0,0 }) << '\n';
	//std::cout << ColorDistance({ 140, 0, 0 }, { 100, 0, 0 }) << '\n';
	//std::cout << ColorDistance({ 110, 0, 0 }, { 100, 0, 0 }) << '\n';
	//std::cout << ColorDistance({ 255, 0, 0 }, { 0, 0, 0 }) << '\n';
	//std::cout << ColorDistance({ 255, 255, 255 }, { 0, 0, 0 }) << '\n';
	//std::cout << ColorDistance({ 33, 65, 198 }, { 15, 155, 215 }) << '\n';
	//std::cout << ColorDistance({ 89, 177, 1 }, { 227, 91, 2 }) << '\n';
	tetrominoColors.reserve(7);
	//tetrominoColors[MINO_L] = { 227, 91, 2 };
	//tetrominoColors[MINO_J] = { 33, 65, 198 };
	//tetrominoColors[MINO_Z] = { 215, 15, 55 };
	//tetrominoColors[MINO_S] = { 89, 177, 1 };
	//tetrominoColors[MINO_T] = { 175, 41, 138 };
	//tetrominoColors[MINO_I] = { 15, 155, 215 };
	//tetrominoColors[MINO_O] = { 227, 159, 2 };


	tetrominoColors.push_back({ 227, 91, 2 });
	tetrominoColors.push_back({ 33, 65, 198 });
	tetrominoColors.push_back({ 215, 15, 55 });
	tetrominoColors.push_back({ 89, 177, 1 });
	tetrominoColors.push_back({ 175, 41, 138 });
	tetrominoColors.push_back({ 15, 155, 215 });
	tetrominoColors.push_back({ 227, 159, 2 });


	POINT p;
	int x, y;

	std::cout << "Put your cursor at the top left of the Tetris matrix while keeping this window in focus. Press enter to continue" << '\n';
	system("PAUSE");
	GetCursorPos(&p);
	x = (int)((float)p.x * 1.25f);
	y = (int)((float)p.y * 1.25f);
	matrixX = x; matrixY = y;

	std::cout << "Put your cursor at the bottom right of the Tetris matrix while keeping this window in focus. Press enter to continue" << '\n';
	system("PAUSE");
	GetCursorPos(&p);
	x = (int)((float)p.x * 1.25f);
	y = (int)((float)p.y * 1.25f);
	matrixW = x - matrixX; matrixH = y - matrixY;

	for (int i = 0; i < 5; ++i) {
		std::cout << "Put cursor on next list number " << i + 1 << '\n';
		system("PAUSE");
		GetCursorPos(&p);
		nextListPositions[i] = { (int)((float)p.x * 1.25f), (int)((float)p.y * 1.25f) };
	}


	std::cout << matrixX << " " << matrixY << "  " << matrixW << " " << matrixH << '\n';

	incX = matrixW / 10;
	incY = matrixH / 20;
}

std::vector<int> ts::WindowsTetrisScreenManager::getNextList() {

	for (int i = 0; i < 5; ++i) {
		std::cout << nextListPositions[i].x << " " << nextListPositions[i].y << ' ';
	}std::cout << '\n';

	std::vector<int> next; next.reserve(5);


	HDC hDC = GetDC(NULL);
	for (int i = 0; i < 5; ++i) {
		COLORREF color = GetPixel(hDC, nextListPositions[i].x, nextListPositions[i].y);
		int r = GetRValue(color); int g = GetGValue(color); int b = GetBValue(color);

		// if it is black, then try below
		if (ColorDistance({ (unsigned char)r, (unsigned char)g, (unsigned char)b }, { 0,0,0 }) < 50.0) {
			color = GetPixel(hDC, nextListPositions[i].x, nextListPositions[i].y + incY);
			r = GetRValue(color); g = GetGValue(color); b = GetBValue(color);
		}

		std::cout << r << " " << g << " " << b << '\n';


		double minColorDistance = 100000;
		int closestColor = -1;
		for (int j = 0; j < 7; ++j) {
			double score = ColorDistance({ (unsigned char)r, (unsigned char)g, (unsigned char)b }, tetrominoColors[j]);
			if (score < minColorDistance) {
				closestColor = j;
				minColorDistance = score;
			}
		}

		std::cout << closestColor << '\n';

	}
	std::cout << '\n';

	return next;
}

int ts::WindowsTetrisScreenManager::getHold() {
	return 0;
}

void ts::WindowsTetrisScreenManager::updateMatrixFromScreen(int(&matrix)[WIDTH][HEIGHT]) {
	memset(matrix, 0, sizeof(matrix[0][0]) * WIDTH * HEIGHT);

	HDC hDC;
	get_Bitmap(matrixX, matrixY, hDC, matrixW, matrixH);

	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 2; j < 20; ++j) { // start 2 from the top so that it doesn't count the player's tetromino

									   //int x = matrixX + (i * incX) + incX/2;
									   //int y = matrixY + (j * incY) + incX/2;
			int x = (i * incX) + incX / 2;
			int y = (j * incY) + incX / 2;

			COLORREF color = GetPixel(hDC, x, y);
			int r = GetRValue(color); int g = GetGValue(color); int b = GetBValue(color);
			//std::cout << r << " " << g << " " << b << '\n';

			if (r > 30 || g > 30 || b > 30) matrix[i][j + HEIGHT - 20] = 1; // if the pixel isn't (approximately) black, then it is a tile
			else matrix[i][j + HEIGHT - 20] = 0;

		}
	}
	ReleaseDC(0, hDC);


	system("CLS");
	//std::cout << '\n';
	// print the matrix
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			std::cout << matrix[j][i] << " ";
		}
		std::cout << '\n';
	}
}

//https://www.youtube.com/watch?v=xTqvtSfCla0&ab_channel=CasualGamer
int ts::WindowsTetrisScreenManager::get_Bitmap(int x, int y, HDC& hdcMemory, int width, int height) {
	HDC hdcSource = GetDC(NULL);
	hdcMemory = CreateCompatibleDC(hdcSource);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, width, height);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);
	if (!BitBlt(hdcMemory, 0, 0, width, height, hdcSource, x, y, CAPTUREBLT | SRCCOPY)) {
		std::cout << "BitBlt failed!\n";
	}

	//clean up
	DeleteObject(hBitmapOld);
	DeleteObject(hBitmap);
	ReleaseDC(NULL, hdcSource);

	return 0;
}
