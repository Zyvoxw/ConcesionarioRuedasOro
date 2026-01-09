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
    int disponible;
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
    char vendedor[50];
    float comision;
} Venta;


void leerCadena(char *cadena, int n);
int leerEnteroConRango(int inicio, int fin);
float leerFlotanteConRango(float inicio, float fin);

int validarCedula(char *cedula);
int soloLetras(char *cadena);

int menu();

void registrarCliente();
int existeClientePorID(int id);
void listarClientes();
void menuVehiculos();
void añadirVehiculo();
void cambiarEstadoVehiculo();
void cargarVehiculosIniciales();
void listarVehiculos();
void venderVehiculo();
void listarVentas();
void listarVehiculosTodos();
int seleccionarEstado();

Fecha ingresarFecha();
float calcularCredito(float precio);
