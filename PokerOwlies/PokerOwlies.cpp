// PokerOwlies.cpp: define el punto de entrada de la aplicación.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

typedef std::pair<char, char> card;

enum HAND_RESULT {

	HIGH_CARD = 1,
	ONE_PAIR = 2, 
	TWO_PAIR = 3, 
	THREE_KIND = 4, 
	STRAIGHT = 5, 
	FLUSH = 6, 
	FULL_HOUSE = 7, 
	FOUR_KIND = 8, 
	STRAIGHT_FLUSH = 9, 
	ROYAL_FLUSH = 10, 

};

bool opGreaterThan(card _a, card _b);
bool opConsecutiveOf(card _a, card _b);
void sortTable(std::vector<card>& _t);

bool opGreaterThan(card _a, card _b) {
	std::vector<char> numberCards = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };

	int A = std::distance(numberCards.begin(), std::find(numberCards.begin(), numberCards.end(), _a.first));
	int B = std::distance(numberCards.begin(), std::find(numberCards.begin(), numberCards.end(), _b.first));

	return A > B;
}


bool opConsecutiveOf(card _a, card _b) {
	std::vector<char> numberCards = { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' };

	int A = std::distance(numberCards.begin(), std::find(numberCards.begin(), numberCards.end(), _a.first));
	int B = std::distance(numberCards.begin(), std::find(numberCards.begin(), numberCards.end(), _b.first));

	if (B == A + 1)
		return true;

	return false;
}

void sortTable(std::vector<card>& _t) {
	std::vector<card> result;
	card minCard;
	int index;
	while (!_t.empty()) {
		minCard = _t[0];
		index = 0;
		for (int i = 0; i < _t.size(); i++) {
			if (opGreaterThan(minCard, _t[i])) {
				minCard = _t[i];
				index = i;
			}
		}

		_t.erase(_t.begin() + index);
		result.push_back(minCard);
	}

	_t = result;
}

std::pair<HAND_RESULT, card> wtf(std::pair<card, card>& _p, std::vector<card>& _t);

std::pair<HAND_RESULT, card> wtf(std::pair<card, card>& _p, std::vector<card>& _t) {
	HAND_RESULT r = HIGH_CARD;
	card c = std::make_pair('A', 'C');

	// Checking for a STRAIGHT FLUSH or ROYAL FLUSH
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;
		m.push_back(_p.first);
		m.push_back(_p.second);
		bool straight = false;
		int countConsecutive = 0;
		sortTable(m);
		for (int i = 0; i < m.size() - 1; i++) {
			if (opConsecutiveOf(m[i], m[i + 1]) && m[i].second == m[i+1].second) {
				countConsecutive++;
			}
			else {
				break;
			}

			if (countConsecutive >= 5) {
				r = STRAIGHT_FLUSH;
				c = m[i + 1];
			}
		}

		if (r == STRAIGHT_FLUSH && c.first == 'A') {
			r = ROYAL_FLUSH;
		}
	}

	// Checking for a FOUR OF A KIND
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;
		m.push_back(_p.first);
		m.push_back(_p.second);

		for (auto _m : m) {
			int count = 0;
			std::for_each(m.begin(), m.end(), [&](card _c) { if (_c.first == _m.first) count++; });
			if (count == 4) {
				r = FOUR_KIND;
				c = _m;
			}
		}
	}

	// Checking for a FULL HOUSE
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;
		m.push_back(_p.first);
		m.push_back(_p.second);

		for (auto _m : m) {
			int count = 0;
			std::for_each(m.begin(), m.end(), [&](card _c) { if (_c.first == _m.first) count++; });
			if (count == 3) {
				r = THREE_KIND;
				c = _m;
			}
		}

		if (r == THREE_KIND) {
			for (auto _m : m) 
			{
				if (_m != c) 
				{
					int count = 0;
					std::for_each(m.begin(), m.end(), [&](card _c) { if (_c.first == _m.first) count++; });
					if (count == 2) {
						r = FULL_HOUSE;
					}
				}
			}
		}

		if (r != FULL_HOUSE)
			r = HIGH_CARD;
	}

	// Checking for a FLUSH
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;
		m.push_back(_p.first);
		m.push_back(_p.second);

		std::vector<char> colors = { 'D', 'S', 'H', 'C' };
		
		for (auto col: colors) {
			int count = 0;
			std::vector<card> result;
			std::for_each(m.begin(), m.end(), 
			[&](card _m) { 
				if (_m.second == col) { 
					count++; 
					result.push_back(_m);
				} 
			});

			if (count >= 5) {
				sortTable(result);
				c = result[result.size()-1];
				r = FLUSH;
			}
		}
	}

	// Checking for a STRAIGHT
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;
		m.push_back(_p.first);
		m.push_back(_p.second);
		bool straight = false;
		int countConsecutive = 0;
		sortTable(m);
		for ( int i = 0; i < m.size() - 1; i++ ) {
			if (opConsecutiveOf(m[i], m[i+1])) {
				countConsecutive++;
			}
			else {
				break;
			}

			if (countConsecutive >= 5) {
				r = STRAIGHT;
				c = m[i+1];
			}
		}
	}

	// Checking for a THREE OF A KIND
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;
		m.push_back(_p.first);
		m.push_back(_p.second);

		for (auto _m : m) {
			int count = 0;
			std::for_each(m.begin(), m.end(), [&](card _c) { if (_c.first == _m.first) count++; });
			if (count == 3) {
				r = THREE_KIND;
				c = _m;
			}
		}
	}

	// Checking for a TWO PAIR
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;

		m.push_back(_p.first);
		m.push_back(_p.second);

		bool hasOnePair = false;
		for (auto _m : m) {
			int count = 0;
			std::for_each(m.begin(), m.end(), [&](card _c) { if (_c.first == _m.first) count++; });
			if (count == 2) {
				if (hasOnePair && _m.first != c.first) {
					c = opGreaterThan(_m, c) ? _m : c;
					r = TWO_PAIR;
				}
				else {
					c = _m;
					hasOnePair = true;
				}
			}
		}
	}

	// Checking for a ONE PAIR
	if (r == HIGH_CARD) {
		std::vector<card> m = _t;

		m.push_back(_p.first);
		m.push_back(_p.second);

		for (auto _m : m) {
			int count = 0;
			std::for_each(m.begin(), m.end(), [&](card _c) { if (_c.first == _m.first) count++; });
			if (count == 2) {
				if (r == ONE_PAIR) {
					c = opGreaterThan(_m, c) ? _m : c;
				}
				else {
					c = _m;
					r = ONE_PAIR;
				}
			}
		}
	}

	// Checking for a HIGH CARD
	if (r == HIGH_CARD) {
		c = opGreaterThan(_p.first, _p.second) ? _p.first : _p.second;

		for (auto t : _t) {
			c = opGreaterThan(t, c) ? t : c;
		}
	}


	return std::make_pair(r, c);
}

int main()
{
	srand(time(0));
	long long counter = 0;
	do {

		std::vector<char> colorCards = { 'D', 'S', 'H', 'C' };
		std::vector<char> numberCards = { 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K' };
		std::vector<card> originalDeck;
		std::vector<std::pair<card, card>> player;
		std::vector<card> table;

		for (auto c : colorCards) {
			for (auto n : numberCards) {
				originalDeck.push_back(std::make_pair(n, c));
			}
		}

		unsigned int numberPlayers = 9;
	
		std::vector<card> gameDeck = originalDeck;
		std::random_shuffle(gameDeck.begin(), gameDeck.end());

		for (int i = 0; i < numberPlayers; i++) {
			card _cardA = gameDeck.back();
			gameDeck.pop_back();

			card _cardB = gameDeck.back();
			gameDeck.pop_back();

			player.push_back(std::make_pair(_cardA, _cardB));
	
			std::cout << "Player [" << i + 1 << "] has " << player.back().first.first << player.back().first.second << " , " << player.back().second.first << player.back().second.second << std::endl;
		}

		gameDeck.pop_back(); // Fire the front card.

		// Flop
		for (int i = 0; i < 3; i++) {
			card _cardA = gameDeck.back();
			gameDeck.pop_back();

			table.push_back(_cardA);
			std::cout << table.back().first << table.back().second << " - ";
		}

		// Turn
		gameDeck.pop_back(); // Fire the front card

		card _cardTurn = gameDeck.back();
		gameDeck.pop_back();

		table.push_back(_cardTurn);
		std::cout << table.back().first << table.back().second << " - ";

		// River
		gameDeck.pop_back(); // Fire the front card

		card _cardRiver = gameDeck.back();
		gameDeck.pop_back();

		table.push_back(_cardRiver);
		std::cout << table.back().first << table.back().second << std::endl;

		std::cout << "========" << std::endl;

		bool straightFlush = false;
		for (int i = 0; i < numberPlayers; i++) {
			if (wtf(player[i], table).first == STRAIGHT_FLUSH || wtf(player[i], table).first == ROYAL_FLUSH) {
				straightFlush = true;
			}
			std::cout << "Player [" << i + 1 << "] with " << player[i].first.first << player[i].first.second << " , " << player[i].second.first << player[i].second.second << " has " << wtf(player[i], table).first << " with " << wtf(player[i], table).second.first << std::endl;
		}
		std::cout << "Counting..." << counter << std::endl;
		counter++;

		if (straightFlush) { 
			std::cout << "BINGO!!! after " << counter << std::endl;
			system("pause");
			counter = 0;
		
		}

		system("cls");
	
	} while (true);


	return 0;
}
