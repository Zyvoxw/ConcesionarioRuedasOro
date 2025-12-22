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
                ingresarCantidadVehiculo();
                break;
            case 4:
                venderVehiculo();
                break;
            case 5:
                listarVentas();
                break;
            case 6:
                printf("Saliendo del sistema...\n");
                break;
                }

    } while (opcion != 6);

    return 0;
}
