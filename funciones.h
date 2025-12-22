typedef struct {
    int id;
    char nombre[50];
    char cedula[20];
} Cliente;

typedef struct {
    int id;
    char marca[30];
    char modelo[30];
    char tipo[20];   
    int usado;       
    float precio;
    int cantidad;  
} Vehiculo;

typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

typedef struct {
    Cliente cliente;
    Vehiculo vehiculo;
    Fecha fechaVenta;
    float totalPagado;
} Venta;


/* Utilidades */
void leerCadena(char *cadena, int n);
int leerEnteroConRango(int inicio, int fin);
int soloLetras(char *cadena);

/* Menús */
int menu();
int menuMarcas();
int seleccionarEstado();

/* Clientes */
void registrarCliente();

/* Vehículos */
void buscarVehiculos();
void ingresarCantidadVehiculo();
void cargarVehiculosIniciales();

/* Ventas */
void venderVehiculo();
void listarVentas();

/* Fecha */
Fecha ingresarFecha();


