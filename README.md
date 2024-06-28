# Fut-test en C

Este proyecto es un juego de preguntas y respuestas desarrollado en lenguaje C. El objetivo es proporcionar una plataforma donde los usuarios puedan jugar solos o en modo versus, respondiendo preguntas de diferentes dificultades y tipos.

## Instrucciones para Compilar y Ejecutar

Para una configuración y ejecución rápida, sigue estos pasos:

1. **Visita Repl.it:**
   Ve a [Repl.it](https://repl.it/) en tu navegador web.

2. **Inicia Sesión o Crea una Cuenta:**
   Si ya tienes una cuenta, inicia sesión. De lo contrario, crea una nueva cuenta.

3. **Importa el Repositorio:**
   En tu dashboard, selecciona "New Repl" y elige "Import from GitHub".

4. **Pega la URL del Repositorio:**
   Pega la URL del repositorio en el campo proporcionado: `https://github.com/laprank/proyecto.git`.

5. **Configura el Entorno:**
   Repl.it clonará el repositorio y configurará un entorno de ejecución automáticamente.

6. **Compila y Ejecuta el Programa:**
   Una vez que Repl.it haya finalizado la configuración, busca el archivo principal del programa (por ejemplo, `main.c`) y presiona el botón "Run" para compilar y ejecutar el programa.

## Funcionalidades

El programa permite jugar a un juego de preguntas y respuestas con varias opciones y modos de juego:

- **Modo Individual**: Permite a un solo jugador responder preguntas y registrar su puntaje.
- **Modo Versus**: Permite a dos jugadores competir entre sí respondiendo preguntas alternadamente.
- **Configuración**: Permite configurar la cantidad de preguntas a responder.
- **Tabla de Líderes**: Muestra los puntajes más altos por cada dificultad.
- **Ayuda**: Proporciona instrucciones y contacto para soporte técnico.

## Ejemplo de Uso

### Menú Principal:

Al ejecutar el programa, verás un menú con las siguientes opciones:

1. **Modo Individual**:
   Selecciona esta opción para jugar individualmente. El programa pedirá tu nombre, seleccionará la dificultad y te permitirá responder preguntas. Al finalizar, se registrará tu puntaje si está entre los más altos.
   - Has seleccionado la opción 1: Modo Individual

2. **Modo Versus**:
   Selecciona esta opción para jugar en modo versus. El programa pedirá los nombres de los dos jugadores, seleccionará la dificultad y permitirá a los jugadores responder preguntas alternadamente. Al finalizar, se mostrará el puntaje de cada jugador y el ganador.
   - Has seleccionado la opción 2: Modo Versus

3. **Configuración**:
   Selecciona esta opción para configurar la cantidad de preguntas a responder en cada partida.
   - Has seleccionado la opción 3: Configuración

4. **Tabla de Líderes**:
   Selecciona esta opción para ver los puntajes más altos por cada dificultad.
   - Has seleccionado la opción 4: Tabla de Líderes

5. **Ayuda**:
   Selecciona esta opción para ver las instrucciones y detalles de contacto para soporte técnico.
   - Has seleccionado la opción 5: Ayuda

6. **Salir**:
   Selecciona esta opción para cerrar el programa.
   - Has seleccionado la opción 6: Salir


### Modo Individual:

El programa permitirá al jugador responder preguntas, mostrará los resultados y actualizará el puntaje acumulado. Al finalizar, se registrará el puntaje si está entre los más altos.

### Modo Versus:

El programa alternará entre los jugadores, permitiendo a cada uno responder preguntas. Mostrará los resultados y actualizará los puntajes acumulados. Al finalizar, se mostrará el ganador.

## Contribuciones

### Franco Ugarte:
- Implementó las estructuras de datos de los TDAs.
- realizo las funciones de cargar preguntas en el mapa y generar lista de preguntas.
- supervisó la realizacion de todas la funciones
- Diseñó y redactó la mayoria del README.
### Maurico Perez:
- Implemento los menus de la app.
- Implemento el apartado de ayuda y congifuracion.
### Joshua Leon:
- Implemento el modo individual.
- Redacto partes del README.
### Francisco Henriquez
- Implemento el modo versus
- Ayudo en la logica de los puntajes

---

## Problemas Conocidos

- La funcionalidad para agregar nuevas preguntas y respuestas está en desarrollo.
- La comparacion de respuestas pide demaciada presicion por ende se implemento para la comodidad de la revision un printf para mostrar la respuesta correcta(se recomienda copiarla con el click izquerdo)

## Áreas de Mejora

- Mejorar la precisión y variedad de las preguntas.
- Mejorar la comparacion de la respuesta del usuario con la respuesta correcta.