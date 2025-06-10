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

public:
    void addElement(unique_ptr<Component> newComponent);
    bool removeElement(const string& componentName);
    Component* findElement(const string& componentName);
    void displayCircuit() const;
    void simulateTransient(double startTime, double endTime, double timeStep);
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

void displayMenu();
int getUserChoice();
void handleAddComponent(Circuit& circuit);
void handleRemoveElement(Circuit& circuit);
void handleModifyComponent(Circuit& circuit);
void handleErrorComponentNotFound(const string& componentName) {
    cout << "Error: Component '" << componentName << "' not found in the circuit." << endl;
}

void handleErrorNodeNotFound(int node) {
    cout << "Error: Node '" << node << "' not found in the circuit." << endl;
}
void pauseSystem();
void handleTransientAnalysis(Circuit& circuit);
void handleMultipleVariablesAnalysis(Circuit& circuit);
void handleDisplayNodes(const Circuit& circuit);
void handleRenameNode(Circuit& circuit);
bool safelyReadDouble(double& val, const string& prompt);
void safelyReadDouble(double& val);
bool safelyReadInt(int& val, const string& prompt);
void safelyReadInt(int& val);

int main() {
    Circuit myCircuit;
    bool running = true;
    while (running) {
        displayMenu();
        int choice = getUserChoice();
        switch (choice) {
            case 1: handleAddComponent(myCircuit); pauseSystem(); break;
            case 2: myCircuit.displayCircuit(); pauseSystem(); break;
            case 3: handleRemoveElement(myCircuit); break;
            case 4: handleModifyComponent(myCircuit); break;
            case 5: handleTransientAnalysis(myCircuit); break;
            case 6: handleMultipleVariablesAnalysis(myCircuit); break;
            case 7: {
                bool sub_menu_running = true;
                while(sub_menu_running) {
                    cout << "\n--- DC Sweep Voltage Analysis ---" << endl;
                    double startVoltage, endVoltage, stepVoltage;
                    if (!safelyReadDouble(startVoltage, "Enter start voltage (or 'b' to Back to Main Menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(endVoltage, "Enter end voltage (or 'b' to Back to Main Menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(stepVoltage, "Enter voltage step (or 'b' to Back to Main Menu): ")) {
                        sub_menu_running = false;
                        break;
                    }

                    myCircuit.simulateDCVoltageSweep(startVoltage, endVoltage, stepVoltage);
                    pauseSystem();
                }
                break;
            }
            case 8: {
                bool sub_menu_running = true;
                while(sub_menu_running) {
                    cout << "\n--- DC Sweep Current Analysis ---" << endl;
                    double startCurrent, endCurrent, stepCurrent;
                    if (!safelyReadDouble(startCurrent, "Enter start current (or 'b' to Back to Main Menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(endCurrent, "Enter end current (or 'b' to Back to Main Menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    if (!safelyReadDouble(stepCurrent, "Enter current step (or 'b' to Back to Main Menu): ")) {
                        sub_menu_running = false;
                        break;
                    }
                    myCircuit.simulateDCCurrentSweep(startCurrent, endCurrent, stepCurrent);
                    pauseSystem();
                }
                break;
            }
            case 9: handleDisplayNodes(myCircuit); break;
            case 10: handleRenameNode(myCircuit); break;
            case 11: running = false; cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl; pauseSystem(); break;
        }
    }
    return 0;
}

void handleDisplayNodes(const Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        cout << "\n--- Display Existing Nodes ---" << endl;
        circuit.displayNodes();
        cout << "\nPress 'b' to Back to Main Menu" << endl;
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

        if (!safelyReadInt(oldNum, "Enter the node number you want to rename (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadInt(newNum, "Enter the new node number (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }

        circuit.renameNode(oldNum, newNum);
        pauseSystem();
    }
}

void Circuit::addElement(unique_ptr<Component> newComponent) { components.push_back(move(newComponent)); }
void Circuit::displayCircuit() const {
    if (components.empty()) {
        cout << "Circuit is empty." << endl;
        return;
    }

    cout << "-------------------- Circuit Status --------------------" << endl;
    if (this->hasGround()) {
        cout << "Ground Status: Connected." << endl;
    } else {
        cout << ">> WARNING: No ground node (0) found in the circuit! <<" << endl;
    }
    cout << "--------------------------------------------------------" << endl;

    for (const auto& comp : components) {
        comp->display();
    }
    cout << "--------------------------------------------------------" << endl;
}

bool Circuit::setupAndSolveMNA(double time, double timeStep) {
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
        cout << "Error: Circuit matrix is singular. Cannot solve." << endl;
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
        } else if (auto ind = dynamic_cast<Inductor*>(comp.get())) {
            if (timeStep > 0 && ind->getInductance() > 0) {
                double i_prev = previousComponentCurrents.count(ind->getName()) ? previousComponentCurrents.at(ind->getName()) : 0.0;
                double v1_now = getNodeVoltage(ind->getNode1());
                double v2_now = getNodeVoltage(ind->getNode2());
                componentCurrents[ind->getName()] = i_prev + (timeStep / ind->getInductance()) * (v1_now - v2_now);
            } else {
                double v1 = getNodeVoltage(ind->getNode1());
                double v2 = getNodeVoltage(ind->getNode2());
                componentCurrents[ind->getName()] = (v1-v2) * 1e9;
            }
        } else if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
            componentCurrents[cs->getName()] = cs->getValueAtTime(time);
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

void Circuit::simulateTransient(double startTime, double endTime, double timeStep) {
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

    set<int> all_nodes = getAllNodes();
    int targetNode;
    if (!safelyReadInt(targetNode, "Enter the node number to monitor (or 'b' to back): ")){
        return;
    }

    if (all_nodes.find(targetNode) == all_nodes.end() && targetNode != 0) {
        handleErrorNodeNotFound(targetNode);
        return;
    }

    nodeVoltages.clear();
    componentCurrents.clear();
    previousNodeVoltages.clear();
    previousComponentCurrents.clear();

    for(int node : all_nodes) {
        previousNodeVoltages[node] = 0.0;
    }
    for(const auto& comp: components) {
        previousComponentCurrents[comp->getName()] = 0.0;
    }


    for (double time = startTime; time <= endTime + timeStep / 2; time += timeStep) {
        cout << "Time: " << scientific << setprecision(4) << time << "s" << endl;
        if (!setupAndSolveMNA(time, timeStep)) {
            cout << "Circuit analysis failed at this time step." << endl;
            break;
        }

        cout << "Node " << targetNode << " Voltage: " << scientific << setprecision(4) << getNodeVoltage(targetNode) << " V" << endl;
        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << comp->getName() << " (" << comp->getType() << ") Current: "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        cout << "-----------------" << endl;
        previousNodeVoltages = nodeVoltages;
        previousComponentCurrents = componentCurrents;
    }
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


    cout << "\n--- Displaying Voltages and Currents at Different Times ---" << endl;
    for (double time = startTime; time <= endTime + timeStep/2; time += timeStep) {
        cout << "Time: " << scientific << setprecision(4) << time << "s" << endl;
        if (!setupAndSolveMNA(time, timeStep)) {
            cout << "Circuit analysis failed at this time step." << endl;
            break;
        }

        for (auto const& [node, voltage] : nodeVoltages) {
            cout << "Node " << node << " Voltage: " << scientific << setprecision(4) << voltage << " V" << endl;
        }

        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << comp->getName() << " (" << comp->getType() << ") Current: "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        cout << "-----------------" << endl;
        previousNodeVoltages = nodeVoltages;
        previousComponentCurrents = componentCurrents;
    }
}

void Circuit::simulateDCVoltageSweep(double startVoltage, double endVoltage, double stepVoltage) {
    if (stepVoltage == 0) {
        cout << "Error: Voltage step cannot be zero for a sweep." << endl;
        return;
    }
    if ((startVoltage < endVoltage && stepVoltage < 0) || (startVoltage > endVoltage && stepVoltage > 0)) {
        cout << "Error: Step direction is incorrect for the given start and end voltages." << endl;
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
        cout << "No DC Voltage Source found in the circuit to perform a DC sweep." << endl;
        return;
    }

    double originalVoltage = sweepSource->getValueAtTime(0);
    double currentVoltage = startVoltage;
    while ((stepVoltage > 0 && currentVoltage <= endVoltage + stepVoltage/2) || (stepVoltage < 0 && currentVoltage >= endVoltage + stepVoltage/2)) {
        sweepSource->setDCOffsetValue(currentVoltage);
        cout << "Voltage Sweep: " << scientific << setprecision(4) << currentVoltage << "V" << endl;
        if (!setupAndSolveMNA(0, 0.0)) {
            cout << "Circuit analysis failed for this voltage step." << endl;
            break;
        }

        for (auto const& [node, voltage] : nodeVoltages) {
            cout << "Node " << node << " Voltage: " << scientific << setprecision(4) << voltage << " V" << endl;
        }

        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << comp->getName() << " (" << comp->getType() << ") Current: "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        currentVoltage += stepVoltage;
        cout << "-----------------" << endl;
    }
    sweepSource->setDCOffsetValue(originalVoltage);
}

void Circuit::simulateDCCurrentSweep(double startCurrent, double endCurrent, double stepCurrent) {
    if (stepCurrent == 0) {
        cout << "Error: Current step cannot be zero for a sweep." << endl;
        return;
    }
    if ((startCurrent < endCurrent && stepCurrent < 0) || (startCurrent > endCurrent && stepCurrent > 0)) {
        cout << "Error: Step direction is incorrect for the given start and end currents." << endl;
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
        cout << "No DC Current Source found in the circuit to perform a DC sweep." << endl;
        return;
    }

    double originalCurrent = sweepSource->getValueAtTime(0);
    double currentSweepValue = startCurrent;
    while ((stepCurrent > 0 && currentSweepValue <= endCurrent + stepCurrent / 2) || (stepCurrent < 0 && currentSweepValue >= endCurrent + stepCurrent / 2)) {
        sweepSource->setDCOffsetValue(currentSweepValue);
        cout << "Current Sweep: " << scientific << setprecision(4) << currentSweepValue << " A" << endl;
        if (!setupAndSolveMNA(0, 0.0)) {
            cout << "Circuit analysis failed for this current step." << endl;
            break;
        }

        for (auto const& [node, voltage] : nodeVoltages) {
            cout << "Node " << node << " Voltage: " << scientific << setprecision(4) << voltage << " V" << endl;
        }

        for (const auto& comp : components) {
            double current = getComponentCurrent(comp->getName());
            cout << comp->getName() << " (" << comp->getType() << ") Current: "
                 << scientific << setprecision(4) << current << " A" << endl;
        }
        currentSweepValue += stepCurrent;
        cout << "-----------------" << endl;
    }
    sweepSource->setDCOffsetValue(originalCurrent);
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
        cout << "Error: Node " << newNodeNum << " already exists. Cannot merge nodes." << endl;
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

    cout << "Success: Node " << oldNodeNum << " has been renamed to " << newNodeNum << " throughout the circuit." << endl;
    return true;
}
void Circuit::displayNodes() const {
    if (components.empty()) {
        cout << "Circuit is empty, no nodes to display." << endl;
        return;
    }

    auto unique_nodes = getAllNodes();
    cout << "--- Existing Nodes in Circuit ---" << endl;
    if (unique_nodes.empty()) {
        cout << "No nodes found." << endl;
    } else {
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
        if (comp->getName() == componentName) { return comp.get(); }
    }
    return nullptr;
}

void handleTransientAnalysis(Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        double startTime, endTime, timeStep;
        cout << "\n--- Transient Analysis ---" << endl;

        if (!safelyReadDouble(startTime, "Enter start time (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadDouble(endTime, "Enter end time (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadDouble(timeStep, "Enter time step (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }

        circuit.simulateTransient(startTime, endTime, timeStep);
        pauseSystem();
        sub_menu_running = false;
    }
}

void handleMultipleVariablesAnalysis(Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        double startTime, endTime, timeStep;
        cout << "\n--- Multiple Variables Analysis ---" << endl;

        if (!safelyReadDouble(startTime, "Enter start time (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadDouble(endTime, "Enter end time (or 'b' to Back to Main Menu): ")) {
            sub_menu_running = false;
            break;
        }
        if (!safelyReadDouble(timeStep, "Enter time step (or 'b' to Back to Main Menu): ")) {
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
    cout << "\n--- Add New Component ---" << endl;
    cout << "Select component type:" << endl;
    cout << "1. Resistor" << endl;
    cout << "2. Capacitor" << endl;
    cout << "3. Inductor" << endl;
    cout << "4. Voltage Source" << endl;
    cout << "5. Current Source" << endl;
    cout << "0. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
    safelyReadInt(type_choice);

    if (type_choice == 0) {
        return;
    }

    ComponentType type_enum = intToComponentType(type_choice);
    if (type_enum == ComponentType::UNKNOWN) {
        cout << "Error: Invalid component type choice. Please try again." << endl;
        return;
    }

    string line;
    int n1, n2;
    unique_ptr<Component> newComponent = nullptr;

    switch (type_enum) {
        case ComponentType::RESISTOR:
        case ComponentType::CAPACITOR:
        case ComponentType::INDUCTOR: {
            cout << "Enter details in format: <name> <node1> <node2> <value> (e.g., R1 1 0 1k)" << endl;
            getline(cin, line);
            stringstream ss(line);
            string value_str;
            ss >> name >> n1 >> n2 >> value_str;
            if (circuit.findElement(name) != nullptr) {
                cout << "Error: A component with name '" << name << "' already exists." << endl;
                return;
            }
            double value = parseEngineeringValue(value_str);
            if (type_enum == ComponentType::RESISTOR)
                newComponent = make_unique<Resistor>(name, value, n1, n2);
            else if (type_enum == ComponentType::CAPACITOR)
                newComponent = make_unique<Capacitor>(name, value, n1, n2);
            else if (type_enum == ComponentType::INDUCTOR)
                newComponent = make_unique<Inductor>(name, value, n1, n2);
            break;
        }
        case ComponentType::VOLTAGE_SOURCE: {
            string waveform_str;
            cout << "Enter waveform type for Voltage Source (DC or SINE): ";
            getline(cin, waveform_str);
            transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);
            if (waveform_str == "DC") {
                cout << "Enter details in format: <name> <node1> <node2> <value> (e.g., V1 1 0 9V)" << endl;
                getline(cin, line);
                stringstream ss(line);
                string value_str;
                ss >> name >> n1 >> n2 >> value_str;
                if (circuit.findElement(name) != nullptr) {
                    cout << "Error: A component with name '" << name << "' already exists." << endl; return;}
                newComponent = make_unique<VoltageSource>(name, parseEngineeringValue(value_str), n1, n2);
            } else if (waveform_str == "SINE") {
                cout << "Enter details in format: <name> <node1> <node2> <offset> <amplitude> <frequency>" << endl;
                getline(cin, line);
                stringstream ss(line);
                double offset, amp, freq;
                ss >> name >> n1 >> n2 >> offset >> amp >> freq;
                if (circuit.findElement(name) != nullptr) {
                    cout << "Error: A component with name '" << name << "' already exists." << endl; return;}
                newComponent = make_unique<VoltageSource>(name, offset, amp, freq, n1, n2);
            } else {
                cout << "Error: Invalid waveform type for Voltage Source. Not added." << endl;
            }
            break;
        }
        case ComponentType::CURRENT_SOURCE: {
            string waveform_str;
            cout << "Enter waveform type for Current Source (DC or SINE): ";
            getline(cin, waveform_str);
            transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);
            if (waveform_str == "DC") {
                cout << "Enter details in format: <name> <node1> <node2> <value> (e.g., I1 1 0 500m)" << endl;
                getline(cin, line);
                stringstream ss(line);
                string value_str;
                ss >> name >> n1 >> n2 >> value_str;
                if (circuit.findElement(name) != nullptr) {
                    cout << "Error: A component with name '" << name << "' already exists." << endl; return;}
                newComponent = make_unique<CurrentSource>(name, parseEngineeringValue(value_str), n1, n2);
            } else if (waveform_str == "SINE") {
                cout << "Enter details in format: <name> <node1> <node2> <offset> <amplitude> <frequency>" << endl;
                getline(cin, line);
                stringstream ss(line);
                double offset, amp, freq;
                ss >> name >> n1 >> n2 >> offset >> amp >> freq;
                if (circuit.findElement(name) != nullptr) {
                    cout << "Error: A component with name '" << name << "' already exists." << endl; return;}
                newComponent = make_unique<CurrentSource>(name, offset, amp, freq, n1, n2);
            } else {
                cout << "Error: Invalid waveform type for Current Source. Not added." << endl;
            }
            break;
        }
        case ComponentType::UNKNOWN:
            break;
    }

    if (newComponent != nullptr) {
        cout << "Info: Component '" << name << "' added successfully." << endl;
        circuit.addElement(move(newComponent));
    } else {
        cout << "Error: Component not added due to invalid input or type." << endl;
    }
}

void handleRemoveElement(Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        string name;
        cout << "\n--- Remove Component ---" << endl;
        circuit.displayCircuit();
        cout << "Enter the name of the component to remove (or type 'b' to Back to Main Menu): ";
        getline(cin, name);

        if (name == "b" || name == "B") {
            sub_menu_running = false;
            break;
        }

        if (circuit.removeElement(name)) {
            cout << "Success: Component '" << name << "' has been removed." << endl;
        } else {
            handleErrorComponentNotFound(name);
        }
        pauseSystem();
    }
}

void handleModifyComponent(Circuit& circuit) {
    bool sub_menu_running = true;
    while (sub_menu_running) {
        string name;
        cout << "\n--- Modify Component ---" << endl;
        circuit.displayCircuit();
        cout << "Enter the name of the component to modify (or type 'b' to Back to Main Menu): ";
        getline(cin, name);

        if (name == "b" || name == "B") {
            sub_menu_running = false;
            break;
        }

        Component* comp = circuit.findElement(name);
        if (comp == nullptr) {
            handleErrorComponentNotFound(name);
            pauseSystem();
            continue;
        }

        cout << "Component found. What do you want to modify?" << endl;
        int choice;

        if (Resistor* res = dynamic_cast<Resistor*>(comp)) {
            cout << "1. Resistance Value\n2. Nodes\n0. Back to Component Selection" << endl;
            cout << "Enter your choice: ";
            safelyReadInt(choice);
            if (choice == 0) {
                pauseSystem();
                continue;
            }
            if (choice == 1) {
                string value_str;
                cout << "Enter new resistance: ";
                getline(cin, value_str);
                res->setResistance(parseEngineeringValue(value_str));
                cout << "Success: Resistance updated." << endl;
            } else if (choice == 2) {
                int n1, n2;
                cout << "Enter new node 1: "; safelyReadInt(n1);
                cout << "Enter new node 2: "; safelyReadInt(n2);
                res->setNodes(n1, n2);
                cout << "Success: Nodes updated." << endl;
            } else {
                cout << "Invalid choice." << endl;
            }
        } else if (Capacitor* cap = dynamic_cast<Capacitor*>(comp)) {
            cout << "1. Capacitance Value\n2. Nodes\n0. Back to Component Selection" << endl;
            cout << "Enter your choice: ";
            safelyReadInt(choice);
            if (choice == 0) {
                pauseSystem();
                continue;
            }
            if (choice == 1) {
                string value_str;
                cout << "Enter new capacitance: ";
                getline(cin, value_str);
                cap->setCapacitance(parseEngineeringValue(value_str));
                cout << "Success: Capacitance updated." << endl;
            } else if (choice == 2) {
                int n1, n2;
                cout << "Enter new node 1: "; safelyReadInt(n1);
                cout << "Enter new node 2: "; safelyReadInt(n2);
                cap->setNodes(n1, n2);
                cout << "Success: Nodes updated." << endl;
            } else {
                cout << "Invalid choice." << endl;
            }
        } else if (Inductor* ind = dynamic_cast<Inductor*>(comp)) {
            cout << "1. Inductance Value\n2. Nodes\n0. Back to Component Selection" << endl;
            cout << "Enter your choice: ";
            safelyReadInt(choice);
            if (choice == 0) {
                pauseSystem();
                continue;
            }
            if (choice == 1) {
                string value_str;
                cout << "Enter new inductance: ";
                getline(cin, value_str);
                ind->setInductance(parseEngineeringValue(value_str));
                cout << "Success: Inductance updated." << endl;
            } else if (choice == 2) {
                int n1, n2;
                cout << "Enter new node 1: "; safelyReadInt(n1);
                cout << "Enter new node 2: "; safelyReadInt(n2);
                ind->setNodes(n1, n2);
                cout << "Success: Nodes updated." << endl;
            } else {
                cout << "Invalid choice." << endl;
            }
        } else if (CurrentSource* is = dynamic_cast<CurrentSource*>(comp)) {
            if (is->getWaveformType() == CurrentSource::Waveform::DC) {
                cout << "1. DC Value\n2. Nodes\n0. Back to Component Selection" << endl;
                cout << "Enter your choice: ";
                safelyReadInt(choice);
                if (choice == 0) {
                    pauseSystem();
                    continue;
                }
                if (choice == 1) {
                    string value_str;
                    cout << "Enter new DC value: ";
                    getline(cin, value_str);
                    is->setDCOffsetValue(parseEngineeringValue(value_str));
                    cout << "Success: DC value updated." << endl;
                } else if (choice == 2) {
                    int n1, n2;
                    cout << "Enter new nodes: "; safelyReadInt(n1); safelyReadInt(n2);
                    is->setNodes(n1, n2);
                    cout << "Success: Nodes updated." << endl;
                } else { cout << "Invalid choice." << endl; }
            } else {
                cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes\n0. Back to Component Selection" << endl;
                cout << "Enter your choice: ";
                safelyReadInt(choice);
                if (choice == 0) {
                    pauseSystem();
                    continue;
                }
                switch(choice) {
                    case 1: { double v; cout << "Enter new offset: "; safelyReadDouble(v); is->setDCOffsetValue(v); cout << "Success.\n"; break; }
                    case 2: { double v; cout << "Enter new amplitude: "; safelyReadDouble(v); is->setAmplitude(v); cout << "Success.\n"; break; }
                    case 3: { double v; cout << "Enter new frequency: "; safelyReadDouble(v); is->setFrequency(v); cout << "Success.\n"; break; }
                    case 4: { int n1, n2; cout << "Enter new nodes: "; safelyReadInt(n1); safelyReadInt(n2); is->setNodes(n1, n2); cout << "Success.\n"; break; }
                    default: { cout << "Invalid choice.\n"; break; }
                }
            }
        } else if (VoltageSource* vs = dynamic_cast<VoltageSource*>(comp)) {
            if (vs->getWaveformType() == VoltageSource::Waveform::DC) {
                cout << "1. DC Value\n2. Nodes\n0. Back to Component Selection" << endl;
                cout << "Enter your choice: ";
                safelyReadInt(choice);
                if (choice == 0) {
                    pauseSystem();
                    continue;
                }
                if (choice == 1) {
                    string value_str;
                    cout << "Enter new DC value: ";
                    getline(cin, value_str);
                    vs->setDCOffsetValue(parseEngineeringValue(value_str));
                    cout << "Success: DC value updated." << endl;
                } else if (choice == 2) {
                    int n1, n2;
                    cout << "Enter new nodes: "; safelyReadInt(n1); safelyReadInt(n2);
                    vs->setNodes(n1, n2);
                    cout << "Success: Nodes updated." << endl;
                } else { cout << "Invalid choice." << endl; }
            } else {
                cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes\n0. Back to Component Selection" << endl;
                cout << "Enter your choice: ";
                safelyReadInt(choice);
                if (choice == 0) {
                    pauseSystem();
                    continue;
                }
                switch(choice) {
                    case 1: { double v; cout << "Enter new offset: "; safelyReadDouble(v); vs->setDCOffsetValue(v); cout << "Success.\n"; break; }
                    case 2: { double v; cout << "Enter new amplitude: "; safelyReadDouble(v); vs->setAmplitude(v); cout << "Success.\n"; break; }
                    case 3: { double v; cout << "Enter new frequency: "; safelyReadDouble(v); vs->setFrequency(v); cout << "Success.\n"; break; }
                    case 4: { int n1, n2; cout << "Enter new nodes: "; safelyReadInt(n1); safelyReadInt(n2); vs->setNodes(n1, n2); cout << "Success.\n"; break; }
                    default: { cout << "Invalid choice.\n"; break; }
                }
            }
        }
        pauseSystem();
    }
}

void displayMenu() {
    cout << "\n\n========== Circuit Editor Menu ==========" << endl;
    cout << "1. Add Component" << endl;
    cout << "2. Display Circuit" << endl;
    cout << "3. Remove Component" << endl;
    cout << "4. Modify Component" << endl;
    cout << "5. Run Transient Analysis" << endl;
    cout << "6. Run Multiple Variables Analysis" << endl;
    cout << "7. Run DC Voltage Sweep Analysis" << endl;
    cout << "8. Run DC Current Sweep Analysis" << endl;
    cout << "9. Display Existing Nodes" << endl;
    cout << "10. Rename Node" << endl;
    cout << "11. Exit" << endl;
    cout << "=======================================" << endl;
    cout << "Enter your choice: ";
}

int getUserChoice() {
    int choice;
    string line;
    getline(cin, line);
    stringstream ss(line);
    if (ss >> choice && (ss >> ws).eof()) {
        return choice;
    }
    return -1;
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