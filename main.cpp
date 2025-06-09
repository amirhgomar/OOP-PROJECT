#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <memory>
#include <cctype>
#include <cmath>
// Enum for component types
enum class ComponentType {
    RESISTOR,
    CAPACITOR,
    INDUCTOR,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE,
    UNKNOWN
};

// --- Abstract Base Class 'Component' ---
class Component {
protected:
    std::string name;
    int node1;
    int node2;

public:
    Component(const std::string& name, int n1, int n2) : name(name), node1(n1), node2(n2) {}
    virtual ~Component() = default;

    virtual void display() const = 0;
    virtual std::string getType() const = 0;

    std::string getName() const { return name; }
    void setNodes(int newNode1, int newNode2) {
        this->node1 = newNode1;
        this->node2 = newNode2;
    }
    // NEW: A public way to access nodes for checking the ground connection
    int getNode1() const { return node1; }
    int getNode2() const { return node2; }
};

// --- Derived Class 'Resistor' ---
class Resistor : public Component {
private:
    double resistance;
    std::string unit;

public:
    Resistor(const std::string& name, double res, int n1, int n2)
            : Component(name, n1, n2), resistance(res), unit("Ohm") {}


    void display() const override {
        std::cout << "  - Element: " << name << " | Type: " << getType()
                  << " | Value: " << std::scientific << std::setprecision(4) << resistance << " " << unit
                  << " | Nodes: (" << node1 << ", " << node2 << ")" << std::endl;
    }
    std::string getType() const override { return "Resistor"; }
    void setResistance(double res) { this->resistance = res; }
};

// --- Derived Class 'Capacitor' ---
class Capacitor : public Component {
private:
    double capacitance;
    std::string unit;

public:
    Capacitor(const std::string& name, double cap, int n1, int n2)
            : Component(name, n1, n2), capacitance(cap), unit("F") {}

    void display() const override {
        std::cout << "  - Element: " << name << " | Type: " << getType() << " | Value: " << std::scientific << std::setprecision(4) << capacitance << " " << unit << " | Nodes: (" << node1 << ", " << node2 << ")" << std::endl;
    }
    std::string getType() const override { return "Capacitor"; }
    void setCapacitance(double cap) { this->capacitance = cap; }
};

// --- Derived Class 'Inductor' ---
class Inductor : public Component {
private:
    double inductance;
    std::string unit;

public:
    Inductor(const std::string& name, double ind, int n1, int n2)
            : Component(name, n1, n2), inductance(ind), unit("H") {}

    void display() const override {
        std::cout << "  - Element: " << name << " | Type: " << getType() << " | Value: " << std::scientific << std::setprecision(4) << inductance << " " << unit << " | Nodes: (" << node1 << ", " << node2 << ")" << std::endl;
    }
    std::string getType() const override { return "Inductor"; }
    void setInductance(double ind) { this->inductance = ind; }
};

// UPDATED: The VoltageSource class is now much more powerful
class VoltageSource : public Component {
public:
    enum class Waveform { DC, SINE };
private:
    Waveform waveformType;
    double offset_or_dc_value;
    double amplitude;
    double frequency;
    std::string unit;
public:
    VoltageSource(const std::string& name, double dc_val, const std::string& unit, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::DC), offset_or_dc_value(dc_val),
              amplitude(0), frequency(0), unit("V") {}

    VoltageSource(const std::string& name, double offset, double amp, double freq, const std::string& unit, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::SINE), offset_or_dc_value(offset),
              amplitude(amp), frequency(freq), unit("V") {}

    double getValueAtTime(double time) const {
        if (waveformType == Waveform::SINE) return offset_or_dc_value + amplitude * std::sin(2 * M_PI * frequency * time);
        return offset_or_dc_value;
    }

    void display() const override {
        std::cout << "  - Element: " << name << " | Type: " << getType();
        if (waveformType == Waveform::DC) {
            std::cout << " (DC) | Value: " << std::scientific << std::setprecision(4) << offset_or_dc_value << " " << unit;
        } else {
            std::cout << " (SINE) | Params: Offset=" << offset_or_dc_value << ", Amp=" << amplitude << ", Freq=" << frequency << "Hz";
        }
        std::cout << " | Nodes: (" << node1 << ", " << node2 << ")" << std::endl;
    }

    std::string getType() const override { return "Voltage Source"; }

    // These methods were added/completed
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
    std::string unit;
public:
    // Constructor for DC
    CurrentSource(const std::string& name, double dc_val, const std::string& unit, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::DC), offset_or_dc_value(dc_val),
              amplitude(0), frequency(0), unit("A") {}

    // Constructor for SINE
    CurrentSource(const std::string& name, double offset, double amp, double freq, const std::string& unit, int n1, int n2)
            : Component(name, n1, n2), waveformType(Waveform::SINE), offset_or_dc_value(offset),
              amplitude(amp), frequency(freq), unit("A") {}

    double getValueAtTime(double time) const {
        if (waveformType == Waveform::SINE) return offset_or_dc_value + amplitude * std::sin(2 * M_PI * frequency * time);
        return offset_or_dc_value;
    }

    void display() const override {
        std::cout << "  - Element: " << name << " | Type: " << getType();
        if (waveformType == Waveform::DC) {
            std::cout << " (DC) | Value: " << std::scientific << std::setprecision(4) << offset_or_dc_value << " " << unit;
        } else {
            std::cout << " (SINE) | Params: Offset=" << offset_or_dc_value << ", Amp=" << amplitude << ", Freq=" << frequency << "Hz";
        }
        std::cout << " | Nodes: (" << node1 << ", " << node2 << ")" << std::endl;
    }

    std::string getType() const override { return "Current Source"; }
    Waveform getWaveformType() const { return waveformType; }
    void setDCOffsetValue(double val) { this->offset_or_dc_value = val; }
    void setAmplitude(double amp) { this->amplitude = amp; }
    void setFrequency(double freq) { this->frequency = freq; }
};

// --- Circuit Class ---
class Circuit {
private:
    std::vector<std::unique_ptr<Component>> components;
public:
    void addElement(std::unique_ptr<Component> newComponent);
    bool removeElement(const std::string& componentName);
    Component* findElement(const std::string& componentName);
    void displayCircuit() const;
    bool hasGround() const;
};

// --- Helper Functions ---
double parseEngineeringValue(const std::string& valStr) {
    if (valStr.empty()) return 0.0;
    size_t first_char_pos = std::string::npos;
    for (size_t i = 0; i < valStr.length(); ++i) {
        if (std::isalpha(valStr[i])) {
            first_char_pos = i;
            break;
        }
    }
    if (first_char_pos == std::string::npos) {
        try { return std::stod(valStr); } catch(...) { return 0.0; }
    }
    std::string numberPart = valStr.substr(0, first_char_pos);
    std::string suffixPart = valStr.substr(first_char_pos);
    std::transform(suffixPart.begin(), suffixPart.end(), suffixPart.begin(), ::tolower);
    double number = 0.0;
    try { number = std::stod(numberPart); } catch(...) { return 0.0; }
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

ComponentType stringToComponentType(const std::string& str) {
    std::string lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
    if (lower_str == "resistor") return ComponentType::RESISTOR;
    if (lower_str == "capacitor") return ComponentType::CAPACITOR;
    if (lower_str == "inductor") return ComponentType::INDUCTOR;
    if (lower_str == "voltage source" || lower_str == "vsource") return ComponentType::VOLTAGE_SOURCE;
    if (lower_str == "current source" || lower_str == "isource") return ComponentType::CURRENT_SOURCE;
    return ComponentType::UNKNOWN;
}
// --- Function Prototypes for Menu ---
void displayMenu();
int getUserChoice();
void handleAddComponent(Circuit& circuit);
void handleRemoveElement(Circuit& circuit);
void handleModifyComponent(Circuit& circuit);
void pauseSystem();

// --- Main Function ---
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
            case 5: running = false; std::cout << "Exiting..." << std::endl; break;
            default: std::cout << "Invalid choice." << std::endl; pauseSystem(); break;
        }
    }

    return 0;
}

// --- Circuit Class Method Implementations ---
void Circuit::addElement(std::unique_ptr<Component> newComponent) { components.push_back(std::move(newComponent)); }
void Circuit::displayCircuit() const {
    if (components.empty()) {
        std::cout << "Circuit is empty." << std::endl;
        return;
    }

    std::cout << "-------------------- Circuit Status --------------------" << std::endl;
    if (this->hasGround()) {
        std::cout << "Ground Status: Connected." << std::endl;
    } else {
        std::cout << ">> WARNING: No ground node (0) found in the circuit! <<" << std::endl;
    }
    std::cout << "--------------------------------------------------------" << std::endl;

    for (const auto& comp : components) {
        comp->display();
    }
    std::cout << "--------------------------------------------------------" << std::endl;
}
// NEW: Implementation of hasGround
bool Circuit::hasGround() const {
    for (const auto& comp : components) {
        if (comp->getNode1() == 0 || comp->getNode2() == 0) {
            return true; // Found a component connected to ground
        }
    }
    return false; // No component connected to ground
}
bool Circuit::removeElement(const std::string& componentName) {
    for (auto it = components.begin(); it != components.end(); ++it) {
        if ((*it)->getName() == componentName) { components.erase(it); return true; }
    }
    return false;
}
Component* Circuit::findElement(const std::string& componentName) {
    for (const auto& comp : components) {
        if (comp->getName() == componentName) { return comp.get(); }
    }
    return nullptr;
}

// --- Menu Helper Function Implementations ---
void handleAddComponent(Circuit& circuit) {
    std::string name, type_str;

    std::cout << "--- Add New Component ---" << std::endl;
    std::cout << "Enter name (e.g., R1): ";
    std::getline(std::cin, name);
    if (circuit.findElement(name) != nullptr) {
        std::cout << "Error: A component with name '" << name << "' already exists." << std::endl;
        return;
    }

    ComponentType type_enum;
    while (true) {
        std::cout << "Enter type (Resistor, Capacitor, Inductor, Voltage Source, Current Source): ";
        std::getline(std::cin, type_str);
        type_enum = stringToComponentType(type_str);
        if (type_enum != ComponentType::UNKNOWN) break;
        std::cout << "Error: Invalid component type. Please try again." << std::endl;
    }

    int n1, n2;
    std::cout << "Enter node 1: "; std::cin >> n1;
    std::cout << "Enter node 2: "; std::cin >> n2;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::unique_ptr<Component> newComponent = nullptr;

    switch (type_enum) {
        case ComponentType::VOLTAGE_SOURCE: {
            std::string waveform_str, unit;
            std::cout << "Enter waveform type for Voltage Source (DC or SINE): ";
            std::getline(std::cin, waveform_str);
            std::transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);


            if (waveform_str == "DC") {
                std::string value_str;
                std::cout << "Enter DC value (e.g., 9V): ";
                std::getline(std::cin, value_str);
                newComponent = std::make_unique<VoltageSource>(name, parseEngineeringValue(value_str), unit, n1, n2);
            } else if (waveform_str == "SINE") {
                double offset, amp, freq;
                std::cout << "Enter DC Offset value: "; std::cin >> offset;
                std::cout << "Enter Amplitude value: "; std::cin >> amp;
                std::cout << "Enter Frequency (Hz) value: "; std::cin >> freq;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                newComponent = std::make_unique<VoltageSource>(name, offset, amp, freq, unit, n1, n2);
            } else {
                std::cout << "Error: Invalid waveform type." << std::endl;
            }
            break;
        }
        case ComponentType::CURRENT_SOURCE: {
            std::string waveform_str, unit;
            std::cout << "Enter waveform type for Current Source (DC or SINE): ";
            std::getline(std::cin, waveform_str);
            std::transform(waveform_str.begin(), waveform_str.end(), waveform_str.begin(), ::toupper);


            if (waveform_str == "DC") {
                std::string value_str;
                std::cout << "Enter DC value (e.g., 1A, 500m): ";
                std::getline(std::cin, value_str);
                newComponent = std::make_unique<CurrentSource>(name, parseEngineeringValue(value_str), unit, n1, n2);
            } else if (waveform_str == "SINE") {
                double offset, amp, freq;
                std::cout << "Enter DC Offset value: "; std::cin >> offset;
                std::cout << "Enter Amplitude value: "; std::cin >> amp;
                std::cout << "Enter Frequency (Hz) value: "; std::cin >> freq;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                newComponent = std::make_unique<CurrentSource>(name, offset, amp, freq, unit, n1, n2);
            } else {
                std::cout << "Error: Invalid waveform type." << std::endl;
            }
            break;
        }
        case ComponentType::RESISTOR:
        case ComponentType::CAPACITOR:
        case ComponentType::INDUCTOR: {
            std::string value_str, unit;
            std::cout << "Enter value (e.g., 1k, 10u): ";
            std::getline(std::cin, value_str);

            double value = parseEngineeringValue(value_str);

            if (type_enum == ComponentType::RESISTOR)
                newComponent = std::make_unique<Resistor>(name, value, n1, n2);
            else if (type_enum == ComponentType::CAPACITOR)
                newComponent = std::make_unique<Capacitor>(name, value, n1, n2);
            else if (type_enum == ComponentType::INDUCTOR)
                newComponent = std::make_unique<Inductor>(name, value, n1, n2);
            break;
        }
        case ComponentType::UNKNOWN:
            // Do nothing, newComponent remains nullptr
            break;
    }

    if (newComponent != nullptr) {
        std::cout << "Info: Component '" << name << "' added successfully." << std::endl;
        circuit.addElement(std::move(newComponent));
    }
}


void handleRemoveElement(Circuit& circuit) {
    std::string name;
    std::cout << "--- Remove Component ---" << std::endl;
    std::cout << "Enter the name of the component to remove: ";
    std::getline(std::cin, name);
    if (circuit.removeElement(name)) {
        std::cout << "Success: Component '" << name << "' has been removed." << std::endl;
    } else {
        std::cout << "Error: Component '" << name << "' not found." << std::endl;
    }
}

void handleModifyComponent(Circuit& circuit) {
    std::string name;
    std::cout << "--- Modify Component ---" << std::endl;
    std::cout << "Enter the name of the component to modify: ";
    std::getline(std::cin, name);

    Component* comp = circuit.findElement(name);
    if (comp == nullptr) {
        std::cout << "Error: Component '" << name << "' not found." << std::endl;
        return;
    }

    std::cout << "Component found. What do you want to modify?" << std::endl;

    // --- Resistor ---
    if (Resistor* res = dynamic_cast<Resistor*>(comp)) {
        std::cout << "1. Resistance Value\n2. Nodes" << std::endl;
        int choice = getUserChoice();
        if (choice == 1) {
            std::string value_str;
            std::cout << "Enter new resistance: ";
            std::getline(std::cin, value_str);
            res->setResistance(parseEngineeringValue(value_str));
            std::cout << "Success: Resistance updated." << std::endl;
        } else if (choice == 2) {
            int n1, n2;
            std::cout << "Enter new node 1: "; std::cin >> n1;
            std::cout << "Enter new node 2: "; std::cin >> n2;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            res->setNodes(n1, n2);
            std::cout << "Success: Nodes updated." << std::endl;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
        // --- Capacitor ---
    else if (Capacitor* cap = dynamic_cast<Capacitor*>(comp)) {
        std::cout << "1. Capacitance Value\n2. Nodes" << std::endl;
        int choice = getUserChoice();
        if (choice == 1) {
            std::string value_str;
            std::cout << "Enter new capacitance: ";
            std::getline(std::cin, value_str);
            cap->setCapacitance(parseEngineeringValue(value_str));
            std::cout << "Success: Capacitance updated." << std::endl;
        } else if (choice == 2) {
            int n1, n2;
            std::cout << "Enter new node 1: "; std::cin >> n1;
            std::cout << "Enter new node 2: "; std::cin >> n2;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cap->setNodes(n1, n2);
            std::cout << "Success: Nodes updated." << std::endl;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
        // --- Inductor ---
    else if (Inductor* ind = dynamic_cast<Inductor*>(comp)) {
        std::cout << "1. Inductance Value\n2. Nodes" << std::endl;
        int choice = getUserChoice();
        if (choice == 1) {
            std::string value_str;
            std::cout << "Enter new inductance: ";
            std::getline(std::cin, value_str);
            ind->setInductance(parseEngineeringValue(value_str));
            std::cout << "Success: Inductance updated." << std::endl;
        } else if (choice == 2) {
            int n1, n2;
            std::cout << "Enter new node 1: "; std::cin >> n1;
            std::cout << "Enter new node 2: "; std::cin >> n2;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            ind->setNodes(n1, n2);
            std::cout << "Success: Nodes updated." << std::endl;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
        // --- Current Source ---
    else if (CurrentSource* is = dynamic_cast<CurrentSource*>(comp)) {
        if (is->getWaveformType() == CurrentSource::Waveform::DC) {
            std::cout << "1. DC Value\n2. Nodes" << std::endl;
            int choice = getUserChoice();
            if (choice == 1) {
                std::string value_str;
                std::cout << "Enter new DC value: ";
                std::getline(std::cin, value_str);
                is->setDCOffsetValue(parseEngineeringValue(value_str));
                std::cout << "Success: DC value updated." << std::endl;
            } else if (choice == 2) {
                int n1, n2;
                std::cout << "Enter new nodes: "; std::cin >> n1 >> n2;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                is->setNodes(n1, n2);
                std::cout << "Success: Nodes updated." << std::endl;
            } else { std::cout << "Invalid choice." << std::endl; }
        } else { // SINE
            std::cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes" << std::endl;
            int choice = getUserChoice();
            switch(choice) {
                case 1: { double v; std::cout << "Enter new offset: "; std::cin >> v; is->setDCOffsetValue(v); std::cout << "Success.\n"; break; }
                case 2: { double v; std::cout << "Enter new amplitude: "; std::cin >> v; is->setAmplitude(v); std::cout << "Success.\n"; break; }
                case 3: { double v; std::cout << "Enter new frequency: "; std::cin >> v; is->setFrequency(v); std::cout << "Success.\n"; break; }
                case 4: { int n1, n2; std::cout << "Enter new nodes: "; std::cin >> n1 >> n2; is->setNodes(n1, n2); std::cout << "Success.\n"; break; }
                default: { std::cout << "Invalid choice.\n"; break; }
            }
            if (choice >= 1 && choice <= 4) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
        // --- Voltage Source ---
    else if (VoltageSource* vs = dynamic_cast<VoltageSource*>(comp)) {
        if (vs->getWaveformType() == VoltageSource::Waveform::DC) {
            std::cout << "1. DC Value\n2. Nodes" << std::endl;
            int choice = getUserChoice();
            if (choice == 1) {
                std::string value_str;
                std::cout << "Enter new DC value: ";
                std::getline(std::cin, value_str);
                vs->setDCOffsetValue(parseEngineeringValue(value_str));
                std::cout << "Success: DC value updated." << std::endl;
            } else if (choice == 2) {
                int n1, n2;
                std::cout << "Enter new nodes: "; std::cin >> n1 >> n2;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                vs->setNodes(n1, n2);
                std::cout << "Success: Nodes updated." << std::endl;
            } else { std::cout << "Invalid choice." << std::endl; }
        } else { // SINE
            std::cout << "1. Offset\n2. Amplitude\n3. Frequency\n4. Nodes" << std::endl;
            int choice = getUserChoice();
            switch(choice) {
                case 1: { double v; std::cout << "Enter new offset: "; std::cin >> v; vs->setDCOffsetValue(v); std::cout << "Success.\n"; break; }
                case 2: { double v; std::cout << "Enter new amplitude: "; std::cin >> v; vs->setAmplitude(v); std::cout << "Success.\n"; break; }
                case 3: { double v; std::cout << "Enter new frequency: "; std::cin >> v; vs->setFrequency(v); std::cout << "Success.\n"; break; }
                case 4: { int n1, n2; std::cout << "Enter new nodes: "; std::cin >> n1 >> n2; vs->setNodes(n1, n2); std::cout << "Success.\n"; break; }
                default: { std::cout << "Invalid choice.\n"; break; }
            }
            if (choice >= 1 && choice <= 4) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void displayMenu() {
    std::cout << "\n\n========== Circuit Editor Menu ==========" << std::endl;
    std::cout << "1. Add Component" << std::endl;
    std::cout << "2. Display Circuit" << std::endl;
    std::cout << "3. Remove Component" << std::endl;
    std::cout << "4. Modify Component" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Enter your choice: ";
}

int getUserChoice() {
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void pauseSystem() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
