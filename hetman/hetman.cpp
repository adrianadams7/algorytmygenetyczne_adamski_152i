#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#define ILOSC_HETMANOW 8

class Osobnik {
public:
	int hetmani[ILOSC_HETMANOW];
	int plansza[ILOSC_HETMANOW * ILOSC_HETMANOW];
	Osobnik() {
		for (int i = 0; i < ILOSC_HETMANOW * ILOSC_HETMANOW; i++)
		{
			plansza[i] = 0;
		}
		for (int i = 0; i < ILOSC_HETMANOW; i++)
		{
			do
			{
				hetmani[i] = rand() % (ILOSC_HETMANOW * ILOSC_HETMANOW);
			} while (plansza[hetmani[i]]);
			plansza[hetmani[i]] = 1;
		}
	}

	Osobnik(Osobnik *o) {
		for (int i = 0; i < ILOSC_HETMANOW * ILOSC_HETMANOW; i++)
		{
			plansza[i] = 0;
		}
		for (int i = 0; i < ILOSC_HETMANOW; i++)
		{
			hetmani[i] = o->hetmani[i];
			plansza[hetmani[i]] = 1;
		}
	}

	Osobnik(Osobnik const &o) {
		for (int i = 0; i < ILOSC_HETMANOW * ILOSC_HETMANOW; i++)
		{
			plansza[i] = 0;
		}
		for (int i = 0; i < ILOSC_HETMANOW; i++)
		{
			hetmani[i] = o.hetmani[i];
			plansza[hetmani[i]] = 1;
		}
	}

	friend std::ostream& operator<< (std::ostream &out, const Osobnik &o);

	bool ustawHetmana(int hetman, int pole)
	{
		if (plansza[pole])
			return false;
		//czyszczenie starego miejsca na plaszy
		plansza[hetmani[hetman]] = 0;
		hetmani[hetman] = pole;
		//ustawianie hetmana na nowym polu
		plansza[pole] = 1;
		return true;

	}

	void mutacja()
	{
		Osobnik o = Osobnik(this);
		int fit = fitness();
		int proby = 0;
		do
		{
			proby++;
			//ustawianie hetmana na losowym pustym miejscu
			while (!o.ustawHetmana(rand() % ILOSC_HETMANOW, rand() % (ILOSC_HETMANOW * ILOSC_HETMANOW)));
			//jezeli znajdziemy lepszy w wyniku mutacji zamieniamy aktualny na lepszy i konczymy
			if (o.fitness() > fit || o.fitness() == 0)
			{
				for (int i = 0; i < ILOSC_HETMANOW * ILOSC_HETMANOW; i++)
				{
					plansza[i] = 0;
				}
				for (int i = 0; i < ILOSC_HETMANOW; i++)
				{
					hetmani[i] = o.hetmani[i];
					plansza[hetmani[i]] = 1;
				}
				return;
			}
			//inaczej kontynuujemy mutacje najlepszego czyli aktualnego
			else
				o = Osobnik(this);
			// jezeli x prob zawiedzie to losowo przestawiamy hetmana, 
			// aby zapobiec sytuacji ze 7 hetmanow blokuje wszystkie pola na 
			// planszy uniemozliwiajac 8memu wejscie na jakiekolwiek pole
		} while (proby < 100);
		while (!ustawHetmana(rand() % ILOSC_HETMANOW, rand() % (ILOSC_HETMANOW * ILOSC_HETMANOW)));
	}

	//fitness zwraca -1 * ilosc mozliwych atakow do wykonania przez hetmanow
	int fitness() {
		int bicia = 0;
		for (int i = 0; i < ILOSC_HETMANOW; i++)
		{
			int x = hetmani[i] % ILOSC_HETMANOW;
			int y = hetmani[i] / ILOSC_HETMANOW;
			//szukanie hetmanow w osi x (lacznie ze sprawdzanym)
			for (int j = 0; j < ILOSC_HETMANOW; j++)
			{
				if (plansza[y * ILOSC_HETMANOW + j])
					bicia--;
			}
			//odejmujemy 1 hetmana ktorego znalazla poprzednia petla, dla ktorego szukaliscmy bic
			bicia++;

			//szukanie hetmanow w osi y (lacznie ze sprawdzanym)
			for (int j = 0; j < ILOSC_HETMANOW; j++)
			{
				if (plansza[j * ILOSC_HETMANOW + x])
					bicia--;
			}
			//odejmujemy 1 hetmana ktorego znalazla poprzednia petla, dla ktorego szukaliscmy bic
			bicia++;
			//szukanie innych hetmanow w przekatnej gora lewo dol prawo
			for (int j = 1; j < ILOSC_HETMANOW; j++)
			{
				if (y + j < ILOSC_HETMANOW && x + j < ILOSC_HETMANOW)
				{
					if (plansza[(y + j) * ILOSC_HETMANOW + (x + j)])
					{
						bicia--;
					}
				}
				else break;
			}
			for (int j = 1; j < ILOSC_HETMANOW; j++)
			{
				if (y - j >= 0 && x - j >= 0)
				{
					if (plansza[(y - j) * ILOSC_HETMANOW + (x - j)])
					{
						bicia--;
					}
				}
				else
					break;
			}

			//szukanie innych hetmanow w przekatnej dol lewo gora prawo
			for (int j = 1; j < ILOSC_HETMANOW; j++)
			{
				if (y - j >= 0 && x + j < ILOSC_HETMANOW)
				{
					if (plansza[(y - j) * ILOSC_HETMANOW + (x + j)])
					{
						bicia--;
					}

				}
				else break;
			}
			for (int j = 1; j < ILOSC_HETMANOW; j++)
			{
				if (y + j < ILOSC_HETMANOW && x - j >= 0)
				{
					if (plansza[(y + j) * ILOSC_HETMANOW + (x - j)])
					{
						bicia--;
					}
				}
				else
					break;
			}
		}
		return bicia;
	}
};
std::ostream& operator<< (std::ostream &out, const Osobnik &o) {
	for (int i = 0; i < ILOSC_HETMANOW; i++)
	{
		for (int j = 0; j < ILOSC_HETMANOW; j++)
		{
			if (o.plansza[i * ILOSC_HETMANOW + j])
				out << "Q|";
			else
				out << ".|";
		}
		out << std::endl;
	}
	return out;
}

class GraHetmanow {
public:
	GraHetmanow() {}

	void zgaduj()
	{
		Osobnik o = Osobnik();
		do {
			o.mutacja();
			if (o.fitness() == 0)
				break;
			std::cout << "Najlepszy: " << o.fitness() << std::endl << o << std::endl;
		} while (true);
		std::cout << "Znaleziono haslo: " << std::endl << o << std::endl;
	}
};


int main()
{
	std::srand(std::time(NULL));
	GraHetmanow gh = GraHetmanow();
	gh.zgaduj();
    return 0;
}

