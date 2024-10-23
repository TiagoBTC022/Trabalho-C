#define NOTA_La4  440 //Lá na quarta oitava do piano.
#define NOTA_La5  880 //Lá na quinta oitava do piano.
#define NOTA_Si5  987 //Si na quinta oitava.


// Notas da melodia principal (primeira oitava do tema de Halloween):
int melodia[] = 
{
  NOTA_La5, NOTA_La4, NOTA_La4, NOTA_La5, NOTA_La4, NOTA_La4,
  NOTA_La5, NOTA_La4, NOTA_Si5, NOTA_La4,
};



// Duração das notas: 4 = semínima, 8 = colcheia, etc.
//No tema de Halloween essas notas não são colcheias, mas elas compensam pela velocidade da música.
int duracaoNotas[] = 
{
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};



void setup() 
{
  // Código vazio pois a melodia vai ser tocada no loop.
}



void loop() {
  
  // Iterar sobre as notas da melodia.
  for (int notaAtual = 0; notaAtual < 10; notaAtual++)
    
  {
    // Calcula a duração da nota: 1000 ms (1 segundo) dividido pelo tipo de nota.
    //"8" no comando tone é indicação de qual porta vai sair.
    int duracaoNota = 1000 / duracaoNotas[notaAtual];
    tone(8, melodia[notaAtual], duracaoNota);
	
    // Definir uma pausa entre as notas (pausa de 60%).
    //% definido por tentativa e erro.
    //a pausa tem que ser maior que a nota pra não virar uma cacofonia.
    int pausaEntreNotas = duracaoNota * 1.60;
    delay(pausaEntreNotas);
    
    // Para o som por uma nota antes de tocar a próxima nota.
    noTone(8);
  }
}
