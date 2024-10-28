#include <iostream>
#include <vector>
using namespace std;

class Term {
private:
    int Coefficient;
    int Exponent;
    Term* Next;

public:
    Term(int c, int e) {
        this->Coefficient = c;
        this->Exponent = e;
        this->Next = nullptr;
    }

    int getCoefficient() {
        return this->Coefficient;
    }

    int getExponent() {
        return this->Exponent;
    }

    void setCoefficient(int c) {
        this->Coefficient = c;
    }

    Term* getNext() {
        return this->Next;
    }

    void setNext(Term* Next) {
        this->Next = Next;
    }
};

class Polynomial {
private:
    int min;
    int max;
    Term* headptr;
    Term* Lastptr;

public:
    Polynomial() {
        headptr = Lastptr = nullptr;
        min = 0;
        max = 0;
    }

    void InsertAtFront(int c, int e) {
        Term* newTerm = new Term(c, e);
        newTerm->setNext(headptr);
        headptr = newTerm;
        if (!Lastptr) Lastptr = newTerm; // Initialize Lastptr if this is the first term
        max = (max == 0 || e > max) ? e : max;
        min = (min == 0 || e < min) ? e : min;
    }

    void InsertAtLast(int c, int e) {
        Term* newTerm = new Term(c, e);
        if (Lastptr) {
            Lastptr->setNext(newTerm);
            Lastptr = newTerm;
        } else {
            headptr = Lastptr = newTerm;
        }
        min = (min == 0 || e < min) ? e : min;
    }

    void Insert(int c, int e) {
        if (!headptr || e > headptr->getExponent()) {
            InsertAtFront(c, e);
            return;
        }

        Term* currentPtr = headptr;
        Term* prevPtr = nullptr;
        
        while (currentPtr != nullptr && currentPtr->getExponent() > e) {
            prevPtr = currentPtr;
            currentPtr = currentPtr->getNext();
        }

        if (currentPtr != nullptr && currentPtr->getExponent() == e) {
            currentPtr->setCoefficient(currentPtr->getCoefficient() + c);
            if (currentPtr->getCoefficient() == 0) { // If coefficient becomes 0, remove the term
                if (prevPtr) prevPtr->setNext(currentPtr->getNext());
                else headptr = currentPtr->getNext();
                delete currentPtr;
            }
        } else {
            Term* newTerm = new Term(c, e);
            newTerm->setNext(currentPtr);
            if (prevPtr) prevPtr->setNext(newTerm);
            else headptr = newTerm;
            if (!currentPtr) Lastptr = newTerm;
        }
    }

    void DisplayPolynomial() {
        if (isEmpty()) {
            cout << "0" << endl;
            return;
        }

        Term* currentPtr = headptr;
        bool isFirstTerm = true;

        while (currentPtr != nullptr) {
            int coeff = currentPtr->getCoefficient();
            int exp = currentPtr->getExponent();

            if (!isFirstTerm && coeff > 0) cout << " + ";
            if (coeff < 0) {
                cout << " - ";
                coeff = -coeff;
            }

            if (coeff != 1 || exp == 0) cout << coeff;
            if (exp > 0) {
                cout << "x";
                if (exp > 1) cout << "^" << exp;
            }

            currentPtr = currentPtr->getNext();
            isFirstTerm = false;
        }
        cout << endl;
    }

    double Evaluate() {
    double x;
    cout << "\nEnter the Value of x: ";
    cin >> x;
    double result = 0;
    Term* current = headptr;
    
    while (current != nullptr) {
        result += current->getCoefficient() * pow(x, current->getExponent());
        current = current->getNext();
    }
    
    return result;
}

    bool isEmpty() {
        return headptr == nullptr;
    }

    int getMin() {
        return this->min;
    }

    int getMax() {
        return this->max;
    }

    Term* getHeadPtr() {
        return headptr;
    }

    friend Polynomial operator+(Polynomial& A, Polynomial& B);
    friend Polynomial operator-(Polynomial& A, Polynomial& B);
    friend Polynomial operator*(Polynomial& A, Polynomial& B);

};

Polynomial operator+(Polynomial& A, Polynomial& B) {
    Polynomial combinedPolynomial;

    Term* currentA = A.getHeadPtr();
    while (currentA != nullptr) {
        combinedPolynomial.Insert(currentA->getCoefficient(), currentA->getExponent());
        currentA = currentA->getNext();
    }

    Term* currentB = B.getHeadPtr();
    while (currentB != nullptr) {
        combinedPolynomial.Insert(currentB->getCoefficient(), currentB->getExponent());
        currentB = currentB->getNext();
    }

    return combinedPolynomial;
}

Polynomial operator-(Polynomial& A, Polynomial& B) {
    Polynomial invertedB;

    Term* currentB = B.getHeadPtr();
    while (currentB != nullptr) {
        invertedB.Insert(currentB->getCoefficient() * -1, currentB->getExponent());
        currentB = currentB->getNext();
    }

    Polynomial combinedPolynomial = A + invertedB;

    return combinedPolynomial;
}

Polynomial operator*(Polynomial& A, Polynomial& B){
    Polynomial productPolynomial;

    Term* currentA = A.getHeadPtr();
    while (currentA != nullptr) {
        Term* currentB = B.getHeadPtr();
        while (currentB != nullptr) {
            int newCoefficient = currentA->getCoefficient() * currentB->getCoefficient();
            int newExponent = currentA->getExponent() + currentB->getExponent();
            
            productPolynomial.Insert(newCoefficient, newExponent);

            currentB = currentB->getNext();
        }
        currentA = currentA->getNext();
    }

    return productPolynomial;
}

void MainMenu() {
    cout << "\n------- Main Menu -------\n";
    cout << "1.Insert Polynomial\n";
    cout << "2.Add Two Polynomials\n";
    cout << "3.Subtract Two Polynomials\n";
    cout << "4.Multiply Two Polynomials\n";
    cout << "5.Evaluate a Polynomial at given x\n";
    cout << "6.Display all Polynomials\n";
    cout << "7.Exit\n";
    cout << "\nEnter your Choice: ";
}

int PolynomialCounter = 0;

int main() {
    cout << endl << endl;
    vector<Polynomial> Polynomials;
    bool isRunning = true;

    while (isRunning) {
        MainMenu();
        int choice;
        cin >> choice;

        if (choice == 7) {
            cout << "\nGoodbye!\n";
            isRunning = false;
        } else if (choice == 1) {
            Polynomial newPolynomial;
            cout << "\nHow many terms will the Polynomial have: ";
            int terms;
            cin >> terms;

            for (int i = 0; i < terms; i++) {
                int c, e;
                cout << "\nEnter the Coefficient for term " << i + 1 << ": ";
                cin >> c;
                cout << "\nEnter the Exponent for term " << i + 1 << ": ";
                cin >> e;

                if (e > newPolynomial.getMax()) {
                    newPolynomial.InsertAtFront(c, e);
                } else if (e < newPolynomial.getMin()) {
                    newPolynomial.InsertAtLast(c, e);
                } else {
                    newPolynomial.Insert(c, e);
                }
            }

            Polynomials.push_back(newPolynomial);
            PolynomialCounter++;
        } else if (choice == 6) {
            cout << "\n------ Polynomials -----\n\n";
            for (int i = 0; i < PolynomialCounter; i++) {
                cout << i + 1 << ". ";
                Polynomials[i].DisplayPolynomial();
            }
        } else if (choice == 2) {
            cout << "\n------ Polynomials -----\n\n";
            for (int i = 0; i < PolynomialCounter; i++) {
                cout << i + 1 << ". ";
                Polynomials[i].DisplayPolynomial();
            }
            int x, y;
            cout << "\nSelect any two Polynomials: ";
            cin >> x >> y;
            Polynomial Sum = Polynomials[x - 1] + Polynomials[y - 1];
            cout << endl << "Sum: ";
            Sum.DisplayPolynomial();
        } else if (choice == 3) {
            cout << "\n------ Polynomials -----\n\n";
            for (int i = 0; i < PolynomialCounter; i++) {
                cout << i + 1 << ". ";
                Polynomials[i].DisplayPolynomial();
            }
            int x, y;
            cout << "\nSelect any two Polynomials: ";
            cin >> x >> y;
            Polynomial Diff = Polynomials[x - 1] - Polynomials[y - 1];
            cout << endl << "Difference: ";
            Diff.DisplayPolynomial();
        } else if (choice == 4) {
            cout << "\n------ Polynomials -----\n\n";
            for (int i = 0; i < PolynomialCounter; i++) {
                cout << i + 1 << ". ";
                Polynomials[i].DisplayPolynomial();
            }
            int x, y;
            cout << "\nSelect any two Polynomials: ";
            cin >> x >> y;
            Polynomial Prod = Polynomials[x - 1] * Polynomials[y - 1];
            cout << endl << "Product: ";
            Prod.DisplayPolynomial();
        } else if(choice == 5){
            cout << "\n------ Polynomials -----\n\n";
            for (int i = 0; i < PolynomialCounter; i++) {
                cout << i + 1 << ". ";
                Polynomials[i].DisplayPolynomial();
            }
            int x;
            cout << "\nSelect a Polynomial: ";
            cin >> x;
            double result = Polynomials[x - 1].Evaluate();
            cout << "\nThe Selected Polynomial at given x is: " << result << endl; 
        } else{
            cout << "\nInvalid Input!\n";
            cout << "\nTry Again!\n";
        }
    }
    cout << endl << endl;
    return 0;
}
