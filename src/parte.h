#ifndef PARTE_H
#define PARTE_H
#include <iostream>
using std::string;

class Parte{

	/*                    AMICI                   */
	friend std::ostream& operator<<(std::ostream& ,const Parte&);

private:

	/*                 PARTE PRIVATA              */

	string nome;
	bool main;
	class Nodo_dipendenza{ //definisco una classe interna per la gestione delle dipendenze
	public:
		string info;
		Nodo_dipendenza * next;
		Nodo_dipendenza(string nome_act="",Nodo_dipendenza * next_act=NULL);
		~Nodo_dipendenza();
		//Nodo_dipendenza(const Nodo_dipendenza&);
	};
	Nodo_dipendenza * first; //inizio delle dipendenze
	static Nodo_dipendenza * elimina_doppioni_dipendenze(Nodo_dipendenza *, string,bool trovato = false);
	//metodi di utilità per la classe Nodo_dipendenza
	static Nodo_dipendenza * copia(const Nodo_dipendenza *);

public:

	/*                 PARTE PUBBLICA              */

	// - ITERATORE
	class It{
		friend class Parte;
	private:
		Nodo_dipendenza * index;
	public:
		bool operator==(It i) const;
		bool operator!=(It i) const;
		It& operator++(); //prefisso
		It operator++(int); //postfisso
	};

	// - METODI DI ISTANZA
	void add_dipendenza(string s);
	bool is_main() const;
	void set_main(bool);
	void compatta_doppioni_dipendenze(); //se ci sono due dipendenze duplicate(poiche incluso lo stesso file.h sia nel .cpp che nel.h) ne toglie una
	string get_nome() const;


	// - METODI CHE USANO It
	It begin() const;
	It end() const;
	string& operator[](It) const;

	// - COSTRUTTORI & DISTRUTTORE
	Parte(string nome_act="");
	Parte(const Parte&); //costruttore di copia
	~Parte();

	//- OPERATORI
	Parte& operator=(const Parte&);
};

#endif