#include <iostream>
#include <math.h>

const float base = 10;
const float k    = 150;
// const float k    = 32;

class ELO {
private:
	float rating1, rating2;
	float transRating1, transRating2;
	float winProb1, winProb2;

public:
	ELO(float rating1, float rating2);

	float getUpdatedRating1(float score1);
	float getUpdatedRating2(float score2);
	void printOutcomes();
};

ELO::ELO(float rating1, float rating2) {
	(*this).rating1 = rating1;
	(*this).rating2 = rating2;

	transRating1 = pow(base, rating1 / 400);
	transRating2 = pow(base, rating2 / 400);

	winProb1 = transRating1 / (transRating1 + transRating2);
	winProb2 = transRating2 / (transRating1 + transRating2);
}

float ELO::getUpdatedRating1(float score1) {
	return rating1 + k * (score1 - winProb1);
}
float ELO::getUpdatedRating2(float score2) {
	return rating2 + k * (score2 - winProb2);
}
void ELO::printOutcomes() {
	printf(
	"+---------+---------+---------+\n"
	"| Outcome | Rating1 | Rating2 |\n"
	"+---------+---------+---------+\n"
	"| Current | %7d | %7d |\n"
	"| 1 Wins  | %7d | %7d |\n"
	"| 1 Loses | %7d | %7d |\n"
	"| Draw    | %7d | %7d |\n"
	"+---------+---------+---------+\n", 
	(int)rating1,                (int)rating2,
	(int)getUpdatedRating1(1),   (int)getUpdatedRating2(0),
	(int)getUpdatedRating1(0),   (int)getUpdatedRating2(1),
	(int)getUpdatedRating1(0.5), (int)getUpdatedRating2(0.5));
}

int main() {
	float rating1, rating2;

	std::cout << "Enter rating1: ";
	std::cin >> rating1;
	std::cout << "Enter rating2: ";
	std::cin >> rating2;

	ELO elo(rating1, rating2);
	elo.printOutcomes();

	// float target = 1139;
	// for (float i = 0; i < 200; i++) {
	// 	k = i;
	// 	if ((int)elo.getUpdatedRating1(1) == target)
	// 		std::cout << i << std::endl;
	// }

	return 0;
}
