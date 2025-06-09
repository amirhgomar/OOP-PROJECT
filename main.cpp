#include <bits/stdc++.h>
#include <memory>
#include <cstddef>
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
public:
    void addElement(unique_ptr<Component> newComponent);
    bool removeElement(const string& componentName);
    Component* findElement(const string& componentName);
    void displayCircuit() const;
    void simulateTransient(double startTime, double endTime, double timeStep);
    double calculateVoltageDrop(Resistor* res, double time, int targetNode) ;
    double calculateVoltage(Capacitor* cap, double time, int targetNode) ;
    double calculateVoltage(Inductor* ind, double time, int targetNode) ;
    void simulateMultipleVariables(double startTime, double endTime, double timeStep);
    void simulateDCVoltageSweep(double startVoltage, double endVoltage, double stepVoltage);
    bool hasGround() const;
    void displayNodes() const;
    bool renameNode(int oldNodeNum, int newNodeNum);
    const vector<unique_ptr<Component>>& getComponents() const { return components; }
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

ComponentType stringToComponentType(const string& str) {
    string lower_str = str;
    transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    if (lower_str == "resistor") return ComponentType::RESISTOR;
    if (lower_str == "capacitor") return ComponentType::CAPACITOR;
    if (lower_str == "inductor") return ComponentType::INDUCTOR;
    if (lower_str == "voltage source" || lower_str == "vsource") return ComponentType::VOLTAGE_SOURCE;
    if (lower_str == "current source" || lower_str == "isource") return ComponentType::CURRENT_SOURCE;
    return ComponentType::UNKNOWN;
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
void safelyReadDouble(double& val);
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
            case 3: handleRemoveElement(myCircuit); pauseSystem(); break;
            case 4: handleModifyComponent(myCircuit); pauseSystem(); break;
            case 5: handleTransientAnalysis(myCircuit); pauseSystem(); break;
            case 6: handleMultipleVariablesAnalysis(myCircuit); pauseSystem(); break;
            case 7: {
                double startVoltage, endVoltage, stepVoltage;
                cout << "--- DC Sweep Voltage Analysis ---" << endl;
                cout << "Enter start voltage: ";
                safelyReadDouble(startVoltage);
                cout << "Enter end voltage: ";
                safelyReadDouble(endVoltage);
                cout << "Enter voltage step: ";
                safelyReadDouble(stepVoltage);
                myCircuit.simulateDCVoltageSweep(startVoltage, endVoltage, stepVoltage);
                pauseSystem();
                break;
            }
            case 8: handleDisplayNodes(myCircuit); pauseSystem(); break;
            case 9: handleRenameNode(myCircuit); pauseSystem(); break;
            case 10: running = false; cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl; pauseSystem(); break;
        }
    }
    return 0;
}

void handleDisplayNodes(const Circuit& circuit) {
    circuit.displayNodes();
}

void handleRenameNode(Circuit& circuit) {
    int oldNum, newNum;
    cout << "--- Rename Node ---" << endl;
    cout << "Enter the node number you want to rename: ";
    safelyReadInt(oldNum);
    cout << "Enter the new node number: ";
    safelyReadInt(newNum);
    circuit.renameNode(oldNum, newNum);
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
void Circuit::simulateTransient(double startTime, double endTime, double timeStep) {
    if (timeStep <= 0) {
        cout << "Error: Time step must be a positive value." << endl;
        return;
    }
    if (startTime > endTime) {
        cout << "Error: Start time cannot be greater than end time." << endl;
        return;
    }

    int targetNode;
    cout << "Enter the node number to monitor: ";
    cin >> targetNode;

    for (double time = startTime; time <= endTime + timeStep / 2; time += timeStep) {
        cout << "Time: " << scientific << setprecision(4) << time << "s" << endl;

        bool sourcesFound = false;

        for (const auto& comp : components) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                cout << "Voltage Source " << vs->getName() << " Voltage: "
                     << scientific << setprecision(4) << vs->getValueAtTime(time) << " V" << endl;
                sourcesFound = true;
            } else if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                cout << "Current Source " << cs->getName() << " Current: "
                     << scientific << setprecision(4) << cs->getValueAtTime(time) << " A" << endl;
                sourcesFound = true;
            }
        }

        for (const auto& comp : components) {
            if (comp->getNode1() == targetNode || comp->getNode2() == targetNode) {
                if (auto res = dynamic_cast<Resistor*>(comp.get())) {
                    double voltageDrop = calculateVoltageDrop(res, time, targetNode);
                    double current = voltageDrop / res->getResistance();
                    cout << "At Node " << targetNode << " (Resistor " << res->getName() << "): "
                         << "Voltage: " << scientific << setprecision(4) << voltageDrop << " V, "
                         << "Current: " << scientific << setprecision(4) << current << " A" << endl;
                } else if (auto cap = dynamic_cast<Capacitor*>(comp.get())) {
                    double voltage = calculateVoltage(cap, time, targetNode);
                    cout << "At Node " << targetNode << " (Capacitor " << cap->getName() << "): "
                         << "Voltage: " << scientific << setprecision(4) << voltage << " V" << endl;
                } else if (auto ind = dynamic_cast<Inductor*>(comp.get())) {
                    double voltage = calculateVoltage(ind, time, targetNode);
                    cout << "At Node " << targetNode << " (Inductor " << ind->getName() << "): "
                         << "Voltage: " << scientific << setprecision(4) << voltage << " V" << endl;
                }
            }
        }

        if (!sourcesFound) {
            cout << "No active sources found to simulate." << endl;
        }
        cout << "-----------------" << endl;
    }
}

double Circuit::calculateVoltageDrop(Resistor* res, double time, int targetNode) {
    return res->getResistance();
}

double Circuit::calculateVoltage(Capacitor* cap, double time, int targetNode) {
    return cap->getCapacitance();
}

double Circuit::calculateVoltage(Inductor* ind, double time, int targetNode) {
    return ind->getInductance();
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
    cout << "\n--- Displaying Voltage and Current at Different Times ---" << endl;
    for (double time = startTime; time <= endTime + timeStep/2; time += timeStep) {
        cout << "Time: " << scientific << setprecision(4) << time << "s" << endl;
        bool sourcesFound = false;
        for (const auto& comp : components) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                cout << "Voltage Source " << vs->getName() << " Voltage: "
                     << scientific << setprecision(4) << vs->getValueAtTime(time) << " V" << endl;
                sourcesFound = true;
            }
            if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                cout << "Current Source " << cs->getName() << " Current: "
                     << scientific << setprecision(4) << cs->getValueAtTime(time) << " A" << endl;
                sourcesFound = true;
            }
        }
        if (!sourcesFound) {
            cout << "No active sources found to simulate." << endl;
        }
        cout << "-----------------" << endl;
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

    bool voltageSourceFound = false;
    for (const auto& comp : components) {
        if (dynamic_cast<VoltageSource*>(comp.get())) {
            voltageSourceFound = true;
            break;
        }
    }
    if (!voltageSourceFound) {
        cout << "No voltage sources found in the circuit to perform a DC sweep." << endl;
        return;
    }

    double currentVoltage = startVoltage;
    while ((stepVoltage > 0 && currentVoltage <= endVoltage + stepVoltage/2) || (stepVoltage < 0 && currentVoltage >= endVoltage + stepVoltage/2)) {
        cout << "Voltage Sweep: " << scientific << setprecision(4) << currentVoltage << "V" << endl;
        for (const auto& comp : components) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                vs->setDCOffsetValue(currentVoltage);
                cout << "Voltage Source " << vs->getName() << " Voltage: "
                     << scientific << setprecision(4) << vs->getValueAtTime(0) << " V" << endl;
            }
        }
        for (const auto& comp : components) {
            if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                cout << "Current Source " << cs->getName() << " Current: "
                     << scientific << setprecision(4) << cs->getValueAtTime(0) << " A" << endl;
            }
        }
        currentVoltage += stepVoltage;
        cout << "-----------------" << endl;
    }
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
    double startTime, endTime, timeStep;
    cout << "--- Transient Analysis ---" << endl;
    cout << "Enter start time: ";
    safelyReadDouble(startTime);
    cout << "Enter end time: ";
    safelyReadDouble(endTime);
    cout << "Enter time step: ";
    safelyReadDouble(timeStep);
    circuit.simulateTransient(startTime, endTime, timeStep);
}

void handleMultipleVariablesAnalysis(Circuit& circuit) {
    double startTime, endTime, timeStep;
    cout << "--- Multiple Variables Analysis ---" << endl;
    cout << "Enter start time: ";
    safelyReadDouble(startTime);
    cout << "Enter end time: ";
    safelyReadDouble(endTime);
    cout << "Enter time step: ";
    safelyReadDouble(timeStep);

    circuit.simulateMultipleVariables(startTime, endTime, timeStep);
}

void handleAddComponent(Circuit& circuit) {
    string name, type_str;

    cout << "--- Add New Component ---" << endl;
    cout << "Enter name (e.g., R1): ";
    getline(cin, name);
    if (circuit.findElement(name) != nullptr) {
        cout << "Error: A component with name '" << name << "' already exists." << endl;
        return;
    }

    ComponentType type_enum;
    while (true) {
        cout << "Enter type (Resistor, Capacitor, Inductor, Voltage Source, Current Source): ";
        getline(cin, type_str);
        type_enum = stringToComponentType(type_str);
        if (type_enum != ComponentType::UNKNOWN) break;
        cout << "Error: Invalid component type. Please try again." << endl;
    }

    int n1, n2;
    cout << "Enter node 1: ";
    safelyReadInt(n1);
    cout << "Enter node 2: ";
    safelyReadInt(n2);

    unique_ptr<Component> newComponent = nullptr;

    switch (type_enum) {
        case ComponentType::VOLTAGE_SOURCE: {
            string waveform_str;
            cout << "Enter waveform type for Voltage Source (DC or SINE): ";
            getline(cin, waveform_str);
            transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);

            if (waveform_str == "DC") {
                string value_str;
                cout << "Enter DC value (e.g., 9V): ";
                getline(cin, value_str);
                newComponent = make_unique<VoltageSource>(name, parseEngineeringValue(value_str), n1, n2);
            } else if (waveform_str == "SINE") {
                double offset, amp, freq;
                cout << "Enter DC Offset value: "; safelyReadDouble(offset);
                cout << "Enter Amplitude value: "; safelyReadDouble(amp);
                cout << "Enter Frequency (Hz) value: "; safelyReadDouble(freq);
                newComponent = make_unique<VoltageSource>(name, offset, amp, freq, n1, n2);
            } else {
                cout << "Error: Invalid waveform type for Voltage Source." << endl;
            }
            break;
        }
        case ComponentType::CURRENT_SOURCE: {
            string waveform_str;
            cout << "Enter waveform type for Current Source (DC or SINE): ";
            getline(cin, waveform_str);
            transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);

            if (waveform_str == "DC") {
                string value_str;
                cout << "Enter DC value (e.g., 1A, 500m): ";
                getline(cin, value_str);
                newComponent = make_unique<CurrentSource>(name, parseEngineeringValue(value_str), n1, n2);
            } else if (waveform_str == "SINE") {
                double offset, amp, freq;
                cout << "Enter DC Offset value: "; safelyReadDouble(offset);
                cout << "Enter Amplitude value: "; safelyReadDouble(amp);
                cout << "Enter Frequency (Hz) value: "; safelyReadDouble(freq);
                newComponent = make_unique<CurrentSource>(name, offset, amp, freq, n1, n2);
            } else {
                cout << "Error: Invalid waveform type for Current Source." << endl;
            }
            break;
        }
        case ComponentType::RESISTOR:
        case ComponentType::CAPACITOR:
        case ComponentType::INDUCTOR: {
            string value_str;
            cout << "Enter value (e.g., 1k, 10u): ";
            getline(cin, value_str);

            double value = parseEngineeringValue(value_str);

            if (type_enum == ComponentType::RESISTOR)
                newComponent = make_unique<Resistor>(name, value, n1, n2);
            else if (type_enum == ComponentType::CAPACITOR)
                newComponent = make_unique<Capacitor>(name, value, n1, n2);
            else if (type_enum == ComponentType::INDUCTOR)
                newComponent = make_unique<Inductor>(name, value, n1, n2);
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
    string name;
    cout << "--- Remove Component ---" << endl;
    cout << "Enter the name of the component to remove: ";
    getline(cin, name);
    if (circuit.removeElement(name)) {
        cout << "Success: Component '" << name << "' has been removed." << endl;
    } else {
        handleErrorComponentNotFound(name);
    }
}

void handleModifyComponent(Circuit& circuit) {
    string name;
    cout << "--- Modify Component ---" << endl;
    cout << "Enter the name of the component to modify: ";
    getline(cin, name);

    Component* comp = circuit.findElement(name);
    if (comp == nullptr) {
        handleErrorComponentNotFound(name);
        return;
    }

    cout << "Component found. What do you want to modify?" << endl;

    if (Resistor* res = dynamic_cast<Resistor*>(comp)) {
        cout << "1. Resistance Value\n2. Nodes" << endl;
        int choice = getUserChoice();
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
        cout << "1. Capacitance Value\n2. Nodes" << endl;
        int choice = getUserChoice();
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
        cout << "1. Inductance Value\n2. Nodes" << endl;
        int choice = getUserChoice();
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
            cout << "1. DC Value\n2. Nodes" << endl;
            int choice = getUserChoice();
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
            cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes" << endl;
            int choice = getUserChoice();
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
            cout << "1. DC Value\n2. Nodes" << endl;
            int choice = getUserChoice();
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
            cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes" << endl;
            int choice = getUserChoice();
            switch(choice) {
                case 1: { double v; cout << "Enter new offset: "; safelyReadDouble(v); vs->setDCOffsetValue(v); cout << "Success.\n"; break; }
                case 2: { double v; cout << "Enter new amplitude: "; safelyReadDouble(v); vs->setAmplitude(v); cout << "Success.\n"; break; }
                case 3: { double v; cout << "Enter new frequency: "; safelyReadDouble(v); vs->setFrequency(v); cout << "Success.\n"; break; }
                case 4: { int n1, n2; cout << "Enter new nodes: "; safelyReadInt(n1); safelyReadInt(n2); vs->setNodes(n1, n2); cout << "Success.\n"; break; }
                default: { cout << "Invalid choice.\n"; break; }
            }
        }
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
    cout << "8. Display Existing Nodes"<< endl;
    cout << "9. Rename Node"<< endl;
    cout << "10. Exit" << endl;
    cout << "=======================================" << endl;
    cout << "Enter your choice: ";
}

int getUserChoice() {
    int choice;
    cin >> choice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void pauseSystem() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

void safelyReadDouble(double& val) {
    while (!(cin >> val)) {
        cout << "Invalid input. Please enter a numeric value: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void safelyReadInt(int& val) {
    while (!(cin >> val)) {
        cout << "Invalid input. Please enter an integer value: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}