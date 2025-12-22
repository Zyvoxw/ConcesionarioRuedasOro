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
            printf("2. Ingresar cantidad de vehículos\n");
            printf("3. Buscar vehículos\n");
            printf("4. Vender vehículo\n");
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

        printf("Cédula: ");
        leerCadena(c.cedula, 20);

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

    

    void buscarVehiculos(){
        Vehiculo v;
        FILE *f = fopen("vehiculos.dat", "rb");
        if(!f){
            printf("No se pudo abrir el archivo.\n");
            return;
        }

        
        int opMarca = menuMarcas();
        char marca[30];

        if(opMarca == 1) strcpy(marca, "Chevrolet");
        else if(opMarca == 2) strcpy(marca, "Toyota");
        else strcpy(marca, "Honda");

        
        int usado = seleccionarEstado();  

        printf("\nVehículos disponibles (%s - %s):\n",
            marca, usado ? "Usado" : "Nuevo");
        printf("----------------------------------------\n");

        int encontrados = 0;   

      
        while(fread(&v, sizeof(Vehiculo), 1, f)){
            if(v.cantidad > 0 &&
            strcmp(v.marca, marca) == 0 &&
            v.usado == usado){

                printf("ID:%d | %s | %s | $%.2f | Stock:%d\n",
                    v.id,
                    v.modelo,
                    v.tipo,
                    v.precio,
                    v.cantidad);

                encontrados = 1;
            }
        }

        if(!encontrados){
            printf("No hay vehículos con esas características.\n");
        }

        fclose(f);
    }

    void venderVehiculo(){
       Cliente c;
        Vehiculo v;
        Venta venta;
        int idVehiculo;
        int clienteEncontrado = 0;
        int vehiculoEncontrado = 0;

        FILE *fc = fopen("clientes.dat", "rb");
        FILE *fv = fopen("vehiculos.dat", "rb+");
        FILE *fven = fopen("ventas.dat", "ab");

        if (!fc || !fv || !fven) {
            printf("Error al abrir los archivos.\n");
            if (fc) fclose(fc);
            if (fv) fclose(fv);
            if (fven) fclose(fven);
            return;
        }

        /* ================== BUSCAR CLIENTE ================== */
        printf("Ingrese ID del cliente: ");
        venta.cliente.id = leerEnteroConRango(1, 9999);

        while (fread(&c, sizeof(Cliente), 1, fc)) {
            if (c.id == venta.cliente.id) {
                venta.cliente = c;
                clienteEncontrado = 1;
                break;
            }
        }

        if (!clienteEncontrado) {
            printf("Cliente no encontrado.\n");
            fclose(fc);
            fclose(fv);
            fclose(fven);
            return;
        }

        /* ================== MOSTRAR VEHÍCULOS ================== */
        buscarVehiculos();

        printf("\nIngrese ID del vehículo a vender: ");
        idVehiculo = leerEnteroConRango(1, 9999);

        rewind(fv);

        /* ================== BUSCAR Y ACTUALIZAR VEHÍCULO ================== */
        while (fread(&v, sizeof(Vehiculo), 1, fv)) {
            if (v.id == idVehiculo && v.cantidad > 0) {

                v.cantidad--;               
                venta.vehiculo = v;

                fseek(fv, -(long)sizeof(Vehiculo), SEEK_CUR);
                fwrite(&v, sizeof(Vehiculo), 1, fv);

                vehiculoEncontrado = 1;
                break;
            }
        }

        if (!vehiculoEncontrado) {
            printf("Vehículo no disponible o no existe.\n");
            fclose(fc);
            fclose(fv);
            fclose(fven);
            return;
        }

        printf("\nIngrese la fecha de la venta:\n");
        venta.fechaVenta = ingresarFecha();   // ← AQUÍ se integra la fecha
        venta.totalPagado = venta.vehiculo.precio;

        /* ================== REGISTRAR VENTA ================== */
        fwrite(&venta, sizeof(Venta), 1, fven);

        fclose(fc);
        fclose(fv);
        fclose(fven);

        printf("Venta registrada correctamente.\n");
    }

    void listarVentas(){
        Venta v;
        FILE *f = fopen("ventas.dat", "rb");
        if(!f) return;

        printf("\n--- VENTAS REGISTRADAS ---\n");
        while(fread(&v, sizeof(Venta), 1, f)){
            printf("Cliente: %s\n", v.cliente.nombre);
            printf("Vehículo: %s %s\n",
                v.vehiculo.marca,
                v.vehiculo.modelo);
            printf("Total pagado: $%.2f\n", v.totalPagado);
            printf("Fecha: %02d/%02d/%d\n",
                v.fechaVenta.dia,
                v.fechaVenta.mes,
                v.fechaVenta.anio);
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

