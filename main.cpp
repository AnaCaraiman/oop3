#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

class IDGen {
private:
    std::unordered_map<const void*, int> ids;
    std::unordered_map<int, const void*> objects;
    int nextid;

public:
    IDGen() : nextid(1) {}

    template <typename T>
    int generateID(const T* obj) {
        const void* address = static_cast<const void*>(obj);
        auto it = ids.find(address);
        if (it != ids.end()) {
            return it->second; // ID already exists for the object
        }

        int id = nextid++;
        ids.insert({address, id});
        objects.insert({id, address});
        return id;
    }

    template <typename T>
    const T* getObjectByID(int id) const {
        auto it = objects.find(id);
        if (it != objects.end()) {
            const void* address = it->second;
            return static_cast<const T*>(address);
        }
        return nullptr;
    }
};

class Floare {
protected:
    int id;
    static IDGen idGen;
    std::string nume;
    std::string culoare;

public:
    Floare(const std::string& nume, const std::string& culoare)
            : id(idGen.generateID(this)), nume(nume), culoare(culoare) {}

    virtual ~Floare() {}

    int getID() const {
        return id;
    }

    virtual std::unique_ptr<Floare> clone() const = 0;

    void setNume(const std::string& nume) {
        this->nume = nume;
    }

    void setCuloare(const std::string& culoare) {
        this->culoare = culoare;
    }
    static const IDGen& getIDGen() {
        return idGen;
    }

    virtual void adaugaDepozit(const std::string& numeDepozit, const std::string& culoareDepozit, int stocDepozit) = 0;

    friend std::ostream& operator<<(std::ostream& os, const Floare& floare) {
        os << "ID: " << floare.id << ", Nume: " << floare.nume << ", Culoare: " << floare.culoare;
        return os;
    }
};

IDGen Floare::idGen; // Inițializăm IDGen-ul partajat între obiectele Floare

class Depozit : public Floare {
private:
    int stoc;

public:
    Depozit() : Floare("", ""), stoc(0) {}

    Depozit(const std::string& nume, const std::string& culoare, int stoc)
            : Floare(nume, culoare), stoc(stoc) {}

    void setStoc(int stoc) {
        this->stoc = stoc;
    }

    std::unique_ptr<Floare> clone() const override {
        return std::make_unique<Depozit>(*this);
    }

    void adaugaStoc(int cantitate) {
        stoc += cantitate;
    }

    void adaugaDepozit(const std::string& numeDepozit, const std::string& culoareDepozit, int stocDepozit) override {
        std::unique_ptr<Floare> depozit = std::make_unique<Depozit>(numeDepozit, culoareDepozit, stocDepozit);
        static_cast<Depozit*>(depozit.get())->id = id;
    }

    int getStoc() const {
        return stoc;
    }

    friend std::ostream& operator<<(std::ostream& os, const Depozit& depozit) {
        os << "ID: " << depozit.id << ", Nume: " << depozit.nume << ", Culoare: " << depozit.culoare << ", Stoc: " << depozit.stoc;
        return os;
    }
};
template<typename T>
class MyVector {
    T *arr;
    int size;

public:
    MyVector(int size) {
        arr = new T[size];
        this->size = size;
    }

    MyVector(int size, const T &defaultValue) {
        arr = new T[size];
        this->size = size;
        for (int i = 0; i < size; ++i) {
            arr[i] = defaultValue;
        }
    }

    MyVector(const MyVector<T> &rhs) {
        arr = new T[rhs.size];
        size = rhs.size;
        std::copy(rhs.arr, rhs.arr + rhs.size, arr);
    }

    MyVector<T> &operator=(const MyVector<T> &rhs) {
        if (rhs.arr != arr) {
            delete[] arr;
            arr = new T[rhs.size];
            size = rhs.size;
            std::copy(rhs.arr, rhs.arr + rhs.size, arr);
        }

        return *this;
    }

    T &operator[](int pos) {
        return arr[pos];
    }

    friend std::ostream &operator<<(std::ostream &os, const MyVector<T> &vector) {
        for (int i = 0; i < vector.size; ++i) {
            os << vector.arr[i] << std::endl;
        }
        return os;
    }

    ~MyVector() {
        delete[] arr;
    }
};
class StocInsuficientException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Stocul este insuficient!";
    }
};

class IdNegasitException : public std::exception {
public:
    const char* what() const noexcept override {
        return "ID-ul nu a fost gasit!";
    }
};
template<typename T>
void citesteElement(MyVector<T>& vector, int pozitie) {
    std::string nume, culoare;
    int stoc;

    std::cout << "Numele Florii " << pozitie + 1 << ": ";
    std::cin >> nume;
    std::cout << "Culoarea florii " << pozitie + 1 << ": ";
    std::cin >> culoare;
    std::cout << "Nr flor " << pozitie + 1 << ": ";
    std::cin >> stoc;

    Depozit depozit(nume, culoare, stoc);
    vector[pozitie] = depozit;
}

int main() {
    int nrmaximcomenzi,poz=0,nrfloriindepozi=3;
    nrmaximcomenzi=100;
    MyVector<Depozit> vector(nrmaximcomenzi);

    Depozit *nr = new Depozit[1];
    nr[0].adaugaDepozit("Trandafir","Rosu",200);
    nr[1].adaugaDepozit("Lalele","Roz",300);
    nr[2].adaugaDepozit("Crini","Albi",100);
    try{
        start:

        std::cout << "1 - Plaseaza comanda" << "\n";
        std::cout << "0 - Iesire Meniu" << "\n";
        int nn, l;
        std:: cin >> nn;
        switch (nn) {
            case 1: {
                int i = 1;
                citesteElement(vector, poz);
                int sw=0;
                for(int i=0;i<=nrfloriindepozi;i++){
                    if(nr[i].getID()=vector[poz].getID())
                    { sw=1;
                    if(nr[i].getStoc()<vector[poz].getStoc())
                        catch (const StocInsuficientException& e)
                        {std::cout  << e.what() << std::endl;
                        }
                    else std::cout<<"Comanda plasata";
                    }
                }
                std::cout << "Inapoi la meniu?(1 pt DA sau 0 pt NU)" << "\n";
                std::cin >> nn;
                if (nn == 1)
                    goto start;
                else
                    break;
            }
        }

    }catch (const IdNegasitException& e)
    {std::cout  << e.what() << std::endl;
    }




    return 0;
}