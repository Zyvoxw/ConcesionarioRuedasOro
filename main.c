#include <stdio.h>
#include "funciones.h"

int main() {
    int opcion;

    cargarVehiculosIniciales();

    do {
        opcion = menu();

        switch (opcion) {
            case 1:
                registrarCliente();
                break;

            case 2:
                buscarVehiculos();
                break;

            case 3:
                venderVehiculo();
                break;

            case 4:
                listarVentas();
                break;

            case 5:
                printf("Saliendo del sistema...\n");
                break;
        }

    } while (opcion != 5);

    return 0;
}
