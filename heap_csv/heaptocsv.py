#Para v = 2^10

# Lee el archivo tiempos_dijkstra_heap.txt
with open('tiempos_dijkstra_heap.txt', 'r') as file:
    lines = file.readlines()

# Abre el archivo CSV para escribir
with open('tiempos_heap_2^10.csv', 'w') as csv_file:
    # Cabecera
    csv_file.write('aristas, tiempo\n')

    # Recorre cada línea del archivo original
    for i, line in enumerate(lines[:150]):
        # Divide la línea en sus componentes
        parts = line.split()
        # Obtiene E y T según la estructura conocida
        aristasAux = parts[9][:-1]  # parts[10] corresponde a aristas
        aristas = int(aristasAux[0]) ** int(aristasAux[2]+aristasAux[3]) # Se eleva 2 al exponente correspondiente, 16,...,18
        tiempo = parts[10]  # parts[6] corresponde a tiempo  
        # Escribe la línea en el archivo CSV
        csv_file.write(f'{aristas}, {tiempo}\n')

print("Archivo tiempos_heap_2^10.csv generado exitosamente.")

#Para v = 2^12

# Abre el archivo CSV para escribir
with open('tiempos_heap_2^12.csv', 'w') as csv_file:
    # Cabecera
    csv_file.write('aristas, tiempo\n')

    # Recorre cada línea del archivo original
    for i, line in enumerate(lines[150:500]):
        # Divide la línea en sus componentes
        parts = line.split()
        # Obtiene E y T según la estructura conocida
        aristasAux = parts[9][:-1]  # parts[10] corresponde a aristas
        aristas = int(aristasAux[0]) ** int(aristasAux[2]+aristasAux[3]) # Se eleva 2 al exponente correspondiente, 16,...,22
        tiempo = parts[10]  # parts[6] corresponde a tiempo

        # Escribe la línea en el archivo CSV
        csv_file.write(f'{aristas}, {tiempo}\n')

print("Archivo tiempos_heap_2^12.csv generado exitosamente.")

# Para v = 2^14

# Abre el archivo CSV para escribir
with open('tiempos_heap_2^14.csv', 'w') as csv_file:
    # Cabecera
    csv_file.write('aristas, tiempo\n')

    # Recorre cada línea del archivo original
    for i, line in enumerate(lines[500:850]):
        # Divide la línea en sus componentes
        parts = line.split()
        # Obtiene E y T según la estructura conocida
        aristasAux = parts[9][:-1]  # parts[10] corresponde a aristas
        aristas = int(aristasAux[0]) ** int(aristasAux[2]+aristasAux[3]) # Se eleva 2 al exponente correspondiente, 16,...,22
        tiempo = parts[10]  # parts[6] corresponde a tiempo

        # Escribe la línea en el archivo CSV
        csv_file.write(f'{aristas}, {tiempo}\n')

print("Archivo tiempos_heap_2^14.csv generado exitosamente.")
