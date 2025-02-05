# Simulador de CPU x86 - Documentação do Projeto

Este projeto é um simulador básico de uma CPU x86, implementado em C, que executa um conjunto limitado de instruções, manipulando registradores e memória. A seguir está uma explicação sobre a estrutura do código, as funções e como o simulador se comporta.

## Estrutura do Código

### 1. **Definições e Tipos**

- **MEMORY_SIZE**: Define o tamanho da memória simulada. O valor é 1024 bytes.
- **Cpu_Registers**: Estrutura que contém os registradores da CPU, como `AX`, `BX`, `CX`, `DX`, entre outros.
- **Instructions (Enum)**: Enumeração que define os opcodes das instruções suportadas, como `MOV_IMM`, `ADD`, `SUB`, etc.
- **memory**: Um array de 1024 bytes que simula o espaço de memória.
- **Register**: Uma variável global do tipo `Cpu_Registers`, que armazena os valores dos registradores da CPU.
- **regs**: Array de ponteiros para os registradores, permitindo o acesso direto aos registradores através de um índice.

### 2. **Funções do Código**

#### `fetch()`
- Função que lê o próximo byte da memória (o opcode) e incrementa o ponteiro de instrução (`IP`).
- Exemplo de uso: Em cada ciclo de execução, o `fetch()` é chamado para recuperar o próximo byte de código da memória.

#### `reset_registers()`
- Função que inicializa todos os registradores da CPU a 0, e configura o ponteiro da pilha (`SP`) para o final da memória simulada.
- Reseta o valor do registrador de flags (`FLAGS`) também.

#### `load_program(uint8_t *program, size_t size)`
- Função que carrega um programa na memória simulada.
- O programa é copiado para o array `memory`, e o ponteiro de instrução (`IP`) é resetado para 0.

#### `execute()`
- Função principal de execução do simulador.
- Um loop infinito que busca (fetch) e executa instruções na memória até encontrar uma instrução `RET`, que termina a execução.
- Dependendo do opcode da instrução, a função realiza a operação apropriada, como `MOV`, `ADD`, `SUB`, `MOV_REG`, `TEST`, etc.

## Instruções Suportadas

### 1. **MOV_IMM (0xB8)**
A instrução `MOV_IMM` move um valor imediato (imediato de 16 bits) para um dos registradores.

Exemplo:

```c
MOV_IMM, 0, 0x32, 0x12  // MOV AX, 0x1232
```

### 2. ADD (0x83)
A instrução ADD adiciona um valor imediato a um dos registradores.

Exemplo:
    ``` 
    ADD, 0, 0x45, 0x15  // ADD AX, 0x1545
    ```
### 3. SUB (0x84)
A instrução SUB subtrai um valor imediato de um registrador.

Exemplo:
 ```
    SUB, 1, 0x14, 0x00  // SUB BX, 0x0014
```

### 4. MOV_REG (0x89)
A instrução MOV_REG move o conteúdo de um registrador para outro.

Exemplo:
```
MOV_REG, 2, 0  // MOV CX, AX
```

### 5. TEST (0x85)
A instrução TEST compara dois registradores e ajusta as flags (ZF e CF) com base no resultado.

Exemplo:
````
TEST, 2, 0  // TEST CX, AX
````

7. RET (0xC3)
A instrução RET encerra a execução do programa, retornando do loop de execução.


# Comportamento do Simulador

### Carregamento do Programa

O programa (sequência de instruções em formato binário) é carregado na memória.
O ponteiro de instrução (IP) começa em 0, e a execução das instruções começa a partir daí.

### Execução de Instruções

O simulador executa cada instrução até encontrar uma instrução RET.
Para cada instrução, o simulador modifica os registradores da CPU e o valor das flags (FLAGS) conforme necessário.

### Pilha e Registradores

A pilha (registrador SP) é inicializada no final da memória simulada. No entanto, o código atual não realiza operações de push/pop, mas a pilha está disponível para futuras implementações.

### Flags

As flags são usadas para armazenar informações sobre o estado da execução. O simulador implementa as flags Zero Flag (ZF) e Carry Flag (CF) para indicar quando um resultado é zero ou quando houve um "carry" durante uma operação.

### Saída do Simulador
Após a execução, o simulador exibe os valores finais dos registradores e das flags.

Exemplo de Saída:
````
Instruções carregadas:
B8 00 32 12
83 00 45 15
B8 01 20 00
84 01 14 00
89 02 00 85
02 00 84 01
15 23 C3

Registradores:
AX: 0x2777
BX: 0xDCF7
CX: 0x2777
DX: 0x0000
SP: 0x03FF
BP: 0x0000

FLAGS: 0x0001
ZF (Zero Flag): Set
CF (Carry Flag): Clear
````

# Como Usar o Simulador
* Compile o código utilizando um compilador C adequado.
* Execute o programa.
* O programa carregará as instruções definidas em programa[] na memória.
* O simulador executará as instruções uma a uma e exibirá o estado dos registradores e flags ao final da execução.

# Possíveis Melhorias
* Implementação de operações de pilha (PUSH, POP).
* Suporte para mais instruções x86.
* Implementação de operações de manipulação de memória (como MOV de memória para registrador e vice-versa).
* Melhoria na visualização do estado da CPU e na adição de mais flags.
