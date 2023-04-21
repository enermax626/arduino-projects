#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#define col 16 // Serve para definir o numero de colunas do display utilizado
#define row  2 // Serve para definir o numero de linhas do display utilizado
#define lcd_address  0x27 // Serve para definir o endereço do display.

LiquidCrystal_I2C lcd(lcd_address, col, row);

void setup() {
  lcd.init(); // Serve para iniciar a comunicação com o display já conectado
  lcd.setBacklight(HIGH);
}

void loop() {
  
  delay(2000);  // delay de 5 segundos com todas as mensagens na tela
  lcd.setCursor(0,0); // Coloca o cursor do display na coluna 1 e linha 1
  lcd.print("T:27C     16:43"); // Comando de saída com a mensagem que deve aparecer na coluna 2 e linha 1.
  lcd.setCursor(0, 1); //Coloca o cursor do display na coluna 1 e linha 2
  lcd.print("UA:66%    US:27%");  // Comando de saida com a mensagem que deve aparecer na coluna 2 e linha 2
  delay(3000);  // delay de 5 segundos com todas as mensagens na tela

  lcd.clear(); // Limpa o display até o loop ser reiniciado 
}
