#include <stdio.h>
#include "funciones.h"

int main(){
    int opcion;

    cargarVehiculosIniciales();

    do {
        opcion = menu();

        switch (opcion) {
            case 1:
                registrarCliente();
                break;

            case 2:
                ingresarStockPorMarca();
                break;

            case 3:
                buscarVehiculos();
                break;
            case 4:
                venderVehiculo();
                break;
            case 5:
                listarVentas();
                break;
            case 6:
                listarClientes();
                break;
            case 7:
                printf("FIN DEL PROGRAMA...\n");
                break;
        }
    }while (opcion != 7);

    return 0;
}
