# Sistema de Save na Memória Flash com Raspberry Pi Pico

Este projeto é um exemplo de como implementar um sistema para gravar, ler e apagar dados na memória flash interna do microcontrolador RP2040, utilizando a placa BitDogLab e o Pico SDK. A demonstração persiste dados de progresso de um jogo hipotético (nível e pontuação máxima), garantindo que as informações não sejam perdidas ao desligar a placa.

O código é modularizado em `saveSystem.c` e `saveSystem.h`, facilitando a sua integração em outros projetos.

---

## ⚙️ Funcionalidades

-   **Gravação de Dados:** Salva um buffer de dados em um endereço específico da memória flash.
-   **Leitura de Dados:** Carrega os dados previamente salvos da memória flash para um buffer em RAM.
-   **Limpeza de Dados:** Apaga de forma segura o setor da memória flash onde os dados estão armazenados.
-   **Empacotamento de Dados:** Demonstra como encapsular diferentes tipos de dados (como `uint8_t` e `uint16_t`) em um único buffer de bytes para gravação.
-   **Operações Seguras:** Utiliza desativação de interrupções durante as operações de escrita e exclusão para garantir a integridade dos dados e evitar corrupção.

---

## 📂 Estrutura do Projeto

O código-fonte está organizado da seguinte forma para promover a reutilização:

-   `main.c`: Código principal de exemplo. Demonstra o ciclo completo de encapsular, salvar, carregar, recuperar e limpar os dados.
-   `saveSystem.h`: Arquivo de cabeçalho da biblioteca. Define a interface pública com as funções `saveProgress()`, `loadProgress()` e `clearSaveData()`.
-   `saveSystem.c`: Arquivo de implementação da biblioteca. Contém a lógica de baixo nível para interagir com o hardware de flash do RP2040.

---

## 📋 Pré-requisitos

-   **Hardware:**
    -   Placa BitDogLab (com RP2040)
-   **Software:**
    -   Ambiente de desenvolvimento C/C++ para Pico SDK devidamente configurado.

---

## 🚀 Como compilar e executar

### 1. Clone o repositório e compile o projeto:

Siga os passos padrão para compilar um projeto com o Pico SDK.

```bash
# Clone o repositório
git clone https://github.com/LabirasIFPI/bitdoglab_memoria_flash
cd bitdoglab_memoria_flash

# Crie e acesse o diretório de build
mkdir build && cd build

# Gere os arquivos de compilação com o CMake
cmake ..

# Compile o projeto
make
```

### 2. Grave o arquivo .uf2 gerado na Pico:

-   Conecte a Pico ao seu computador enquanto mantém o botão **BOOTSEL** pressionado.
-   A placa será montada como um dispositivo de armazenamento em massa.
-   Copie o arquivo `.uf2` gerado (localizado na pasta `build`) para a unidade USB da Pico. A placa irá reiniciar automaticamente.

### 3. Execute e visualize o resultado:

-   Abra um monitor serial (ex: PuTTY, Serial Monitor do VS Code, etc.) para se conectar à porta serial da Pico (baud rate: 115200).
-   Ao iniciar, o programa irá:
    1.  Gravar os valores `level = 5` e `highScore = 12345` na flash.
    2.  Ler os dados de volta da flash.
    3.  Exibir "Level carregado: 5" e "HighScore carregado: 12345" no terminal.
    4.  Aguardar 5 segundos e, em seguida, limpar os dados da flash.

---

## 💡 Conceitos Importantes

-   **Endereço de Destino na Flash (`FLASH_TARGET_OFFSET`):**
    -   No código, definimos um offset de `(256 * 1024)`, o que significa que os dados são salvos a 256KB do início da memória flash.
    -   É crucial escolher um endereço que não conflite com o seu código de programa. O espaço após o seu programa é geralmente seguro para uso.

-   **Natureza da Memória Flash:**
    -   A memória flash precisa ser **apagada** antes de poder ser escrita novamente. Uma operação de apagar define todos os bits de uma região (um setor) como '1'. A operação de programar (escrever) só pode mudar os bits de '1' para '0'.
    -   É por isso que a função `saveProgress` chama `flash_range_erase()` antes de `flash_range_program()`.

-   **Proteção contra Interrupções:**
    -   As operações de apagar e escrever na flash levam tempo. Se uma interrupção (IRQ) ocorresse no meio de uma dessas operações e o código da interrupção precisasse ler da flash, o sistema poderia travar.
    -   Para evitar isso, desativamos as interrupções com `save_and_disable_interrupts()` antes de modificar a flash e as restauramos com `restore_interrupts()` logo após, garantindo que a operação seja atômica e segura.