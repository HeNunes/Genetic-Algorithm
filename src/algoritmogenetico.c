// Trabalho 1 de ICC1 - Algoritmo genético simplificado
// Enzo Nunes Sedenho & Pedro Augusto Monteiro Delgado.
// BCC - Junho de 2022.
// O objetivo do algoritmo é encontrar, se possível, uma raiz real de uma equação do quinto grau, cujos coeficientes são informados pelo usuário.
// Se o algoritmo encontrar uma raiz, ela será impressa e o programa se encerra. Caso contrário, a resposta mais próxima de uma raiz será impressa.

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define RESET  "\x1b[0m"

int main(void)
{
    // 'tam' indica o tamanho das populações, 'gen_max' indica o número máximo de gerações, 'elitismo_max' indica o coeficiente de elitismo.   
    int tam, gen_max, elitismo_max;

    // Imprimindo a mensagem inicial do programa
    printf("            A L G O R I T M O  G E N É T I C O\n           ");
    printf(YELLOW"¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨\n"RESET);
    printf("Este algoritmo busca encontrar uma raiz inteira exata entre\n");
    printf("-255 e 255 para equações de até 5º grau, cujos coeficientes\n"); 
    printf("são informados pelo usuário. O tamanho da população inicial\n");
    printf("o número máximo de gerações a serem criadas e o coeficiente\n");
    printf("de elitismo também são informados pelo usuário. O modelo da\n");
    printf("equação a ser informada está mostrado abaixo.\n");
    printf(YELLOW"===========================================================\n"RESET);
    printf("             ax⁵ + bx⁴ + cx³ + dx² + ex + f\n" );
    printf(YELLOW"===========================================================\n"RESET);
    printf(YELLOW"-> "RESET"O coeficiente de elitismo indica o limite tolerável para\n");
    printf("geração de populações elitizadas - indivíduos cujos valores\n");
    printf("estão muito próximos entre si, mas diferentes de uma raiz. \n");
    printf(YELLOW"-----------------------------------------------------------\n"RESET);
    
    // Lendo os valores informados pelo usuário e verificando se os valores são válidos
    printf(YELLOW">> "RESET"Informe o tamanho da população (nº par): ");
    scanf("%d", &tam);
    while(tam % 2 == 1 || tam <= 0)
    {
        printf(YELLOW"Atenção!"RESET" Informe um valor válido: ");
        scanf("%d", &tam);
    }

    printf(YELLOW">> "RESET"Informe o número máximo de gerações: ");
    scanf("%d", &gen_max);
    while(gen_max <= 0)
    {            
        printf(YELLOW"Atenção!"RESET" Informe um valor válido: ");
        scanf("%d", &gen_max);
    }

    printf(YELLOW">> "RESET"Informe o coeficiente de elitismo: ");
    scanf("%d", &elitismo_max);
    while(elitismo_max <= 0)
    {            
        printf(YELLOW"Atenção!"RESET" Informe um valor válido: ");
        scanf("%d", &elitismo_max);
    }

    // 'gena' e 'genb' armazenam os indivíduos das populações. 'prob' armazena as probabilidades de cada indivíduo ser selecionado,
    //'prob_mutacao' indica a probabilidade do bit analisado sofrer mutação e 'masc' é utilizada para realizar operações bitwise com os indivíduos durante a mutação e o cruzamento.
    // 'i', 'j', 'gen' e 'elitismo' são contadores. 
    // 'melhor_x', 'melhor_y' armazenam, para cada geração, o melhor indivíduo e seu valor na equação.
    // 'mais_proximo_x', 'mais_proximo_y' e 'mais_proximo_gen' armazenam, entre todas as gerações, o melhor indivíduo, seu valor na equação e sua geração.
    // 'a', 'b', 'c', 'd', 'e', 'f' são os coeficientes lineares da equação.
    int gena[tam], genb[tam], prob[tam], melhores_x[gen_max-1],  prob_mutacao, masc = 1, melhor_x, mais_proximo_x, media = 0;
    unsigned char i, j, gen = 0, mais_proximo_gen, elitismo = 0;
    long long int y, melhor_y, mais_proximo_y, melhores_y[gen_max-1];
    int a, b, c, d, e, f;   

    // Lendo os valores informados pelo usuário
    printf(YELLOW">> "RESET"Informe o coeficiente a: ");
    scanf("%d", &a);
    printf(YELLOW">> "RESET"Informe o coeficiente b: ");
    scanf("%d", &b);
    printf(YELLOW">> "RESET"Informe o coeficiente c: ");
    scanf("%d", &c);
    printf(YELLOW">> "RESET"Informe o coeficiente d: ");
    scanf("%d", &d);
    printf(YELLOW">> "RESET"Informe o coeficiente e: ");
    scanf("%d", &e);
    printf(YELLOW">> "RESET"Informe o coeficiente f: ");
    scanf("%d", &f);

    // Imprimindo a equação com base nos coeficientes lidos
    printf(YELLOW"\n===========================================================\n"RESET);
    printf("Equação informada: %dx⁵ + %dx⁴ + %dx³ + %dx² + %dx + %d\n", a, b, c, d, e, f);
    printf(YELLOW"===========================================================\n"RESET);

    // Gerando aleatoriamente a população inicial (gen0). Cada indíviduo possui 8 bits e possui chances iguais de ser positivo ou negativo.
    for(i = 0; i < tam; i++)
    {
        gena[i] = rand() % 256;
        if(rand() % 2 == 1)
        {
            gena[i] *= -1;
        }
    }

    // Definindo o limite de funcionamento do programa por meio de um laço
    while(gen < gen_max)
    {   
          
        // Inicializando o melhor indivíduo da geração (mais próximo da raiz) e o valor de sua equação.
        melhor_x = gena[0];
        melhor_y = a*pow(gena[0], 5) + b*pow(gena[0], 4) + c*pow(gena[0], 3) + d*pow(gena[0], 2) + e*gena[0] + f;
        
        // Inicializando, na geração 0, o melhor indivíduo de todas as gerações e o valor de sua equação.
        if(gen == 0)
        {
            mais_proximo_y = melhor_y;
            mais_proximo_x = melhor_x;
        }
       
        // Lendo toda a geração e procurando o melhor indivíduo.
        for(i = 0; i < tam; i++)
        {
            // Calculando a equação relativa ao indivíduo analisado.
            y = a*pow(gena[i], 5) + b*pow(gena[i], 4) + c*pow(gena[i], 3) + d*pow(gena[i], 2) + e*gena[i] + f;

            // Avaliando se o indivíduo é melhor que o então melhor indivíduo.
            if(labs(y) <= labs(melhor_y))
            {
                // Armazenando o melhor indivíduo da geração e o valor de sua equação.
                melhor_y = y;
                melhor_x = gena[i];
                melhores_x[gen] = melhor_x;
                melhores_y[gen] = melhor_y;

                // Armazenando o melhor indivíduo de todas as gerações, o valor de sua equação e a sua geração.
                if(labs(melhor_y) < labs(mais_proximo_y))
                {
                    mais_proximo_y = y;
                    mais_proximo_x = gena[i];
                    mais_proximo_gen = gen;
                }
            }
        }
        
        // Gerando o vetor relativo às probabilidades dos indivíduos de serem selecionados para a próxima geração.
        // O melhor indivíduo possui 99% de chance e o restante possui probabilidades proporcionais à essa.
        for(i = 0; i < tam; i++)
        {   
            y = a*pow(gena[i], 5) + b*pow(gena[i], 4) + c*pow(gena[i], 3) + d*pow(gena[i], 2) + e*gena[i] + f;

            // Calculando a probabilidade através de uma regra de 3 amortizada.
            prob[i] = 990*pow(labs(melhor_y), 0.25) / pow(labs(y), 0.25);

            // Corrigindo probabilidades muito pequenas para manter a coerência do algoritmo genético 
            // (todos os indivíduos devem apresentar uma chance de ser selecionados).
            if(prob[i] < 1)
            {
                prob[i] = 1;
            }
        }

        // Calculando a média aritmética entre os valores da população para auxiliar no controle de populações elitizadas.
        for(i = 0; i < tam; i++)
        {
            media += gena[i];
        }
        media /= tam;

        // Avaliando a distância entre a média dos indivíduos e o melhor deles. Caso seja menor ou igual a 1, a população é considerada elitizada.
        if(abs(media - melhor_x) <= 1)
        {
            elitismo++;
        }
        media = 0;

        // Imprimindo as informações sobre cada geração em forma de tabela.
        printf("\nInformações sobre a geração %d\n", gen);
        printf(YELLOW"¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨"RESET);
        printf("\n Indivíduo | Score |   Valor na equação \n");
        printf("-----------+-------+---------------------\n");

        for(i = 0; i < tam; i++)
        {
            printf(" [%02d] ", i);
            printf("%*d ", 4, gena[i]);

            y = a*pow(gena[i], 5) + b*pow(gena[i], 4) + c*pow(gena[i], 3) + d*pow(gena[i], 2) + e*gena[i] + f;

            if(melhor_y == 0)
            {
                if (gena[i] != melhor_x)
                {
                    printf("| ----- ");
                }
                else printf("| "YELLOW"RAIZ! "RESET);
            }
            else printf("| %*.1f%% ", 4, (float)prob[i]/10);
            printf("| %*lld\n", 19,  y);
        }
        printf(YELLOW"\n-> "RESET "O melhor indivíduo é " GREEN "x = %d " RESET "com" RED " y = %lld.\n" RESET, melhor_x, melhor_y);

        if(elitismo != 0)
        {
            printf(YELLOW"-> "RESET "A população está elitizada em %d/%d\n", elitismo, elitismo_max);
            if(elitismo == elitismo_max)
            {
                printf(YELLOW"ELITISMO DETECTADO!"RESET" Uma nova população será gerada!\n");
            }
        }
    
        // Verificando se melhor_y é 0, caso isso ocorra, melhor_x é raiz da equação e, assim, o programa pode ser encerrado.
        if(melhor_y == 0) 
        {
            if (gen_max == 1)
            {
                printf(YELLOW"\n===========================================================\n"RESET);
                printf("Em "YELLOW"uma"RESET" geração, o algoritmo encontrou que "GREEN"x = %d é raiz"RESET" da\n", melhor_x);
                printf("equação informada: %dx⁵ + %dx⁴ + %dx³ + %dx² + %dx + %d.", a, b, c, d, e, f);
                printf(YELLOW"\n===========================================================\n"RESET);
                break;
            }
            else
            {
                printf(YELLOW"\n===========================================================\n"RESET);
                printf("Em "YELLOW"%2d"RESET" gerações, o algoritmo encontrou que "GREEN"x = %d é raiz"RESET" da\n", gen+1, melhor_x);
                printf("equação informada: %dx⁵ + %dx⁴ + %dx³ + %dx² + %dx + %d", a, b, c, d, e, f);
                printf(YELLOW"\n===========================================================\n"RESET);
                break;
            }
        }     
        // A próxima geração será criada a partir de agora. Por isso, incrementamos o contador 'gen'.
        gen++;

        // Selecionando os indivíduos com base em suas probabilidades geradas anteriormente.
        for(i = 0, j = 0; (i < tam) && (j < (tam / 2)); i++)
        {
            if(prob[i] >= rand() % 1000)
            {
                genb[j] = gena[i];
                prob[i] = -1;
                j++;
            }
            // Caso o número de indivíduos selecionados não tenha atingido metade da população, 'i' é zerado.
            if(i == (tam - 1)) i = 0;
        }

        // Cruzando os indivíduos selecionados por meio de operações bit-a-bit para preencher o restante da população.
        for(i = 0; i < (tam/2); i++)
        {
            genb[i + (tam/2)] = 0;
            for(j = 0; j < 9; j++)
            {   
                masc = 1;
                if((abs(genb[i]) & ~(abs(genb[i + 1])) != 0)) // Indivíduos com, pelo menos, um bit em comum
                {
                    if((abs(genb[i]) & masc) == (abs(genb[i + 1]) & masc)) // Bits de mesmo valor
                    {
                        genb[i + (tam/2)] += (abs(genb[i]) & masc);   
                    }
                    else if(rand() % 2 == 1) // Bits de valores diferentes (50% de chance do filho receber 1 ou 0 no bit)
                    {
                        genb[i + (tam/2)] +=  masc;
                    }
                    if((j == 8) && ((genb[i] < 0) && (genb[i + 1] < 0))) // Bits de sinal iguais e negativos 
                    {
                        genb[i + (tam/2)] *= -1;
                    }
                    else if((j == 8) && ((genb[i] < 0) || (genb[i + 1] < 0))) // Bits de sinal diferentes (50% de chance do bit ser positivo ou negativo)
                    {
                        if(rand() % 2 == 1)
                        {
                            genb[i + (tam/2)] *= -1;
                        }
                    }
                    masc = masc << 1; // Deslocando o bit à ser analisado
                }
                else // Indivíduos com nenhum bit em comum
                {
                    j = 8;
                    genb[i + (tam/2)] += (genb[i] + genb[i + 1]) / 2;
                }
            }
        }

        // Aplicando mutação por meio de operações bit-a-bit com base na probabilidade de cada bit. Bits mais significativos possuem menor chance de sofrer mutação e vice-versa.
        masc = 1;
        for(i = (tam/2); i < tam; i++)
        {
            prob_mutacao = 512;
            for(j = 0; j < 8; j++)
            {
                if((rand() % 3000 < prob_mutacao) && genb[i] >= 0) // Mutação em números positivos 
                {
                    if(genb[i] & masc == 0)
                    {
                        genb[i] += masc; 
                    }
                    else genb[i] -= masc;
                }
                else if((rand() % 3000 < prob_mutacao) && genb[i] < 0) // Mutação em números negativos
                {
                    genb[i] *= -1;
                    if(genb[i] & masc == 0)
                    {
                        genb[i] += masc;
                    }
                    else genb[i] -= masc;
                    genb[i] *= -1;
                }
                prob_mutacao = prob_mutacao >> 1; // Dividindo por 2 a probabilidade de mutação do próximo bit
                masc << 1; // Deslocando o bit à ser analisado
            }
        }   

        // Quando o número de populações elitizadas atinge o limite informado pelo usuário, uma nova população é gerada assim como ocorreu com a geração 0.
        if(elitismo == elitismo_max) 
        {
            for(i = 0; i < tam; i ++)
            {
                genb[i] = rand() % 256;
                if(rand() % 2 == 1) genb[i] *= -1;
            }
            elitismo = 0;
        }

        // Reatribuindo o vetor 'gena' com os elementos de 'genb'.
        for(i = 0; i < tam; i++)
        {
            gena[i] = genb[i];
        }
        
        // Encerrando o programa caso o número máximo de gerações seja atingido, imprimindo o melhor indivíduo de todas as gerações, o valor de sua equação e a sua geração.
        if(gen >= gen_max) 
        {
            if (gen_max == 1)
            {
                printf(YELLOW"\n===========================================================\n"RESET);
                printf("Em "YELLOW"uma"RESET" geração, o algoritmo não encontrou uma raiz inteira \n");
                printf("entre -255 e 255 para a equação informada. Contudo, o valor\n");
                printf("mais próximo de uma raiz dessa equação foi encontrado, pri-\n");
                printf("meiramente na geração %d, sendo "GREEN"x = %d"RESET" e "RED"y = %lld.\n", mais_proximo_gen+1, mais_proximo_x, mais_proximo_y);
                printf(YELLOW"===========================================================\n"RESET);
                break;
            }
            else
            {
                printf(YELLOW"\n===========================================================\n"RESET);
                printf("Em "YELLOW"%2d"RESET" gerações, o algoritmo não encontrou uma raiz inteira\n", gen_max);
                printf("entre -255 e 255 para a equação informada. Contudo, o valor\n");
                printf("mais próximo de uma raiz dessa equação foi encontrado, pri-\n");
                printf("meiramente na geração %d, sendo "GREEN"x = %d"RESET" e "RED"y = %lld.\n", mais_proximo_gen+1, mais_proximo_x, mais_proximo_y);
                printf(YELLOW"===========================================================\n"RESET);
                break;
            }
        }
    }

    // Imprimindo os melhores indivíduos de cada geração e seus valores na equação.
    printf("\nEvolução das gerações\n");
    printf(YELLOW"¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨"RESET);
    printf("\n Gen | Melhor x | Valor na equação \n");
    printf("-----+----------+------------------\n");
    for(i = 0; i < gen; i++)
    {
        printf("%*d |", 4, i);
        printf("%*d |", 9, melhores_x[i]);
        printf("%*lld\n", 17, melhores_y[i]);
        
    }
    if(melhor_y == 0)
    {
        printf(YELLOW"%*d |", 4, i);
        printf("%*d |", 9, melhor_x);
        printf("%*lld\n", 17, melhor_y);
    }
    return 0;
}