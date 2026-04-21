#include "Entity.h"
#include "CommandCenter.h"
#include <vector>

using namespace std;

// 1. Comando como Función Libre
void comandoStatus(Entity& e, const list<string>& args) {
    if (!args.empty()) throw string("El comando 'status' no recibe argumentos.");
    cout << ">>> " << e.obtenerEstado() << "\n";
}

// 2. Comando como Functor (Clase con estado interno)
class ComandoCurar {
private:
    Entity& entidad;
    int usosRestantes = 5; // Estado interno propio
public:
    ComandoCurar(Entity& e) : entidad(e) {}
    void operator()(const list<string>& args) {
        if (args.empty()) throw string("Faltan argumentos para 'curar'.");
        if (usosRestantes <= 0) throw string("Pociones de curacion agotadas.");

        try {
            int cantidad = stoi(args.front());
            entidad.modificarVida(cantidad);
            usosRestantes--;
        } catch (...) {
            throw string("El argumento de 'curar' debe ser un entero.");
        }
    }
};

int main() {
    Entity player("Thiago", 100);
    CommandCenter motor(player);

    // --- REGISTRO DE COMANDOS ---

    // Registro de Función Libre (usando lambda wrapper simple)
    motor.registerCommand("status", [&](const list<string>& a) { comandoStatus(player, a); });

    // 3. Registro de Lambda (Captura por referencia)
    motor.registerCommand("mover", [&](const list<string>& args) {
        if (args.size() < 2) throw string("El comando 'mover' requiere X e Y.");
        auto it = args.begin();
        try {
            float x = stof(*it++);
            float y = stof(*it);
            player.teletransportar(x, y);
        } catch (...) {
            throw string("Argumentos invalidos para 'mover'. Deben ser flotantes.");
        }
    });

    motor.registerCommand("dano", [&](const list<string>& args) {
        if (args.empty()) throw string("Falta argumento para 'dano'.");
        player.modificarVida(-stoi(args.front()));
    });

    // Registro de Functor
    motor.registerCommand("curar", ComandoCurar(player));

    // --- REGISTRO DE 3 MACROS ---
    motor.registerMacro("combo_ataque", {
        {"mover", {"10", "5"}},
        {"dano", {"20"}},
        {"status", {}}
    });

    motor.registerMacro("escape_tactico", {
        {"mover", {"-50", "-50"}},
        {"curar", {"15"}},
        {"status", {}}
    });

    motor.registerMacro("chequeo_doble", {
        {"status", {}},
        {"status", {}}
    });

    // --- DEMOSTRACIÓN (Con manejo de excepciones nativo) ---
    cout << "--- PRUEBAS DE COMANDOS INDIVIDUALES ---\n";
    try {
        motor.execute("mover", {"5.5", "2.1"});
        motor.execute("curar", {"10"});
        motor.execute("status", {});

        cout << "\n--- PRUEBAS DE MACROS ---\n";
        motor.executeMacro("combo_ataque");
        motor.executeMacro("escape_tactico");

        cout << "\n--- PRUEBA DE ELIMINACION Y ERROR ---\n";
        motor.removeCommand("dano");
        // Esto lanzará excepción porque 'dano' ya no existe
        motor.execute("dano", {"10"});

    } catch (string e) {
        cout << "[EXCEPCION CAPTURADA]: " << e << "\n";
    }

    motor.showHistory();

    return 0;
}