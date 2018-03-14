#include "parte.h"

// - METODI DI ISTANZA

void Parte::add_dipendenza(string s){
	first = new Nodo_dipendenza(s,first);
}

bool Parte::is_main() const{
	return main;
}
void Parte::set_main(bool flag){
	main = flag;
}
void Parte::compatta_doppioni_dipendenze(){
	for(Parte::It i = (*this).begin() ; i != (*this).end() ; i++){
		//SE MAI TROVERAI DEGLI ERRORI QUELLI STANNO QUI, O NELLA FUNZIONE CHIAMATA
		first = elimina_doppioni_dipendenze(first,(*this)[i]);
	}
}
Parte::Nodo_dipendenza * Parte::elimina_doppioni_dipendenze(Nodo_dipendenza * p,string s,bool trovato){ //dovrebbe essere della classe Nodo_dipendenza?
	if(! p) return NULL; //caso base
	if(p->info == s){
		if(!trovato){
			//è la prima della lista: la devo lasciare
			p->next = elimina_doppioni_dipendenze(p->next,s,true);
			return p;
		} else {
			//è un doppione che va eliminato
			Nodo_dipendenza *aux = p;
			p = elimina_doppioni_dipendenze(p->next,s,true);
			aux->next = 0; //devo fare cosi perchè ho ridefinito il costruttore
			delete aux;
			return p;
		}
	} else {
		p->next = elimina_doppioni_dipendenze(p->next,s);
		return p;
	}
}
string Parte::get_nome() const{
	return nome;
}

// - COSTRUTTORI E DISTRUTTORE

Parte::Parte(string nome_act) : nome(nome_act), first(NULL), main(false) {}

Parte::Parte(const Parte& c) : nome(c.nome), first(copia(c.first)) , main(c.main) {}

Parte::~Parte() {  
	if(first) delete first; //distruzione profonda provocata dal fatto è stato ridefinito anche il modo in cui si distrugge la first
}

// - OPERATORI

Parte& Parte::operator=(const Parte& c){
	if(this != &c){
		nome = c.nome;
		first = copia(c.first);
	}
	return *this;
}

/* - CLASSE Nodo_dipendenza - */

// - COSTRUTTORE A 0,1,2 argomenti
Parte::Nodo_dipendenza::Nodo_dipendenza(string nome_act, Nodo_dipendenza * next_act) : info(nome_act), next(next_act) {}

/*

// - COSTRUTTORE DI COPIA
Parte::Nodo_dipendenza::Nodo_dipendenza(const Nodo_dipendenza& n) : info(n.info) , next(copia(&n)) {} //temo sia sbagliato

*/

// - DISTRUTTORE, distrugge tutti gli elementi che seguono il nodo e poi il nodo stesso
Parte::Nodo_dipendenza::~Nodo_dipendenza(){
	if(next) delete next;
}

// - METODI DI Parte UTILI A Nodo_dipendenza

Parte::Nodo_dipendenza * Parte::copia(const Nodo_dipendenza * p){ //copia profonda della lista
	if(!p) return NULL;
	return new Nodo_dipendenza(p->info,copia(p->next));
}

/* - CLASSE It - */

// - METODI DI It

bool Parte::It::operator==(It i) const{
	return  index == i.index;
}
bool Parte::It::operator!=(It i) const{
	return index != i.index;
}
Parte::It& Parte::It::operator++(){ //prefisso
	if(index) index = index->next;
	return *this;
}
Parte::It Parte::It::operator++(int){ //postfisso
	It aux;
	aux.index = index;
	if(index) index = index->next;
	return aux;
}

// - METODI CHE USANO It

Parte::It Parte::begin() const{
	It aux;
	aux.index = first;
	return aux;
}
Parte::It Parte::end() const{
	It aux;
	aux.index = NULL;
	return aux;
}
string& Parte::operator[](It i) const{
	return (i.index)->info;
}

/* - FUNZIONI AMICHE - */

std::ostream& operator<<(std::ostream& o,const Parte& p){
	o<<"> Nome: "<<p.nome;
	o<<"\n> Main: "<<p.main;
	o<<"\n> Dipendenze: \n";
	for(Parte::It i = p.begin();i != p.end(); i++){
		o << "- "<<p[i]<<"\n";
	}
	return o;
}