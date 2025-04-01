// Biblioteca padrão e da SDK
#include <stdio.h>
#include <stdint.h>

#include "state_machine.h"

int main() {
    // Inicializa a comunicação serial para permitir o uso de printf para depuração.
    stdio_init_all();

    while(true) {
        // Executa a máquina de estados do programa continuamente
        run_state_machine();
    }

    return 0;
}
