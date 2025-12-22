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

        int validarCedula(char *cedula){
            int longitud = strlen(cedula);

            if(longitud != 10)
                return 0;

            for(int i = 0; i < longitud; i++){
                if(cedula[i] < '0' || cedula[i] > '9')
                    return 0;
            }

            return 1;
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
                printf("2. Ingresar stock de vehiculos\n");
                printf("3. Buscar vehiculos\n");
                printf("4. Vender vehiculo\n");
                printf("5. Listar ventas\n");
                printf("6. Salir\n");
                printf(">> ");

                op = leerEnteroConRango(1,6);
            }while(op < 1 || op > 6);
            return op;
        }

        int menuMarcas(){
            printf("\nSeleccione la marca:\n");
            printf("1. Chevrolet\n");
            printf("2. Toyota\n");
            printf("3. Honda\n");
            printf(">> ");
            return leerEnteroConRango(1,3);
        }

        void registrarCliente(){
            Cliente c;
            FILE *f = fopen("clientes.dat", "ab");
            if(!f) return;

            printf("ID cliente: ");
            c.id = leerEnteroConRango(1,9999);

            do{
                printf("Nombre: ");
                leerCadena(c.nombre, 50);
                if(!soloLetras(c.nombre))
                    printf("Error: solo letras y espacios.\n");
            }while(!soloLetras(c.nombre));

            do{
                printf("Cédula: ");
                leerCadena(c.cedula, 20);

                if(!validarCedula(c.cedula)){
                    printf("la cédula debe tener exactamente 10 números y no letras.\n");
                }

            }while(!validarCedula(c.cedula));


            fwrite(&c, sizeof(Cliente), 1, f);
            fclose(f);

            printf("Cliente registrado correctamente.\n");
        }

void ingresarCantidadVehiculo() {
            Vehiculo v;
            int id, encontrada = 0;

            FILE *f = fopen("vehiculos.dat", "rb+");
            if (!f) return;

            printf("Ingrese ID del vehículo: ");
            id = leerEnteroConRango(1, 9999);

            while (fread(&v, sizeof(Vehiculo), 1, f)) {
                if (v.id == id) {
                    printf("Cantidad actual: %d\n", v.cantidad);
                    printf("Ingrese nueva cantidad: ");
                    v.cantidad = leerEnteroConRango(0, 100);

                    fseek(f, -(long)sizeof(Vehiculo), SEEK_CUR);
                    fwrite(&v, sizeof(Vehiculo), 1, f);

                    encontrada = 1;
                    break;
                }
            }

            if (!encontrada)
                printf("Vehículo no encontrado.\n");

            fclose(f);
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

    Vehiculo buscarVehiculos(){
        Vehiculo v, lista[20];
        FILE *f = fopen("vehiculos.dat", "rb");
        if(!f){
            printf("No se pudo abrir el archivo.\n");
            return (Vehiculo){0}; // Devuelve un vehiculo vacío
        }

        int opMarca = menuMarcas();
        char marca[30];
        if(opMarca == 1) strcpy(marca, "Chevrolet");
        else if(opMarca == 2) strcpy(marca, "Toyota");
        else strcpy(marca, "Honda");

        int usado = seleccionarEstado();

        // INGRESAR PRESUPUESTO
        float presupuesto;
        printf("Ingrese su presupuesto maximo: $");
        presupuesto = leerFlotanteConRango(0, 1000000);

        printf("\nMODELOS DISPONIBLES (%s - %s, hasta $%.2f)\n",
            marca, usado ? "Usado" : "Nuevo", presupuesto);
        printf("--------------------------------\n");

        int count = 0;
        while(fread(&v, sizeof(Vehiculo), 1, f)){
            if(v.cantidad > 0 &&
            strcmp(v.marca, marca) == 0 &&
            v.usado == usado &&
            v.precio <= presupuesto){

                lista[count] = v;
                printf("%d. %s ($%.2f)\n", count + 1, v.modelo, v.precio);
                count++;
            }
        }

        if(count == 0){
            printf("No hay vehículos disponibles dentro de su presupuesto.\n");
            fclose(f);
            return (Vehiculo){0};
        }

        int op;
        printf("\nSeleccione el número del modelo: ");
        op = leerEnteroConRango(1, count);

        Vehiculo seleccionado = lista[op - 1];
        fclose(f);
        return seleccionado;
    }

void venderVehiculo() {
    Cliente c;
    Vehiculo vehiculos[50];
    int totalVehiculos = 0;
    Venta venta;
    int clienteEncontrado = 0;

    FILE *fc = fopen("clientes.dat", "rb");
    FILE *fv = fopen("vehiculos.dat", "rb");
    FILE *fven = fopen("ventas.dat", "ab");

    if (!fc || !fv || !fven) {
        printf("Error al abrir archivos.\n");
        if (fc) fclose(fc);
        if (fv) fclose(fv);
        if (fven) fclose(fven);
        return;
    }

    // ================== Buscar cliente ==================
    printf("Ingrese ID del cliente: ");
    venta.cliente.id = leerEnteroConRango(1, 9999);
    while (fread(&c, sizeof(Cliente), 1, fc)) {
        if (c.id == venta.cliente.id) {
            venta.cliente = c;
            clienteEncontrado = 1;
            break;
        }
    }
    fclose(fc);

    if (!clienteEncontrado) {
        printf("Cliente no encontrado.\n");
        fclose(fv);
        fclose(fven);
        return;
    }

    // ================== Presupuesto inicial ==================
    float presupuesto;
    printf("Ingrese su presupuesto maximo: $");
    presupuesto = leerFlotanteConRango(0, 1000000);

    // ================== Cargar vehículos en memoria ==================
    while (fread(&vehiculos[totalVehiculos], sizeof(Vehiculo), 1, fv)) {
        totalVehiculos++;
    }
    fclose(fv);

    // ================== Filtrar vehículos dentro del presupuesto ==================
    int opciones[50], count = 0;
    for (int i = 0; i < totalVehiculos; i++) {
        if (vehiculos[i].cantidad > 0 && vehiculos[i].precio <= presupuesto) {
            opciones[count] = i;
            printf("%d. %s %s ($%.2f) - Stock: %d\n", count + 1, vehiculos[i].marca, vehiculos[i].modelo, vehiculos[i].precio, vehiculos[i].cantidad);
            count++;
        }
    }

    int credito = 0;
    float montoCredito = 0;
    int mesesCredito = 0;

    // ================== Si no hay vehículos dentro del presupuesto ==================
    if (count == 0) {
        printf("\nNo hay vehiculos disponibles dentro de su presupuesto.\n");
        printf("Desea financiar con credito? (1. Sí / 2. No): ");
        int opCredito = leerEnteroConRango(1, 2);
        if (opCredito == 1) {
            credito = 1;

            // Preguntar cuánto desea financiar
            printf("Ingrese monto que desea financiar: $");
            montoCredito = leerFlotanteConRango(0, 1000000);

            // Preguntar cantidad de meses
            printf("Ingrese cantidad de meses para el credito (1-60): ");
            mesesCredito = leerEnteroConRango(1, 60);

            // Calcular precio máximo = presupuesto inicial + crédito con interés
            float interes = montoCredito * 0.05 * (mesesCredito / 12.0);
            float maxPago = presupuesto + montoCredito + interes;

            printf("\nVehiculos disponibles con credito :\n", maxPago);
            count = 0;

            for (int i = 0; i < totalVehiculos; i++) {
                if (vehiculos[i].cantidad > 0 && vehiculos[i].precio <= maxPago) {
                    opciones[count] = i;
                    printf("%d. %s %s ($%.2f) - Stock: %d\n", count + 1, vehiculos[i].marca, vehiculos[i].modelo, vehiculos[i].precio, vehiculos[i].cantidad);
                    count++;
                }
            }

            if (count == 0) {
                printf("Aun con credito no hay vehiculos disponibles.\n");
                fclose(fven);
                return;
            }
        } else {
            fclose(fven);
            return;
        }
    }

    // ================== Selección del vehículo ==================
    int op;
    printf("\nSeleccione el numero del vehiculo: ");
    op = leerEnteroConRango(1, count);
    int idx = opciones[op - 1];
    venta.vehiculo = vehiculos[idx];
    vehiculos[idx].cantidad--;

    // ================== Registrar fecha ==================
    printf("\nIngrese la fecha de la venta:\n");
    venta.fechaVenta = ingresarFecha();

    // ================== Calcular pago ==================
    if (credito) {
        float interes = montoCredito * 0.05 * (mesesCredito / 12.0);
        venta.totalPagado = venta.vehiculo.precio + interes;
        printf("\nPrecio del vehiculo: $%.2f\n", venta.vehiculo.precio);
        printf("Credito: $%.2f\n", montoCredito);
        printf("Interes sobre credito: $%.2f\n", interes);
        printf("Total a pagar: $%.2f\n", venta.totalPagado);
        printf("Cuotas mensuales: $%.2f\n", venta.totalPagado / mesesCredito);
    } else {
        venta.totalPagado = venta.vehiculo.precio;
        printf("Pago al contado: $%.2f\n", venta.totalPagado);
    }

    // ================== Guardar venta ==================
    fwrite(&venta, sizeof(Venta), 1, fven);
    fclose(fven);

    // ================== Actualizar stock ==================
    fv = fopen("vehiculos.dat", "wb");
    for (int i = 0; i < totalVehiculos; i++) {
        fwrite(&vehiculos[i], sizeof(Vehiculo), 1, fv);
    }
    fclose(fv);

    printf("\nVenta registrada correctamente.\n");
}

void listarVentas(){
            Venta v;
            FILE *f = fopen("ventas.dat", "rb");

            if(!f){
                printf("\nNo se ha realizado ninguna venta.\n");
                return;
            }

            int hayVentas = 0;

            printf("\n--- VENTAS REGISTRADAS ---\n");

            while(fread(&v, sizeof(Venta), 1, f)){
                printf("---------------------------------\n");
                printf("Cliente: %s\n", v.cliente.nombre);
                printf("Vehiculo: %s %s\n", v.vehiculo.marca, v.vehiculo.modelo);
                printf("Estado: %s\n", v.vehiculo.usado ? "Usado" : "Nuevo");
                printf("Fecha: %02d/%02d/%d\n",
                    v.fechaVenta.dia,
                    v.fechaVenta.mes,
                    v.fechaVenta.anio);
                printf("Total pagado: $%.2f\n", v.totalPagado);
                hayVentas = 1;
            }

            if(!hayVentas){
                printf("\nNo se ha realizado ninguna venta.\n");
            }

            fclose(f);
        }


        void cargarVehiculosIniciales(){
            FILE *f = fopen("vehiculos.dat","rb");
            if(f){ fclose(f); return; }

            f = fopen("vehiculos.dat","wb");

            Vehiculo v[] = {
                {1,"Chevrolet","Onix","Auto",0,19999,5},
                {2,"Chevrolet","D-Max","Camioneta",1,24000,3},
                {3,"Toyota","Yaris","Auto",0,20000,4},
                {4,"Toyota","Hilux","Camioneta",1,28000,2},
                {5,"Honda","CR-V","SUV",1,30000,2}
            };

            fwrite(v,sizeof(Vehiculo),5,f);
            fclose(f);
        }

    Fecha ingresarFecha() {
        Fecha f;

        do {
            printf("Ingrese día (1-31): ");
            f.dia = leerEnteroConRango(1, 31);

            printf("Ingrese mes (1-12): ");
            f.mes = leerEnteroConRango(1, 12);

            printf("Ingrese año (2000-2026): ");
            f.anio = leerEnteroConRango(2000, 2100);

            if (f.mes == 2 && f.dia > 29) {
                printf("Febrero no tiene más de 29 días.\n");
            }

            if ((f.mes == 4 || f.mes == 6 || f.mes == 9 || f.mes == 11) && f.dia > 30) {
                printf("Ese mes solo tiene 30 días.\n");
            }

        } while (
            (f.mes == 2 && f.dia > 29) ||
            ((f.mes == 4 || f.mes == 6 || f.mes == 9 || f.mes == 11) && f.dia > 30)
        );

        return f;
    }

void ingresarStockPorMarca(){
        Vehiculo v;
        FILE *f = fopen("vehiculos.dat", "wb");
        if(!f){
            printf("Error al abrir archivo.\n");
            return;
        }

        int stock;

        /* ========== CHEVROLET ========== */
        printf("\n--- CHEVROLET ---\n");

        printf("Stock Chevrolet Onix (Nuevo): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){1,"Chevrolet","Onix","Auto",0,19999,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        printf("Stock Chevrolet Groove (Nuevo): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){2,"Chevrolet","Groove","SUV",0,27999,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        printf("Stock Chevrolet D-Max (Usado): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){3,"Chevrolet","D-Max","Camioneta",1,24000,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        /* ========== TOYOTA ========== */
        printf("\n--- TOYOTA ---\n");

        printf("Stock Toyota Yaris (Nuevo): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){4,"Toyota","Yaris","Auto",0,20000,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        printf("Stock Toyota Corolla Cross (Nuevo): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){5,"Toyota","Corolla Cross","SUV",0,31000,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        printf("Stock Toyota Hilux (Usado): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){6,"Toyota","Hilux","Camioneta",1,28000,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        /* ========== HONDA ========== */
        printf("\n--- HONDA ---\n");

        printf("Stock Honda Civic (Nuevo): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){7,"Honda","Civic","Auto",0,42990,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        printf("Stock Honda CR-V (Usado): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){8,"Honda","CR-V","SUV",1,35000,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        printf("Stock Honda HR-V (Nuevo): ");
        stock = leerEnteroConRango(0,50);
        v = (Vehiculo){9,"Honda","HR-V","SUV",0,32000,stock};
        fwrite(&v,sizeof(Vehiculo),1,f);

        fclose(f);
        printf("\nStock ingresado correctamente.\n");
    }

float calcularCredito(float precio){
        int opcion, meses;
        printf("\nSeleccione forma de pago:\n");
        printf("1. Contado\n");
        printf("2. Credito (máximo 60 meses, 5%% de interes anual)\n");
        printf(">> ");

        opcion = leerEnteroConRango(1,2);

        if(opcion == 1){
            printf("Pago al contado: $%.2f\n", precio);
            return precio;
        } else {
            printf("Ingrese cantidad de meses para el credito (1-60): ");
            meses = leerEnteroConRango(1,60);

            float interes = precio * 0.05 * (meses / 12.0); 
            float total = precio + interes;

            printf("\nPago a credito %d meses:\n", meses);
            printf("Precio base: $%.2f\n", precio);
            printf("Interes (5%% anual proporcional): $%.2f\n", interes);
            printf("Total a pagar: $%.2f\n", total);
            printf("Cuotas mensuales: $%.2f\n", total / meses);

            return total;
        }
    }

