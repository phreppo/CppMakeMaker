#include "makemaker.h"

/* - METODI DI ISTANZA - */

void Makemaker::make_magic(){
	set_parti();
	if(parti){
		set_dipendenze();
		compatta_dipendenze();
		scrivi_makefile();
	}
}

void Makemaker::set_dipendenze(){
	for(Makemaker::I i = begin(); i != end() ; i++){
		//giro tutte le parti, prima nel file .cpp e poi nel .h
		string da_aprire((*this)[i].get_nome() + ".cpp");
		std::ifstream curf(da_aprire.c_str());

		//giro il file.cpp
		if(curf){
			while(curf){
				string tmp;
				getline(curf,tmp);
				//ho la linea corrente del file cpp
				if(tmp.substr(0,10) == "#include \""){
					//ho trovato un'inclusione da fare
					int posizione_del_h = tmp.find(".h");
					string dipendenza = tmp.substr(10,posizione_del_h-10);
					//files[i] += (dipendenza+" ");
					(*this)[i].add_dipendenza(dipendenza);
				}
				if(tmp.substr(0,10) == "int main()"){
					//ho trovato il main e lo marco
					//files[i] += (" "+marcatore_main);
					(*this)[i].set_main(true);
					nome_progetto = (*this)[i].get_nome();
				}
			}
		curf.close();
    	}

    	if(!(*this)[i].is_main()){
    		//se la parte del programma he sto scorrendo non è un main avrà un file.h da includere, dunque quel file potrebbe avere altri include al suo interno (e li avrà) -> dobbiamo andare a scorrere anche il file.h
			//giro il file.h
			da_aprire = (*this)[i].get_nome() + ".h";
			curf.open(da_aprire.c_str());
			if(curf){
				while(curf){
					string tmp;
					getline(curf,tmp);
					//ho la linea corrente del file cpp
					if(tmp.substr(0,10) == "#include \""){
						//ho trovato un'inclusione da fare
						int posizione_del_h = tmp.find(".h");
						string dipendenza = tmp.substr(10,posizione_del_h-10);
						//files[i] += (dipendenza+" ");
						(*this)[i].add_dipendenza(dipendenza);
					}
				}
			curf.close();
	    	}
    	}
	}
}

void Makemaker::set_parti(){
	//imposta le parti dell oggetto di invocazione
	DIR *pDIR;
    struct dirent *entry;
    if( pDIR=opendir(".") ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
            	//QUESTO CICLO GIRA TUTTI I FILES NELLA CARTELLA CORRENTE
            	string nome = string(entry->d_name);
            		if(nome.size() >= string(".cpp").size() 
            			&& nome.substr( nome.size() - 4, 4   ) == ".cpp")
            			//se passa questo test il file prima di tutto può essere .cpp avendo almeno 4 caratteri, poi è sicuramente .cpp dato che passa anche la seconda parte del test
            			add_parte(Parte(nome.substr(0,nome.size() - string(".cpp").size())));
            }
        }
        closedir(pDIR);
    }
}

void Makemaker::scrivi_makefile() const{
	std::ofstream makefile(string("Makefile").c_str());

	makefile << "CC=g++\n";
	makefile << "OPT=-fno-elide-constructors\n";
	makefile << "NAME=" << nome_progetto <<"\n\n";

	makefile << "MAIN_PROCEDURE_EXE : ";

	for(Makemaker::I i = (*this).begin(); i != (*this).end(); i++){
		makefile << (*this)[i].get_nome() << " ";
	} makefile<<"\n\t";

	makefile << "$(CC) $(OPT) -o $(NAME)_exe "; //comando di linking

	for(Makemaker::I i = (*this).begin(); i != (*this).end(); i++){
		makefile << (*this)[i].get_nome() << ".o ";
	} makefile<<"\n";

	makefile <<"\trm *.o\n";
	makefile <<"\tclear\n";
	makefile <<"\t./$(NAME)_exe\n\n";

	//CICLO PER SCRIVERE TARGET: DIPENDENZE E SOTTO COMPILAZIONE
	for(Makemaker::I i = (*this).begin(); i != (*this).end(); i++){
		makefile << (*this)[i].get_nome() << " : "<<(*this)[i].get_nome()<<".cpp ";
		Parte& current = (*this)[i];
		for(Parte::It j = current.begin(); j != current.end(); j++){
			makefile <<current[j]<<".h ";
		} makefile<<"\n\t";

		makefile << "$(CC) $(OPT) -c "<<current.get_nome()<<".cpp -o "<<current.get_nome()<<".o\n\n";
	}

	makefile.close();
}

void Makemaker::add_parte(const Parte& p){
	parti = new Nodo_parte(p,parti);
}

void Makemaker::compatta_dipendenze(){
	for(Makemaker::I i = (*this).begin(); i != (*this).end() ; i++){
		//per ogni parte compatto le dipendenze
		(*this)[i].compatta_doppioni_dipendenze();
	}
}

/* - COSTRUTTORI E DISTRUTTORI - */

Makemaker::Makemaker() : parti(0) {}
Makemaker::Makemaker(const Makemaker& c) : parti(copia(c.parti)) {} //potevo anche evitare di ridefinire questa costruzione di copia, nel programma per ora mi sembra difficile dover istanzire due oggetti makemaker, lo scrivo per correttezza
Makemaker::~Makemaker() {
	delete parti;
}

/* - CLASSE NODO_PARTE - */

Makemaker::Nodo_parte::Nodo_parte(const Parte& p,Nodo_parte * next_act) : info(p), next(next_act) {} //NB che uso il costruttore di copia ridefinito nella classe Parte

Makemaker::Nodo_parte::~Nodo_parte(){
	if(next) delete next;
}

/* - FUNZIONI DU UTILITA PER L USO DELLA CLASSE NODO_PARTE - */

Makemaker::Nodo_parte * Makemaker::copia(const Nodo_parte * p){ //copia profonda della lista
	if(!p) return NULL;
	return new Nodo_parte(p->info,copia(p->next));
}

/* - CLASSE I - */

// - METODI DI I

bool Makemaker::I::operator==(I i) const{
	return  index == i.index;
}
bool Makemaker::I::operator!=(I i) const{
	return index != i.index;
}
Makemaker::I& Makemaker::I::operator++(){ //prefisso
	if(index) index = index->next;
	return *this;
}
Makemaker::I Makemaker::I::operator++(int){ //postfisso
	I aux;
	aux.index = index;
	if(index) index = index->next;
	return aux;
}

// - METODI CHE USANO I

Makemaker::I Makemaker::begin() const{
	I aux;
	aux.index = parti;
	return aux;
}
Makemaker::I Makemaker::end() const{
	I aux;
	aux.index = NULL;
	return aux;
}
Parte& Makemaker::operator[](I i) const{
	return (i.index)->info;
}

/* - FUNZIONI AMICHE - */

std::ostream& operator<<(std::ostream& o,const Makemaker& p){
	o << "Stampa delle parti\n";
	for(Makemaker::I i = p.begin() ; i != p.end() ; i++){
		o<<"Parte\n"<<p[i];
	}
	return o;
}