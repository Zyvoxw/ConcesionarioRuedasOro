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
    Cliente cliente;
    Vehiculo vehiculo;
} Venta;

void leerCadena(char *cadena, int n);
int leerEnteroConRango(int inicio, int fin);
float leerFlotanteConRango(float inicio, float fin);
int soloLetras(char *cadena);
int menu();
void registrarCliente();
int seleccionarEstado();
int obtenerTiposPorEstado(int usado, char tipos[][20]);
void listarVehiculosPorEstadoYTipo(int usado, char *tipo);
void buscarVehiculos();
void cargarVehiculosIniciales();
void venderVehiculo();
void listarVentas();

