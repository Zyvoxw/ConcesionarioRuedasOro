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


void leerCadena(char *cadena, int n);
int leerEnteroConRango(int inicio, int fin);
int validarCedula(char *cedula);

int soloLetras(char *cadena);


int menu();
int menuMarcas();
int seleccionarEstado();


void registrarCliente();
int existeClientePorID(int id);

void ingresarStockPorMarca();

Vehiculo buscarVehiculos();
void cargarVehiculosIniciales();


void venderVehiculo();
void listarVentas();
void listarClientes();

Fecha ingresarFecha();
float calcularCredito(float precio);
