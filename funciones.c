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
        if(!((cadena[i] >= 'A' && cadena[i] <= 'Z') || (cadena[i] >= 'a' && cadena[i] <= 'z') ||
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
        printf("2. Añadir/editar estado de Vehiculos\n");
        printf("3. listar vehiculos\n");
        printf("4. Vender vehiculo\n");
        printf("5. Listar ventas\n");
        printf("6. Listar Clientes\n");
        printf("7. Salir\n");
        printf(">> ");
        op = leerEnteroConRango(1,7);
    }while(op < 1 || op > 7);
    return op;
}

int existeClientePorID(int id){
    Cliente c;
    FILE *f = fopen("clientes.dat", "rb");
    if(!f) return 0;

    while(fread(&c, sizeof(Cliente), 1, f)){
        if(c.id == id){
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

void menuVehiculos(){
    int op;
    do{
        printf("\n--- GESTION DE VEHICULOS ---\n");
        printf("1. Añadir vehiculo\n");
        printf("2. Cambiar estado (Disponible / No disponible)\n");
        printf("3. Volver\n");
        printf(">> ");
        op = leerEnteroConRango(1,3);

        switch(op){
            case 1: añadirVehiculo();
             break;
            case 2: cambiarEstadoVehiculo();
             break;
        }
    }while(op != 3);
}



void registrarCliente(){
    Cliente c;
    FILE *f = fopen("clientes.dat", "ab");
    if(!f) return;
    do{
    printf("ID cliente: ");
    c.id = leerEnteroConRango(1,9999);

    if(existeClientePorID(c.id)){
        printf("El ID ya esta registrado.\n");
    }

    }while(existeClientePorID(c.id));

    do{
        printf("Nombre: ");
        leerCadena(c.nombre, 50);
        if(!soloLetras(c.nombre))
            printf("solo se aceptan letras y espacios.\n");
         }while(!soloLetras(c.nombre));
        do{
            printf("Cedula: ");
            leerCadena(c.cedula, 20);
            if(!validarCedula(c.cedula)){
                printf("la cedula debe tener exactamente 10 numeros y no letras.\n");
            }

        }while(!validarCedula(c.cedula));
        fwrite(&c, sizeof(Cliente), 1, f);
        fclose(f);
        printf("Cliente registrado correctamente.\n");
}

void listarClientes(){
    Cliente c;
    FILE *f = fopen("clientes.dat", "rb");

    if(!f){
        printf("\nNo hay clientes registrados.\n");
        return;
    }

    int hayClientes = 0;

    printf("\n--- LISTADO DE CLIENTES ---\n");

    while(fread(&c, sizeof(Cliente), 1, f)){
        printf("---------------------------\n");
        printf("ID: %d\n", c.id);
        printf("Nombre: %s\n", c.nombre);
        printf("Cédula: %s\n", c.cedula);
        hayClientes = 1;
    }

    if(!hayClientes){
        printf("\nNo hay clientes registrados.\n");
    }

    fclose(f);
}


int seleccionarEstado(){
    int op;
    printf("Seleccione estado del vehiculo:\n");
    printf("1. Nuevo\n");
    printf("2. Usado\n");
    printf(">> ");
    op = leerEnteroConRango(1,2);
    return (op == 2);
}

void cambiarEstadoVehiculo(){
    Vehiculo v;
    FILE *f = fopen("vehiculos.dat", "rb+");
    if(!f){
        printf("No hay vehiculos.\n");
        return;
    }

    listarVehiculosTodos();

    printf("\nIngrese ID del vehiculo: ");
    int id = leerEnteroConRango(1,9999);

    rewind(f);

    while(fread(&v, sizeof(Vehiculo), 1, f)){
        if(v.id == id){
            printf("Estado actual: %s\n",
                   v.disponible ? "Disponible" : "No disponible");

            printf("Nuevo estado (1 = Disponible, 0 = No disponible): ");
            v.disponible = leerEnteroConRango(0,1);

            fseek(f, -sizeof(Vehiculo), SEEK_CUR);
            fwrite(&v, sizeof(Vehiculo), 1, f);

            printf("Estado actualizado correctamente.\n");
            fclose(f);
            return;
        }
    }

    printf("Vehiculo no encontrado.\n");
    fclose(f);
}


void añadirVehiculo(){

    Vehiculo v, aux;
    FILE *f = fopen("vehiculos.dat", "ab");
    FILE *f2;

    if(!f){
        printf("Error al abrir archivo.\n");
        return;
    }

    int ultimoID = 0;
    f2 = fopen("vehiculos.dat", "rb");
    if(f2){
        while(fread(&aux, sizeof(Vehiculo), 1, f2)){
            ultimoID = aux.id;
        }
        fclose(f2);
    }
    v.id = ultimoID + 1;

    printf("\n--- NUEVO VEHICULO ---\n");
    printf("ID asignado: %d\n", v.id);

    printf("Marca: ");
    leerCadena(v.marca, 30);

    printf("Modelo: ");
    leerCadena(v.modelo, 30);

    printf("Tipo (Auto / SUV / Camioneta): ");
    leerCadena(v.tipo, 20);

    v.usado = seleccionarEstado();

    printf("Precio: $");
    v.precio = leerFlotanteConRango(0, 1000000);

    
    v.disponible = 1;   

    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);

    printf("\nVehiculo agregado correctamente.\n");
}

void listarVehiculosTodos(){
    Vehiculo v;
    FILE *f = fopen("vehiculos.dat", "rb");

    if(!f){
        printf("No hay vehiculos registrados.\n");
        return;
    }

    printf("\n--- LISTADO COMPLETO DE VEHICULOS ---\n");
    printf("ID | Marca | Modelo | Tipo | Estado | Precio | Disponibilidad\n");
    printf("---------------------------------------------------------------\n");

    while(fread(&v, sizeof(Vehiculo), 1, f)){
        printf("%d | %s | %s | %s | %s | $%.2f | %s\n",
               v.id,
               v.marca,
               v.modelo,
               v.tipo,
               v.usado ? "Usado" : "Nuevo",
               v.precio,
               v.disponible ? "Disponible" : "No disponible");
    }

    fclose(f);
}


void listarVehiculos(){
    Vehiculo v;
    FILE *f = fopen("vehiculos.dat", "rb");

    if(!f){
        printf("No hay vehiculos registrados.\n");
        return;
    }

    printf("\n--- VEHICULOS DISPONIBLES ---\n");
    printf("ID | Marca | Modelo | Tipo | Estado | Precio\n");
    printf("---------------------------------------------\n");

    int hay = 0;

    while(fread(&v, sizeof(Vehiculo), 1, f)){
        if(v.disponible){
            printf("%d | %s | %s | %s | %s | $%.2f\n",
                   v.id,
                   v.marca,
                   v.modelo,
                   v.tipo,
                   v.usado ? "Usado" : "Nuevo",
                   v.precio);
            hay = 1;
        }
    }

    if(!hay){
        printf("No hay vehiculos disponibles.\n");
    }

    fclose(f);
}

void venderVehiculo(){

    Cliente c;
    Vehiculo vehiculos[100];
    Venta venta;

    int totalVehiculos = 0;
    int clienteEncontrado = 0;

    FILE *fc = fopen("clientes.dat", "rb");
    FILE *fv = fopen("vehiculos.dat", "rb");
    FILE *fven = fopen("ventas.dat", "ab");

    if(!fc || !fv || !fven){
        printf("Error al abrir archivos.\n");
        if(fc) fclose(fc);
        if(fv) fclose(fv);
        if(fven) fclose(fven);
        return;
    }

    printf("Ingrese ID del cliente: ");
    int idCliente = leerEnteroConRango(1,9999);

    while(fread(&c, sizeof(Cliente), 1, fc)){
        if(c.id == idCliente){
            venta.cliente = c;
            clienteEncontrado = 1;
            break;
        }
    }
    fclose(fc);

    if(!clienteEncontrado){
        printf("Cliente no encontrado.\n");
        fclose(fv);
        fclose(fven);
        return;
    }

    float presupuesto;
    printf("Ingrese presupuesto maximo del cliente: $");
    presupuesto = leerFlotanteConRango(0,1000000);

    /* ================= CARGAR VEHICULOS ================= */
    while(fread(&vehiculos[totalVehiculos], sizeof(Vehiculo), 1, fv)){
        totalVehiculos++;
    }
    fclose(fv);

    int opciones[100];
    int count = 0;

    printf("\nVEHICULOS DISPONIBLES SEGUN PRESUPUESTO\n");
    printf("ID | Marca | Modelo | Tipo | Estado | Precio\n");
    printf("---------------------------------------------\n");

    for(int i = 0; i < totalVehiculos; i++){
        if(vehiculos[i].disponible &&
           vehiculos[i].precio <= presupuesto){

            printf("%d | %s | %s | %s | %s | $%.2f\n",
                   vehiculos[i].id,
                   vehiculos[i].marca,
                   vehiculos[i].modelo,
                   vehiculos[i].tipo,
                   vehiculos[i].usado ? "Usado" : "Nuevo",
                   vehiculos[i].precio);

            opciones[count++] = i;
        }
    }

    if(count == 0){
        printf("\nNo hay vehiculos dentro del presupuesto.\n");
        fclose(fven);
        return;
    }


    printf("\nIngrese ID del vehiculo a vender: ");
    int idVehiculo = leerEnteroConRango(1,9999);

    int idx = -1;
    for(int i = 0; i < count; i++){
        if(vehiculos[opciones[i]].id == idVehiculo){
            idx = opciones[i];
            break;
        }
    }

    if(idx == -1){
        printf("Vehiculo invalido.\n");
        fclose(fven);
        return;
    }

    venta.vehiculo = vehiculos[idx];

    
    printf("\nIngrese fecha de la venta:\n");
    venta.fechaVenta = ingresarFecha();


    venta.totalPagado = calcularCredito(venta.vehiculo.precio);

 
    printf("\nIngrese nombre del vendedor: ");
    leerCadena(venta.vendedor, 50);

    venta.comision = venta.totalPagado * 0.10;
    printf("Comision del vendedor (10%%): $%.2f\n", venta.comision);

    vehiculos[idx].disponible = 0;

    
    fwrite(&venta, sizeof(Venta), 1, fven);
    fclose(fven);

    
    fv = fopen("vehiculos.dat", "wb");
    for(int i = 0; i < totalVehiculos; i++){
        fwrite(&vehiculos[i], sizeof(Vehiculo), 1, fv);
    }
    fclose(fv);

    printf("\nVenta registrada correctamente.\n");
}


void listarVentas() {

    Venta v;
    FILE *f = fopen("ventas.dat", "rb");

    if(!f){
        printf("\nNo se ha realizado ninguna venta.\n");
        return;
    }

    int hayVentas = 0;

    printf("\n========== LISTADO DE VENTAS ==========\n");

    while(fread(&v, sizeof(Venta), 1, f)){
        hayVentas = 1;

        printf("--------------------------------------\n");
        printf("Cliente: %s (ID: %d)\n",
               v.cliente.nombre, v.cliente.id);

        printf("Vehiculo: %s %s\n",
               v.vehiculo.marca, v.vehiculo.modelo);

        printf("Tipo: %s\n", v.vehiculo.tipo);
        printf("Estado: %s\n",
               v.vehiculo.usado ? "Usado" : "Nuevo");

        printf("Fecha de venta: %02d/%02d/%d\n",
               v.fechaVenta.dia,
               v.fechaVenta.mes,
               v.fechaVenta.anio);

        printf("Precio base: $%.2f\n", v.vehiculo.precio);
        printf("Total pagado: $%.2f\n", v.totalPagado);

        if(v.totalPagado > v.vehiculo.precio){
            printf("Forma de pago: Credito\n");
        } else {
            printf("Forma de pago: Contado\n");
        }

        printf("Vendedor: %s\n", v.vendedor);
        printf("Comision (10%%): $%.2f\n", v.comision);
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
    {1,"Chevrolet","Onix","Auto",0,19999,1},
    {2,"Chevrolet","D-Max","Camioneta",1,24000,1},
    {3,"Toyota","Yaris","Auto",0,20000,1},
    {4,"Toyota","Hilux","Camioneta",1,28000,1},
    {5,"Honda","CR-V","SUV",1,30000,1}
    };

    fwrite(v,sizeof(Vehiculo),5,f);
    fclose(f);
    }

Fecha ingresarFecha() {
    Fecha f;
        do {
            printf("Ingrese dia (1-31): ");
            f.dia = leerEnteroConRango(1, 31);

            printf("Ingrese mes (1-12): ");
            f.mes = leerEnteroConRango(1, 12);

            printf("Ingrese anio (2000-2026): ");
            f.anio = leerEnteroConRango(2000, 2100);

            if (f.mes == 2 && f.dia > 29) {
                printf("Febrero no tiene mas de 29 dias.\n");
            }

            if ((f.mes == 4 || f.mes == 6 || f.mes == 9 || f.mes == 11) && f.dia > 30) {
                printf("Ese mes solo tiene 30 dias.\n");
            }

        } while (
            (f.mes == 2 && f.dia > 29) ||
            ((f.mes == 4 || f.mes == 6 || f.mes == 9 || f.mes == 11) && f.dia > 30)
        );

        return f;
    }


float calcularCredito(float precio){
        int opcion, meses;
        printf("\nSeleccione forma de pago:\n");
        printf("1. Contado\n");
        printf("2. Credito (maximo 36 meses, 10%% de interes anual)\n");
        printf(">> ");

        opcion = leerEnteroConRango(1,2);

        if(opcion == 1){
            printf("Pago al contado: $%.2f\n", precio);
            return precio;
        } else {
            printf("Ingrese cantidad de meses para el credito (1-36): ");
            meses = leerEnteroConRango(1,36);

            float interes = precio * 0.10 * (meses / 12.0); 
            float total = precio + interes;

            printf("\nPago a credito %d meses:\n", meses);
            printf("Precio base: $%.2f\n", precio);
            printf("Interes (10%% anual proporcional): $%.2f\n", interes);
            printf("Total a pagar: $%.2f\n", total);
            printf("Cuotas mensuales: $%.2f\n", total / meses);

            return total;
        }
    }
