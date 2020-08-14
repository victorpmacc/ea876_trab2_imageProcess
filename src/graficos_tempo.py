import numpy as np
import matplotlib.pyplot as plt

#Função de anotação na figura do boxplot
def annotate_boxplot(bpdict, texto ,annotate_params=None,
                     x_offset=0.05, x_loc=0,
                     text_offset_x= 50,
                     text_offset_y= 15):
    if annotate_params is None:
        annotate_params = dict(xytext=(text_offset_x, text_offset_y), textcoords='offset points')

    plt.annotate(texto, (x_loc + 1 + x_offset, bpdict['medians'][x_loc].get_ydata()[0]), **annotate_params)

#Leitura dos registros de tempo
tempos_linear = open("../doc/tempos_linear.txt", "r").read().split(',')
t1 = list(map(float, tempos_linear))
tempos_multiprocess = open("../doc/tempos_multiprocess.txt", "r").read().split(',')
t2 = list(map(float, tempos_multiprocess))
tempos_multithread = open("../doc/tempos_multithread.txt", "r").read().split(',')
t3= list(map(float, tempos_multithread))

t4 = [t1, t2, t3]

#Cálculo das médias e desvios padrão
t1m = np.mean(t1)
t1std = np.std(t1)
t2m = np.mean(t2)
t2std = np.std(t2)
t3m = np.mean(t3)
t3std = np.std(t3)

#Criação do boxplot
fig = plt.figure(1, figsize = (18,12))
ax = fig.add_subplot(111)
bp = ax.boxplot(t4, showmeans = True, meanline = True)
ax.set_xticklabels(['linear', 'multiprocess', 'multithread'])

#Inserção das anotações
textot1 = "Média: "+ str(t1m)[0:7] + "\nDp:" + str(t1std)[0:8]
textot2 = "Média: "+ str(t2m)[0:7] + "\nDp:" + str(t2std)[0:8]
textot3 = "Média: "+ str(t3m)[0:7] + "\nDp:" + str(t3std)[0:8]

annotate_boxplot(bp, textot1, x_loc=0)
annotate_boxplot(bp, textot2, x_loc=1)
annotate_boxplot(bp, textot3,x_loc=2)

plt.title("Distribuição do tempo de execução das versões de implementação do Blur")
plt.ylabel("segundos")
plt.xlabel("versão")

#Download do pdf com a figura
fig.savefig('../doc/resultados_nteste100.pdf', bbox_inches='tight')

