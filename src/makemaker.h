#ifndef MAKEMAKER_H
#define MAKEMAKER_H
#include <iostream>
#include <string.h>
#include <fstream> //serve per leggere dentro i file .cpp e .h
#include <dirent.h> //serve per leggere la directory corrente
#include "parte.h"

using std::string;




class Makemaker{

	/*                    AMICI                   */
	friend std::ostream& operator<<(std::ostream& ,const Makemaker&);

private:

	/*                 PARTE PRIVATA              */
	class Nodo_parte{
	public:
		Parte info;
		Nodo_parte * next;
		Nodo_parte(const Parte& p,Nodo_parte * next_act=NULL);
		~Nodo_parte();
	};

	Nodo_parte * parti;
	string nome_progetto;

	//METODI DI ISTANZA PER LA SCRITTURA DEL MAKEFILE
	void set_parti();
	void set_dipendenze(); //imposta anche il nome_progetto con il nome del file che contiene il main
	void scrivi_makefile() const;
	void add_parte(const Parte&);
	void compatta_dipendenze();

	static Nodo_parte * copia(const Nodo_parte *);

public:

	/*                 PARTE PUBBLICA              */

	// - CLASSE ITERATORE
	class I{
		friend class Makemaker;
	private:
		Nodo_parte * index;
	public:
		bool operator==(I i) const;
		bool operator!=(I i) const;
		I& operator++(); //prefisso
		I operator++(int); //postfisso
	};

	// - METODI CHE USANO I
	I begin() const;
	I end() const;
	Parte& operator[](I) const;

	// - METODI DI ISTANZA
	void make_magic();

	// - COSTRUTTORI, DISTRUTTORE
	Makemaker();
	Makemaker(const Makemaker&);
	~Makemaker();
};

#endif