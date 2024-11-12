#include "kernel/types.h"
#include "user/user.h"
#include "kernel/memlayout.h"

int main() {
    // Reserva una página de memoria (4096 bytes)
    char *addr = sbrk(4096);
    if (addr == (char *)-1) {
        printf("Error: no se pudo asignar memoria\n");
        exit(1);
    }

    // Escribir en la memoria reservada
    printf("Escribiendo 'Hola' en la memoria\n");
    strcpy(addr, "Hola");

    // Marcar la página como solo lectura
    printf("Aplicando mprotect para hacer la memoria solo lectura\n");
    if (mprotect(addr, 1) < 0) {
        printf("Error en mprotect\n");
        exit(1);
    }

    // Intento de escritura: esto debería fallar si la protección está funcionando
    printf("Intentando escribir en memoria protegida...\n");
    if (fork() == 0) {
        addr[0] = 'A';  // Esto debería causar un fallo de segmentación
        printf("Error: La escritura en memoria protegida no falló\n");
        exit(1);
    } else {
        wait(0);
    }

    // Quitar la protección
    printf("Aplicando munprotect para restaurar permisos de escritura\n");
    if (munprotect(addr, 1) < 0) {
        printf("Error en munprotect\n");
        exit(1);
    }

    // Intento de escritura después de quitar la protección
    printf("Intentando escribir en memoria después de quitar protección\n");
    addr[0] = 'B';  // Esto debería funcionar
    printf("Escritura exitosa: %s\n", addr);

    exit(0);
}
