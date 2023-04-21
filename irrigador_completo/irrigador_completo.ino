#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Sensor.h>
//Declaracao do pino conectado ao sensor
const int PINO_SENSOR_SOLO = A0;


//Declaracao do pino conectado ao rele
const int PINO_RELE = 2;

//Declaracao do pino do sensor de temperatura e umidade DHT22
#define DHTTYPE DHT22
const int PINO_SENSOR_TEMPERATURA = 5;

DHT dht(PINO_SENSOR_TEMPERATURA, DHTTYPE);

//#### LCD #####
#define col 16            // Serve para definir o numero de colunas do display utilizado
#define row 2             // Serve para definir o numero de linhas do display utilizado
#define lcd_address 0x27  // Serve para definir o endereço do display.

LiquidCrystal_I2C lcd(lcd_address, col, row);

//################

//#### BOTAO ####
//Pino botão ligar

const int PINO_BOTAO_LIGAR = 3;

int valor_botao_ligar = 0;
int ligado = 1;

//################

//#### LED INTERNO ARDUINO ####

const int LED_INTERNO = 13;

//#################

//Declaracao da variavel que armazena as leituras do sensor
int leitura_sensor_solo = 15;

int leitura_sensor_temperatura = 27;

//Declaracao da variavel auxiliar para o "delay" de 10 segundos
bool pausa = false;

long soma_pausa = 0;

//numero de vezes que foi regado

int num_regas = 0;

//Declaracao das variaveis que armazenam os valores de calibracao
const int VALOR_MAXIMO = 570;  //Valor com solo seco
const int VALOR_MINIMO = 190;  //Valor com solo umido
//Altere o valor das variaveis acima de acordo com as medias obtidas atraves do codigo do projeto anterior

//Declaracao das variaveis que armazenam a concentracao maxima e minima desejada
const int 
UMIDADE_SOLO_MINIMA = 64;
//Altere as variaveis acima com os valores ideais para a sua planta

void setup() {

  lcd.init();  // Serve para iniciar a comunicação com o display já conectado
  lcd.setBacklight(HIGH);

  dht.begin();

  //Define o pino conectado ao sensor como uma entrada do sistema
  pinMode(PINO_SENSOR_SOLO, INPUT);

  //Define o pino conectado ao sensor como uma entrada do sistema
  pinMode(PINO_SENSOR_TEMPERATURA, INPUT);

  //Pino botao ligar como INPUT no sistema
  pinMode(PINO_BOTAO_LIGAR, INPUT);

  //Led interno do arduino
  pinMode(LED_INTERNO, OUTPUT);

  //Define o pino conectado ao rele como uma saida do sistema
  pinMode(PINO_RELE, OUTPUT);

  //Inicia o pino conectado ao rele com nivel logico baixo
  digitalWrite(PINO_RELE, LOW);

  printarMensagemLCD("Pressione botao", "para iniciar");
}

void piscarLedInterno(int iterations, int delayValue) {
  for (int i = 0; i < iterations; i++) {
    digitalWrite(LED_INTERNO, HIGH);  // Turn the LED on
    delay(delayValue);                // Wait for 250ms
    digitalWrite(LED_INTERNO, LOW);   // Turn the LED off
    delay(delayValue);                // Wait for 250ms
  }
}

void verificarBotaoLigar() {
  valor_botao_ligar = digitalRead(PINO_BOTAO_LIGAR);
  if (valor_botao_ligar == HIGH) {
    //    piscarLedInterno(1, 1200);
    if (ligado == 1) {
      printarMensagemLCD("Desligando...", "");
      ligado = 0;
      pausa = false;
      soma_pausa=0;
      digitalWrite(PINO_RELE, LOW);
      piscarLedInterno(2, 140);
      delay(1200);
      lcd.clear();
      lcd.setBacklight(LOW);

    } else {
      lcd.setBacklight(HIGH);
      printarMensagemLCD("Iniciando...", "");
      piscarLedInterno(5, 140);
      delay(1600);
      ligado = 1;
    }
  }
}

void exibirDadosLCD(int valorUmidadeSolo, float valorUmidadeAr, float valorTemperatura, int num_regas) {
  lcd.clear();
  String line1 = String("Solo:" + String(valorUmidadeSolo, DEC) + " Ar:" + String(valorUmidadeAr, DEC));
  String line2 = String("Temp:" + String(valorTemperatura, DEC));

  lcd.setCursor(0, 0);   // Coloca o cursor do display na coluna 1 e linha 1
  lcd.print(line1);      // Comando de saída com a mensagem que deve aparecer na coluna 2 e linha 1.
  lcd.setCursor(0, 1);   //Coloca o cursor do display na coluna 1 e linha 2
  lcd.print(line2);      // Comando de saida com a mensagem que deve aparecer na coluna 2 e linha 2
  lcd.setCursor(10, 1);  //Coloca o cursor do display na coluna 1 e linha 2
  lcd.print(" r:"+String(num_regas,DEC)+"   ");  // Comando de saida com a mensagem que deve aparecer na coluna 2 e linha 2
}

void printarMensagemLCD(String linha1, String linha2) {
  lcd.clear();
  lcd.setCursor(0, 0);  // Coloca o cursor do display na coluna 1 e linha 1
  lcd.print(linha1);    // Comando de saída com a mensagem que deve aparecer na coluna 2 e linha 1.
  lcd.setCursor(0, 1);  //Coloca o cursor do display na coluna 1 e linha 2
  lcd.print(linha2);    // Comando de saida com a mensagem que deve aparecer na coluna 2 e linha 2
}


void exibirValorUmidadeDEBUG(int valorSensor) {
  int dezena = valorSensor / 10;
  int unidade = valorSensor % 10;
  piscarLedInterno(1, 3000);
  piscarLedInterno(dezena, 230);
  delay(2000);
  piscarLedInterno(unidade, 230);
}

float obterTemperatura() {
  float leitura = dht.readTemperature();
  long val = (long)(leitura * 10L);  // val = 1234
  leitura = (float)val / 10.0;       // x = 1234 / 10.0 = 123.4
  return leitura;
}

float obterUmidade() {
  float leitura = dht.readHumidity();
  long val = (long)(leitura * 10L);  // val = 1234
  leitura = (float)val / 10.0;       // x = 1234 / 10.0 = 123.4
  return leitura;
}

void debugPausa(){
    delay(2000);

    String line1 = String("p:" + String(pausa) + " vp" + String(soma_pausa, DEC));
    lcd.clear();
    lcd.setCursor(0, 0);   // Coloca o cursor do display na coluna 1 e linha 1
    lcd.print(line1);      // Comando de saída com a mensagem que deve aparecer na coluna 2 e linha 1.
    delay(2000);

}

void loop() {

  verificarBotaoLigar();
  if (ligado == 1) {
    //Realiza a leitura do sensor, a mapeia entre 0 e 100 % e exibe o valor no LCD
    leitura_sensor_solo = analogRead(PINO_SENSOR_SOLO);

    leitura_sensor_solo = map(leitura_sensor_solo, VALOR_MINIMO, VALOR_MAXIMO, 1, 99);

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float leitura_umidade_ar = obterUmidade();
    // Read temperature as Celsius (the default)
    float leitura_temperatura = obterTemperatura();



    exibirDadosLCD(leitura_sensor_solo, leitura_umidade_ar, leitura_temperatura, num_regas);

    if (pausa == false) {
      //Verifica se a leitura esta abaixo da concetracao minima desejada
      if (leitura_sensor_solo > UMIDADE_SOLO_MINIMA || leitura_umidade_ar < 68.0) {  //Se sim
      num_regas+=1;
        digitalWrite(PINO_RELE, HIGH);
        delay(8500);
        digitalWrite(PINO_RELE, LOW);
        pausa = true;  //Configura a variavel "pausa" como falsa
      }
    }

    debugPausa();
    //Verifica se a variavel "pausa" e verdadeira
    if (pausa == true) {  //Se sim
      if (soma_pausa > 600000) {
        pausa = false;
        soma_pausa=0;
      } else {
        delay(1500); 
        soma_pausa += 1500;
      }
    }
    delay(1400);
  }

}