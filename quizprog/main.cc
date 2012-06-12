#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;

vector<string> question;
map<string,bool> answer;
vector<bool> asked;
double numCorrect = 0.0;
vector<string> incorrect;
int counter = 1;

void LoadQuiz(string filename) {
	ifstream infile;
	infile.open(filename.c_str());
	string buf;
	while(getline(infile,buf)) {
		question.push_back(buf);
		getline(infile,buf);
		stringstream ss;
		ss << buf;
		bool ans;
		ss >> ans;
		answer[question.back()] = ans;
	}
	asked.resize(question.size());
}

void PrintQuestions() {
	for(int i=0; i<question.size(); i++)
		cout << (i+1) << ". " << question[i] << endl;
}

void PrintQuestionsWithAnswers() {
	for(int i=0; i<question.size(); i++) {
		cout << (i+1) << ". " << question[i] << endl;
		string ans = (answer[question[i]] ? "True" : "False");
		cout << ans << endl;
	}
}

bool Exhausted() {
	for(int i=0; i<asked.size(); i++)
		if(!asked[i]) return false;
	return true;
}

void AskQuestion() {
	int choice = rand() % question.size();
	while(asked[choice])
		choice = rand() % question.size();
	asked[choice] = true;
	string resp = "";
	for( ; ; ) {
		if(cin.eof()) exit(0);
		system("clear");
		cout << "Q" << counter << endl;
		cout << question[choice] << endl;
		cout << "T/F/S?" << endl;
		cin >> resp;
		cout << endl;
		if(resp == "T" || resp == "t" || resp == "F" || resp == "f" || resp == "S" || resp == "s")
			break;
	}
	bool respb;
	bool skipped;
	if(resp == "T" || resp == "t")
		respb = true;
	else if(resp == "F" || resp == "f")
		respb = false;
	else if(resp == "S" || resp == "s")
		skipped = true;	
	else {
		cout << "error" << endl;
		exit(0);
	}
	if(skipped) {
		numCorrect += 0.5;
		incorrect.push_back(question[choice]);
	}
	else {
		bool correct = (respb == answer[question[choice]]);
		if(correct) numCorrect += 1;
		else incorrect.push_back(question[choice]);
		string msg = (correct ? "Correct!" : "Incorrect.");
		cout << msg << endl << endl;
		cin.ignore();	
		cin.get();
	}
	counter++;
}

void PrintIncorrect() {
	if(incorrect.size() == 0) return;
	cout << "Incorrectly answered/skipped questions:" << endl;
	for(int i=0; i<incorrect.size(); i++) {
		cout << incorrect[i] << endl;
		string ans = (answer[incorrect[i]] ? "True" : "False");
		cout << "Answer: " << ans << endl;
		cout << endl;
	}
}

void GiveQuiz() {
	for( ; ; ) {
		if(Exhausted()) break;
		AskQuestion();
	}
}

void ScoreQuiz() {
	system("clear");
	cout << "Results" << endl;
	cout << "=======" << endl;
	int size = question.size();
	printf("Score: %3.2f/%3.2f\n",numCorrect,(double)size);
	double percent = numCorrect/(double)size*100;
	printf("Percent: %5.2f%%\n",percent);
	cout << endl;
}

int main() {
	srand(time(0));
	LoadQuiz("quizzes.txt");
	GiveQuiz();
	ScoreQuiz();
	PrintIncorrect();
	cout << endl;
}
