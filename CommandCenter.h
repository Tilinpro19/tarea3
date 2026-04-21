#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <functional>
#include "Entity.h"

using namespace std;

// Firma obligatoria según el PDF
using Command = function<void(const list<string>&)>;

class CommandCenter {
private:
    Entity& entidad;
    map<string, Command> comandos;
    list<string> historial;
    map<string, list<pair<string, list<string>>>> macros;

public:
    CommandCenter(Entity& e) : entidad(e) {}

    void registerCommand(const string& name, Command cmd) {
        comandos[name] = cmd;
    }

    void removeCommand(const string& name) {
        map<string, Command>::iterator it = comandos.find(name);
        if (it != comandos.end()) {
            comandos.erase(it);
        }
    }

    void execute(const string& name, const list<string>& args) {
        map<string, Command>::iterator it = comandos.find(name);

        if (it == comandos.end()) {
            throw string("Error: Comando '" + name + "' no registrado.");
        }

        string antes = entidad.obtenerEstado();
        it->second(args); // Ejecuta el comando
        string despues = entidad.obtenerEstado();

        historial.push_back("Cmd: " + name + " | " + antes + " -> " + despues);
    }

    // Firmas obligatorias para Macros
    void registerMacro(const string& name, const list<pair<string, list<string>>>& steps) {
        macros[name] = steps;
    }

    void executeMacro(const string& name) {
        map<string, list<pair<string, list<string>>>>::iterator m_it = macros.find(name);

        if (m_it != macros.end()) {
            list<pair<string, list<string>>>::iterator it;
            for (it = m_it->second.begin(); it != m_it->second.end(); ++it) {
                execute(it->first, it->second);
            }
        } else {
            throw string("Error: Macro '" + name + "' no encontrada.");
        }
    }

    void showHistory() {
        cout << "\n--- HISTORIAL DE EJECUCION ---\n";
        list<string>::iterator it;
        for (it = historial.begin(); it != historial.end(); ++it) {
            cout << *it << "\n";
        }
    }
};

#endif