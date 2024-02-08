#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <ctime>

using namespace std;

struct candle {
	double min;
	double open;
	double close;
	double max;
};

void menu(ofstream& logs);

void saveLogs(ofstream& logs, const char action[], const char value[] = "\0");

void skipLinesIn(int n, ofstream& chart, int length);

void skipLinesOut(int n, ifstream& data);

double yScaleRound(double a, double yScale);

double dRound(double a, int b);

bool compare(double a, char sign, double b);

bool compareDates(char data[], char date2[]);

void fillFile(ofstream& chart, int height, int length, double cursor, double yScale, int xScale, char startDate[], char finishDate[]);

void count(ifstream& data, double& maxVal, double& minVal, int& startLine, int& finishLine, char startDate[], char finishDate[]);

void drawGraph(ofstream& logs, char startDate[], char finishDate[], int xScale, char fileNameIn[], char fileNameOut[], int height);

void candleDraw(ofstream& chart, candle graph, int i, double cursor, int length, double yScale);


int main() {
	ofstream logs;
	logs.open("history.log");
	saveLogs(logs, "Uruchomiono program");
	menu(logs);
}

void menu(ofstream& logs) {
	char startDate[11];
	char finishDate[11];
	char fileNameIn[50];
	char fileNameOut[50];
	char convert[50];
	int xScale;
	int height;

	char input;
	cout << "Program rysujacy wykresy swiecowe (wykresy Sakata)\n";
	cout << "- czyli wykresy pomagajace w analizie wynikow spolek gieldowych.\n";
	cout << "Autor Projektu: Maciej Nowicki 197643 ACiR\n\n";

	cout << "g - narysuj wykres dla pliku intc_us_data.csv\n";
	cout << "d - narysuj wykres dla dowolnego pliku \".csv\"\n";
	cout << "q - zamknij program\n";

	cin >> input;

	if (input == 'g') {
		saveLogs(logs, "Wcisnieto g - rysowanie wykresu podstawowego");
		height = 50;
		strcpy_s(startDate, "2023-02-13");
		strcpy_s(finishDate, "2023-11-28");
		xScale = 1;
		strcpy_s(fileNameIn, "intc_us_data.csv");
		strcpy_s(fileNameOut, "chart.txt");

		drawGraph(logs, startDate, finishDate, xScale, fileNameIn, fileNameOut, height);
	}
	else if (input == 'd') {
		saveLogs(logs, "Wcisnieto d");
		system("CLS");

		cout << "Daty nalezy podawac w formacie \"rrrr-mm-dd\"\n";
		cout << "Podaj wysokosc wykresu (liczba calkowita): ";
		cin >> height;
		sprintf_s(convert, "%d", height);
		saveLogs(logs, "Podano wysokość wykresu:", convert);

		cout << "\nPodaj zakres czasowy reprezentowany przez 1 swiece: \n1. Dzien";
		cout << "\n5. Tydzien(przyjmujemy 5 dni)";
		cout << "\n20. Miesiac(przyjmujemy 20 dni)";
		cout << "\nDla innych liczb, program bedzie loczyl podana liczbe swiec";
		cout << "\nUWAGA! Gdy wybrano 5 lub 20, a daty nie beda sie roznily wielokrotnoscia wybranego okresu czasu,\nto program (zaokragili) utnie lub dopelni ostatni okres do wybranego\n";
		cin >> xScale;
		sprintf_s(convert, "%d", xScale);
		saveLogs(logs, "Podano zakres 1 swiecy:", convert);

		cout << "\nPodaj date poczatkowa: ";
		cin >> startDate;
		saveLogs(logs, "Podano date poczatkowa:", startDate);
		cout << "\nPodaj date koncowa: ";
		cin >> finishDate;
		saveLogs(logs, "Podano date koncowa:", finishDate);

		cout << "\nPodaj nazwe pliku z danymi \".csv\" (np. \"fileName.csv\"): ";
		cin >> fileNameIn; 
		saveLogs(logs, "Podano nazwe pliku z danymi:", fileNameIn);
		cout << "\nPodaj nazwe pliku z wyjsciowego: ";
		cin >> fileNameOut;
		saveLogs(logs, "Podano nazwe pliku wyjsciowego:", fileNameOut);
		saveLogs(logs, "Rysowanie wykresu");

		drawGraph(logs, startDate, finishDate, xScale, fileNameIn, fileNameOut, height);
	}
	else if (input == 'q') {
		exit(1);
		logs.close();
	}
	else {
		system("CLS");
		menu(logs);
	}
}

void saveLogs(ofstream& logs, const char action[], const char value[]) {
	char log[26];
	time_t currTime = time(NULL);

	if (logs.is_open()) {
		ctime_s(log, sizeof log, &currTime);
		logs << log << " - " << action << ' ' << value << endl;
	}
	else {
		cout << "\nBlad otwarcie pliku history.log\n";
	}
}

void skipLinesIn(int n, ofstream& chart, int length) { // pomin linie przy wpisywaniu do pliku
	for (int i = 0; i < n; i++) {
		chart.seekp(length + 2, ios::cur);
	}
}

void skipLinesOut(int n, ifstream& data) { // pomin linie przy czytaniu pliku
	char useless[100];
	for (int i = 0; i < n; i++) {
		data.getline(useless, 100);
	}
}

double yScaleRound(double a, double yScale) {
	if (compare((a / yScale), '>', (int(a / yScale) + 0.5))) {
		a = (int(a / yScale) + 1) * yScale;
	}
	else {
		a = int(a / yScale) * yScale;
	}
	return a;
}

double dRound(double a, int b) { // decimal Round
	a = int(round(a * pow(10.0, b))) / pow(10.0, b);
	return a;
}

bool compare(double a, char sing, double b) {
	if (sing == '=') { // ==
		if (fabs(dRound(a, 4) - dRound(b, 4)) < DBL_EPSILON) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (sing == '>') { // >=
		if (fabs(dRound(a, 4) - dRound(b, 4)) < DBL_EPSILON || dRound(a, 4) > dRound(b, 4)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else if (sing == '<') { // <=
		if (fabs(dRound(a, 4) - dRound(b, 4)) < DBL_EPSILON || dRound(a, 4) < dRound(b, 4)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

bool compareDates(char date1[], char date2[]) { // sprawdza czy data nie jest większa niz szukana ergo - szukanej nie ma w pliku a zamiast niej zostanie uzyta inna
	int year1 = 1000 * (int)date1[0] + 100 * (int)date1[1] + 10 * (int)date1[2] + (int)date1[3];
	int year2 = 1000 * (int)date2[0] + 100 * (int)date2[1] + 10 * (int)date2[2] + (int)date2[3];
	int month1 = (int)date1[5] * 10 + (int)date1[6];
	int month2 = (int)date2[5] * 10 + (int)date2[6];
	int day1 = (int)date1[8] * 10 + (int)date1[9];
	int day2 = (int)date2[8] * 10 + (int)date2[9];

	if ((year1 > year2) || (year2 == year1 && month1 > month2) || (year1 == year2 && month1 == month2 && day1 > day2)) {
		return 1;
	}
	else {
		return 0;
	}
}

void fillFile(ofstream& chart, int height, int length, double cursor, double yScale, int xScale, char startDate[], char finishDate[]) {
	chart << setw(11) << setiosflags(ios::right) << yScale << "^\n";
	for (int row = 0; row < height; row++, cursor -= yScale) {
		chart << setw(11) << setiosflags(ios::right) << cursor << '|';
		for (int col = 0; col < length; col++) {
			chart << ' ';
		}
		chart << '\n';
	}
	chart << setw(12) << '|';
	for (int col = 0; col < length; col++) {
		chart << '_';
	}
	chart << ">\n";
	chart << setw(13) << '|';
	for (int i = 0; i < length - 2; i++) {
		chart << ' ';
	}
	chart << "| " << xScale << "dni" << '\n';

	for (int i = 0; i < 3; i++) {
		chart << ' ';
	}

	chart << startDate;
	for (int i = 0; i < length - 2; i++) {
		chart << ' ';
	}
	chart << finishDate;
}

void count(ifstream& data, double& maxVal, double& minVal, int& startLine, int& finishLine, char startDate[], char finishDate[]) {
	candle values;
	int file_len = 0;
	char useless[100];
	char delim;
	char date[11] = "0000-00-00";

	file_len++;
	data.getline(useless, 100);
	while (strcmp(date, startDate) != 0) {
		file_len++;
		data.getline(date, 11, ',');
		data.getline(useless, 100);
		if (compareDates(date, startDate)) {
			strcpy_s(startDate, 11, date);
			break;
		}
	}
	startLine = file_len;
	file_len++;
	data.getline(date, 11, ',');
	data >> values.open >> delim >> maxVal >> delim >> minVal;
	data.getline(useless, 100);
	while (data.peek() != EOF) {
		file_len++;
		data.getline(date, 11, ',');
		if (strcmp(date, finishDate) == 0) {
			finishLine = file_len;
			break;
		}
		else if (compareDates(date, finishDate)) {
			strcpy_s(finishDate, 11, date);
			finishLine = file_len - 1;
			break;
		}

		data >> delim >> values.open >> delim >> values.max >> delim >> values.min;
		if (values.max > maxVal) {
			maxVal = values.max;
		}
		else if (values.min < minVal) {
			minVal = values.min;
		}

		data.getline(useless, 100);
	}
	if (finishLine == 0) {
		strcpy_s(finishDate, 11, date);
		finishLine = file_len;
	}
	data.seekg(0);
}

void drawGraph(ofstream& logs, char startDate[], char finishDate[], int xScale, char fileNameIn[], char fileNameOut[], int height) {
	double maxVal, minVal;
	int startLine, finishLine = 0;

	char useless[100]; // uselsess text here
	char delim; //delimiter - separator
	
	ifstream data;
	ofstream chart;
	data.open(fileNameIn);//intc_us_data.csv
	chart.open(fileNameOut);
	if (data.is_open()) {
		if (chart.is_open()) {
			candle graph;
			candle candlePart;

			count(data, maxVal, minVal, startLine, finishLine, startDate, finishDate);

			int length = round((finishLine - startLine + 1) / double(xScale));
			double yScale = dRound((maxVal - minVal) / (height - 1), 4);
			double cursor = yScaleRound(maxVal, yScale);

			fillFile(chart, height, length, cursor, yScale, xScale, startDate, finishDate);

			skipLinesOut(startLine - 1, data);
			for (int i = 0; i < length; i++) {
				if (xScale == 1) {
					data.seekg(11, ios::cur); // pomijamy datę bo jej tu nie używamy
					data >> graph.open >> delim >> graph.max >> delim >> graph.min >> delim >> graph.close;
					data.getline(useless, 100);
				}
				else if (xScale == 5 || xScale == 20) {
					data.seekg(11, ios::cur); // pomijamy datę bo jej tu nie używamy
					data >> graph.open >> delim >> graph.max >> delim >> graph.min;

					for (int i = 0; i < xScale - 1; i++) {
						data.getline(useless, 100);
						data.seekg(11, ios::cur); // pomijamy datę bo jej tu nie używamy
						data >> candlePart.open >> delim >> candlePart.max >> delim >> candlePart.min >> delim;
						if (candlePart.max > graph.max) {
							graph.max = candlePart.max;
						}
						if (candlePart.min < graph.min) {
							graph.min = candlePart.min;
						}
					}
					data >> graph.close;
					data.getline(useless, 100);
				}
				else {
					system("CLS");
					cout << "Podano nieprawidlowy zakres" << endl;
					menu(logs);
				}
				candleDraw(chart, graph, i, cursor, length, yScale);
				cursor = yScaleRound(maxVal, yScale);
			}
			chart.close();
		}
		else {
			system("CLS");
			cout << "Nie mozna otworyc pliku do narysowania wykresu\n\n";
			menu(logs);
		}
	}
	else {
		system("CLS");
		cout << "Nie mozna otworyc pliku z danymi\n\n";
		menu(logs);
	}
	data.close();

	system("CLS");
	cout << "Wykres narysowano z sukcesem!" << endl;
	menu(logs);
}

void candleDraw(ofstream& chart, candle graph, int i, double cursor, int length, double yScale) {
	chart.seekp(26 + i, ios::beg);
	for (; compare(cursor, '>', yScaleRound(graph.min, yScale)); cursor -= yScale) {
		if (compare(cursor, '<', yScaleRound(graph.max, yScale))) {
			if (compare(cursor, '<', yScaleRound(graph.open, yScale))
				&& (compare(cursor, '>', yScaleRound(graph.close, yScale)))
				&& (compare(graph.open, '>', graph.close))) {

				chart << '#';
			}
			else if (compare(cursor, '<', yScaleRound(graph.close, yScale))
				&& compare(cursor, '>', yScaleRound(graph.open, yScale))
				&& (graph.open < graph.close)) {

				chart << 'O';
			}
			else {
				chart << '|';
				
			}
			chart.seekp(-1, ios::cur);
		}
		skipLinesIn(1, chart, length + 1 + 11);
	}
}