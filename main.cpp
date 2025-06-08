#include <bits/stdc++.h>
#include <sstream>

using namespace std;

class Element {
public:
    string type;
    string name;
    int node1;
    int node2;
    double value;
};

class Circuit {
public:
    vector<Element> elements;
    int nodeCount;

    Circuit(int n) : nodeCount(n) {}

    void addElement(const Element& e) {
        elements.push_back(e);
    }

    void displayCircuit() {
        if (elements.empty()) {
            cout << "No elements in the circuit." << endl;
            return;
        }
        cout << "Current Circuit Elements:" << endl;
        for (auto& e : elements) {
            cout << "Type: " << e.type << ", Name: " << e.name
                 << ", Node1: " << e.node1 << ", Node2: " << e.node2
                 << ", Value: " << e.value << endl;
        }
    }

    void removeElement(const string& name) {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (it->name == name) {
                elements.erase(it);
                cout << "Element " << name << " removed." << endl;
                return;
            }
        }
        cout << "Element with name " << name << " not found." << endl;
    }

    void modifyElement(const string& name) {
        for (auto& e : elements) {
            if (e.name == name) {
                cout << "Modify element " << name << endl;
                cout << "Enter new type (Resistor, Capacitor, Inductor): ";
                cin >> e.type;
                cout << "Enter new node1: ";
                cin >> e.node1;
                cout << "Enter new node2: ";
                cin >> e.node2;
                cout << "Enter new value: ";
                cin >> e.value;
                cout << "Element " << name << " modified." << endl;
                return;
            }
        }
        cout << "Element with name " << name << " not found." << endl;
    }
};

class MatrixSolver {
public:
    vector<vector<double>> A;
    vector<double> b;
    vector<double> x;
    int size;

    MatrixSolver(int n) : size(n) {
        A = vector<vector<double>>(n, vector<double>(n, 0));
        b = vector<double>(n, 0);
        x = vector<double>(n, 0);
    }

    void solveGaussian() {
        for (int i = 0; i < size; ++i) {
            int maxRow = i;
            for (int k = i + 1; k < size; ++k)
                if (abs(A[k][i]) > abs(A[maxRow][i]))
                    maxRow = k;
            swap(A[i], A[maxRow]);
            swap(b[i], b[maxRow]);

            for (int k = i + 1; k < size; ++k) {
                double factor = A[k][i] / A[i][i];
                for (int j = i; j < size; ++j)
                    A[k][j] -= factor * A[i][j];
                b[k] -= factor * b[i];
            }
        }

        for (int i = size - 1; i >= 0; --i) {
            x[i] = b[i];
            for (int j = i + 1; j < size; ++j)
                x[i] -= A[i][j] * x[j];
            x[i] /= A[i][i];
        }
    }
};

void displayMenu() {
    cout << "\n--- Circuit Analysis Menu ---" << endl;
    cout << "1. Add Element to Circuit" << endl;
    cout << "2. Modify Element" << endl;
    cout << "3. Remove Element" << endl;
    cout << "4. Display Circuit" << endl;
    cout << "5. Solve Circuit" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int nodeCount;
    cout << "Enter the number of nodes in the circuit: ";
    cin >> nodeCount;

    Circuit c(nodeCount);
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        if (choice == 1) {
            string type, name;
            int node1, node2;
            double value;
            cout << "Enter element type (Resistor, Capacitor, Inductor): ";
            cin >> type;
            cout << "Enter element name: ";
            cin >> name;
            cout << "Enter node1 and node2 for element: ";
            cin >> node1 >> node2;
            cout << "Enter value for the element: ";
            cin >> value;

            Element e = {type, name, node1, node2, value};
            c.addElement(e);
        }
        else if (choice == 2) {
            string name;
            cout << "Enter the name of the element to modify: ";
            cin >> name;
            c.modifyElement(name);
        }
        else if (choice == 3) {
            string name;
            cout << "Enter the name of the element to remove: ";
            cin >> name;
            c.removeElement(name);
        }
        else if (choice == 4) {
            c.displayCircuit();
        }
        else if (choice == 5) {
            if (c.elements.empty()) {
                cout << "No elements to solve." << endl;
                continue;
            }
            double h = 1e-6;
            double v_prev = 0;
            double V1 = 5;

            double A_val = 1.0 / 1000 + 1.0e-6 / h;
            double b_val = V1 / 1000 + 1.0e-6 / h * v_prev;

            MatrixSolver solver(1);
            solver.A[0][0] = A_val;
            solver.b[0] = b_val;
            solver.solveGaussian();

            cout << "Voltage at node 1 at next timestep: " << solver.x[0] << endl;
        }
        else if (choice == 6) {
            cout << "Exiting..." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }



    
    return 0;
}
