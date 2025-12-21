    #include <stdio.h>
    #include <string.h>
    #include "funciones.h"

    void leerCadena(char *cadena, int n){
        fflush(stdin);
        fgets(cadena, n, stdin);
        cadena[strcspn(cadena, "\n")] = '\0';
    }

    int leerEnteroConRango(int inicio, int fin){
        int num, val;
        do{
            val = scanf("%d", &num);
            if(val != 1 || num < inicio || num > fin){
                printf("Valor incorrecto. Intente nuevamente.\n");
                fflush(stdin);
            }
        }while(val != 1 || num < inicio || num > fin);
        return num;
    }

    float leerFlotanteConRango(float inicio, float fin){
        float num;
        int val;
        do{
            val = scanf("%f", &num);
            if(val != 1 || num < inicio || num > fin){
                printf("Valor incorrecto. Intente nuevamente.\n");
                fflush(stdin);
            }
        }while(val != 1 || num < inicio || num > fin);
        return num;
    }

    int soloLetras(char *cadena){
        int i = 0;
        while(cadena[i] != '\0'){
            if(!((cadena[i] >= 'A' && cadena[i] <= 'Z') ||
                (cadena[i] >= 'a' && cadena[i] <= 'z') ||
                cadena[i] == ' ')){
                return 0;
            }
            i++;
        }
        return 1;
    }

    int menu(){
        int op;
        do{
            printf("\n---- CONCESIONARIA RUEDAS DE ORO ----\n");
            printf("1. Registrar cliente\n");
            printf("2. Buscar vehículos\n");
            printf("3. Vender vehículo\n");
            printf("4. Listar ventas\n");
            printf("5. Salir\n");
            printf(">> ");
            op = leerEnteroConRango(1,5);
        }while(op < 1 || op > 5);
        return op;
    }

    void registrarCliente(){
        Cliente c;
        FILE *f = fopen("clientes.dat", "ab");
        if(!f) return;

        printf("ID cliente: ");
        c.id = leerEnteroConRango(1,9999);

        do{
            printf("Nombre (solo letras): ");
            leerCadena(c.nombre, 50);
            if(!soloLetras(c.nombre))
                printf("Error: solo letras y espacios.\n");
        }while(!soloLetras(c.nombre));

        printf("Cédula: ");
        leerCadena(c.cedula, 20);

        fwrite(&c, sizeof(Cliente), 1, f);
        fclose(f);

        printf("Cliente registrado correctamente.\n");
    }

    int seleccionarEstado(){
        int op;
        printf("Seleccione estado del vehículo:\n");
        printf("1. Nuevo\n");
        printf("2. Usado\n");
        printf(">> ");
        op = leerEnteroConRango(1,2);
        return (op == 2);
    }

    int obtenerTiposPorEstado(int usado, char tipos[][20]){
        Vehiculo v;
        FILE *f = fopen("vehiculos.dat", "rb");
        int total = 0;
        if(!f) return 0;

        while(fread(&v, sizeof(Vehiculo), 1, f)){
            if(v.disponible && v.usado == usado){
                int existe = 0;
                for(int i = 0; i < total; i++){
                    if(strcmp(tipos[i], v.tipo) == 0){
                        existe = 1;
                        break;
                    }
                }
                if(!existe){
                    strcpy(tipos[total], v.tipo);
                    total++;
                }
            }
        }
        fclose(f);
        return total;
    }

    void listarVehiculosPorEstadoYTipo(int usado, char *tipo){
        Vehiculo v;
        FILE *f = fopen("vehiculos.dat", "rb");
        if(!f) return;

        printf("\nVehículos disponibles:\n");
        while(fread(&v, sizeof(Vehiculo), 1, f)){
            if(v.disponible && v.usado == usado && strcmp(v.tipo, tipo) == 0){
                printf("ID:%d | %s %s | $%.2f\n",
                    v.id, v.marca, v.modelo, v.precio);
            }
        }
        fclose(f);
    }

    void buscarVehiculos(){
        int usado = seleccionarEstado();

        char tipos[10][20];
        int total = obtenerTiposPorEstado(usado, tipos);

        if(total == 0){
            printf("No hay vehículos disponibles.\n");
            return;
        }

        printf("\nTipos disponibles:\n");
        for(int i = 0; i < total; i++){
            printf("%d. %s\n", i + 1, tipos[i]);
        }

        printf("Seleccione tipo: ");
        int op = leerEnteroConRango(1, total);
        listarVehiculosPorEstadoYTipo(usado, tipos[op - 1]);
    }

    void venderVehiculo(){
        Cliente c;
        Vehiculo v;
        Venta venta;
        int idVehiculo, encontrado = 0;

        FILE *fc = fopen("clientes.dat", "rb");
        FILE *fv = fopen("vehiculos.dat", "rb+");
        FILE *fven = fopen("ventas.dat", "ab");

        if(!fc || !fv || !fven) return;

        printf("ID cliente: ");
        venta.cliente.id = leerEnteroConRango(1,9999);

        while(fread(&c, sizeof(Cliente), 1, fc)){
            if(c.id == venta.cliente.id){
                venta.cliente = c;
                encontrado = 1;
                break;
            }
        }

        if(!encontrado){
            printf("Cliente no encontrado.\n");
            fclose(fc); fclose(fv); fclose(fven);
            return;
        }

        buscarVehiculos();

        printf("Ingrese ID del vehículo a vender: ");
        idVehiculo = leerEnteroConRango(1,9999);

        while(fread(&v, sizeof(Vehiculo), 1, fv)){
            if(v.id == idVehiculo && v.disponible){
                v.disponible = 0;
                venta.vehiculo = v;
                fseek(fv, -sizeof(Vehiculo), SEEK_CUR);
                fwrite(&v, sizeof(Vehiculo), 1, fv);
                break;
            }
        }

        fwrite(&venta, sizeof(Venta), 1, fven);

        fclose(fc); fclose(fv); fclose(fven);
        printf("Venta realizada correctamente.\n");
    }

    void listarVentas(){
        Venta v;
        FILE *f = fopen("ventas.dat", "rb");
        if(!f) return;

        printf("\n--- VENTAS REGISTRADAS ---\n");
        while(fread(&v, sizeof(Venta), 1, f)){
            printf("Cliente: %s | Vehículo: %s %s | $%.2f\n",
                v.cliente.nombre,
                v.vehiculo.marca,
                v.vehiculo.modelo,
                v.vehiculo.precio);
        }
        fclose(f);
    }

    void cargarVehiculosIniciales(){
        FILE *f = fopen("vehiculos.dat", "rb");
        if(f){ fclose(f); return; }

        f = fopen("vehiculos.dat", "wb");

        Vehiculo v[] = {
            {1,"Chevrolet","D-Max","Camioneta",1,14000,1},
            {2,"Toyota","Hilux","Camioneta",0,28000,1},
            {3,"Kia","Sportage","SUV",1,16500,1},
            {4,"Hyundai","Tucson","SUV",0,32000,1},
            {5,"Chevrolet","Sail","Sedan",0,15500,1}
        };

        fwrite(v, sizeof(Vehiculo), 5, f);
        fclose(f);
    }

    int main(){
        cargarVehiculosIniciales();

        int op;
        do{
            op = menu();
            switch(op){
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
            }
        }while(op != 5);

        return 0;
    }
