# Aplicando Blur sob vários métodos: comparação entre execução única, multi-thread e multi-process

**O trabalho consiste em aplicar um filtro Blur sobre a imagem de um cachorro utilizando três versões de código diferentes: execução única, multi-thread e multi-process. O tempo de execução de cada etapa deve ser comparado - média e desvio padrão para 100 execuções de cada.**

**Instruções**
Os códigos em C, assim como a rotina de comparação entre os métodos em Python, encontram-se em `src`.
Os arquivos de análise dos tempos de execução econtram-se em `doc`.
Por facilidade, há um pdf fixo para distribuição de 100 testes em `doc`.
A imagem fonte está em `data`.
A imagem resultado (blur aplicado) para cada versão é produzida em `out`.

**Instruções Makefile**
No diretório raíz:
	`make`: compila as versões `.c`;
	`make test`: executa as 3 versões e gera gráfico comparativo em `doc`. OBS: Checar `QTTEST` em cada código `.c` antes de compilar. Pode demorar alguns minutos dependendo do número de testes;
	`make clean`: remove os executáveis `.o` da pasta `src`;
