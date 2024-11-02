#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define UMIDADE_MIN 760 // Valor mínimo do sensor
#define UMIDADE_MAX 880 // Valor máximo do sensor
#define MAX_MEDIDAS_SENSOR 10
#define INTERVALO 1000 // 100 ms
// Definição dos pinos
#define PINO_SENSOR_UMIDADE A0
#define PINO_RELE 7

int lerUmidadeSolo();
int obterMediaMedidasSensor();
int simularSensor();
void atualizarValorUmidade();
void atualizarEstadoUmidade();

// Endereço do LCD I2C e tamanho do display (16 colunas e 2 linhas)
LiquidCrystal_I2C lcd(0x27, 16, 2); // O endereço do I2C pode ser 0x27 ou 0x3F, dependendo do módulo

unsigned long tempoAnterior = 0;
int tempoDecorrido = 0;

int umidade;
int umidadeAnterior = -1;

char estado[10] = "         "; // Espaço para armazenar estado


void setup()
{
  // Inicializa a comunicação serial
  Serial.begin(9600);

  // Configura pino do role
  pinMode(PINO_RELE, OUTPUT);

  // Inicializa o LCD com o número de colunas e linhas
  lcd.init();

  // Define o brilho do LCD (opcional, dependendo do módulo)
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.setCursor(0, 1);
  lcd.print("Nivel: ");
}

void loop()
{

  umidade = simularSensor();

  // Só atualiza se a umidade mudou
  if (umidade != umidadeAnterior)
  {
    // Atualiza a primeira linha com a umidade
    atualizarValorUmidade();
    atualizarEstadoUmidade();

    // Armazena o valor atual da umidade
    umidadeAnterior = umidade;

    if (umidade >= 30)
    {
      digitalWrite(PINO_RELE, HIGH);
    }
    else
    {
      digitalWrite(PINO_RELE, LOW);
    }
  }

  delay(500);
}

void atualizarValorUmidade()
{
  char buffer[4];
  sprintf(buffer, "%d%%", umidade); // Formata o texto
  lcd.setCursor(11, 0);
  lcd.print("        ");
  lcd.setCursor(11, 0);
  lcd.print(buffer);
}

void atualizarEstadoUmidade()
{
  char novoEstado[10]; // Variável para armazenar o novo estado
  lcd.setCursor(7, 1); // Move o cursor para a posição do estado

  if (umidade < 30)
  {
    strcpy(novoEstado, "Seco :(");
  }
  else if (umidade > 80)
  {
    strcpy(novoEstado, "Alagado!");
  }
  else
  {
    strcpy(novoEstado, "Umido :D");
  }

  // Atualiza o LCD apenas se o estado mudou
  if (strcmp(novoEstado, estado) != 0)
  {
    strcpy(estado, novoEstado); // Atualiza o estado anterior
    lcd.print("         "); // Limpa a linha antes de imprimir o novo estado
    lcd.setCursor(7, 1); // Move o cursor para a posição do estado
    lcd.print(estado); // Imprime o novo estado
  }
}

int lerUmidadeSolo()
{
  // Lê o valor do sensor conectado ao pino analógico 0
  int mediaMedidas = obterMediaMedidasSensor();
  // Converte o valor do sensor para porcentagem
  int porcentagem = map(mediaMedidas, UMIDADE_MIN, UMIDADE_MAX, 0, 100);
  return constrain(porcentagem, 0, 100);
}

int obterMediaMedidasSensor()
{
  int mediaMedidas = 0;

  for (int i = 0; i < MAX_MEDIDAS_SENSOR; i++)
  {
    int valorSensor = analogRead(PINO_SENSOR_UMIDADE);
    delay(100);
    mediaMedidas += valorSensor;
  }
  mediaMedidas /= MAX_MEDIDAS_SENSOR;
  return mediaMedidas;
}

int simularSensor()
{
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoAnterior >= INTERVALO)
  {
    tempoAnterior = tempoAtual; // Atualiza o tempo anterior
    tempoDecorrido += 5;
    if (tempoDecorrido > 100)
    {
      tempoDecorrido = 0;
      delay(5000);
    }
  }
  return tempoDecorrido;
}
