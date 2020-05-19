/* 
   Mapeamento de Hardware
            DS3231    -> Arduino MEGA 2560
            -----------------------------------------
            VCC pin   -> Arduino 5V
            GND pin   -> Arduino GND
            SDA pin   -> Arduino Digital 20 (SDA)  
            SCL pin   -> Arduino Digital 21 (SCL)  
            -----------------------------------------

            LCD 20x4    -> Arduino MEGA 2560
            -----------------------------------------
            RS pin   -> Arduino Digital 12
            EN pin   -> Arduino Digital 11
            D4 pin   -> Arduino Digital  5
            D5 pin   -> Arduino Digital  4
            D6 pin   -> Arduino Digital  3
            D7 pin   -> Arduino Digital  2
            -----------------------------------------

            Outros Componentes    -> Arduino MEGA 2560
            -----------------------------------------
            Button_tomou_na_hora pin -> Arduino Digital 13
            Led1 pin   -> Arduino Digital  9
            Led2 pin   -> Arduino Digital  8
            Button_tomou_atrasado pin -> Arduino Digital  6
            -----------------------------------------
*/

// --- Bibliotecas ---
#include <DS1307.h>             //Inclui a biblioteca do DS3231 Shield
#include <LiquidCrystal.h>      //Biblioteca para o display LCD

// --- Comunicação I2C ---
DS1307  rtc(SDA, SCL);

// --- Hardware do LCD ---
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// --- Variáveis ---
Time T;
int hora;
int minuto;
int segundo;
int estado;
//char estado;
int buttonPin_tomouNaHora = 13;
int buttonPin_atraso = 6;
int vetLedPin[2] = {9,8};
int control = 0;
int notf=0;
int cont_aux=0;
  char not_compra;

// --- Declaração dos Compartimentos de Remédios ---
typedef struct{
  char nome[30];
  char dosagem[20];
  int hora;
  int minuto;
  int segundo;
  int periodo_entre_doses[2];
  int atraso;
}Medicamento;

Medicamento remedio[2] = {{"Dramin","1 Comprimido",22,12,0,{0,5},0},{"Paracetamol","1 Comprimido",22,13,0,{0,6},0}};

// --- Configurações Iniciais ---
void setup(){
  Serial.begin(9600);   //Inicia comunicações Serial em 9600 baud rate
  Serial1.begin(9600);   //Inicia comunicações Serial 1 em 9600 baud rate
  Serial2.begin(9600);   //Inicia comunicações Serial 1 em 9600 baud rate
  pinMode(vetLedPin[0], OUTPUT);  //Inicializa o pino do Led 1 como de saída
  pinMode(vetLedPin[1], OUTPUT);  //Inicializa o pino do Led 2 como de saída
  pinMode(buttonPin_tomouNaHora, INPUT);      //Inicializa o pino do botão como de entrada
  pinMode(buttonPin_atraso, INPUT);      //Inicializa o pino do botão como de entrada
  rtc.halt(false);    //Aciona o relogio
  lcd.begin(20,4);    //Inicializa LCD 20 x 4
  lcd.clear();        //Limpa LCD
  
// --- Configuração Inicial da Data e Hora do Sistema ---
  //Descomentar as linhas a seguir para configurar o horário, 
  //após comente e faça o upload novamente para o Arduino
//  rtc.setDOW(TUESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(20,59, 0);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(25,06,2019);
} //end setup

// --- Loop Infinito ---
void loop(){
  int i=0;
  
  if (!control){
    horario();
  }
  //Confere se está na hora do usuário comprar algum remédio
  if(Serial2.available()){
        not_compra = Serial2.read();
        Serial.print("recebeu: ");
        Serial.println(not_compra);
      }
      if(not_compra=='x'){
        Serial.print("recebeu: ");
        Serial.println(not_compra);
          Serial.println("mandou sinal de repor pro app");
          Serial1.println(2);
      }
  do{
    T = rtc.getTime();  //Variável que armazena o horário atual para fazer as comparações adequadamente
    hora = T.hour;
    minuto = T.min;
    segundo = T.sec;
  
    if ((hora==remedio[0].hora && (minuto==remedio[0].minuto)) || (hora==remedio[0].hora && (minuto==remedio[0].minuto + 1)) || (hora==remedio[0].hora + 1 && (minuto== 0))){
      control = 1;
      remedio[0].atraso=1;
      
      estado = digitalRead(buttonPin_tomouNaHora);
      if(estado){
          remedio[0].atraso=0;
          atualiza(0);
      }
      else{
          notificacao(0);
      }
    }
  
    if ((hora==remedio[1].hora && (minuto==remedio[1].minuto)) || (hora==remedio[1].hora && (minuto==remedio[1].minuto + 1)) || (hora==remedio[1].hora + 1 && (minuto== 0))){
      control = 1;
      remedio[1].atraso=1;
      
      estado = digitalRead(buttonPin_tomouNaHora);
      if(estado){
          remedio[1].atraso=0;
          atualiza(1);
      }
      else{
          notificacao(1);
      }
    }
    
    if((minuto==(remedio[0].minuto+2)) || (minuto==(remedio[1].minuto+2))){
      control=0;
      if(!notf){
        Serial.println("atraso");
        Serial1.println(12);//mandar a notificação para o aplicativo notificando o atraso
        notf=1;
      }
    }
  }while(control);
  
  if(!control){
    if((remedio[0].atraso==1 || remedio[1].atraso==1) && (digitalRead(buttonPin_atraso)==HIGH)){
      if(remedio[0].atraso==1){
        atualiza(0);
        notf=0;
      }
      if(remedio[1].atraso==1){
        atualiza(1);
        notf=0;
      }
    }
    horario();
    control=0;
    if(remedio[0].atraso==1 || remedio[1].atraso==1){
        delay(50);
        Serial2.write('2');
        Serial.println("mandou dois");   
    }
    estado = digitalRead(buttonPin_tomouNaHora);  
    delay (100); //taxa de atualização
  }
} //end loop

// ------------------- Imprime a hora/data -----------------------------------
void horario(){
  lcd.clear();
  lcd.setCursor(0,0);                 //Posiociona cursor na coluna 1, linha 2
  lcd.print(" Medicine Controller");  //Imprime a data
  lcd.setCursor(6,1);                 //Posiciona cursor na coluna 1, linha 1
  lcd.print(rtc.getTimeStr());        //Imprime horário
  lcd.setCursor(5,2);                 //Posiociona cursor na coluna 1, linha 2
  lcd.print(rtc.getDateStr(2,1,'/')); //Imprime a data
  lcd.setCursor(6,3);                 //Posiociona cursor na coluna 1, linha 2
  lcd.print(rtc.getDOWStr());         //Imprime a dia da semana  
} //end horario

// ------------------- Envia a notificação pro usuário ------------------------
void notificacao(int i){
  long tempo;

  tempo=millis();
  while(millis()-3000<tempo){
    Serial2.print('b');
    digitalWrite(vetLedPin[i], HIGH); //  Acende Luz de alerta
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("HORA DO REMEDIO!");
    lcd.setCursor(5,1);
    lcd.print(remedio[i].nome);   //  Imprime o nome do remédio
    lcd.setCursor(4,2);
    lcd.print(remedio[i].dosagem);  //  Imprime a dosagem do remédio
    delay(100);
  }
  if(!cont_aux){  //Envia notificação no celular do usuário
    Serial1.println(22);
    Serial.println("hora do remedio");
    cont_aux=1;
  }
} //end notificação

//--------------------- Atualiza o horário de tomar o remédio -------------------
void atualiza(int i){
    T = rtc.getTime();  //Variável que armazena o horário atual para fazer as comparações adequadamente
    hora = T.hour;
    minuto = T.min;
    segundo = T.sec;

    cont_aux=0;
    control=0;
    estado =0;
    if(i==0){
      Serial2.write('0');
      Serial.println("mandou zero");
    }
    delay(100);
    if(i==1){
      Serial2.write('1');
      Serial.println("mandou um");
    }
    digitalWrite(vetLedPin[i], LOW); //apaga o led do remédio correspondente
    
    //  Atualização do horário da próxima notificação caso haja atraso
    if(remedio[i].atraso){
      remedio[i].hora = remedio[i].periodo_entre_doses[0] + hora;
      if(remedio[i].hora>=24){
        remedio[i].hora= remedio[i].hora - 24;
      }
      remedio[i].minuto = remedio[i].periodo_entre_doses[1]+ minuto;
      if(remedio[i].minuto>=60){
        remedio[i].minuto= remedio[i].minuto - 60;
        remedio[i].hora= remedio[i].hora + 1;
      }
      remedio[i].atraso=0;
    }
    else{ //  Atualização do horário da próxima notificação caso não haja atraso
      remedio[i].hora = remedio[i].periodo_entre_doses[0] + remedio[i].hora;
      if(remedio[i].hora>=24){
        remedio[i].hora= remedio[i].hora - 24;
      }
      remedio[i].minuto = remedio[i].periodo_entre_doses[1]+ remedio[i].minuto;
      if(remedio[i].minuto>=60){
        remedio[i].minuto= remedio[i].minuto - 60;
        remedio[i].hora= remedio[i].hora + 1;
      }
      remedio[i].atraso=0;
    }
}
