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
    VoltageSource(const string& name, double dc_val, const string& unit, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::DC), offset_or_dc_value(dc_val),
              amplitude(0), frequency(0), unit("V") {}

    VoltageSource(const string& name, double offset, double amp, double freq, const string& unit, int n1, int n2)
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
            cout << " (SINE) | Params: Offset=" << offset_or_dc_value << ", Amp=" << amplitude << ", Freq=" << frequency << "Hz";
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
    CurrentSource(const string& name, double dc_val, const string& unit, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::DC), offset_or_dc_value(dc_val),
              amplitude(0), frequency(0), unit("A") {}

    CurrentSource(const string& name, double offset, double amp, double freq, const string& unit, int n1, int n2)
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
            cout << " (SINE) | Params: Offset=" << offset_or_dc_value << ", Amp=" << amplitude << ", Freq=" << frequency << "Hz";
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
public:
    void addElement(unique_ptr<Component> newComponent);
    bool removeElement(const string& componentName);
    Component* findElement(const string& componentName);
    void displayCircuit() const;
    void simulateTransient(double startTime, double endTime, double timeStep);
    void simulateMultipleVariables(double startTime, double endTime, double timeStep);
    void simulateDCVoltageSweep(double startVoltage, double endVoltage, double stepVoltage);
    bool hasGround() const;
    const vector<unique_ptr<Component>>& getComponents() const { return components; }
};

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
void handleTransientAnalysis(Circuit& circuit) {
    double startTime, endTime, timeStep;
    cout << "--- Transient Analysis ---" << endl;
    cout << "Enter start time: ";
    cin >> startTime;
    cout << "Enter end time: ";
    cin >> endTime;
    cout << "Enter time step: ";
    cin >> timeStep;
    circuit.simulateTransient(startTime, endTime, timeStep);
}
void handleMultipleVariablesAnalysis(Circuit& circuit) {
    double startTime, endTime, timeStep;
    cout << "--- Multiple Variables Analysis ---" << endl;
    cout << "Enter start time: ";
    cin >> startTime;
    cout << "Enter end time: ";
    cin >> endTime;
    cout << "Enter time step: ";
    cin >> timeStep;

    cout << "\n--- Displaying Voltage and Current at Different Times ---" << endl;

    for (double time = startTime; time <= endTime; time += timeStep) {
        cout << "Time: " << time << "s" << endl;
        for (const auto& comp : circuit.getComponents()) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                cout << "Voltage Source " << vs->getName() << " Voltage: "
                     << scientific << setprecision(4) << vs->getValueAtTime(time) << " V" << endl;
            } else if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                cout << "Current Source " << cs->getName() << " Current: "
                     << scientific << setprecision(4) << cs->getValueAtTime(time) << " A" << endl;
            }
        }
        cout << "-----------------" << endl;
    }
}

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
                cin >> startVoltage;
                cout << "Enter end voltage: ";
                cin >> endVoltage;
                cout << "Enter voltage step: ";
                cin >> stepVoltage;
                myCircuit.simulateDCVoltageSweep(startVoltage, endVoltage, stepVoltage);
                pauseSystem();
                break;
            }
            case 8: running = false; cout << "Exiting..." << endl; break;
            default: cout << "Invalid choice." << endl; pauseSystem(); break;
        }
    }

    return 0;
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
    for (double time = startTime; time <= endTime; time += timeStep) {
        cout << "Time: " << time << "s" << endl;
        for (const auto& comp : components) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                cout << "Voltage Source " << vs->getName() << " Voltage: "
                     << scientific << setprecision(4) << vs->getValueAtTime(time) << " V" << endl;
            } else if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                cout << "Current Source " << cs->getName() << " Current: "
                     << scientific << setprecision(4) << cs->getValueAtTime(time) << " A" << endl;
            }
        }
        cout << "-----------------" << endl;
    }
}
void Circuit::simulateMultipleVariables(double startTime, double endTime, double timeStep) {
    for (double time = startTime; time <= endTime; time += timeStep) {
        cout << "Time: " << time << "s" << endl;

        for (const auto& comp : components) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                cout << "Voltage Source " << vs->getName() << " Voltage: "
                     << scientific << setprecision(4) << vs->getValueAtTime(time) << " V" << endl;
            }
            if (auto cs = dynamic_cast<CurrentSource*>(comp.get())) {
                cout << "Current Source " << cs->getName() << " Current: "
                     << scientific << setprecision(4) << cs->getValueAtTime(time) << " A" << endl;
            }
        }
        cout << "-----------------" << endl;
    }
}
void Circuit::simulateDCVoltageSweep(double startVoltage, double endVoltage, double stepVoltage) {
    for (double voltage = startVoltage; voltage <= endVoltage; voltage += stepVoltage) {
        cout << "Voltage Sweep: " << voltage << "V" << endl;
        for (const auto& comp : components) {
            if (auto vs = dynamic_cast<VoltageSource*>(comp.get())) {
                vs->setDCOffsetValue(voltage);
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

bool Circuit::removeElement(const string& componentName) {
    for (auto it = components.begin(); it != components.end(); ++it) {
        if ((*it)->getName() == componentName) { components.erase(it); return true; }
    }
    return false;
}

Component* Circuit::findElement(const string& componentName) {
    for (const auto& comp : components) {
        if (comp->getName() == componentName) { return comp.get(); }
    }
    return nullptr;
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
    cout << "Enter node 1: "; cin >> n1;
    cout << "Enter node 2: "; cin >> n2;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    unique_ptr<Component> newComponent = nullptr;

    switch (type_enum) {
        case ComponentType::VOLTAGE_SOURCE: {
            string waveform_str, unit;
            cout << "Enter waveform type for Voltage Source (DC or SINE): ";
            getline(cin, waveform_str);
            transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);

            if (waveform_str == "DC") {
                string value_str;
                cout << "Enter DC value (e.g., 9V): ";
                getline(cin, value_str);
                newComponent = make_unique<VoltageSource>(name, parseEngineeringValue(value_str), unit, n1, n2);
            } else if (waveform_str == "SINE") {
                double offset, amp, freq;
                cout << "Enter DC Offset value: "; cin >> offset;
                cout << "Enter Amplitude value: "; cin >> amp;
                cout << "Enter Frequency (Hz) value: "; cin >> freq;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                newComponent = make_unique<VoltageSource>(name, offset, amp, freq, unit, n1, n2);
            } else {
                cout << "Error: Invalid waveform type." << endl;
            }
            break;
        }
        case ComponentType::CURRENT_SOURCE: {
            string waveform_str, unit;
            cout << "Enter waveform type for Current Source (DC or SINE): ";
            getline(cin, waveform_str);
            transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);

            if (waveform_str == "DC") {
                string value_str;
                cout << "Enter DC value (e.g., 1A, 500m): ";
                getline(cin, value_str);
                newComponent = make_unique<CurrentSource>(name, parseEngineeringValue(value_str), unit, n1, n2);
            } else if (waveform_str == "SINE") {
                double offset, amp, freq;
                cout << "Enter DC Offset value: "; cin >> offset;
                cout << "Enter Amplitude value: "; cin >> amp;
                cout << "Enter Frequency (Hz) value: "; cin >> freq;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                newComponent = make_unique<CurrentSource>(name, offset, amp, freq, unit, n1, n2);
            } else {
                cout << "Error: Invalid waveform type." << endl;
            }
            break;
        }
        case ComponentType::RESISTOR:
        case ComponentType::CAPACITOR:
        case ComponentType::INDUCTOR: {
            string value_str, unit;
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
            cout << "Enter new node 1: "; cin >> n1;
            cout << "Enter new node 2: "; cin >> n2;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            cout << "Enter new node 1: "; cin >> n1;
            cout << "Enter new node 2: "; cin >> n2;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
            cout << "Enter new node 1: "; cin >> n1;
            cout << "Enter new node 2: "; cin >> n2;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                cout << "Enter new nodes: "; cin >> n1 >> n2;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                is->setNodes(n1, n2);
                cout << "Success: Nodes updated." << endl;
            } else { cout << "Invalid choice." << endl; }
        } else {
            cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes" << endl;
            int choice = getUserChoice();
            switch(choice) {
                case 1: { double v; cout << "Enter new offset: "; cin >> v; is->setDCOffsetValue(v); cout << "Success.\n"; break; }
                case 2: { double v; cout << "Enter new amplitude: "; cin >> v; is->setAmplitude(v); cout << "Success.\n"; break; }
                case 3: { double v; cout << "Enter new frequency: "; cin >> v; is->setFrequency(v); cout << "Success.\n"; break; }
                case 4: { int n1, n2; cout << "Enter new nodes: "; cin >> n1 >> n2; is->setNodes(n1, n2); cout << "Success.\n"; break; }
                default: { cout << "Invalid choice.\n"; break; }
            }
            if (choice >= 1 && choice <= 4) cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                cout << "Enter new nodes: "; cin >> n1 >> n2;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                vs->setNodes(n1, n2);
                cout << "Success: Nodes updated." << endl;
            } else { cout << "Invalid choice." << endl; }
        } else {
            cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes" << endl;
            int choice = getUserChoice();
            switch(choice) {
                case 1: { double v; cout << "Enter new offset: "; cin >> v; vs->setDCOffsetValue(v); cout << "Success.\n"; break; }
                case 2: { double v; cout << "Enter new amplitude: "; cin >> v; vs->setAmplitude(v); cout << "Success.\n"; break; }
                case 3: { double v; cout << "Enter new frequency: "; cin >> v; vs->setFrequency(v); cout << "Success.\n"; break; }
                case 4: { int n1, n2; cout << "Enter new nodes: "; cin >> n1 >> n2; vs->setNodes(n1, n2); cout << "Success.\n"; break; }
                default: { cout << "Invalid choice.\n"; break; }
            }
            if (choice >= 1 && choice <= 4) cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    cout << "8. Exit" << endl;
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
