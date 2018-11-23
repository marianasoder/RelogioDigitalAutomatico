
#include <LiquidCrystal.h>
//Define os pinos que serão utilizados para ligação ao display
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String aux;
char buff[22];
const String semana[] = {"Seg", "Ter", "Qua", "Qui", "Sex", "Sab", "Dom"};
const String condClim[] = {
  "   Tempestade",
  "  Chuviscando",
  "    Chovendo",
  "    Nevando",
  "    Nebuloso",
  "  Tempo limpo",
  "    Nublado"
};

int cont = 0;
int temp, umid, condicao;
int seg = 00, minu= 00, hor = 00; 
int dia = 00, mes = 00, ano = 0000, diaSem = 0;
const int diasPorMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};

bool checkAnoBis(){
  /*
   * Ano bissexto:
   *      - Divisivel por 4 e nao divisivel por 100
   *      - Nao Divisivel por 4 e divisivel por 400
   * 
   * Ref: https://escolakids.uol.com.br/calculo-do-ano-bissexto.htm
   */
   
  bool ret;
  
  if (ano%4 == 0){
    ret = (ano%100 != 0) ? true : false;
  } else {
    ret = (ano%400 == 0) ? true : false;
  }
  
  return ret;
}

void updateDia(){
  // Se for fevereiro e ano bissexto
  if (checkAnoBis() && ((mes) == 2)){
    dia = dia%29 + 1;
  } else {
    dia = dia%diasPorMes[mes-1] + 1; // Conta de 1 ate a qntd de dias
  }

  // Se o dia virou pro dia 1º
  // eh necessario virar mes
  if (dia == 1){
    mes = mes%12 + 1;

    // Se o dia e o mes voltaram ao 1
    // eh necessario virar o ano
    if (mes == 1){
      ano += 1;
    }
  }
  
}

void updateHora(){
  seg = (seg+1)%60;
  
  // se os segundos forem 0
  if(!seg){
    minu = (minu+1)%60;

    // se os min e os segs forem 0
    if(!minu){
      hor = (hor+1)%24;

      // checa se eh necessario
      // mudar o dia
      if(!hor){
        updateDia();
      }
    }
  }
  
  
}

void getData(){
  Serial.flush();
  
  Serial.println("data");

  Serial.flush();
  aux  = Serial.readString();

  aux.toCharArray(buff, 22);
  
  dia    = atoi(strtok(buff, " "));
  mes    = atoi(strtok(NULL, " "));
  ano    = atoi(strtok(NULL, " "));
  hor    = atoi(strtok(NULL, " "));
  minu   = atoi(strtok(NULL, " "));
  seg    = atoi(strtok(NULL, " "));
  diaSem = atoi(strtok(NULL, " "));
}

void getClima(){
  Serial.flush();

  Serial.println("weather");

  Serial.flush();
  aux = Serial.readString();

  aux.toCharArray(buff, 22);

  temp     = atoi(strtok(buff, " "));
  umid     = atoi(strtok(NULL, " "));
  condicao = atoi(strtok(NULL, " "));
  
}

void lcdPrintTwoDigNum(int num){
  char buffer[3];

  sprintf(buffer, "%02d", num);
  lcd.print(buffer);
}

void printData(){
    //Limpa a tela
    lcd.clear();
   
    lcd.setCursor(4, 0);
    lcdPrintTwoDigNum(hor);
    lcd.print("h");
    lcdPrintTwoDigNum(minu);
    lcd.print("m");
    lcdPrintTwoDigNum(seg);
    
    lcd.setCursor(1, 1);
    lcd.print(semana[diaSem]);
    lcd.print(" ");
    lcdPrintTwoDigNum(dia);
    lcd.print("/");
    lcdPrintTwoDigNum(mes);
    lcd.print("/");
    lcd.print(ano);
  
    delay(1000);
  
    updateHora();
}

void printClima(){
  //Limpa a tela
    lcd.clear();
   
    lcd.setCursor(2, 0);
    lcdPrintTwoDigNum(temp);
    lcd.print(" \337C / H ");
    
    lcdPrintTwoDigNum(umid);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print(condClim[condicao]);
  
    delay(1000);
}

void setup(){
  Serial.begin(9600);
  //Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //getData();
}
 
void loop()
{ 
  cont = 0;
  
  getData();
  while(cont < 20){
    printData();
    cont = (cont+1);
  }
  
  cont = 0;
  
  getClima();
  while(cont < 10){
    printClima();
    cont = (cont+1);
  }
}
