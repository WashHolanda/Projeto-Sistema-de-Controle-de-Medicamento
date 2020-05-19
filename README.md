# **Título do projeto: Sistema de controle e gerenciamento de medicamentos controlados**
### Nome do aluno: Washington Holanda de Oliveira
**1) Resumo do projeto** 
O tema do projeto foi escolhido com base nos temas propostos para os alunos.
Entre todos eles um sistema de gerenciamento de medicamentos controlados chamou
a atenção devido a sua importancia nos dias atuais, pois em muitas situações o
paciente acaba esquecendo ou atrasando o remédio que está sendo tomado devido a
correria, o que acaba atrasando o tratamento, e no pior dos casos criando bactérias
resistentes aos medicamentos por uma má administração desses remédios.
O sistema será desenvolvido de forma que o paciente tenha um auxílio nesse
gerenciamento, sendo notificado de diversas maneiras sobre momento correto que
cada medicamento deve ser tomado, além de ser informado sobre a dosagem correta a
ser tomada e também sobre quando o remédio deve ser reposto.

O projeto foi dividido em duas partes sendo a primeira delas um sistema mais simples 
com algumas funcionalidades a menos, utilizando apenas o Arduino. Já a segunda parte
engloba todas as funcionalidades propostas fazendo uso do Arduino e no PIC para imple-
mentá-las.

**2) Lista das principais funcionalidades que são implementadas no
sistema:**
**Parte 1: Arduino**
- Sinalização luminosa indicando qual medicamento é o que deve ser tomado;
- Informações como nome do medicamento e dosagem sendo informados no display
do sistema;
- Envio de notificação para o smartphone do usuário quando algum
medicamento deve ser tomado;
- Botão no qual o usuário irá informar o sistema que o medicamento foi
tomado.

**Parte 2: Arduino + PIC**
- Aviso sonoro para horários em que algum medicamento deve ser tomado;
- Informações sobre a quantidade de medicamentos que ainda restam no sistema;
- Botões pelos quais o usuário informa o sistema sobre o aumento na quantidade
de certo remédio;
- Envio de notificação para smartphone do usuário quando algum remédio precisa
ser reposto.

**3) Lista de componentes básicos**
- Arduino Mega 2560 – Microcontrolador;
- Kit PIC18F4520 - Kit com microcontrolador secundário (buzzer, botões e display);
- Shield Arduino - Módulo RTC de Precisão DS1307 – Relógio;
- Shield Arduino - Módulo Bluetooth HC-05 – Comunicação com Smartphone;
- Display LCD 20x4 Azul– Interface visual;
- Leds - Sinalização Luminosa;
- Resistores de 300Ω- Controle de corrente dos LED’s;
- Potenciometro de 10KΩ- Controle de contraste do LCD;
- Protoboard - Matriz para prototipagem do projeto;
- Jumpers - Conectores básicos.
