#include "stdafx.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#define ILOSC_OSOBNIKOW 8	//potega 2
#define LOGI 0

class Osobnik {
public:
	std::string slownik = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int dlugosc;
	char *chromosom;
	Osobnik(int d):dlugosc(d) {
		chromosom = new char[dlugosc];
		for (int i = 0; i < dlugosc; i++)
		{
			chromosom[i] = slownik[rand() % (slownik.length())];
		}
	}
	//konstruktor kopiujacy aby uniknac przypisywania tego samego chromosomu
	Osobnik(const Osobnik& o):dlugosc(o.dlugosc)
	{
		chromosom = new char[dlugosc];
		for (int i = 0; i < dlugosc; i++)
		{
			chromosom[i] = o.chromosom[i];
		}
	}

	friend std::ostream& operator<< (std::ostream &out, const Osobnik &o);

	int fitness(std::string haslo) {
		int wartosc = 0;
		for (int i = 0; i < dlugosc; i++)
		{
			if (chromosom[i] == haslo[i])
				wartosc++;
		}
		return wartosc;
	}
};
std::ostream& operator<< (std::ostream &out, const Osobnik &o) {
	out << "|";
	for (int i = 0; i < o.dlugosc; i++)
	{
		out << o.chromosom[i] << "|";
	}
	return out;
}

class ZgadnijSlowo {
private:
	std::string haslo;
	std::vector<Osobnik> populacja;
public:
	ZgadnijSlowo(std::string h):haslo(h) {
		int dlugoscHasla = h.length();
		for (int i = 0; i < ILOSC_OSOBNIKOW; i++)
		{
			Osobnik o = Osobnik(dlugoscHasla);
			populacja.push_back(o);
			if(LOGI) std::cout << i << ". " << o << std::endl;
		}
	}
	void zgaduj()
	{
		do {
			//tworzenie nowej populacji i szukanie najlepszego osobnika -> sprawdzanie czy znaleziono haslo
			nowaPopulacja();
			Osobnik najlepszy = populacja[0];
			if (najlepszy.fitness(haslo) == haslo.length())
				break;
		} while(true);
		std::cout << "Znaleziono haslo: " << populacja[0] << std::endl;
	}
	void nowaPopulacja()
	{
		//sortowanie populacji po fitness()
		for (int i = 0; i < ILOSC_OSOBNIKOW; i++)
		{
			for (int j = i; j < ILOSC_OSOBNIKOW; j++)
			{
				if (populacja[j].fitness(haslo) > populacja[i].fitness(haslo))
				{
					Osobnik o = populacja[j];
					populacja[j] = populacja[i];
					populacja[i] = o;
				}
			}
		}
		std::cout << "Najlepszy: " << populacja[0] << std::endl;
		if (LOGI) for (int i = 0; i < populacja.size(); i++)
		{
			if (LOGI) std::cout << i << ". " << populacja[i] << std::endl;
		}
		//usuwanie po³owy najslabszych osobnikow
		populacja.erase(populacja.begin() + ILOSC_OSOBNIKOW/2, populacja.end());
		//dodajemy polowe brakujacych osobnikow powstalych w wyniku krzyzowania losowych par
		for (int i = 0; i < ILOSC_OSOBNIKOW/8; i++)
		{
			std::vector<Osobnik> wynik = krzyzowanie(populacja[rand()%populacja.size()], populacja[rand() % populacja.size()], rand() % haslo.length());
			populacja.insert(end(populacja), begin(wynik), end(wynik));
		}
		//kolejna polowa brakujacych osobników powstaje przez mutacje losowych osobnikow
		for (int i = 0; i < ILOSC_OSOBNIKOW/4; i++)
		{
			populacja.push_back(mutacja(populacja[rand() % populacja.size()]));
		}

	}

	Osobnik mutacja(Osobnik o) {
		if (LOGI) std::cout << "MUTACJA: " << std::endl;
		if (LOGI) std::cout << "PRZED:" << o << std::endl;
		int fit = o.fitness(haslo);
		do
		{
			Osobnik nowy(o);
			nowy.chromosom[rand() % o.dlugosc] = o.slownik[rand() % (o.slownik.length())];
			if (nowy.fitness(haslo) > o.fitness(haslo) || nowy.fitness(haslo) == haslo.length())
			{
				o = nowy;
				break;
			}
		} while (true);
		if (LOGI) std::cout << "PO:   " << o << std::endl;
		return o;
	}

	std::vector<Osobnik> krzyzowanie(Osobnik o1, Osobnik o2, int punktPodzialu) {
		if (LOGI) std::cout << "KRZYZOWANIE: " << punktPodzialu << std::endl;
		if (LOGI) std::cout << "PRZED:" << o1 << "       " << o2 << std::endl;
		for (int i = punktPodzialu; i < o1.dlugosc; i++)
		{
			int tmp = o1.chromosom[i];
			o1.chromosom[i] = o2.chromosom[i];
			o2.chromosom[i] = tmp;
		}
		if (LOGI) std::cout << "PO:   " << o1 << "       " << o2 << std::endl;
		std::vector<Osobnik> wynik;
		wynik.push_back(o1);
		wynik.push_back(o2);
		return wynik;
	}
};


int main()
{
	std::srand(std::time(NULL));
	ZgadnijSlowo zs = ZgadnijSlowo("AdrianAdamski");
	zs.zgaduj();
    return 0;
}

