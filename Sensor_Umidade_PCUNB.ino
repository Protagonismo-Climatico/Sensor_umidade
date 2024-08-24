#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Endereço do LCD I2C e tamanho do display (16 colunas e 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // O endereço do I2C pode ser 0x27 ou 0x3F, dependendo do módulo

// Define o 100% e o 0% de umidade
const int umidadeMax = 760;   // Valor mínimo do sensor
const int umidadeMin = 880; // Valor máximo do sensor
char buffer[16]; 
char status[16];

void setup() {
  // Inicializa a comunicação serial
  Serial.begin(9600);
  
  // Inicializa o LCD com o número de colunas e linhas
  lcd.init();
  
  // Define o brilho do LCD (opcional, dependendo do módulo)
  lcd.backlight();
  

}

void loop() {
  // Lê o valor do sensor conectado ao pino analógico 0
  int valorSensor = analogRead(0);
  
  // Converte o valor do sensor para porcentagem
  int porcentagem = map(valorSensor, umidadeMin, umidadeMax, 0, 100);
  int umidade = constrain(porcentagem, 0, 100);
  
  // Formata o texto 
  sprintf(buffer, "Umidade: %d%%", umidade);  // Formata o texto

  // Imprime o valor da umidade no monitor serial
  Serial.println(umidade);
  Serial.println(valorSensor);
  Serial.println();
  // Move o cursor para a primeira coluna da primeira linha
  lcd.setCursor(0, 0); 
  
  // Limpa a linha do LCD com espaços
  lcd.print("                "); // Limpa a linha (16 espaços)
  
  // Move o cursor de volta para a primeira coluna da primeira linha
  lcd.setCursor(0, 0);

  
  // Exibe a string formatada no LCD
  lcd.print(buffer);

  lcd.setCursor(0, 1); // Move o cursor para a primeira coluna da segunda linha
  
  
  if (umidade < 30) {
    sprintf(status, "Estado: seco :(");  // Formata o texto
  }
  else if (umidade > 80) {
    sprintf(status, "Estado: alagado :(");  // Formata o texto
  }
  else {
    sprintf(status, "Estado: umido :)");  // Formata o texto
  }

  lcd.setCursor(0, 1); // Move o cursor para a nona coluna da primeira linha
  
  // Limpa a linha do LCD com espaços
  lcd.print("                "); // Limpa a linha (16 espaços)
  
    // Move o cursor de volta para a primeira coluna da primeira linha
  lcd.setCursor(0, 1);

  // Exibe o status do solo
  lcd.print(status);

  // Aguarda 500 milissegundos antes de atualizar o display
  delay(500);
}
