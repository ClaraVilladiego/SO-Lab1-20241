#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 1024

// Función para obtener el nombre del archivo a partir de la ruta
const char *obtener_nombre_archivo(const char *ruta) {
    const char *nombre = strrchr(ruta, '/');  // Buscar el último '/' (Unix/Linux)
    return (nombre) ? nombre + 1 : ruta;      // Si encuentra el separador, retorna lo que viene después
}

// Función para invertir las líneas del archivo
void reverse_lines(FILE *input, FILE *output)
{
    char **lines = NULL;
    size_t line_count = 0;
    size_t capacity = INITIAL_BUFFER_SIZE;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    lines = malloc(capacity * sizeof(char *));
    if (!lines)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // Leer todas las líneas del archivo de entrada
    while ((read = getline(&line, &len, input)) != -1)
    {
        if (line_count >= capacity)
        {
            capacity *= 2;
            char **temp = realloc(lines, capacity * sizeof(char *));
            if (!temp)
            {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            lines = temp;
        }

        if (read > 0 && line[read - 1] == '\n')
        {
            line[read - 1] = '\0';
        }

        lines[line_count] = strdup(line);
        if (!lines[line_count])
        {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        line_count++;
    }

    // Liberar la memoria del buffer de línea
    free(line);

    // Imprimir las líneas en orden inverso
    for (size_t i = line_count; i-- > 0;)
    {
        fprintf(output, "%s\n", lines[i]);
        free(lines[i]);
    }

    // Liberar la memoria del array de líneas
    free(lines);
}

int main(int argc, char *argv[])
{
    FILE *input = stdin;
    FILE *output = stdout;

    // Verificar el número de argumentos
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    // Abrir el archivo de entrada si se proporciona
    if (argc >= 2)
    {
        input = fopen(argv[1], "r");
        if (!input)
        {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            return 1;
        }
    }

    // Abrir el archivo de salida si se proporciona
    if (argc == 3)
    {
        // Verificar que los archivos de entrada y salida sean diferentes
        const char *nombre_input = obtener_nombre_archivo(argv[1]);
        const char *nombre_output = obtener_nombre_archivo(argv[2]);

        if (strcmp(nombre_input, nombre_output) == 0)
        {
            fprintf(stderr, "reverse: input and output file must differ\n");
            return 1;
        }

        output = fopen(argv[2], "w");
        if (!output)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            return 1;
        }
    }

    // Invertir las líneas
    reverse_lines(input, output);

    // Cerrar los archivos si se abrieron
    if (input != stdin)
        fclose(input);
    if (output != stdout)
        fclose(output);

    return 0;
}