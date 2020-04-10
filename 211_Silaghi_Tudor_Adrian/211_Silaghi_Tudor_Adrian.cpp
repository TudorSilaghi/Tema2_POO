#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>

#define PRET_VIN_VARSAT_PER_100_ML 9
#define PRET_VIN_LA_STICLA 47
#define PRET_BERE 11
#define PRET_APA 7
#define PRET_CIORBA 16
#define PRET_FRIPTURA_PUI 20
#define PRET_FRIPTURA_VITA 32
#define PRET_GARNITURA 10.5
#define PRET_CUPA_INGHETATA 4
#define PRET_TORT_KRANTZ 14.5

using namespace std;

enum intrebari_comanda
{
	START,
	BAUTURA,
	CIORBA,
	FEL_PRINCIPAL,
	DESERT,
	MULTUMESC
};

class Dish
{
protected:
	char *denumire;
	float pret;
	int index;

public:
    Dish()
    {
        this->denumire = new char[20];
        strcpy(this->denumire, "");
        this->pret = 0;
        this->index = START;
    }

    Dish(Dish &dish_obj)        /*constructor copiere*/
    {
        strcpy(this->denumire, dish_obj.denumire);
        this->pret = dish_obj.pret;
        this->index = dish_obj.index;
    }

    virtual void citire(istream &in);
    virtual void afisare(ostream &out);

    friend istream& operator>>(istream &in, Dish& dish_obj);
    friend ostream& operator<<(ostream &out, Dish& dish_obj);
    Dish& operator=(Dish& dish_obj);

    friend class Client;
    friend class Masa;

    virtual void ask_question();

	virtual ~Dish();            /*destructor*/
};

void Dish::ask_question()
{
    if(index == START)
    {
        cout << "\n-Doriti de baut?\n-";
        index = BAUTURA;
    }else if (index == BAUTURA)
    {
        cout << "-Doriti ciorba?\n-";
        index = CIORBA;
    }else if (index == CIORBA)
    {
        cout << "-Doriti fel principal?\n-";
        index = FEL_PRINCIPAL;
    }else if (index == FEL_PRINCIPAL)
    {
        cout << "-Doriti desert?\n-";
        index = DESERT;
    }else if (index == DESERT)
    {
        cout << "-Multumesc!" << endl;
        index = MULTUMESC;
    }else if (index == MULTUMESC)
        index = START;
    else cout << "Eroare.";
}

Dish& Dish::operator=(Dish& dish_obj)
{
    strcpy(denumire, dish_obj.denumire);
    pret = dish_obj.pret;
    index = dish_obj.index;
}

Dish::~Dish()
{
    delete []denumire;
    this->pret = 0;
    this->index = START;
}

void Dish::citire(istream &in)
{
    cout << "\nTip: ";
    in >> this->index;
    cout << "Denumire: ";
    in >> this->denumire;
    cout << "Pret (in lei): ";
    in >> this->pret;
}

istream& operator>>(istream& in, Dish& dish_obj)
{
    dish_obj.citire(in);
    return in;
}

void Dish::afisare(ostream &out)
{
    out << denumire;
    out << " (";
    out << index;
    out << ") - care costa ";
    out << pret;
    out << " lei.\n";
}

ostream& operator<<(ostream& out, Dish& dish_obj)
{
    dish_obj.afisare(out);
    return out;
}

class Bauturi:public Dish
{
public:
    Bauturi() {}
    void ask_question()
	{
		cout << "-Vin, bere, apa?\n-";
	}
	~Bauturi() {}
};

class Ciorbe:public Dish
{
public:
    Ciorbe() {}
    void ask_question()
    {
        cout << "-De burta, de legume sau de ciuperci?\n-De ";
    }
    ~Ciorbe() {}
};

class Feluri_principale:public Dish
{
public:
    Feluri_principale() {}
    void ask_question()
    {
        cout << "-Ce sa fie? Friptura de pui sau de vita?\n-De ";
    }
    ~Feluri_principale() {}
};

class Deserturi:public Dish
{
public:
    Deserturi() {}
    void ask_question()
    {
        cout << "-Inghetata sau tort Krantz?\n-";
    }
    ~Deserturi() {};
};

class Client
{
protected:
    int id_client;
	float suma_total_plata;

public:
    Client()
    {
        this->suma_total_plata = 0;
    }

    Client(Client &client_obj)
    {
        this->suma_total_plata = client_obj.suma_total_plata;
    }

    void citire(istream &in);
    void afisare(ostream &out);

    friend istream& operator>>(istream &in, Client& client_obj);
    friend ostream& operator<<(ostream &out, Client& client_obj);
    Client& operator=(Client& client_obj);

    friend class Masa;
    friend void menu();
    float take_order(Client);

    ~Client();
};

void Client::citire(istream &in)
{
    /*Nu am nevoie sa citesc clientii niciodata
    (deoarece le atribui automat cate un id
     in momentul in care intra in restaurant).
     Totusi, am facut si operatorul>>, pentru ca
     scria in cerinta*/
}

istream& operator>>(istream& in, Client& client_obj)
{
    client_obj.citire(in);
    return in;
}

void Client::afisare(ostream &out)
{
    out << "Clientul ";
    out << id_client;
    out << " trebuie sa plateasca ";
    out << suma_total_plata;
}

ostream& operator<<(ostream& out, Client& client_obj)
{
    client_obj.afisare(out);
    return out;
}

Client& Client::operator=(Client& client_obj)
{
    id_client = client_obj.id_client;
    suma_total_plata = client_obj.suma_total_plata;
}

float Client::take_order(Client client_obj)
{
    /**incepe lista de intrebari*/
    char *raspuns = (char*)malloc(51*sizeof(char));
    bool comanda_on_going = true;
    Dish *dish_obj = new Dish();

    while(comanda_on_going == true)
    {
        dish_obj->ask_question();
        if(dish_obj->index == MULTUMESC)
        {
            comanda_on_going = false;
        }
        else
        {
            cin >> raspuns;     /// [ Da \ Nu ]
        }
        if(strcmp(raspuns, "Da") == 0)
        {
            Dish *different_obj;
            if(dish_obj->index == BAUTURA)
            {
                different_obj = new Bauturi();
                different_obj->ask_question();
                cin >> raspuns;             /// [ Vin \ Bere \ Apa ]
                if(strcmp(raspuns, "Vin") == 0)
                {
                    cout << "-Varsat sau la sticla?\n-";
                    fflush(stdin);
                    fgets(raspuns, 50, stdin);         /// [ Varsat \ La sticla ]
                    if(strcmp(raspuns, "Varsat\n") == 0)
                    {
                        cout << "-Alb sau rosu?\n-";
                        cin >> raspuns;
                        cout << "-Cati ml?\n-";
                        int ml;
                        cin >> ml;
                        client_obj.suma_total_plata += ml*PRET_VIN_VARSAT_PER_100_ML/100;
                    }
                    else if(strcmp(raspuns, "La sticla\n") == 0)
                    {
                        cout << "-Va aduc Bordeux din 1996 sau Castel Bolovanu din 2004?\n-";
                        fflush(stdin);
                        fgets(raspuns, 50, stdin);
                        client_obj.suma_total_plata += PRET_VIN_LA_STICLA;
                    }
                }
                else if(strcmp(raspuns, "Bere") == 0)
                {
                    cout << "-Cu sau fara alcool?\n-";
                    cin >> raspuns;
                    client_obj.suma_total_plata += PRET_BERE;
                }
                else if(strcmp(raspuns, "Apa") == 0)
                {
                    cout << "-Plata sau minerala?\n-";
                    cin >> raspuns;
                    client_obj.suma_total_plata += PRET_APA;
                }
                delete different_obj;
            }
            else if(dish_obj->index == CIORBA)
            {
                different_obj = new Ciorbe();
                different_obj->ask_question();
                cin >> raspuns;         /// [ burta \ legume \ ciuperci ]
                if(strcmp(raspuns, "ciuperci") != 0)
                {
                    cout << "-Cu smantana?\n-";
                    cin >> raspuns;
                    cout << "-Ardei va aduc?\n-";
                    cin >> raspuns;
                }
                client_obj.suma_total_plata += PRET_CIORBA;
                delete different_obj;
            }
            else if(dish_obj->index == FEL_PRINCIPAL)
            {
                different_obj = new Feluri_principale();
                different_obj->ask_question();
                cin >> raspuns;         /// [ pui \ vita ]
                if(strcmp(raspuns, "pui") == 0)
                {
                    cout << "-Tocana sau frigarui?\n-";
                    cin >> raspuns;
                    client_obj.suma_total_plata += PRET_FRIPTURA_PUI;
                }
                else if(strcmp(raspuns, "vita") == 0)
                {
                    cout << "-Chateaubriand, stroganoff sau file?\n-";
                    cin >> raspuns;
                    cout << "-In sange, mediu sau bine facuta?\n-";
                    fflush(stdin);
                    fgets(raspuns, 50, stdin);
                    client_obj.suma_total_plata += PRET_FRIPTURA_VITA;
                }
                cout << "-Si garnitura ce sa fie? Cartofi prajiti, piure sau orez sarbesc?\n-";
                fflush(stdin);
                fgets(raspuns, 50, stdin);
                client_obj.suma_total_plata += PRET_GARNITURA;
                delete different_obj;
            }
            else if(dish_obj->index == DESERT)
            {
                different_obj = new Deserturi();
                different_obj->ask_question();
                fflush(stdin);
                fgets(raspuns, 50, stdin);         /// [ Inghetata \ Tort Krantz ]
                if(strcmp(raspuns, "Inghetata\n") == 0)
                {
                    cout << "-Cate cupe?\n-";
                    int cupe;
                    cin >> cupe;
                    client_obj.suma_total_plata += cupe*PRET_CUPA_INGHETATA;
                }
                else client_obj.suma_total_plata += PRET_TORT_KRANTZ;
                delete different_obj;
            }
        }
    }

    return client_obj.suma_total_plata;
}

Client::~Client()
{
    this->id_client = 0;
    this->suma_total_plata = 0;
}

class Masa
{
protected:
    int id_masa;
	int capacitate_max;
	int nr_clienti;
	bool free_table;
	Client *vector_clienti;
	float nota_totala;
	float nota_achitata;

public:
    Masa()
    {
        this->nr_clienti = 0;
        this->free_table = true;
        this->vector_clienti = (Client*)malloc(this->nr_clienti*sizeof(Client));
        this->nota_totala = 0;
        this->nota_achitata = 0;
    }

    Masa(Masa&);

    void citire(istream &in);
    void afisare(ostream &out);

    friend istream& operator>>(istream &in, Masa& masa_obj);
    friend ostream& operator<<(ostream &out, Masa& masa_obj);
    Masa& operator=(Masa& masa_obj);

    friend class Client;
    friend void menu();

	~Masa();
};

Masa::Masa(Masa& masa_obj)
{
    this->id_masa = masa_obj.id_masa;
    this->capacitate_max = masa_obj.capacitate_max;
    this->nr_clienti = masa_obj.nr_clienti;
    this->free_table = masa_obj.free_table;
    this->vector_clienti = masa_obj.vector_clienti;
    this->nota_totala = masa_obj.nota_totala;
    this->nota_achitata = masa_obj.nota_achitata;
}

void Masa::citire(istream &in)
{
    cout << "Masa " << this->id_masa << " are capacitatea maxima: ";
    in >> this->capacitate_max;
}

istream& operator>>(istream& in, Masa& masa_obj)
{
    masa_obj.citire(in);
    return in;
}

void Masa::afisare(ostream &out)
{
    if(free_table == true)
    {
        out << "\nMasa " << id_masa << " este libera!";
    }
    else
    {
        out << "\nLa masa " << id_masa;
        out << ", mai sunt " << nr_clienti << " clienti.\n";
        out << "Nota totala: " << nota_totala;
        out << "\tNota achitata: " << nota_achitata;
    }
}

ostream& operator<<(ostream& out, Masa& masa_obj)
{
    masa_obj.afisare(out);
    return out;
}

Masa& Masa::operator=(Masa& masa_obj)
{
    id_masa = masa_obj.id_masa;
	capacitate_max = masa_obj.capacitate_max;
	nr_clienti = masa_obj.nr_clienti;
	free_table = masa_obj.free_table;
	for(int i = 0; i < masa_obj.capacitate_max; i++)
    {
        vector_clienti[i] = masa_obj.vector_clienti[i];
    }
	nota_totala = masa_obj.nota_totala;
	nota_achitata = masa_obj.nota_achitata;
}

Masa::~Masa()
{
    this->id_masa = 0;
	this->capacitate_max = 0;
	this->nr_clienti = 0;
	this->free_table = false;
	delete []vector_clienti;
	this->nota_totala = 0;
    this->nota_achitata = 0;
}


//=======================================================


void menu_output()
{
    cout << "\n\nSilaghi Tudor-Adrian - 211 - Tema 15 - Restaurant: \n";
    cout << "\n\t MENIU:";
    cout << "\n===========================================\n\n";

    cout << "1. Condu clientii tocmai veniti la o masa." << endl;
    cout << "2. Ia comanda de la o masa." << endl;
    cout << "3. Elibereaza nota pentru un client." << endl;
    cout << "4. Elibereaza nota pentru toti clientii ramasi la masa." << endl;
    cout << "0. Iesire." << endl;
    /**Observatie: Meniul este facut tinand cont de cronologia venirii clientilor
            in restaurant. Astfel, prima oara trebuie apasat 1, apoi 2 si abia
            apoi se poate elibera nota (adica se poate apasa 2 sau 3) */
}

void menu()
{
    int option;                 /* optiunea aleasa din meniu */
    option = 0;

    int nr_mese;
    cout << "Citeste numarul de mese din restaurat: ";
    cin >> nr_mese;
    Masa mese[nr_mese];
    cout << "Citeste mesele:\n";

    for(int i = 0; i < nr_mese; i++)
    {
        mese[i].id_masa = i + 1;      /*mesele vor fi numerotate cu 1, 2, 3, etc...*/
        mese[i].nota_achitata = 0;
        mese[i].nota_totala = 0;
        mese[i].free_table = true;
        cin >> mese[i];
    }

    /**Am adaptat putin meniul (fata de cel dat pe dontpad.com), astfel: */
    menu_output();

    do
    {
        cout << "\nIntroduceti numarul actiunii: ";
        cin >> option;

        int clienti_nou_veniti, j, k;
        int id_masa;
        if (option == 1)
        {
            cout << "Clienti tocmai veniti: ";
            cin >> clienti_nou_veniti;

            for(j = 0; j < nr_mese; j++)
            {
                if(mese[j].free_table == true) break;
            }

            if(j >= nr_mese)
            {
                cout << "-Din pacate, nu mai exista mese libere.\n";
            }
            else
            {
                for(j = 0; j < nr_mese; j++)
                {
                    if(clienti_nou_veniti <= mese[j].capacitate_max && mese[j].free_table == true)
                    {
                        mese[j].free_table = false;
                        mese[j].nr_clienti = clienti_nou_veniti;
                        for(k = 0; k < clienti_nou_veniti; k++)
                        {
                            mese[j].vector_clienti[k].id_client = k + 1;
                        }
                        cout << "S-a ocupat masa " << j + 1 << endl;
                        break;
                    }
                }
                if(j >= nr_mese)
                {
                    cout << "-Ne pare rau, dar nu avem o masa suficient de mare pentru dvs.\n";
                }
            }
        }
        else if(option == 2)
        {
            cout << "Ia comanda de la masa cu id-ul: ";
            cin >> id_masa;

            if (mese[id_masa - 1].free_table == true)
            {
                cout << "Masa " << id_masa << " nu are clienti. Alege alta masa!";
            }
            else
            {
                cout << "\n-Buna ziua! ";
                for(k = 0; k < mese[id_masa - 1].nr_clienti; k++)
                {
                    mese[id_masa - 1].vector_clienti[k].suma_total_plata = mese[id_masa - 1].vector_clienti[k].take_order(mese[id_masa - 1].vector_clienti[k]);
                    mese[id_masa - 1].nota_totala += mese[id_masa - 1].vector_clienti[k].suma_total_plata;
                    cout << "Total plata client " << k + 1 << ": " << mese[id_masa - 1].vector_clienti[k].suma_total_plata << endl;
                }
                cout << "-Imediat va aduc comanda!" << endl;
            }
        }
        else if(option == 3)
        {
            cout << "Adu nota la masa ";
            cin >> id_masa;
            if(mese[id_masa - 1].free_table == true)
            {
                cout << "Ai inteles gresit. La masa " << id_masa << " nu este niciun client!" << endl;
            }
            else
            {
                cout << "Clientului cu id-ul: ";
                cin >> k;           /*aici, k il citim nenul (niciun client nu are id-ul = 0), deci ne vom referi la clientul reprezentat de vector_clienti[k - 1]*/
                cout << "Clientul cu id-ul " << k << " are de platit " << mese[id_masa - 1].vector_clienti[k - 1].suma_total_plata << " lei." << endl;
                mese[id_masa - 1].nota_achitata += mese[id_masa - 1].vector_clienti[k - 1].suma_total_plata;
                mese[id_masa - 1].nota_totala -= mese[id_masa - 1].vector_clienti[k - 1].suma_total_plata;
                mese[id_masa - 1].vector_clienti[k - 1].suma_total_plata = 0;
                mese[id_masa - 1].nr_clienti--;
                if(mese[id_masa - 1].nr_clienti == 0)
                    mese[id_masa - 1].free_table = true;
            }
        }
        else if(option == 4)
        {
            cout << "Elibereaza nota de la masa ";
            cin >> id_masa;
            if(mese[id_masa - 1].free_table == true)
            {
                cout << "Ai inteles gresit. La masa " << id_masa << " nu are clienti!" << endl;
            }
            else
            {
                cout << "\nLa masa " << id_masa << ", nota totala este de " << mese[id_masa - 1].nota_totala << " lei.\n";
                mese[id_masa - 1].free_table = true;
                mese[id_masa - 1].nr_clienti = 0;
                mese[id_masa - 1].nota_totala = 0;
                mese[id_masa - 1].nota_achitata = 0;
            }
        }
        else if(option == 0)
        {
            free(mese);
            cout << "\nEXIT!\n";
            system("pause"); ///Pauza - Press any key to continue...
            system("cls");   ///Sterge continutul curent al consolei
        }
        else cout << "\nSelectie invalida!";
    }while(option != 0);
}

int main()
{
    menu();

    return 0;
}
