# 🚪 Sistema Porta Inteligente

🔗 **[Acesse o Tinkercad do projeto](https://www.tinkercad.com/things/gUvdTCMmGYw-sistema-porta-inteligente)**

Este projeto é um protótipo funcional de um sistema de automação de portas desenvolvido no **Tinkercad**. Ele utiliza um sensor ultrassônico para detectar a aproximação de usuários, controlando a abertura via motor DC e fornecendo feedback visual através de LEDs e um display LCD I2C.

O diferencial deste sistema é a implementação de uma **lógica de estado para emergência**, que permite interromper imediatamente qualquer operação do motor através de um botão físico, garantindo a segurança total do usuário.

Tudo foi simulado e testado utilizando **C++ (Arduino)** e componentes eletrônicos padrão de mercado!

---

## 🚀 Funcionalidades

* **Detecção de Proximidade:** Monitoramento em tempo real da distância através do sensor HC-SR04 (precisão baseada em ondas sonoras).
* **Controle de Acesso Automatizado:** Abertura automática ("Door Unlocked") quando um objeto é detectado a menos de 100cm.
* **Interface LCD Inteligente:** Display 16x2 que exibe o status da porta, distância em centímetros e alertas críticos de segurança.
* **Sistema de Parada de Emergência:** Botão com lógica de "latch" (trava) que interrompe o motor e bloqueia o sistema com um comando prioritário.
* **Feedback Visual por LEDs:** Indicadores luminosos claros para estados de operação:
    * **Verde:** Acesso liberado/Porta aberta.
    * **Vermelho:** Porta fechada ou Sistema em Emergência.
* **Controle de Velocidade PWM:** Gerenciamento da potência do motor via transistor para uma operação suave e controlada.

---

## 🛠️ Tecnologias Utilizadas

* **C++ (Arduino SDK):** Linguagem base para o desenvolvimento da lógica de controle e manipulação de hardware.
* **LiquidCrystal I2C:** Biblioteca para comunicação simplificada com o display LCD via protocolo I2C (apenas 2 fios de dados).
* **Hardware de Automação:**
    * **HC-SR04:** Sensor de ondas ultrassônicas para medição de distância.
    * **Motor DC & Transistor NPN:** Atuador de movimentação com driver de potência.
    * **Display LCD 16x2:** Interface Homem-Máquina (HMI) para feedback de status em tempo real.
* **Tinkercad:** Plataforma utilizada para a prototipagem eletrônica e simulação do circuito.

---

## 📝 Como funciona o Código?

O código foi desenvolvido com foco em legibilidade, utilizando comentários explicativos para que até pessoas leigas possam entender a lógica:
1.  **Leitura:** O sensor emite um som inaudível e calcula o tempo de retorno para medir a distância.
2.  **Decisão:** Se a distância for menor que o limite, o motor é acionado.
3.  **Prioridade:** O botão de emergência possui interrupção lógica, ou seja, se acionado, ele ignora o sensor e trava o sistema imediatamente por segurança.