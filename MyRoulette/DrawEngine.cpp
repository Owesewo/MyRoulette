#include "stdafx.h"

void DrawEngine::Main()
{
	Initiallize();

	while (true)
	{
		system("cls");
		SetColor(FieldColors.first, FieldColors.second);

		std::cout << std::endl;
		std::cout << std::format("{0:^40}", std::string(40, '=')) << std::endl;
		std::cout << std::format("|{0:^38}|", "My european roulette") << std::endl;
		std::cout << std::format("{0:^40}", std::string(40, '=')) << std::endl;
		std::cout << std::endl;

		int i = 0;
		for (auto& v : { "Start game", "Help", "Quit" })
		{
			if (i == mainMenuChoise)
				SetColor(SelectedCellColors.first, SelectedCellColors.second);

			std::cout << std::format("{0:^40}", v) << std::endl;

			SetColor(FieldColors.first, FieldColors.second);
			i++;
		}

		SelectMainOption();

		if (isExitButtonActivated)
			return;

		if (isMainMenuSelected)
		{
			switch (mainMenuChoise)
			{
			case 0: ShowField(); break;
			case 1: ShowHelp(); break;
			default: return;
			}
		}

		isMainMenuSelected = false;
	}
}

void DrawEngine::Initiallize()
{
	hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	moneyAmount = 1000;
	stakes.clear();
	selectedCell = { 1, 0 };
	selectedCellValue = 0;
	selectedChip = 0;
	selectedChipNominal = 0;
	mainMenuChoise = 0;
	isCellSelected = false;
	isSpinActivated = false;
	isExitButtonActivated = false;
	isMainMenuSelected = false;
}

void DrawEngine::SelectMainOption()
{
	char choice = _getch();

	switch (choice) {
	case KEY_ARROW_UP:
	{
		mainMenuChoise--;

		if (mainMenuChoise < 0)
			mainMenuChoise = 0;

		if (mainMenuChoise > 2)
			mainMenuChoise = 2;
	}
	break;
	case KEY_ARROW_DOWN:
	{
		mainMenuChoise++;

		if (mainMenuChoise < 0)
			mainMenuChoise = 0;

		if (mainMenuChoise > 2)
			mainMenuChoise = 2;
	}
	break;
	case KEY_ENTER:
	{
		isMainMenuSelected = true;
	}
	break;
	case KEY_ESC:
	{
		isExitButtonActivated = true;
		return;
	}
	}
}

void DrawEngine::ShowHelp()
{
	system("cls");

	std::pair<int, int> buff = selectedCell;
	selectedCell = { -1, -1 };

	SetColor(FieldColors.first, FieldColors.second);

	std::cout << std::endl;
	std::cout << std::format("{0:^40}", std::string(40, '=')) << std::endl;
	std::cout << std::format("|{0:^38}|", "MER: Help") << std::endl;
	std::cout << std::format("{0:^40}", std::string(40, '=')) << std::endl;
	std::cout << std::endl;

	std::cout << std::format("{0:}", "Roulette is a simple game to play. Players place bets on a single number or a range of numbers, and then spin a wheel with 36 numbered slots.") << std::endl;
	std::cout << std::format("{0:}", "A ball is released onto the wheel, and players must guess where it will end up. If they guess correctly, they win money.") << std::endl;
	std::cout << std::endl;

	std::cout << std::format("{0:}", "European roulette table:") << std::endl;
	DrawField();
	std::cout << std::format("{0:}", "Cells oefficients: ");
	std::cout << std::format("{0:}", "0-36 -> x36") << std::endl;
	std::cout << std::format("{0:}", "1to12 -> x3") << std::endl;
	std::cout << std::format("{0:}", "Dozens -> x3") << std::endl;
	std::cout << std::format("{0:}", "Even, Odd -> x2") << std::endl;
	std::cout << std::format("{0:}", "Black, Red -> x2") << std::endl;
	std::cout << std::format("{0:}", "To18, to36 -> x2") << std::endl;
	std::cout << std::endl;

	std::cout << "Press any button to return...";
	_getch();

	selectedCell = buff;
}

void DrawEngine::ShowField()
{
	while (true)
	{
		system("cls");
		std::cout << std::endl;
		std::cout << std::format("Money amount: {0:}", moneyAmount) << std::endl << std::endl;

		DrawField();
		DrawSpinButton();

		if (isCellSelected)
		{
			if (isSpinActivated)
			{
				SpinRoulette();

				isSpinActivated = false;
				stakes.clear();
			}
			else
			{
				DrawChips();
				DrawStakes();
				SelectChip();
			}
		}
		else
		{
			DrawStakes();
			SelectCell();
		}

		if (isExitButtonActivated)
		{
			isExitButtonActivated = false;
			return;
		}
	}
}

void DrawEngine::DrawField()
{
	std::string delimiter;
	bool isBlack = true;
	int fieldStartNumber = 0;

	SetColor(FieldColors.first, FieldColors.second);

	//main block

	std::cout << std::string(85, '=') << std::endl;

	for (int y = 0; y < 3; y++)
	{
		fieldStartNumber = 3 - y;
		delimiter = "|";
		std::cout << "|";

		for (int x = 0; x < 14; x++)
		{
			//zero
			if (x == 0)
			{
				if (selectedCell.first == 0 && selectedCell.second < 3)
				{
					SetColor(SelectedCellColors.first, SelectedCellColors.second);
					selectedCellValue = 0;
				}


				std::cout << std::format("{0:^5}", y == 1 ? "0" : "");

				SetColor(FieldColors.first, FieldColors.second);
				std::cout << "|";
				continue;
			}

			delimiter += std::format("{0:5}{1:}", std::string(5, '-'), x == 0 || x == 12 || x == 13 ? "|" : "+");

			// 2to1
			if (x == 13)
			{
				SetColor(FieldColors.first, FieldColors.second);

				if (selectedCell.first == 13 && selectedCell.second == y)
				{
					SetColor(SelectedCellColors.first, SelectedCellColors.second);
					selectedCellValue = 100 + y + 1;
				}


				std::cout << std::format("{0:^5}", "1to12");

				SetColor(FieldColors.first, FieldColors.second);
				std::cout << "|";
				continue;
			}

			//other field

			isBlack ? SetColor(BlackCellColors.first, BlackCellColors.second) : SetColor(RedCellColors.first, RedCellColors.second);

			if (selectedCell.first == x && selectedCell.second == y)
			{
				SetColor(SelectedCellColors.first, SelectedCellColors.second);
				selectedCellValue = fieldStartNumber + (3 * (x - 1));
			}

			std::cout << std::format("{0:^5}", fieldStartNumber + (3 * (x - 1)));

			SetColor(FieldColors.first, FieldColors.second);
			std::cout << "|";

			if (x != 12)
				isBlack = !isBlack;
		}

		std::cout << std::endl;

		if (y == 2)
		{
			std::cout << std::string(85, '=') << std::endl;
			delimiter = "";
			break;
		}


		//zero escapes delimiter
		std::cout << "|";
		if (selectedCell.first == 0 && selectedCell.second < 3)
			SetColor(SelectedCellColors.first, SelectedCellColors.second);
		std::cout << "     ";

		SetColor(FieldColors.first, FieldColors.second);
		std::cout << delimiter << std::endl;
	}

	//xto12 block
	delimiter = "      |";
	std::cout << delimiter;
	for (int i = 0; i < 3; i++)
	{
		if (selectedCell.first == i && selectedCell.second == 3)
		{
			SetColor(SelectedCellColors.first, SelectedCellColors.second);
			selectedCellValue = 200 + i + 1;
		}

		std::cout << std::format("{0:^23}", std::format("{0:}{1:}{2:}", i + 1, i == 0 ? "st" : i == 1 ? "nd" : "rd", 12));
		delimiter += std::format("{0:^23}{1:}", std::string(23, '-'), i != 2 ? "+" : "|");

		SetColor(FieldColors.first, FieldColors.second);
		std::cout << "|";
	}
	std::cout << std::endl << delimiter << std::endl;

	//x2stake block
	delimiter = "      |";
	std::cout << delimiter;
	for (int i = 0; i < 6; i++)
	{
		std::string fieldStr;
		std::string formattedField;

		if (i == 2)
			SetColor(BlackCellColors.first, BlackCellColors.second);

		if (i == 3)
			SetColor(RedCellColors.first, RedCellColors.second);

		if (selectedCell.first == i && selectedCell.second == 4)
		{
			SetColor(SelectedCellColors.first, SelectedCellColors.second);

			switch (i)
			{
			case 0: selectedCellValue = 501; break;
			case 1: selectedCellValue = 301; break;
			case 2: selectedCellValue = 401; break;
			case 3: selectedCellValue = 402; break;
			case 4: selectedCellValue = 302; break;
			case 5: selectedCellValue = 502; break;
			}
		}

		switch (i)
		{
		case 0: fieldStr = "1to18"; break;
		case 1: fieldStr = "Even"; break;
		case 2: fieldStr = "Black"; break;
		case 3: fieldStr = "Red"; break;
		case 4: fieldStr = "Odd"; break;
		case 5: fieldStr = "19to36"; break;
		}

		std::cout << std::format("{0:^11}", fieldStr);

		SetColor(FieldColors.first, FieldColors.second);
		std::cout << "|";
	}
	std::cout << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;

	std::cout << std::endl << std::endl;
}

void DrawEngine::DrawSpinButton()
{
	std::string delimiter;
	//spin button block
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
	delimiter = "      |";
	std::cout << delimiter;

	if (selectedCell.second == 5)
		SetColor(SelectedCellColors.first, SelectedCellColors.second);

	std::cout << std::format("{0:^71}", "SPIN");

	SetColor(FieldColors.first, FieldColors.second);
	std::cout << "|" << std::endl;

	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
}

void DrawEngine::SelectCell()
{
	int x = selectedCell.first;
	int y = selectedCell.second;

	char choice = _getch();

	switch (choice) {
	case KEY_ARROW_UP:
	{
		y--;

		if (y < 0)
			y = 0;

		if (y > 5)
			y = 5;

		if (y == 4)
			x = 0;

		if (y == 3)
			x = x / 2;

		if (y == 2)
			x = 1 + (x * 4);
	}
	break;
	case KEY_ARROW_DOWN:
	{
		y++;

		if (y < 0)
			y = 0;

		if (y > 5)
			y = 5;

		if (x == 0 && y < 3)
			y = 3;


		if (y == 3)
		{
			x = (x - 1) / 4;

			if (x == 3)
				x = 2;
		}

		if (y == 4)
			x = x * 2;

		if (y == 5)
			x = 0;
	}
	break;
	case KEY_ARROW_RIGHT:
	{
		x++;

		if (x < 0)
			x = 0;

		if (y <= 2 && x > 13)
			x = 13;

		if (y == 3 && x > 2)
			x = 2;

		if (y == 4 && x > 5)
			x = 5;

		if (y == 5)
			x = 0;
	}
	break;
	case KEY_ARROW_LEFT:
	{
		x--;

		if (x < 0)
			x = 0;

		if (y <= 2 && x > 13)
			x = 13;

		if (y == 3 && x > 2)
			x = 2;

		if (y == 4 && x > 5)
			x = 5;

		if (y == 5)
			x = 0;
	}
	break;
	case KEY_ENTER:
	{
		isCellSelected = true;

		if (selectedCell.second == 5)
			isSpinActivated = true;
	}
	break;
	case KEY_ESC: isExitButtonActivated = true; return;
	}

	selectedCell = std::make_pair(x, y);
}

void DrawEngine::DrawChips()
{
	std::vector<int> chipNominals = { 5, 10, 25, 50, 100, 500 };

	SetColor(FieldColors.first, FieldColors.second);
	std::cout << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
	std::cout << std::format("{0:}|{1:^71}|", "      ", "Select chip") << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
	std::cout << "      |";

	int i = 0;
	for (auto x : chipNominals)
	{
		if (selectedChip == i)
		{
			SetColor(SelectedCellColors.first, SelectedCellColors.second);
			selectedChipNominal = x;
		}

		std::cout << std::format("{0:^11}", x);

		SetColor(FieldColors.first, FieldColors.second);
		std::cout << "|";
		i++;
	}
	std::cout << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
}

void DrawEngine::SelectChip()
{
	int x = selectedChip;

	char choice = _getch();

	switch (choice) {
	case KEY_ARROW_RIGHT:
	{
		x++;

		if (x < 0)
			x = 0;

		if (x > 5)
			x = 5;
	}
	break;
	case KEY_ARROW_LEFT:
	{
		x--;

		if (x < 0)
			x = 0;

		if (x > 5)
			x = 5;
	}
	break;
	case KEY_ENTER:
	{
		if (moneyAmount - selectedChipNominal < 0)
			return;

		stakes.push_back(std::make_pair(selectedCellValue, selectedChipNominal));

		moneyAmount -= selectedChipNominal;

		selectedChip = 1;
		selectedChipNominal = 0;
		isCellSelected = false;
	}
	break;
	case KEY_ESC:
	{
		selectedChip = 1;
		selectedChipNominal = 0;
		isCellSelected = false;

		return;
	}
	}

	selectedChip = x;
}

void DrawEngine::DrawStakes()
{
	std::cout << std::endl << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
	std::cout << std::format("{0:}|{1:^71}|", "      ", "Stakes") << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;
	std::cout << std::format("{0:}|{1:^35}|{2:^35}|", "      ", "Cell", "Value") << std::endl;
	std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;

	int i = 0;
	for (auto& s : stakes)
	{
		std::cout << std::format("{0:}|{1:^35}|{2:^35}|", "      ", GetStakeName(s.first), s.second) << std::endl;
		if (i != stakes.size() - 1)
			std::cout << std::format("{0:}|{1:^35}+{2:^35}|", "      ", std::string(35, '-'), std::string(35, '-')) << std::endl;
		else
			std::cout << std::format("{0:}{1:}", "      ", std::string(73, '=')) << std::endl;

		i++;
	}
}

void DrawEngine::SpinRoulette()
{
	CheckStakes(GenerateRouletteSpin());

	isSpinActivated = false;
	isCellSelected = false;
	selectedCell = { 1, 0 };
}

int DrawEngine::GenerateRouletteSpin()
{
	int i = 0;
	int j = 0;
	std::vector<std::string> wheels = { Wheel1, Wheel2, Wheel3 };

	while (i != 50)
	{
		if (i < 20)
			Sleep(10);

		if (i >= 20 && i < 30)
			Sleep(50);

		if (i >= 30 && i < 40)
			Sleep(75);

		if (i > 40)
			Sleep(100);

		system("cls");

		std::cout << wheels[j];
		std::cout << std::format("{0:^100}", "^") << std::endl;
		std::cout << std::format("{0:^100}", "|") << std::endl;

		std::cout << std::format("{0:40}", std::string(40, '=')) << std::endl;
		std::cout << std::format("|{0:^38}|", std::format("{0:}{1:}", "Win number: ", Randomizer::GetNextInt(0, 36))) << std::endl;
		std::cout << std::format("{0:40}", std::string(40, '=')) << std::endl;

		i++;
		j = j == 2 ? 0 : j + 1;
	}

	system("cls");
	int generatedNum = Randomizer::GetNextInt(0, 36);
	std::cout << wheels[Randomizer::GetNextInt(0, 2)];
	std::cout << std::format("{0:^100}", "^") << std::endl;
	std::cout << std::format("{0:^100}", "|") << std::endl;

	std::cout << std::format("{0:40}", std::string(40, '=')) << std::endl;
	std::cout << std::format("|{0:^38}|", std::format("{0:}{1:}", "Win number: ", generatedNum)) << std::endl;
	std::cout << std::format("{0:40}", std::string(40, '=')) << std::endl;
	std::cout << std::endl;

	return generatedNum;
}

void DrawEngine::CheckStakes(int winCell)
{
	std::vector<std::pair<int, int>> wins;
	std::vector<std::pair<int, int>> loses;
	int prevCash = moneyAmount;

	for (auto& s : stakes)
	{
		prevCash += s.second;

		if (s.first < 100)
		{
			if (s.first == winCell)
			{
				moneyAmount += s.second * 36;
				wins.push_back(std::make_pair(s.first, s.second));
			}
			else
			{
				loses.push_back(std::make_pair(s.first, s.second));
			}

			continue;
		}

		if (winCell == 0)
		{
			loses.push_back(std::make_pair(s.first, s.second));
			continue;
		}

		if (s.first > 100 && s.first < 200)
		{
			int stakeLineNum = s.first - 100;
			int winLineNum = winCell % 3 == 0 ? 1 : winCell % 3 == 2 ? 2 : 3;

			if (stakeLineNum == winLineNum)
			{
				moneyAmount += s.second * 3;
				wins.push_back(std::make_pair(s.first, s.second));
			}
			else
			{
				loses.push_back(std::make_pair(s.first, s.second));
			}

			continue;
		}

		if (s.first > 200 && s.first < 300)
		{
			int stakeDozenNum = s.first - 200;
			int winDozenNum = winCell < 13 ? 1 : winCell > 24 ? 3 : 2;

			if (stakeDozenNum == winDozenNum)
			{
				moneyAmount += s.second * 3;
				wins.push_back(std::make_pair(s.first, s.second));
			}
			else
			{
				loses.push_back(std::make_pair(s.first, s.second));
			}

			continue;
		}


		if (s.first > 300 && s.first < 500)
		{
			int stakeCond = s.first - (s.first < 400 ? 300 : 400);
			int winCond = winCell % 2 == 0 ? 1 : 2;

			if (stakeCond == winCond)
			{
				moneyAmount += s.second * 2;
				wins.push_back(std::make_pair(s.first, s.second));
			}
			else
			{
				loses.push_back(std::make_pair(s.first, s.second));
			}

			continue;
		}

		if (s.first > 500)
		{
			int stakeCond = s.first - 500;
			int winCond = winCell < 19 ? 1 : 2;

			if (stakeCond == winCond)
			{
				moneyAmount += s.second * 2;
				wins.push_back(std::make_pair(s.first, s.second));
			}
			else
			{
				loses.push_back(std::make_pair(s.first, s.second));
			}

			continue;
		}
	}

	std::cout << std::format("{0:40}{1:5}{0:40}", std::string(40, '='), "") << std::endl;
	std::cout << std::format("|{0:^38}|{1:5}|{2:^38}|", "Win stakes", "", "Lose stakes") << std::endl;
	std::cout << std::format("{0:40}{1:5}{0:40}", std::string(40, '='), "") << std::endl;

	int maxRows = wins.size() > loses.size() ? wins.size() : loses.size();

	for (int i = 0; i < maxRows; i++)
	{
		std::pair<int, int> winsPair = { -1, -1 };
		std::pair<int, int> losesPair = { -1, -1 };
		std::string firstVectorRow;
		std::string firstVectorDelimRow;
		std::string secondVectorRow;
		std::string secondVectorDelimRow;

		if (i < wins.size())
			winsPair = wins[i];

		if (i < loses.size())
			losesPair = loses[i];


		firstVectorRow = std::format(
			"{0:^20}{1:^20}",
			winsPair.first == -1 ? "" : std::format("|{0:^19}|", GetStakeName(winsPair.first)),
			winsPair.second == -1 ? "" : std::format("{0:^18}|", winsPair.second)
		);
		firstVectorDelimRow = std::format(
			"{0:40}",
			winsPair.first == -1 ? "" : std::format("|-{0:^18}+{0:^18}|", std::string(18, '-'))
		);

		firstVectorRow = firstVectorRow.substr(0, 40);

		if (i == wins.size() - 1)
			firstVectorDelimRow = std::format("{0:40}", std::string(40, '='));

		if (i > wins.size() - 1)
			firstVectorDelimRow = std::format("{0:40}", "");

		secondVectorRow = std::format(
			"{0:5}{1:^20}{2:^20}",
			"",
			losesPair.first == -1 ? "" : std::format("|{0:^19}|", GetStakeName(losesPair.first)),
			losesPair.second == -1 ? "" : std::format("{0:^18}|", losesPair.second)
		);
		secondVectorDelimRow = losesPair.first == -1 ? "" : std::format("{0:5}|-{1:^18}+{1:^18}|", "", std::string(18, '-'));

		if (i == loses.size() - 1)
		{
			secondVectorDelimRow = std::format("{0:5}{1:40}", "", std::string(40, '='));
		}

		std::cout << std::format("{0:}{1:}", firstVectorRow, secondVectorRow) << std::endl;
		std::cout << std::format("{0:}{1:}", firstVectorDelimRow, secondVectorDelimRow) << std::endl;
	}

	int summary = moneyAmount - prevCash;
	std::cout << std::endl;
	std::cout << std::format("{0:20}", std::string(20, '=')) << std::endl;
	std::cout << std::format("|{0:^18}|", "Summary") << std::endl;
	std::cout << std::format("{0:20}", std::string(20, '=')) << std::endl;
	std::cout << std::format("|{0:^18}|", std::format("{0:}{1:}", summary < 0 ? '-' : '+', std::abs(summary))) << std::endl;
	std::cout << std::format("{0:20}", std::string(20, '=')) << std::endl;

	std::cout << "Press any key to coninue";
	_getch();
}

void DrawEngine::SetColor(ConsoleColor text, ConsoleColor background)
{
	SetConsoleTextAttribute(hConsoleHandle, (WORD)((background << 4) | text));
}

std::string DrawEngine::GetStakeName(const int stakeCode)
{
	switch (stakeCode)
	{
	case 101: return "1st line";
	case 102: return "2nd line";
	case 103: return "3rd line";
	case 201: return "1st dozen";
	case 202: return "2nd dozen";
	case 203: return "3rd dozen";
	case 301: return "Even";
	case 302: return "Odd";
	case 401: return "Black";
	case 402: return "Red";
	case 501: return "1to18";
	case 502: return "19to36";
	default: return std::format("{0:}", stakeCode); break;
	}
}
