from time import sleep
from random import randint
from threading import Thread, Lock, Condition, current_thread

def produtor():
  global buffer
  global produzidos
  for i in range(10):
    sleep(randint(0,2))           # fica um tempo produzindo...
    item = 'item ' + str(i)
    if item in produzidos:
      continue
    with lock:
      if len(buffer) == tam_buffer:
        print('>>> Buffer cheio. Produtor ira aguardar.')
        lugar_no_buffer.wait()    # aguarda que haja lugar no buffer
      buffer.append(item)
      produzidos.append(item)
      print('Produzido %s (ha %i itens no buffer)' % (item,len(buffer)))
      item_no_buffer.notify()

def consumidor():
  global buffer
  global produzidos
  for i in range(10):
    with lock:
      while len(buffer) == 0:
        if len(produzidos) == 10:
          return
        print('>>> Buffer vazio. Consumidor ira aguardar.')
        item_no_buffer.wait()   # aguarda que haja um item para consumir 
      item = buffer.pop(0)
      print('Consumido %s (ha %i itens no buffer)' % (item,len(buffer)))
      lugar_no_buffer.notify()
    sleep(randint(0,2))         # fica um tempo consumindo...

produzidos = []
buffer = []
tam_buffer = 5
lock = Lock()
lugar_no_buffer = Condition(lock)
item_no_buffer = Condition(lock)

produtores = []
consumidores = []
for i in range(2):
  produtores.append(Thread(target=produtor, name=f"{i}")) 
  consumidores.append(Thread(target=consumidor, name=f"C{i}"))

for i in range(2):
  produtores[i].start()
  consumidores[i].start()

for i in range(2):
  produtores[i].join()
  consumidores[i].join()