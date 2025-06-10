#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <cstddef>
#include <set>
#include <map>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <limits>
#include <sstream>
#include <fstream>
#include "Eigen/Dense"

using namespace std;

enum class ComponentType {
    RESISTOR,
    CAPACITOR,
    INDUCTOR,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE,
    UNKNOWN
};

class Component {
protected:
    string name;
    int node1;
    int node2;

public:
    Component(const string& name, int n1, int n2) : name(name), node1(n1), node2(n2) {}
    virtual ~Component() = default;

    virtual void display() const = 0;
    virtual string getType() const = 0;
    virtual string serialize() const = 0;

    string getName() const { return name; }
    void setNodes(int newNode1, int newNode2) {
        this->node1 = newNode1;
        this->node2 = newNode2;
    }

    int getNode1() const { return node1; }
    int getNode2() const { return node2; }
};

class Resistor : public Component {
private:
    double resistance;
    string unit;

public:
    Resistor(const string& name, double res, int n1, int n2)
            : Component(name, n1, n2), resistance(res), unit("Ohm") {}

    void display() const override {
        cout << "  - Element: " << name << " | Type: " << getType()
             << " | Value: " << scientific << setprecision(4) << resistance << " " << unit
             << " | Nodes: (" << node1 << ", " << node2 << ")" << endl;
    }
    string getType() const override { return "Resistor"; }
    string serialize() const override {
        stringstream ss;
        ss << "Resistor " << name << " " << resistance << " " << node1 << " " << node2;
        return ss.str();
    }

    void setResistance(double res) { this->resistance = res; }
    double getResistance() const { return resistance; }
};

class Capacitor : public Component {
private:
    double capacitance;
    string unit;

public:
    Capacitor(const string& name, double cap, int n1, int n2)
            : Component(name, n1, n2), capacitance(cap), unit("F") {}

    void display() const override {
        cout << "  - Element: " << name << " | Type: " << getType() << " | Value: " << scientific << setprecision(4) << capacitance << " " << unit << " | Nodes: (" << node1 << ", " << node2 << ")" << endl;
    }
    string getType() const override { return "Capacitor"; }
    string serialize() const override {
        stringstream ss;
        ss << "Capacitor " << name << " " << capacitance << " " << node1 << " " << node2;
        return ss.str();
    }

    void setCapacitance(double cap) { this->capacitance = cap; }
    double getCapacitance() const { return capacitance; }
};

class Inductor : public Component {
private:
    double inductance;
    string unit;

public:
    Inductor(const string& name, double ind, int n1, int n2)
            : Component(name, n1, n2), inductance(ind), unit("H") {}

    void display() const override {
        cout << "  - Element: " << name << " | Type: " << getType() << " | Value: " << scientific << setprecision(4) << inductance << " " << unit << " | Nodes: (" << node1 << ", " << node2 << ")" << endl;
    }
    string getType() const override { return "Inductor"; }
    string serialize() const override {
        stringstream ss;
        ss << "Inductor " << name << " " << inductance << " " << node1 << " " << node2;
        return ss.str();
    }

    void setInductance(double ind) { this->inductance = ind; }
    double getInductance() const { return inductance; }
};

class VoltageSource : public Component {
public:
    enum class Waveform { DC, SINE };
private:
    Waveform waveformType;
    double offset_or_dc_value;
    double amplitude;
    double frequency;
    string unit;

public:
    VoltageSource(const string& name, double dc_val, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::DC), offset_or_dc_value(dc_val),
              amplitude(0), frequency(0), unit("V") {}

    VoltageSource(const string& name, double offset, double amp, double freq, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::SINE), offset_or_dc_value(offset),
              amplitude(amp), frequency(freq), unit("V") {}

    double getValueAtTime(double time) const {
        if (waveformType == Waveform::SINE) return offset_or_dc_value + amplitude * sin(2 * M_PI * frequency * time);
        return offset_or_dc_value;
    }

    void display() const override {
        cout << "  - Element: " << name << " | Type: " << getType();
        if (waveformType == Waveform::DC) {
            cout << " (DC) | Value: " << scientific << setprecision(4) << offset_or_dc_value << " " << unit;
        } else {
            cout << " (SINE) | Params: Offset=" << scientific << setprecision(4) << offset_or_dc_value << ", Amp=" << scientific << setprecision(4) << amplitude << ", Freq=" << scientific << setprecision(4) << frequency << "Hz";
        }
        cout << " | Nodes: (" << node1 << ", " << node2 << ")" << endl;
    }

    string getType() const override { return "Voltage Source"; }
    string serialize() const override {
        stringstream ss;
        ss << "VoltageSource " << name << " " << (waveformType == Waveform::DC ? "DC" : "SINE") << " "
           << offset_or_dc_value << " " << amplitude << " " << frequency << " " << node1 << " " << node2;
        return ss.str();
    }

    Waveform getWaveformType() const { return waveformType; }
    void setDCOffsetValue(double val) { this->offset_or_dc_value = val; }
    void setAmplitude(double amp) { this->amplitude = amp; }
    void setFrequency(double freq) { this->frequency = freq; }
};

class CurrentSource : public Component {
public:
    enum class Waveform { DC, SINE };
private:
    Waveform waveformType;
    double offset_or_dc_value;
    double amplitude;
    double frequency;
    string unit;

public:
    CurrentSource(const string& name, double dc_val, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::DC), offset_or_dc_value(dc_val),
              amplitude(0), frequency(0), unit("A") {}

    CurrentSource(const string& name, double offset, double amp, double freq, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::SINE), offset_or_dc_value(offset),
              amplitude(amp), frequency(freq), unit("A") {}

    double getValueAtTime(double time) const {
        if (waveformType == Waveform::SINE) return offset_or_dc_value + amplitude * sin(2 * M_PI * frequency * time);
        return offset_or_dc_value;
    }

    void display() const override {
        cout << "  - Element: " << name << " | Type: " << getType();
        if (waveformType == Waveform::DC) {
            cout << " (DC) | Value: " << scientific << setprecision(4) << offset_or_dc_value << " " << unit;
        } else {
            cout << " (SINE) | Params: Offset=" << scientific << setprecision(4) << offset_or_dc_value << ", Amp=" << scientific << setprecision(4) << amplitude << ", Freq=" << scientific << setprecision(4) << frequency << "Hz";
        }
        cout << " | Nodes: (" << node1 << ", " << node2 << ")" << endl;
    }

    string getType() const override { return "Current Source"; }
    string serialize() const override {
        stringstream ss;
        ss << "CurrentSource " << name << " " << (waveformType == Waveform::DC ? "DC" : "SINE") << " "
           << offset_or_dc_value << " " << amplitude << " " << frequency << " " << node1 << " " << node2;
        return ss.str();
    }

    Waveform getWaveformType() const { return waveformType; }
    void setDCOffsetValue(double val) { this->offset_or_dc_value = val; }
    void setAmplitude(double amp) { this->amplitude = amp; }
    void setFrequency(double freq) { this->frequency = freq; }
};

class Circuit {
private:
    vector<unique_ptr<Component>> components;
    set<int> getAllNodes() const;
    map<int, double> nodeVoltages;
    map<string, double> componentCurrents;
    map<int, double> previousNodeVoltages;
    map<string, double> previousComponentCurrents;
    string circuitName;

public:
    Circuit(string name = "Unnamed Circuit") : circuitName(name) {}
    void addElement(unique_ptr<Component> newComponent);
    bool removeElement(const string& componentName);
    Component* findElement(const string& componentName);
    void displayCircuit() const;
    void runTransientAnalysis(double startTime, double endTime, double timeStep);
    void simulateMultipleVariables(double startTime, double endTime, double timeStep);
    void simulateDCVoltageSweep(double startVoltage, double endVoltage, double stepVoltage);
    void simulateDCCurrentSweep(double startCurrent, double endCurrent, double stepCurrent);
    bool hasGround() const;
    void displayNodes() const;
    bool renameNode(int oldNodeNum, int newNodeNum);
    const vector<unique_ptr<Component>>& getComponents() const { return components; }
    bool setupAndSolveMNA(double time, double timeStep);
    double getNodeVoltage(int node) const;
    double getComponentCurrent(const string& name) const;
    void saveCircuit(const string& filename) const;
    bool loadCircuit(const string& filename);
    string getCircuitName() const { return circuitName; }
    void setCircuitName(const string& name) { circuitName = name; }
};

set<int> Circuit::getAllNodes() const {
    set<int> unique_nodes;
    for (const auto& comp : components) {
        unique_nodes.insert(comp->getNode1());
        unique_nodes.insert(comp->getNode2());
    }
    return unique_nodes;
}

double parseEngineeringValue(const string& valStr) {
    if (valStr.empty()) return 0.0;
    size_t first_char_pos = string::npos;
    for (size_t i = 0; i < valStr.length(); ++i) {
        if (isalpha(valStr[i])) {
            first_char_pos = i;
            break;
        }
    }
    if (first_char_pos == string::npos) {
        try { return stod(valStr); } catch(...) { return 0.0; }
    }
    string numberPart = valStr.substr(0, first_char_pos);
    string suffixPart = valStr.substr(first_char_pos);
    transform(suffixPart.begin(), suffixPart.end(), suffixPart.begin(), ::tolower);
    double number = 0.0;
    try { number = stod(numberPart); } catch(...) { return 0.0; }
    double multiplier = 1.0;
    if (suffixPart == "p") multiplier = 1e-12;
    else if (suffixPart == "n") multiplier = 1e-9;
    else if (suffixPart == "u") multiplier = 1e-6;
    else if (suffixPart == "m") multiplier = 1e-3;
    else if (suffixPart == "k") multiplier = 1e3;
    else if (suffixPart == "meg") multiplier = 1e6;
    else if (suffixPart == "g") multiplier = 1e9;
    return number * multiplier;
}

ComponentType intToComponentType(int choice) {
    switch (choice) {
        case 1: return ComponentType::RESISTOR;
        case 2: return ComponentType::CAPACITOR;
        case 3: return ComponentType::INDUCTOR;
        case 4: return ComponentType::VOLTAGE_SOURCE;
        case 5: return ComponentType::CURRENT_SOURCE;
        default: return ComponentType::UNKNOWN;
    }
}

class CircuitManager {
private:
    vector<unique_ptr<Circuit>> circuits;
    int activeCircuitIndex = -1;

public:
    void createNewCircuit();
    void displayAllCircuits() const;
    Circuit* getActiveCircuit() const;
    void selectCircuit();
    void removeActiveCircuit();
};


void displayMenu(const CircuitManager& manager);
int getUserChoice();
void handleAddComponent(Circuit& circuit);
void handleRemoveElement(Circuit& circuit);
void handleModifyComponent(Circuit& circuit);
void handleErrorComponentNotFound(const string& componentName);
void handleErrorNodeNotFound(int node);
void pauseSystem();
void handleTransientAnalysis(Circuit& circuit);
void handleMultipleVariablesAnalysis(Circuit& circuit);
void handleDisplayNodes(const Circuit& circuit);
void handleRenameNode(Circuit& circuit);
bool safelyReadDouble(double& val, const string& prompt);
void safelyReadDouble(double& val);
bool safelyReadInt(int& val, const string& prompt);
void safelyReadInt(int& val);
bool safelyReadString(string& val, const string& prompt);
bool safelyReadComponentParameters(string& inputLine, vector<string>& params);
void handleSaveCircuit(const Circuit& circuit);
void handleLoadCircuit(Circuit& circuit);
void handleNewCircuit(CircuitManager& manager);
void handleDisplayAndSelectCircuits(CircuitManager& manager);

int main() {
    CircuitManager myCircuitManager;
    myCircuitManager.createNewCircuit();

    bool running = true;
    while (running) {
        displayMenu(myCircuitManager);
        int choice = getUserChoice();
        Circuit* activeCircuit = myCircuitManager.getActiveCircuit();

        if (activeCircuit == nullptr && choice != 1 && choice != 14) {
            cout << "No active circuit selected. Please create a new circuit (option 1) or select an existing one (option 14)." << endl;
            pauseSystem();
            continue;
        }

        switch (choice) {
            case 1: handleNewCircuit(myCircuitManager); pauseSystem(); break;
            case 2: handleDisplayAndSelectCircuits(myCircuitManager); pauseSystem(); break;
            case 3: activeCircuit->displayCircuit(); pauseSystem(); break;
            case 4: handleAddComponent(*activeCircuit); pauseSystem(); break;
            case 5: handleRemoveElement(*activeCircuit); break;
            case 6: handleModifyComponent(*activeCircuit); break;
            case 7: handleTransientAnalysis(*activeCircuit); break;
            case 8: handleMultipleVariablesAnalysis(*activeCircuit); break;
            case 9: {
                bool sub_menu_running = true;
                while(sub_menu_running) {
                    cout << "\n--- DC Voltage Sweep Analysis ---" << endl;
                    double startVoltage, endVoltage, stepVoltage;
                    if (!safelyReadDouble(startVoltage, "Enter start voltage (or 'b' to go back to main menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(endVoltage, "Enter end voltage (or 'b' to go back to main menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(stepVoltage, "Enter voltage step (or 'b' to go back to main menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    activeCircuit->simulateDCVoltageSweep(startVoltage, endVoltage, stepVoltage);
                    pauseSystem();
                    sub_menu_running = false;
                }
                break;
            }
            case 10: {
                bool sub_menu_running = true;
                while(sub_menu_running) {
                    cout << "\n--- DC Current Sweep Analysis ---" << endl;
                    double startCurrent, endCurrent, stepCurrent;
                    if (!safelyReadDouble(startCurrent, "Enter start current (or 'b' to go back to main menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(endCurrent, "Enter end current (or 'b' to go back to main menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(stepCurrent, "Enter current step (or 'b' to go back to main menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    activeCircuit->simulateDCCurrentSweep(startCurrent, endCurrent, stepCurrent);
                    pauseSystem();
                    sub_menu_running = false;
                }
                break;
            }
            case 11: handleDisplayNodes(*activeCircuit); break;
            case 12: handleRenameNode(*activeCircuit); break;
            case 13: handleSaveCircuit(*activeCircuit); pauseSystem(); break;
            case 14: handleLoadCircuit(*activeCircuit); pauseSystem(); break;
            case 15: running = false; cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl; pauseSystem(); break;
        }
    }
    return 0;
}

void displayMenu(const CircuitManager& manager) {
    cout << "\n--- Circuit Simulator Menu ---" << endl;
    Circuit* activeCircuit = manager.getActiveCircuit();
    if (activeCircuit) {
        cout << "Active Circuit: " << activeCircuit->getCircuitName() << endl;
    } else {
        cout << "No active circuit." << endl;
    }
    cout << "1. New Circuit" << endl;
    cout << "2. Display and Select Circuit" << endl;
    cout << "3. Display Active Circuit" << endl;
    cout << "4. Add Element to Active Circuit" << endl;
    cout << "5. Remove Element from Active Circuit" << endl;
    cout << "6. Modify Element in Active Circuit" << endl;
    cout << "7. Perform Transient Analysis on Active Circuit" << endl;
    cout << "8. Perform Multiple Variables Analysis on Active Circuit" << endl;
    cout << "9. Perform DC Voltage Sweep Analysis on Active Circuit" << endl;
    cout << "10. Perform DC Current Sweep Analysis on Active Circuit" << endl;
    cout << "11. Display Existing Nodes in Active Circuit" << endl;
    cout << "12. Rename Node in Active Circuit" << endl;
    cout << "13. Save Active Circuit" << endl;
    cout << "14. Load Circuit (into current active circuit)" << endl;
    cout << "15. Exit" << endl;
    cout << "Enter your choice: ";
}

int getUserChoice() {
    int choice;
    while (!(cin >> choice)) {
        cout << "Invalid input. Please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void pauseSystem() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

bool safelyReadDouble(double& val, const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (line == "b" || line == "B") return false;
        stringstream ss(line);
        if (ss >> val && (ss >> ws).eof()) {
            return true;
        }
        cout << "Invalid input. Please enter a numeric value." << endl;
    }
}

void safelyReadDouble(double& val) {
    string line;
    while(true){
        getline(cin, line);
        stringstream ss(line);
        if (ss >> val && (ss >> ws).eof()) {
            return;
        }
        cout << "Invalid input. Please enter a numeric value: ";
    }
}

bool safelyReadInt(int& val, const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (line == "b" || line == "B") return false;
        stringstream ss(line);
        if (ss >> val && (ss >> ws).eof()) {
            return true;
        }
        cout << "Invalid input. Please enter an integer value." << endl;
    }
}

void safelyReadInt(int& val) {
    string line;
    while(true){
        getline(cin, line);
        stringstream ss(line);
        if (ss >> val && (ss >> ws).eof()) {
            return;
        }
        cout << "Invalid input. Please enter an integer value: ";
    }
}

bool safelyReadString(string& val, const string& prompt) {
    cout << prompt;
    getline(cin, val);
    if (val == "b" || val == "B") return false;
    return true;
}

bool safelyReadComponentParameters(string& inputLine, vector<string>& params) {
    getline(cin, inputLine);
    if (inputLine == "b" || inputLine == "B") return false;

    stringstream ss(inputLine);
    string token;
    while (ss >> token) {
        params.push_back(token);
    }
    return true;
}

void handleDisplayNodes(const Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        cout << "\n--- Display Existing Nodes ---" << endl;
        circuit.displayNodes();
        cout << "\nPress 'b' to go back to main menu." << endl;
        cout << "Enter your choice: ";
        string choice;
        getline(cin, choice);
        if (choice == "b" || choice == "B") {
            sub_menu_running = false;
        } else {
            cout << "Invalid choice. Please enter 'b' to go back." << endl;
            pauseSystem();
        }
    }
}

void handleRenameNode(Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        int oldNum, newNum;
        cout << "\n--- Rename Node ---" << endl;
        circuit.displayNodes();

        if (!safelyReadInt(oldNum, "Enter the node number to rename (or 'b' to go back to main menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadInt(newNum, "Enter the new node number (or 'b' to go back to main menu): ")) {
            sub_menu_running = false;
            break;
        }

        circuit.renameNode(oldNum, newNum);
        pauseSystem();
        sub_menu_running = false;
    }
}

void Circuit::addElement(unique_ptr<Component> newComponent) {
    components.push_back(move(newComponent));
}

void Circuit::displayCircuit() const {
    if (components.empty()) {
        cout << "Circuit '" << circuitName << "' is empty." << endl;
        return;
    }

    cout << "-------------------- Circuit Status: " << circuitName << " --------------------" << endl;
    if (this->hasGround()) {
        cout << "Ground status: Connected." << endl;
    } else {
        cout << ">> Warning: No ground node (0) found in the circuit! <<" << endl;
    }
    cout << "--------------------------------------------------------" << endl;

    for (const auto& comp : components) {
        comp->display();
    }
    cout << "--------------------------------------------------------" << endl;
}

bool Circuit::setupAndSolveMNA(double time, double timeStep) {
    if (!hasGround()) {
        cout << "Error: Circuit must have a ground node (0) for simulation." << endl;
        nodeVoltages.clear();
        componentCurrents.clear();
        return false;
    }

    set<int> all_nodes = getAllNodes();
    int max_node = 0;
    if (!all_nodes.empty()){
        max_node = *all_nodes.rbegin();
    }

    if (max_node == 0 && all_nodes.size() <= 1) {
        nodeVoltages[0] = 0.0;
        componentCurrents.clear();
        return true;
    }

    int num_active_nodes = 0;
    map<int, int> node_to_index;
    vector<int> active_nodes_list;
    for (int node : all_nodes) {
        if (node != 0) {
            node_to_index[node] = num_active_nodes++;
            active_nodes_list.push_back(node);
        }
    }

    int num_voltage_sources = 0;
    for (const auto& comp : components) {
        if (dynamic_cast<VoltageSource*>(comp.get())) {
            num_voltage_sources++;
        }
    }

    int total_equations = num_active_nodes + num_voltage_sources;
    if (total_equations == 0) {
        nodeVoltages.clear();
        componentCurrents.clear();
        if (all_nodes.count(0)) nodeVoltages[0] = 0.0;
        return true;
    }

    Eigen::MatrixXd G(total_equations, total_equations);
    Eigen::VectorXd B(total_equations);
    G.setZero();
    B.setZero();

    int current_vs_idx = 0;
    for (const auto& comp : components) {
        int n1 = comp->getNode1();
        int n2 = comp->getNode2();

        int idx1 = (n1 != 0 && node_to_index.count(n1)) ? node_to_index[n1] : -1;
        int idx2 = (n2 != 0 && node_to_index.count(n2)) ? node_to_index[n2] : -1;

        if (auto res = dynamic_cast<Resistor*>(comp.get())) {
            double conductance = 1.0 / res->getResistance();
            if (idx1 != -1) G(idx1, idx1) += conductance;
            if (idx2 != -1) G(idx2, idx2) += conductance;
            if (idx1 != -1 && idx2 != -1) {
                G(idx1, idx2) -= conductance;
                G(idx2, idx1) -= conductance;
            }
        } else if (auto cap = dynamic_cast<Capacitor*>(comp.get())) {
            double G_c = 1e-12;
            if (timeStep > 0 && cap->getCapacitance() > 0) {
                G_c = cap->getCapacitance() / timeStep;
                double v1_prev = previousNodeVoltages.count(n1) ? previousNodeVoltages.at(n1) : 0.0;
                double v2_prev = previousNodeVoltages.count(n2) ? previousNodeVoltages.at(n2) : 0.0;
                double Ieq = G_c * (v1_prev - v2_prev);
                if (idx1 != -1) B(idx1) -= Ieq;
                if (idx2 != -1) B(idx2) += Ieq;
            }
            if (idx1 != -1) G(idx1, idx1) += G_c;
            if (idx2 != -1) G(idx2, idx2) += G_c;
            if (idx1 != -1 && idx2 != -1) {
                G(idx1, idx2) -= G_c;
                G(idx2, idx1) -= G_c;
            }
        } else if (auto ind = dynamic_cast<Inductor*>(comp.get())) {
            double G_l = 1e9;
            if (timeStep > 0 && ind->getInductance() > 0) {
                G_l = timeStep / ind->getInductance();
                double i_prev = previousComponentCurrents.count(ind->getName()) ? previousComponentCurrents.at(ind->getName()) : 0.0;
                if (idx1 != -1) B(idx1) -= i_prev;
                if (idx2 != -1) B(idx2) += i_prev;
            }
            if (idx1 != -1) G(idx1, idx1) += G_l;
            if (idx2 != -1) G(idx2, idx2) += G_l;
            if (idx1 != -1 && idx2 != -1) {
                G(idx1, idx2) -= G_l;
                G(idx2, idx1) -= G_l;
            }
        } else if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
            int vs_eq_idx = num_active_nodes + current_vs_idx;
            if (idx1 != -1) {
                G(idx1, vs_eq_idx) += 1.0;
                G(vs_eq_idx, idx1) += 1.0;
            }
            if (idx2 != -1) {
                G(idx2, vs_eq_idx) -= 1.0;
                G(vs_eq_idx, idx2) -= 1.0;
            }
            B(vs_eq_idx) += vs->getValueAtTime(time);
            current_vs_idx++;
        } else if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
            double current_val = cs->getValueAtTime(time);
            if (idx1 != -1) B(idx1) -= current_val;
            if (idx2 != -1) B(idx2) += current_val;
        }
    }

    Eigen::VectorXd X;
    if (G.determinant() == 0) {
        cout << "Error: Circuit matrix is singular. Cannot be solved. Check for floating nodes or invalid connections." << endl;
        nodeVoltages.clear();
        componentCurrents.clear();
        return false;
    }
    X = G.lu().solve(B);

    nodeVoltages.clear();
    nodeVoltages[0] = 0.0;
    for (int i = 0; i < num_active_nodes; ++i) {
        nodeVoltages[active_nodes_list[i]] = X(i);
    }

    current_vs_idx = 0;
    componentCurrents.clear();
    for (const auto& comp : components) {
        if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
            int vs_eq_idx = num_active_nodes + current_vs_idx;
            componentCurrents[vs->getName()] = X(vs_eq_idx);
            current_vs_idx++;
        } else if (auto res = dynamic_cast<Resistor*>(comp.get())) {
            double v1 = getNodeVoltage(res->getNode1());
            double v2 = getNodeVoltage(res->getNode2());
            componentCurrents[res->getName()] = (v1 - v2) / res->getResistance();
        } else if (auto cap = dynamic_cast<Capacitor*>(comp.get())) {
            if (timeStep > 0 && cap->getCapacitance() > 0) {
                double v1_prev = previousNodeVoltages.count(cap->getNode1()) ? previousNodeVoltages.at(cap->getNode1()) : 0.0;
                double v2_prev = previousNodeVoltages.count(cap->getNode2()) ? previousNodeVoltages.at(cap->getNode2()) : 0.0;
                double v1_now = getNodeVoltage(cap->getNode1());
                double v2_now = getNodeVoltage(cap->getNode2());
                componentCurrents[cap->getName()] = (cap->getCapacitance() / timeStep) * ((v1_now - v2_now) - (v1_prev - v2_prev));
            } else {
                componentCurrents[cap->getName()] = 0.0;
            }
        }
    }

    return true;
}

double Circuit::getNodeVoltage(int node) const {
    if (nodeVoltages.count(node)) {
        return nodeVoltages.at(node);
    }
    return 0.0;
}

double Circuit::getComponentCurrent(const string& name) const {
    if (componentCurrents.count(name)) {
        return componentCurrents.at(name);
    }
    return 0.0;
}


void Circuit::runTransientAnalysis(double startTime, double endTime, double timeStep) {
    if (!hasGround()) {
        cout << "Error: Circuit must have a ground node (0) for analysis." << endl;
        return;
    }
    if (timeStep <= 0) {
        cout << "Error: Time step must be a positive number." << endl;
        return;
    }

    map<int, int> nodeMap;
    int nodeCount = 0;
    for (const auto& comp : components) {
        if (comp->getNode1() != 0 && nodeMap.find(comp->getNode1()) == nodeMap.end()) {
            nodeMap[comp->getNode1()] = ++nodeCount;
        }
        if (comp->getNode2() != 0 && nodeMap.find(comp->getNode2()) == nodeMap.end()) {
            nodeMap[comp->getNode2()] = ++nodeCount;
        }
    }

    map<string, int> inductorMap;
    int inductorCount = 0;
    map<string, int> voltageSourceMap;
    int voltageSourceCount = 0;

    for (const auto& comp : components) {
        if (dynamic_cast<Inductor*>(comp.get())) {
            inductorMap[comp->getName()] = inductorCount++;
        } else if (dynamic_cast<VoltageSource*>(comp.get())) {
            voltageSourceMap[comp->getName()] = voltageSourceCount++;
        }
    }

    int matrixSize = nodeCount + inductorCount + voltageSourceCount;
    if (matrixSize <= 0) {
        cout << "Circuit has no unknowns to solve for." << endl;
        return;
    }

    Eigen::VectorXd x_prev = Eigen::VectorXd::Zero(matrixSize);

    cout << "--- Starting Transient Analysis ---" << endl;
    cout << scientific << setprecision(6);

    for (double time = startTime; time <= endTime; time += timeStep) {
        Eigen::MatrixXd A = Eigen::MatrixXd::Zero(matrixSize, matrixSize);
        Eigen::VectorXd z = Eigen::VectorXd::Zero(matrixSize);

        for (const auto& comp : components) {
            int n1 = comp->getNode1();
            int n2 = comp->getNode2();
            int mapped_n1 = (n1 == 0) ? -1 : nodeMap.at(n1) - 1;
            int mapped_n2 = (n2 == 0) ? -1 : nodeMap.at(n2) - 1;

            if (auto r = dynamic_cast<Resistor*>(comp.get())) {
                double g = 1.0 / r->getResistance();
                if (mapped_n1 != -1) A(mapped_n1, mapped_n1) += g;
                if (mapped_n2 != -1) A(mapped_n2, mapped_n2) += g;
                if (mapped_n1 != -1 && mapped_n2 != -1) {
                    A(mapped_n1, mapped_n2) -= g;
                    A(mapped_n2, mapped_n1) -= g;
                }
            }
            else if (auto c = dynamic_cast<Capacitor*>(comp.get())) {
                double c_h = c->getCapacitance() / timeStep;
                if (mapped_n1 != -1) A(mapped_n1, mapped_n1) += c_h;
                if (mapped_n2 != -1) A(mapped_n2, mapped_n2) += c_h;
                if (mapped_n1 != -1 && mapped_n2 != -1) {
                    A(mapped_n1, mapped_n2) -= c_h;
                    A(mapped_n2, mapped_n1) -= c_h;
                }
                double v_prev = (mapped_n1 != -1 ? x_prev(mapped_n1) : 0.0) - (mapped_n2 != -1 ? x_prev(mapped_n2) : 0.0);
                double ic_prev = c_h * v_prev;
                if (mapped_n1 != -1) z(mapped_n1) += ic_prev;
                if (mapped_n2 != -1) z(mapped_n2) -= ic_prev;
            }
            else if (auto l = dynamic_cast<Inductor*>(comp.get())) {
                int l_idx = nodeCount + inductorMap.at(l->getName());
                double l_h = l->getInductance() / timeStep;
                if (mapped_n1 != -1) A(mapped_n1, l_idx) += 1.0;
                if (mapped_n2 != -1) A(mapped_n2, l_idx) -= 1.0;
                if (mapped_n1 != -1) A(l_idx, mapped_n1) += 1.0;
                if (mapped_n2 != -1) A(l_idx, mapped_n2) -= 1.0;
                A(l_idx, l_idx) -= l_h;
                z(l_idx) -= l_h * x_prev(l_idx);
            }
            else if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                int vs_idx = nodeCount + inductorCount + voltageSourceMap.at(vs->getName());
                if (mapped_n1 != -1) A(mapped_n1, vs_idx) += 1.0;
                if (mapped_n2 != -1) A(mapped_n2, vs_idx) -= 1.0;
                if (mapped_n1 != -1) A(vs_idx, mapped_n1) += 1.0;
                if (mapped_n2 != -1) A(vs_idx, mapped_n2) -= 1.0;
                z(vs_idx) += vs->getValueAtTime(time);
            }
            else if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                double i_val = cs->getValueAtTime(time);
                if (mapped_n1 != -1) z(mapped_n1) -= i_val;
                if (mapped_n2 != -1) z(mapped_n2) += i_val;
            }
        }

        Eigen::VectorXd x_t = A.colPivHouseholderQr().solve(z);

        cout << "\nTime: " << time << "s" << endl;
        for (auto const& [node_num, matrix_idx] : nodeMap) {
            cout << "  V(node " << node_num << "): " << x_t(matrix_idx - 1) << " V" << endl;
        }
        for (auto const& [l_name, matrix_idx] : inductorMap) {
            cout << "  I(" << l_name << "): " << x_t[nodeCount + matrix_idx] << " A" << endl;
        }
        for (auto const& [vs_name, matrix_idx] : voltageSourceMap) {
            cout << "  I(" << vs_name << "): " << x_t[nodeCount + inductorCount + matrix_idx] << " A" << endl;
        }

        x_prev = x_t;
    }
    cout << "--- Transient Analysis Finished ---" << endl;
}
void Circuit::simulateMultipleVariables(double startTime, double endTime, double timeStep) {
    if (timeStep <= 0) {
        cout << "Error: Time step must be a positive value." << endl;
        return;
    }
    if (startTime > endTime) {
        cout << "Error: Start time cannot be greater than end time." << endl;
        return;
    }
    if (!hasGround()) {
        cout << "Error: Circuit must have a ground node (0) for simulation." << endl;
        return;
    }

    nodeVoltages.clear();
    componentCurrents.clear();
    previousNodeVoltages.clear();
    previousComponentCurrents.clear();
    set<int> all_nodes = getAllNodes();
    for(int node : all_nodes) {
        previousNodeVoltages[node] = 0.0;
    }
    for(const auto& comp: components) {
        previousComponentCurrents[comp->getName()] = 0.0;
    }

    cout << "\n--- Transient Simulation Results (All Variables) ---" << endl;
    for (double time = startTime; time <= endTime + timeStep/2; time += timeStep) {
        cout << "\nTime: " << scientific << setprecision(4) << time << "s" << endl;
        if (!setupAndSolveMNA(time, timeStep)) {
            cout << "Circuit analysis failed at this time step." << endl;
            break;
        }

        cout << "  Node Voltages:" << endl;
        for (auto const& [node, voltage] : nodeVoltages) {
            cout << "    Node " << node << ": " << scientific << setprecision(4) << voltage << " V" << endl;
        }

        cout << "  Component Currents:" << endl;
        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << "    " << comp->getName() << " (" << comp->getType() << "): "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        previousNodeVoltages = nodeVoltages;
        previousComponentCurrents = componentCurrents;
    }
    cout << "--- Transient Simulation Finished ---" << endl;
}

void Circuit::simulateDCVoltageSweep(double startVoltage, double endVoltage, double stepVoltage) {
    if (stepVoltage == 0) {
        cout << "Error: Voltage step for sweep cannot be zero." << endl;
        return;
    }
    if ((startVoltage < endVoltage && stepVoltage < 0) || (startVoltage > endVoltage && stepVoltage > 0)) {
        cout << "Error: Incorrect step direction for given start and end voltages." << endl;
        return;
    }
    if (!hasGround()) {
        cout << "Error: Circuit must have a ground node (0) for simulation." << endl;
        return;
    }

    VoltageSource* sweepSource = nullptr;
    for (const auto& comp : components) {
        if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
            if (vs->getWaveformType() == VoltageSource::Waveform::DC) {
                sweepSource = vs;
                break;
            }
        }
    }

    if (!sweepSource) {
        cout << "No DC voltage source found in the circuit to perform DC sweep. Please add one." << endl;
        return;
    }

    double originalVoltage = sweepSource->getValueAtTime(0);
    double currentVoltage = startVoltage;

    cout << "\n--- DC Voltage Sweep Results (Sweeping " << sweepSource->getName() << ") ---" << endl;
    while ((stepVoltage > 0 && currentVoltage <= endVoltage + stepVoltage/2) || (stepVoltage < 0 && currentVoltage >= endVoltage + stepVoltage/2)) {
        sweepSource->setDCOffsetValue(currentVoltage);
        cout << "\nSweep Voltage (" << sweepSource->getName() << "): " << scientific << setprecision(4) << currentVoltage << "V" << endl;
        if (!setupAndSolveMNA(0, 0.0)) {
            cout << "Circuit analysis failed for this voltage step. Aborting sweep." << endl;
            break;
        }
        cout << "  Node Voltages:" << endl;
        for (auto const& [node, voltage] : nodeVoltages) {
            cout << "    Node " << node << ": " << scientific << setprecision(4) << voltage << " V" << endl;
        }
        cout << "  Component Currents:" << endl;
        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << "    " << comp->getName() << " (" << comp->getType() << "): "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        currentVoltage += stepVoltage;
    }
    sweepSource->setDCOffsetValue(originalVoltage);
    cout << "--- DC Voltage Sweep Finished ---" << endl;
}

void Circuit::simulateDCCurrentSweep(double startCurrent, double endCurrent, double stepCurrent) {
    if (stepCurrent == 0) {
        cout << "Error: Current step for sweep cannot be zero." << endl;
        return;
    }
    if ((startCurrent < endCurrent && stepCurrent < 0) || (startCurrent > endCurrent && stepCurrent > 0)) {
        cout << "Error: Incorrect step direction for given start and end currents." << endl;
        return;
    }
    if (!hasGround()) {
        cout << "Error: Circuit must have a ground node (0) for simulation." << endl;
        return;
    }

    CurrentSource* sweepSource = nullptr;
    for (const auto& comp : components) {
        if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
            if (cs->getWaveformType() == CurrentSource::Waveform::DC) {
                sweepSource = cs;
                break;
            }
        }
    }

    if (!sweepSource) {
        cout << "No DC current source found in the circuit to perform DC sweep. Please add one." << endl;
        return;
    }

    double originalCurrent = sweepSource->getValueAtTime(0);
    double currentSweepValue = startCurrent;

    cout << "\n--- DC Current Sweep Results (Sweeping " << sweepSource->getName() << ") ---" << endl;
    while ((stepCurrent > 0 && currentSweepValue <= endCurrent + stepCurrent / 2) || (stepCurrent < 0 && currentSweepValue >= endCurrent + stepCurrent / 2)) {
        sweepSource->setDCOffsetValue(currentSweepValue);
        cout << "\nSweep Current (" << sweepSource->getName() << "): " << scientific << setprecision(4) << currentSweepValue << " A" << endl;
        if (!setupAndSolveMNA(0, 0.0)) {
            cout << "Circuit analysis failed for this current step. Aborting sweep." << endl;
            break;
        }
        cout << "  Node Voltages:" << endl;
        for (auto const& [node, voltage] : nodeVoltages) {
            cout << "    Node " << node << ": " << scientific << setprecision(4) << voltage << " V" << endl;
        }
        cout << "  Component Currents:" << endl;
        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << "    " << comp->getName() << " (" << comp->getType() << "): "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        currentSweepValue += stepCurrent;
    }
    sweepSource->setDCOffsetValue(originalCurrent);
    cout << "--- DC Current Sweep Finished ---" << endl;
}


bool Circuit::hasGround() const {
    for (const auto& comp : components) {
        if (comp->getNode1() == 0 || comp->getNode2() == 0) {
            return true;
        }
    }
    return false;
}

bool Circuit::renameNode(int oldNodeNum, int newNodeNum) {
    auto allNodes = getAllNodes();
    if (allNodes.find(oldNodeNum) == allNodes.end()) {
        cout << "Error: Node " << oldNodeNum << " does not exist in the circuit." << endl;
        return false;
    }
    if (oldNodeNum != newNodeNum && allNodes.count(newNodeNum) > 0) {
        cout << "Error: Node " << newNodeNum << " already exists. Merging nodes is not supported directly." << endl;
        return false;
    }
    for (auto& comp : components) {
        int n1 = comp->getNode1();
        int n2 = comp->getNode2();
        bool changed = false;
        if (n1 == oldNodeNum) {
            n1 = newNodeNum;
            changed = true;
        }
        if (n2 == oldNodeNum) {
            n2 = newNodeNum;
            changed = true;
        }
        if (changed) {
            comp->setNodes(n1, n2);
        }
    }
    cout << "Success: Node " << oldNodeNum << " renamed to " << newNodeNum << " throughout the circuit." << endl;
    return true;
}

void Circuit::displayNodes() const {
    if (components.empty()) {
        cout << "Circuit is empty, no nodes to display." << endl;
        return;
    }
    auto unique_nodes = getAllNodes();
    cout << "--- Existing Nodes in Circuit '" << circuitName << "' ---" << endl;
    if (unique_nodes.empty()) {
        cout << "No nodes found." << endl;
    } else {
        cout << "Nodes: ";
        for (int node : unique_nodes) {
            cout << node << " ";
        }
        cout << endl;
    }
    cout << "---------------------------------" << endl;
}

bool Circuit::removeElement(const string& componentName) {
    for (auto it = components.begin(); it != components.end(); ++it) {
        if ((*it)->getName() == componentName) {
            components.erase(it);
            return true;
        }
    }
    return false;
}

Component* Circuit::findElement(const string& componentName) {
    for (const auto& comp : components) {
        if (comp->getName() == componentName) {
            return comp.get();
        }
    }
    return nullptr;
}


void handleTransientAnalysis(Circuit& circuit) {
    double startTime = 0.0, endTime = 0.0, timeStep = 0.0;
    cout << "--- Transient Analysis ---" << endl;

    cout << "Enter start time: ";
    cin >> startTime;

    cout << "Enter end time: ";
    cin >> endTime;

    cout << "Enter time step: ";
    cin >> timeStep;


    if (cin.fail()) {
        cout << "\nError: Invalid numeric input. Please enter numbers only." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    circuit.runTransientAnalysis(startTime, endTime, timeStep);
}

void handleMultipleVariablesAnalysis(Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        double startTime, endTime, timeStep;
        cout << "\n--- Multiple Variables Analysis for " << circuit.getCircuitName() << " ---" << endl;
        if (!safelyReadDouble(startTime, "Enter start time (s) (or 'b' to go back to main menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadDouble(endTime, "Enter end time (s) (or 'b' to go back to main menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadDouble(timeStep, "Enter time step (s) (or 'b' to go back to main menu): ")) {
            sub_menu_running = false;
            break;
        }
        circuit.simulateMultipleVariables(startTime, endTime, timeStep);
        pauseSystem();
        sub_menu_running = false;
    }
}

void handleAddComponent(Circuit& circuit) {
    string name;
    int type_choice;
    string inputLine;
    vector<string> params;

    cout << "\n--- Add New Element to " << circuit.getCircuitName() << " ---" << endl;
    cout << "Select element type:" << endl;
    cout << "1. Resistor" << endl;
    cout << "2. Capacitor" << endl;
    cout << "3. Inductor" << endl;
    cout << "4. Voltage Source" << endl;
    cout << "5. Current Source" << endl;
    cout << "Enter your choice (or 'b' to go back to main menu): ";

    string choice_str;
    getline(cin, choice_str);
    if (choice_str == "b" || choice_str == "B") return;

    stringstream ss_choice(choice_str);
    if (!(ss_choice >> type_choice)) {
        cout << "Invalid input. Please enter a number." << endl;
        return;
    }

    ComponentType type = intToComponentType(type_choice);

    cout << "Enter element name: ";
    getline(cin, name);
    if (name == "b" || name == "B") return;
    if (circuit.findElement(name) != nullptr) {
        cout << "Error: Element with this name already exists. Please choose a unique name." << endl;
        return;
    }

    switch (type) {
        case ComponentType::RESISTOR: {
            double resistance;
            cout << "Enter resistance value (e.g., 100, 1k, 2.2meg): ";
            string val_str;
            getline(cin, val_str);
            if (val_str == "b" || val_str == "B") return;
            resistance = parseEngineeringValue(val_str);
            if (resistance <= 0) { cout << "Invalid resistance value. Must be positive." << endl; return; }

            int n1, n2;
            if (!safelyReadInt(n1, "Enter Node 1: ")) return;
            if (!safelyReadInt(n2, "Enter Node 2: ")) return;
            circuit.addElement(make_unique<Resistor>(name, resistance, n1, n2));
            cout << "Resistor '" << name << "' added." << endl;
            break;
        }
        case ComponentType::CAPACITOR: {
            double capacitance;
            cout << "Enter capacitance value (e.g., 10u, 47n, 100p): ";
            string val_str;
            getline(cin, val_str);
            if (val_str == "b" || val_str == "B") return;
            capacitance = parseEngineeringValue(val_str);
            if (capacitance <= 0) { cout << "Invalid capacitance value. Must be positive." << endl; return; }

            int n1, n2;
            if (!safelyReadInt(n1, "Enter Node 1: ")) return;
            if (!safelyReadInt(n2, "Enter Node 2: ")) return;
            circuit.addElement(make_unique<Capacitor>(name, capacitance, n1, n2));
            cout << "Capacitor '" << name << "' added." << endl;
            break;
        }
        case ComponentType::INDUCTOR: {
            double inductance;
            cout << "Enter inductance value (e.g., 10m, 1u, 500n): ";
            string val_str;
            getline(cin, val_str);
            if (val_str == "b" || val_str == "B") return;
            inductance = parseEngineeringValue(val_str);
            if (inductance <= 0) { cout << "Invalid inductance value. Must be positive." << endl; return; }

            int n1, n2;
            if (!safelyReadInt(n1, "Enter Node 1: ")) return;
            if (!safelyReadInt(n2, "Enter Node 2: ")) return;
            circuit.addElement(make_unique<Inductor>(name, inductance, n1, n2));
            cout << "Inductor '" << name << "' added." << endl;
            break;
        }
        case ComponentType::VOLTAGE_SOURCE: {
            cout << "Select voltage source waveform type (1 for DC, 2 for SINE): ";
            string waveform_choice_str;
            getline(cin, waveform_choice_str);
            if (waveform_choice_str == "b" || waveform_choice_str == "B") return;
            stringstream ss_waveform(waveform_choice_str);
            int waveform_choice;
            if (!(ss_waveform >> waveform_choice)) {
                cout << "Invalid input. Please enter 1 or 2." << endl;
                return;
            }
            if (waveform_choice == 1) {
                double dc_val;
                if (!safelyReadDouble(dc_val, "Enter DC voltage value (V): ")) return;
                int n1, n2;
                if (!safelyReadInt(n1, "Enter Node 1: ")) return;
                if (!safelyReadInt(n2, "Enter Node 2: ")) return;
                circuit.addElement(make_unique<VoltageSource>(name, dc_val, n1, n2));
                cout << "DC Voltage Source '" << name << "' added." << endl;
            } else if (waveform_choice == 2) {
                double offset, amplitude, frequency;
                if (!safelyReadDouble(offset, "Enter Offset voltage (V): ")) return;
                if (!safelyReadDouble(amplitude, "Enter Amplitude (V): ")) return;
                if (!safelyReadDouble(frequency, "Enter Frequency (Hz): ")) return;
                if (frequency < 0) { cout << "Frequency cannot be negative." << endl; return; }
                int n1, n2;
                if (!safelyReadInt(n1, "Enter Node 1: ")) return;
                if (!safelyReadInt(n2, "Enter Node 2: ")) return;
                circuit.addElement(make_unique<VoltageSource>(name, offset, amplitude, frequency, n1, n2));
                cout << "Sine Voltage Source '" << name << "' added." << endl;
            } else {
                cout << "Invalid waveform type selected. Please choose 1 or 2." << endl;
            }
            break;
        }
        case ComponentType::CURRENT_SOURCE: {
            cout << "Select current source waveform type (1 for DC, 2 for SINE): ";
            string waveform_choice_str;
            getline(cin, waveform_choice_str);
            if (waveform_choice_str == "b" || waveform_choice_str == "B") return;
            stringstream ss_waveform(waveform_choice_str);
            int waveform_choice;
            if (!(ss_waveform >> waveform_choice)) {
                cout << "Invalid input. Please enter 1 or 2." << endl;
                return;
            }
            if (waveform_choice == 1) {
                double dc_val;
                if (!safelyReadDouble(dc_val, "Enter DC current value (A): ")) return;
                int n1, n2;
                if (!safelyReadInt(n1, "Enter Node 1: ")) return;
                if (!safelyReadInt(n2, "Enter Node 2: ")) return;
                circuit.addElement(make_unique<CurrentSource>(name, dc_val, n1, n2));
                cout << "DC Current Source '" << name << "' added." << endl;
            } else if (waveform_choice == 2) {
                double offset, amplitude, frequency;
                if (!safelyReadDouble(offset, "Enter Offset current (A): ")) return;
                if (!safelyReadDouble(amplitude, "Enter Amplitude (A): ")) return;
                if (!safelyReadDouble(frequency, "Enter Frequency (Hz): ")) return;
                if (frequency < 0) { cout << "Frequency cannot be negative." << endl; return; }
                int n1, n2;
                if (!safelyReadInt(n1, "Enter Node 1: ")) return;
                if (!safelyReadInt(n2, "Enter Node 2: ")) return;
                circuit.addElement(make_unique<CurrentSource>(name, offset, amplitude, frequency, n1, n2));
                cout << "Sine Current Source '" << name << "' added." << endl;
            } else {
                cout << "Invalid waveform type selected. Please choose 1 or 2." << endl;
            }
            break;
        }
        default:
            cout << "Invalid element type selection." << endl;
            break;
    }
}

void handleRemoveElement(Circuit& circuit) {
    cout << "\n--- Remove Element from " << circuit.getCircuitName() << " ---" << endl;
    cout << "Enter the name of the component to remove (or 'b' to go back to main menu): ";
    string name_to_remove;
    getline(cin, name_to_remove);
    if (name_to_remove == "b" || name_to_remove == "B") return;

    if (circuit.removeElement(name_to_remove)) {
        cout << "Element '" << name_to_remove << "' successfully removed." << endl;
    } else {
        handleErrorComponentNotFound(name_to_remove);
    }
    pauseSystem();
}

void handleModifyComponent(Circuit& circuit) {
    cout << "\n--- Modify Element in " << circuit.getCircuitName() << " ---" << endl;
    cout << "Enter the name of the component to modify (or 'b' to go back to main menu): ";
    string name_to_modify;
    getline(cin, name_to_modify);
    if (name_to_modify == "b" || name_to_modify == "B") return;

    Component* comp = circuit.findElement(name_to_modify);
    if (!comp) {
        handleErrorComponentNotFound(name_to_modify);
        pauseSystem();
        return;
    }

    cout << "Element found: " << comp->getName() << " (" << comp->getType() << ")" << endl;
    cout << "What would you like to modify?" << endl;
    cout << "1. Nodes" << endl;

    if (comp->getType() == "Resistor") {
        cout << "2. Resistance" << endl;
    } else if (comp->getType() == "Capacitor") {
        cout << "2. Capacitance" << endl;
    } else if (comp->getType() == "Inductor") {
        cout << "2. Inductance" << endl;
    } else if (comp->getType() == "Voltage Source") {
        cout << "2. DC Value/Offset" << endl;
        if (dynamic_cast<VoltageSource*>(comp)->getWaveformType() == VoltageSource::Waveform::SINE) {
            cout << "3. Amplitude" << endl;
            cout << "4. Frequency" << endl;
        }
    } else if (comp->getType() == "Current Source") {
        cout << "2. DC Value/Offset" << endl;
        if (dynamic_cast<CurrentSource*>(comp)->getWaveformType() == CurrentSource::Waveform::SINE) {
            cout << "3. Amplitude" << endl;
            cout << "4. Frequency" << endl;
        }
    }
    cout << "Enter your choice: ";
    string modify_choice_str;
    getline(cin, modify_choice_str);
    stringstream ss_modify_choice(modify_choice_str);
    int modify_choice;
    if (!(ss_modify_choice >> modify_choice)) {
        cout << "Invalid input. Please enter a number." << endl;
        pauseSystem();
        return;
    }

    if (modify_choice == 1) {
        int n1, n2;
        cout << "Enter new nodes in format 'Node1 Node2' (e.g., 1 2): ";
        string line;
        getline(cin, line);
        if (line == "b" || line == "B") return;
        stringstream ss(line);
        if (!(ss >> n1 >> n2)) {
            cout << "Invalid input. Please enter two numbers for nodes." << endl;
            pauseSystem();
            return;
        }
        comp->setNodes(n1, n2);
        cout << "Nodes successfully updated." << endl;
    } else if (modify_choice == 2) {
        if (auto res = dynamic_cast<Resistor*>(comp)) {
            double new_res;
            cout << "Enter new resistance: ";
            safelyReadDouble(new_res);
            res->setResistance(new_res);
            cout << "Resistance successfully updated." << endl;
        } else if (auto cap = dynamic_cast<Capacitor*>(comp)) {
            double new_cap;
            string cap_str;
            cout << "Enter new capacitance (e.g., 100n for 100 nanofarads, 10u for 10 microfarads): ";
            getline(cin, cap_str);
            if (cap_str == "b" || cap_str == "B") return;
            new_cap = parseEngineeringValue(cap_str);
            if (new_cap <= 0 && (cap_str != "0" && cap_str != "0.0")) {
                cout << "Invalid capacitance value. Please try again." << endl;
                pauseSystem();
                return;
            }
            cap->setCapacitance(new_cap);
            cout << "Capacitance successfully updated." << endl;
        } else if (auto ind = dynamic_cast<Inductor*>(comp)) {
            double new_ind;
            string ind_str;
            cout << "Enter new inductance (e.g., 10m for 10 millihenries, 1u for 1 microhenry): ";
            getline(cin, ind_str);
            if (ind_str == "b" || ind_str == "B") return;
            new_ind = parseEngineeringValue(ind_str);
            if (new_ind <= 0 && (ind_str != "0" && ind_str != "0.0")) {
                cout << "Invalid inductance value. Please try again." << endl;
                pauseSystem();
                return;
            }
            ind->setInductance(new_ind);
            cout << "Inductance successfully updated." << endl;
        } else if (auto vs = dynamic_cast<VoltageSource*>(comp)) {
            double new_val;
            cout << "Enter new DC value/offset: ";
            safelyReadDouble(new_val);
            vs->setDCOffsetValue(new_val);
            cout << "DC value/offset successfully updated." << endl;
        } else if (auto cs = dynamic_cast<CurrentSource*>(comp)) {
            double new_val;
            cout << "Enter new DC value/offset: ";
            safelyReadDouble(new_val);
            cs->setDCOffsetValue(new_val);
            cout << "DC value/offset successfully updated." << endl;
        } else {
            cout << "Invalid modification choice for this component type." << endl;
        }
    } else if (modify_choice == 3) {
        if (auto vs = dynamic_cast<VoltageSource*>(comp)) {
            if (vs->getWaveformType() == VoltageSource::Waveform::SINE) {
                double new_amp;
                cout << "Enter new amplitude: ";
                safelyReadDouble(new_amp);
                vs->setAmplitude(new_amp);
                cout << "Amplitude successfully updated." << endl;
            } else {
                cout << "Invalid modification choice for this component type." << endl;
            }
        } else if (auto cs = dynamic_cast<CurrentSource*>(comp)) {
            if (cs->getWaveformType() == CurrentSource::Waveform::SINE) {
                double new_amp;
                cout << "Enter new amplitude: ";
                safelyReadDouble(new_amp);
                cs->setAmplitude(new_amp);
                cout << "Amplitude successfully updated." << endl;
            } else {
                cout << "Invalid modification choice for this component type." << endl;
            }
        } else {
            cout << "Invalid modification choice for this component type." << endl;
        }
    } else if (modify_choice == 4) {
        if (auto vs = dynamic_cast<VoltageSource*>(comp)) {
            if (vs->getWaveformType() == VoltageSource::Waveform::SINE) {
                double new_freq;
                cout << "Enter new frequency: ";
                safelyReadDouble(new_freq);
                vs->setFrequency(new_freq);
                cout << "Frequency successfully updated." << endl;
            } else {
                cout << "Invalid modification choice for this component type." << endl;
            }
        } else if (auto cs = dynamic_cast<CurrentSource*>(comp)) {
            if (cs->getWaveformType() == CurrentSource::Waveform::SINE) {
                double new_freq;
                cout << "Enter new frequency: ";
                safelyReadDouble(new_freq);
                cs->setFrequency(new_freq);
                cout << "Frequency successfully updated." << endl;
            } else {
                cout << "Invalid modification choice for this component type." << endl;
            }
        } else {
            cout << "Invalid modification choice for this component type." << endl;
        }
    } else {
        cout << "Invalid modification choice." << endl;
    }
    pauseSystem();
}

void handleErrorComponentNotFound(const string& componentName) {
    cout << "Error: Component '" << componentName << "' not found in the circuit." << endl;
}

void handleErrorNodeNotFound(int node) {
    cout << "Error: Node '" << node << "' not found in the circuit." << endl;
}

void Circuit::saveCircuit(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "Error: Could not open file for saving: " << filename << endl;
        return;
    }

    outFile << "CIRCUIT_NAME " << circuitName << endl;
    for (const auto& comp : components) {
        outFile << comp->serialize() << endl;
    }

    outFile.close();
    cout << "Circuit saved to '" << filename << "'." << endl;
}

bool Circuit::loadCircuit(const string& filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Error: Could not open file for loading: " << filename << endl;
        return false;
    }

    components.clear();
    string line;
    string loadedCircuitName = "Unnamed Circuit";
    if (getline(inFile, line)) {
        stringstream ss_name(line);
        string keyword;
        ss_name >> keyword;
        if (keyword == "CIRCUIT_NAME") {
            getline(ss_name, loadedCircuitName);
            if (loadedCircuitName.rfind(' ') == 0) loadedCircuitName.erase(0, 1);
        } else {

            inFile.seekg(0);
        }
    }
    setCircuitName(loadedCircuitName);


    while (getline(inFile, line)) {
        stringstream ss(line);
        string type_str;
        ss >> type_str;

        string name;
        if (!(ss >> name)) {
            cout << "Error loading element: Invalid name for type " << type_str << "." << endl;
            continue;
        }

        int n1, n2;
        if (type_str == "Resistor") {
            double resistance;
            if (!(ss >> resistance >> n1 >> n2)) {
                cout << "Error loading Resistor: Invalid format for '" << name << "'." << endl;
                continue;
            }
            addElement(make_unique<Resistor>(name, resistance, n1, n2));
        } else if (type_str == "Capacitor") {
            double capacitance;
            if (!(ss >> capacitance >> n1 >> n2)) {
                cout << "Error loading Capacitor: Invalid format for '" << name << "'." << endl;
                continue;
            }
            addElement(make_unique<Capacitor>(name, capacitance, n1, n2));
        } else if (type_str == "Inductor") {
            double inductance;
            if (!(ss >> inductance >> n1 >> n2)) {
                cout << "Error loading Inductor: Invalid format for '" << name << "'." << endl;
                continue;
            }
            addElement(make_unique<Inductor>(name, inductance, n1, n2));
        } else if (type_str == "VoltageSource") {
            string waveform_type_str;
            double offset_or_dc_value, amplitude, frequency;
            if (!(ss >> waveform_type_str >> offset_or_dc_value >> amplitude >> frequency >> n1 >> n2)) {
                cout << "Error loading VoltageSource: Invalid format." << endl;
                continue;
            }
            if (waveform_type_str == "DC") {
                addElement(make_unique<VoltageSource>(name, offset_or_dc_value, n1, n2));
            } else if (waveform_type_str == "SINE") {
                addElement(make_unique<VoltageSource>(name, offset_or_dc_value, amplitude, frequency, n1, n2));
            } else {
                cout << "Unknown waveform type for VoltageSource: " << waveform_type_str << endl;
            }
        } else if (type_str == "CurrentSource") {
            string waveform_type_str;
            double offset_or_dc_value, amplitude, frequency;
            if (!(ss >> waveform_type_str >> offset_or_dc_value >> amplitude >> frequency >> n1 >> n2)) {
                cout << "Error loading CurrentSource: Invalid format." << endl;
                continue;
            }
            if (waveform_type_str == "DC") {
                addElement(make_unique<CurrentSource>(name, offset_or_dc_value, n1, n2));
            } else if (waveform_type_str == "SINE") {
                addElement(make_unique<CurrentSource>(name, offset_or_dc_value, amplitude, frequency, n1, n2));
            } else {
                cout << "Unknown waveform type for CurrentSource: " << waveform_type_str << endl;
            }
        } else {
            cout << "Unknown element type: " << type_str << endl;
        }
    }

    inFile.close();
    cout << "Circuit '" << loadedCircuitName << "' loaded from '" << filename << "'." << endl;
    return true;
}

void handleSaveCircuit(const Circuit& circuit) {
    cout << "\n--- Save Circuit ---" << endl;
    cout << "Enter filename to save (e.g., my_circuit.txt): ";
    string filename;
    getline(cin, filename);
    if (filename.empty()) {
        cout << "Filename cannot be empty." << endl;
        return;
    }
    circuit.saveCircuit(filename);
}

void handleLoadCircuit(Circuit& circuit) {
    cout << "\n--- Load Circuit ---" << endl;
    cout << "Enter filename to load (e.g., my_circuit.txt): ";
    string filename;
    getline(cin, filename);
    if (filename.empty()) {
        cout << "Filename cannot be empty." << endl;
        return;
    }
    circuit.loadCircuit(filename);
}

void CircuitManager::createNewCircuit() {
    string name;
    cout << "Enter a name for the new circuit: ";
    getline(cin, name);
    if (name.empty()) {
        name = "Circuit " + to_string(circuits.size() + 1);
    }
    circuits.push_back(make_unique<Circuit>(name));
    activeCircuitIndex = circuits.size() - 1;
    cout << "New circuit '" << name << "' created and set as active." << endl;
}

void CircuitManager::displayAllCircuits() const {
    if (circuits.empty()) {
        cout << "No circuits available." << endl;
        return;
    }
    cout << "\n--- Available Circuits ---" << endl;
    for (size_t i = 0; i < circuits.size(); ++i) {
        cout << i + 1 << ". " << circuits[i]->getCircuitName();
        if (i == activeCircuitIndex) {
            cout << " (Active)";
        }
        cout << endl;
    }
    cout << "--------------------------" << endl;
}

Circuit* CircuitManager::getActiveCircuit() const {
    if (activeCircuitIndex >= 0 && activeCircuitIndex < circuits.size()) {
        return circuits[activeCircuitIndex].get();
    }
    return nullptr;
}

void CircuitManager::selectCircuit() {
    displayAllCircuits();
    if (circuits.empty()) {
        return;
    }
    int choice;
    cout << "Enter the number of the circuit to activate (or 'b' to go back): ";
    string choice_str;
    getline(cin, choice_str);
    if (choice_str == "b" || choice_str == "B") return;

    stringstream ss(choice_str);
    if (!(ss >> choice)) {
        cout << "Invalid input. Please enter a number." << endl;
        return;
    }

    if (choice > 0 && choice <= circuits.size()) {
        activeCircuitIndex = choice - 1;
        cout << "Circuit '" << circuits[activeCircuitIndex]->getCircuitName() << "' is now active." << endl;
    } else {
        cout << "Invalid circuit number." << endl;
    }
}

void CircuitManager::removeActiveCircuit() {
    if (activeCircuitIndex != -1) {
        cout << "Are you sure you want to delete circuit '" << circuits[activeCircuitIndex]->getCircuitName() << "'? (y/n): ";
        string confirm;
        getline(cin, confirm);
        if (confirm == "y" || confirm == "Y") {
            circuits.erase(circuits.begin() + activeCircuitIndex);
            cout << "Active circuit deleted." << endl;
            if (!circuits.empty()) {
                activeCircuitIndex = 0;
                cout << "First circuit '" << circuits[activeCircuitIndex]->getCircuitName() << "' is now active." << endl;
            } else {
                activeCircuitIndex = -1;
                cout << "No circuits left." << endl;
            }
        } else {
            cout << "Circuit deletion cancelled." << endl;
        }
    } else {
        cout << "No active circuit to delete." << endl;
    }
}

void handleNewCircuit(CircuitManager& manager) {
    manager.createNewCircuit();
}

void handleDisplayAndSelectCircuits(CircuitManager& manager) {
    manager.selectCircuit();
}