/*
 * =====================================================================
 * SISTEMA DE CONTAGEM BIDIRECIONAL DE PESSOAS - VERSÃO 1.0
 * =====================================================================
 * 
 * Projeto: Sistema de Contagem Bidirecional
 * Arquivo: Sistema_Contagem_Bidirecional_3636800.ino
 * Autor: Roberto Leonel Stefan
 * RU: 3636800
 * Data: 25/02/2026
 * Versão: 1.0

 * =====================================================================

 * DESCRIÇÃO DO PROJETO:
  
 * Este sistema conta quantas pessoas entram e saem de um ambiente
 * utilizando sensores nas portas da frente e dos fundos
 * Funcionalidades:
 * - Conta pessoas que entram pela porta da frente
 * - Conta pessoas que saem pela porta da frente
 * - Conta pessoas que entram pela porta do fundo
 * - Conta pessoas que saem pela porta do fundo
 * - Mostra o número total de pessoas no ambiente em dois displays
 * - LED acende quando há pelo menos uma pessoa no ambiente e apaga quando vazio
 * - Botão de RESET para zerar a contagem manualmente
 * 
 * =====================================================================

 * COMPONENTES UTILIZADOS:
 * 
 * 1. Arduino Uno (cérebro do sistema)
 * 2. 1 par de Displays de 7 segmentos (mostram números de 0 a 99)
 * 3. 2 pares de Sensores infravermelho (fototransistor) na porta da frente
 * 4. 1 par de Sensor infravermelho (fototransistor) no fundo (externo)
 * 5. 1x LDR (sensor de luz) no fundo (interno)
 * 6. 1x LED (sensor de luz) no fundo (interno)
   7. 1x LED (lâmpada de presença de pessoas)
 * 8. 1x Botão (reset manual)
 * 9. Resistores diversos
  
 * =====================================================================

 * LIGAÇÃO DOS COMPONENTES:
 
 * DISPLAYS:
 * - Segmentos (a,b,c,d,e,f,g) -> Pinos 7,8,4,3,2,6,5
 * - Display ESQUERDO (unidades) -> Pino 9
 * - Display DIREITO (dezenas) -> Pino 11
  
 * SENSORES DA FRENTE:
 * - Sensor externo (EXT_FRENTE) -> Pino A5
 * - Sensor interno (INT_FRENTE) -> Pino A3
 
 * SENSORES DO FUNDO:
 * - Sensor externo (EXT_FUNDO) -> Pino A4
 * - LDR interno (INT_FUNDO) -> Pino A1
 
 * LED INDICADOR:
 * - LED (ânodo) -> Resistor 220Ω -> Pino A0
 * - LED (cátodo) -> GND
 
 * BOTÃO RESET:
 * - Botão -> Pino 13 e GND (sem resistor, usando pull-up interno)

 * =====================================================================

 * FUNCIONAMENTO BÁSICO:
 
 * Imagine duas portas: uma na frente e outra nos fundos

 * Quando alguém entra pela frente:
 *   1. Primeiro passa pelo sensor EXTERNO
 *   2. Depois passa pelo sensor INTERNO
 *   3. O sistema soma 1 pessoa
 
 * Quando alguém sai pela frente:
 *   1. Primeiro passa pelo sensor INTERNO
 *   2. Depois passa pelo sensor EXTERNO
 *   3. O sistema subtrai 1 pessoa
 
 * O mesmo acontece na porta dos fundos
 * O total de pessoas aparece nos displays
 * O LED acende quando tem gente no ambiente
 * O botão zera tudo quando pressionado
 * 
 * =====================================================================
 */

// =========================================================================
// SEÇÃO 1: DEFINIÇÃO DOS PINOS (onde cada componente está conectado)
// =========================================================================

// Pinos dos segmentos dos displays (a,b,c,d,e,f,g)
const int segs_6800[7]={7,8,4,3,2,6,5};

// Pinos que ligam/desligam cada display
const int disp1_6800=9;   // Display ESQUERDO (mostra as UNIDADES: 0 a 9)
const int disp2_6800=11;  // Display DIREITO (mostra as DEZENAS: 0 a 9)

// LED indicador de presença (acende quando tem gente no ambiente)
const int LED_6800 = A0;   // LED no pino A0

// Botão de RESET (zera o contador quando pressionado)
const int BOTAO_RESET_6800 = 13;  // Botão no pino 13

// SENSORES DA PORTA DA FRENTE
const int EXT_FRENTE_6800 = A5;  // Sensor de FORA da porta da frente
const int INT_FRENTE_6800 = A3;  // Sensor de DENTRO da porta da frente

// SENSORES DA PORTA DO FUNDO
const int EXT_FUNDO_6800 = A4;   // Sensor de FORA da porta do fundo (infravermelho)
const int INT_FUNDO_6800 = A1;   // Sensor de DENTRO da porta do fundo (LDR - sensor de luz)

// =========================================================================
// SEÇÃO 2: VARIÁVEIS GLOBAIS (memória do sistema)
// =========================================================================

// Contador principal - quantas pessoas estão no ambiente (0 a 99)
int contador_6800 = 0;

// Estas variáveis separam o contador em dois dígitos para mostrar nos displays
int dezenas_6800 = 0;   // Parte da esquerda (ex: em 23, vale 2)
int unidades_6800 = 0;  // Parte da direita (ex: em 23, vale 3)

// =========================================================================
// SEÇÃO 3: MÁQUINA DE ESTADOS DOS SENSORES
// (Controla a sequência de entrada/saída das pessoas)
// =========================================================================

// Estados da porta da FRENTE
byte estadoFrente_6800 = 0;        // 0 = parado, 1 = entrando, 2 = saindo
unsigned long tempoFrente_6800 = 0; // Relógio para controlar tempo máximo

// Estados da porta do FUNDO
byte estadoFundo_6800 = 0;          // 0 = parado, 1 = entrando, 2 = saindo
unsigned long tempoFundo_6800 = 0;  // Relógio para controlar tempo máximo

// Temporizadores de segurança
const int TIMEOUT_6800 = 1500;      // Se passar 1,5 segundos, cancela a contagem
const int TRAVA_6800 = 350;         // Espera 0,35 segundos entre contagens
unsigned long travaGlobal_6800 = 0; // Relógio para a trava

// =========================================================================
// SEÇÃO 4: CONTROLE DO BOTÃO (evita leituras falsas)
// =========================================================================

bool ultimoEstadoBotao_6800 = HIGH;     // Lembra o estado anterior do botão
unsigned long debounceTempo_6800 = 0;   // Relógio para controle de ruído
const int DEBOUNCE_DELAY_6800 = 50;     // Espera 50ms para confirmar o clique

// =========================================================================
// SEÇÃO 5: CONTROLE DOS DISPLAYS (multiplexação)
// =========================================================================

unsigned long ultimoDisplay_6800 = 0;   // Relógio para alternar os displays
const int TEMPO_DISPLAY_6800 = 8;       // Cada display fica aceso 8ms
bool mostraDezenas_6800 = true;         // Alterna entre dezenas e unidades

// =========================================================================
// SEÇÃO 6: TABELA DOS NÚMEROS (como cada número é formado nos displays)
// =========================================================================

// Cada número de 0 a 9 é formado por 7 segmentos (a,b,c,d,e,f,g)
// 1 significa segmento aceso, 0 significa apagado
//
// Exemplo: número 0 acende os segmentos a,b,c,d,e,f (todos exceto g)
//          número 1 acende apenas b,c
//
//    a
//   ___
// f|   |b
//  |_g_|
// e|   |c
//  |___|
//    d

const byte num_6800[10][7]={
  {1,1,1,1,1,1,0}, // 0 - acende a,b,c,d,e,f (apaga g)
  {0,1,1,0,0,0,0}, // 1 - acende b,c (apaga os outros)
  {1,1,0,1,1,0,1}, // 2 - acende a,b,d,e,g (apaga c,f)
  {1,1,1,1,0,0,1}, // 3 - acende a,b,c,d,g (apaga e,f)
  {0,1,1,0,0,1,1}, // 4 - acende b,c,f,g (apaga a,d,e)
  {1,0,1,1,0,1,1}, // 5 - acende a,c,d,f,g (apaga b,e)
  {1,0,1,1,1,1,1}, // 6 - acende a,c,d,e,f,g (apaga b)
  {1,1,1,0,0,0,0}, // 7 - acende a,b,c (apaga d,e,f,g)
  {1,1,1,1,1,1,1}, // 8 - acende todos os segmentos
  {1,1,1,1,0,1,1}  // 9 - acende a,b,c,d,f,g (apaga e)
};

// =========================================================================
// SEÇÃO 7: FUNÇÃO SETUP (roda UMA VEZ quando liga o Arduino)
// =========================================================================

void setup(){
  // Inicia a comunicação com o computador (para debug)
  Serial.begin(9600);
  Serial.println("=== SISTEMA DE CONTAGEM 6800 INICIADO ===");
  
  // -----------------------------------------------------------------------
  // Configura os pinos dos displays como saída (o Arduino vai controlá-los)
  // -----------------------------------------------------------------------
  
  // Configura os 7 pinos dos segmentos como saída
  for(int i=0;i<7;i++) {
    pinMode(segs_6800[i],OUTPUT);
  }
  
  // Configura os pinos que ligam/desligam cada display como saída
  pinMode(disp1_6800,OUTPUT);
  pinMode(disp2_6800,OUTPUT);
  
  // -----------------------------------------------------------------------
  // Configura o LED indicador
  // -----------------------------------------------------------------------

  pinMode(LED_6800, OUTPUT);      // Pino do LED como saída
  digitalWrite(LED_6800, LOW);    // Começa com LED apagado (ninguém no ambiente)
  
  // -----------------------------------------------------------------------
  // Configura o botão de RESET
  // -----------------------------------------------------------------------

  pinMode(BOTAO_RESET_6800, INPUT_PULLUP);  // Botão com resistor interno
  
  // -----------------------------------------------------------------------
  // Configura os sensores como entrada (o Arduino vai LER os sinais)
  // -----------------------------------------------------------------------

  pinMode(EXT_FRENTE_6800,INPUT);  // Sensor externo da frente
  pinMode(INT_FRENTE_6800,INPUT);  // Sensor interno da frente
  pinMode(EXT_FUNDO_6800,INPUT);   // Sensor externo do fundo
  pinMode(INT_FUNDO_6800,INPUT);   // LDR do fundo (sensor de luz)
  
  // -----------------------------------------------------------------------
  // Garante que os displays começam apagados
  // -----------------------------------------------------------------------

  digitalWrite(disp1_6800, LOW);
  digitalWrite(disp2_6800, LOW);
  
  Serial.println("Sistema pronto! Aguardando pessoas...");
}

// =========================================================================
// SEÇÃO 8: FUNÇÃO MOSTRAR DÍGITO (acende os segmentos de um número)
// =========================================================================

// Esta função recebe um número de 0 a 9 e acende os segmentos corretos
void mostrarDigito_6800(int digito_6800) {
  // Para cada segmento (0 a 6), manda o sinal correspondente da tabela
  digitalWrite(segs_6800[0], num_6800[digito_6800][0]);  // Segmento a
  digitalWrite(segs_6800[1], num_6800[digito_6800][1]);  // Segmento b
  digitalWrite(segs_6800[2], num_6800[digito_6800][2]);  // Segmento c
  digitalWrite(segs_6800[3], num_6800[digito_6800][3]);  // Segmento d
  digitalWrite(segs_6800[4], num_6800[digito_6800][4]);  // Segmento e
  digitalWrite(segs_6800[5], num_6800[digito_6800][5]);  // Segmento f
  digitalWrite(segs_6800[6], num_6800[digito_6800][6]);  // Segmento g
}

// =========================================================================
// SEÇÃO 9: FUNÇÃO LER SENSOR DIGITAL (com filtro de ruído)
// =========================================================================

bool lerDigital_6800(int p_6800){
  // Se o sensor detectar algo (sinal HIGH)
  if(digitalRead(p_6800)){
    // Espera 700 microssegundos (muito rápido, só para eliminar ruído)
    delayMicroseconds(700);
    // Verifica de novo para confirmar
    return digitalRead(p_6800);
  }
  // Se não detectou nada, retorna falso
  return false;
}

// =========================================================================
// SEÇÃO 10: FUNÇÃO LER LDR (sensor de luz do fundo)
// =========================================================================

bool lerLDR_6800(){
  // Lê o valor do LDR (0 a 1023)
  // Valores baixos (<100) = escuro (pessoa cobriu o sensor)
  // Valores altos (>600) = claro (sem ninguém)
  int valor_6800 = analogRead(INT_FUNDO_6800);
  
  // Se estiver escuro (pessoa cobriu), retorna VERDADEIRO
  if(valor_6800 < 100) return true;
  
  // Se estiver claro (sem ninguém), retorna FALSO
  if(valor_6800 > 600) return false;
  
  // Se estiver no meio, mantém o último estado (não muda)
  return false;
}

// =========================================================================
// SEÇÃO 11: FUNÇÃO PROCESSAR (coração do sistema - detecta entrada/saída)
// =========================================================================

void processar_6800(bool ext_6800, bool inte_6800, byte &estado_6800, unsigned long &tempo_6800){
  
  // Se demorar mais que 1,5 segundos, cancela a operação
  if(estado_6800 != 0 && millis() - tempo_6800 > TIMEOUT_6800) {
    estado_6800 = 0;  // Volta para estado parado
  }
  
  // Máquina de estados: controla a sequência de entrada/saída
  switch(estado_6800){
    
    // ESTADO 0: PARADO (ninguém passando)
    case 0:
      // Se o sensor EXTERNO foi ativado primeiro, a pessoa está ENTRANDO
      if(ext_6800) { 
        estado_6800 = 1;          // Vai para estado "ENTRANDO"
        tempo_6800 = millis();     // Marca o horário
      }
      // Se o sensor INTERNO foi ativado primeiro, a pessoa está SAINDO
      else if(inte_6800) { 
        estado_6800 = 2;          // Vai para estado "SAINDO"
        tempo_6800 = millis();     // Marca o horário
      }
      break;
    
    // ESTADO 1: ENTRANDO (primeiro sensor externo, agora espera o interno)
    case 1:
      // Se o sensor INTERNO foi ativado E já passou o tempo de trava
      if(inte_6800 && millis() - travaGlobal_6800 > TRAVA_6800) {
        // Se não estiver lotado (máximo 99 pessoas)
        if(contador_6800 < 99) {
          contador_6800++;  // SOMA UMA PESSOA
          // Mostra no computador para debug
          Serial.print("ENTRADA DETECTADA! Total de pessoas: ");
          Serial.println(contador_6800);
        }
        travaGlobal_6800 = millis();  // Atualiza a trava
        estado_6800 = 0;               // Volta para parado
      }
      break;
    
    // ESTADO 2: SAINDO (primeiro sensor interno, agora espera o externo)
    case 2:
      // Se o sensor EXTERNO foi ativado E já passou o tempo de trava
      if(ext_6800 && millis() - travaGlobal_6800 > TRAVA_6800) {
        // Se não estiver vazio (mínimo 0 pessoas)
        if(contador_6800 > 0) {
          contador_6800--;  // SUBTRAI UMA PESSOA
          // Mostra no computador para debug
          Serial.print("SAIDA DETECTADA! Total de pessoas: ");
          Serial.println(contador_6800);
        }
        travaGlobal_6800 = millis();  // Atualiza a trava
        estado_6800 = 0;               // Volta para parado
      }
      break;
  }
}

// =========================================================================
// SEÇÃO 12: FUNÇÃO BOTÃO PRESSIONADO (detecta clique com filtro)
// =========================================================================

bool botaoPressionado_6800() {
  // Lê o estado atual do botão (LOW = pressionado, HIGH = solto)
  bool estadoBotao_6800 = digitalRead(BOTAO_RESET_6800);
  
  // Se mudou de solto para pressionado, reinicia o contador de tempo
  if(estadoBotao_6800 == LOW && ultimoEstadoBotao_6800 == HIGH) {
    debounceTempo_6800 = millis();
  }
  
  // Se passou 50ms desde a primeira detecção
  if((millis() - debounceTempo_6800) > DEBOUNCE_DELAY_6800) {
    // Se o botão ainda está pressionado, é um clique válido
    if(estadoBotao_6800 == LOW) {
      ultimoEstadoBotao_6800 = estadoBotao_6800;
      return true;  // Botão foi pressionado
    }
  }
  
  // Salva o estado para a próxima comparação
  ultimoEstadoBotao_6800 = estadoBotao_6800;
  return false;  // Botão não foi pressionado
}

// =========================================================================
// SEÇÃO 13: FUNÇÃO VERIFICAR RESET (zera o contador se botão pressionado)
// =========================================================================

void verificarReset_6800() {
  // Se o botão foi pressionado...
  if(botaoPressionado_6800()) {
    contador_6800 = 0;  // Zera o contador
    Serial.println("*** RESET MANUAL ***");
    Serial.println("Contador zerado pelo botão!");
    
    // Feedback visual: LED pisca 3 vezes
    for(int i_6800 = 0; i_6800 < 3; i_6800++) {
      digitalWrite(LED_6800, HIGH);  // Acende
      delay(100);                     // Espera 0,1 segundo
      digitalWrite(LED_6800, LOW);   // Apaga
      delay(100);                     // Espera 0,1 segundo
    }
  }
}

// =========================================================================
// SEÇÃO 14: FUNÇÃO DETECTAR (lê todos os sensores e atualiza contagem)
// =========================================================================

void detectar_6800(){
  // Primeiro verifica se o botão de reset foi pressionado
  verificarReset_6800();
  
  // -----------------------------------------------------------------------
  // Processa a porta da FRENTE
  // -----------------------------------------------------------------------

  processar_6800(
    lerDigital_6800(EXT_FRENTE_6800),   // Lê sensor externo da frente
    lerDigital_6800(INT_FRENTE_6800),   // Lê sensor interno da frente
    estadoFrente_6800,                   // Estado atual da frente
    tempoFrente_6800                      // Temporizador da frente
  );
  
  // -----------------------------------------------------------------------
  // Processa a porta do FUNDO
  // -----------------------------------------------------------------------

  processar_6800(
    lerDigital_6800(EXT_FUNDO_6800),    // Lê sensor externo do fundo
    lerLDR_6800(),                       // Lê LDR do fundo
    estadoFundo_6800,                     // Estado atual do fundo
    tempoFundo_6800                        // Temporizador do fundo
  );
  
  // Garante que o contador nunca passa de 99 e nunca fica negativo
  contador_6800 = constrain(contador_6800, 0, 99);
  
  // Separa o contador em dezenas e unidades para mostrar nos displays
  dezenas_6800 = contador_6800 / 10;   // Ex: 23 / 10 = 2 (parte da esquerda)
  unidades_6800 = contador_6800 % 10;   // Ex: 23 % 10 = 3 (parte da direita)
  
  // -----------------------------------------------------------------------
  // Controla o LED indicador de presença
  // -----------------------------------------------------------------------

  if(contador_6800 > 0) {
    digitalWrite(LED_6800, HIGH);  // Tem gente no ambiente - LED ACESO
  } else {
    digitalWrite(LED_6800, LOW);   // Ambiente vazio - LED APAGADO
  }
}

// =========================================================================
// SEÇÃO 15: FUNÇÃO ATUALIZAR DISPLAY (mostra o número nos displays)
// =========================================================================

void atualizarDisplay_6800() {
  // A cada 8ms alterna entre os displays (multiplexação)
  if(millis() - ultimoDisplay_6800 >= TEMPO_DISPLAY_6800) {
    ultimoDisplay_6800 = millis();  // Atualiza o relógio
    
    // ---------------------------------------------------------------------
    // Desliga os dois displays (para evitar fantasmas)
    // ---------------------------------------------------------------------

    digitalWrite(disp1_6800, LOW);
    digitalWrite(disp2_6800, LOW);
    delayMicroseconds(50);  // Pequena pausa para garantir
    
    // ---------------------------------------------------------------------
    // Alterna entre mostrar dezenas e unidades
    // ---------------------------------------------------------------------

    if(mostraDezenas_6800) {
      // Mostra as DEZENAS no display DIREITO
      mostrarDigito_6800(dezenas_6800);
      digitalWrite(disp2_6800, HIGH);  // Acende display direito
    } else {
      // Mostra as UNIDADES no display ESQUERDO
      mostrarDigito_6800(unidades_6800);
      digitalWrite(disp1_6800, HIGH);  // Acende display esquerdo
    }
    
    // Prepara para mostrar o outro display na próxima vez
    mostraDezenas_6800 = !mostraDezenas_6800;
  }
}

// =========================================================================
// SEÇÃO 16: FUNÇÃO LOOP (roda SEM PARAR enquanto o Arduino estiver ligado)
// =========================================================================

void loop(){
  // 1. Verifica os sensores e atualiza a contagem
  detectar_6800();
  
  // 2. Atualiza os displays com o número atual
  atualizarDisplay_6800();
  
  // 3. Pequena pausa para dar tempo aos displays (estabilidade)
  delay(1);
}

// =========================================================================
// FIM DO CÓDIGO
// =========================================================================