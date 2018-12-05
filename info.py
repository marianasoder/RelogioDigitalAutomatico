# encoding: utf-8
# encoding: iso-8859-1

import urllib2 #para efetuar a requisicao
import json #para ler o JSON
from datetime import date, datetime
import serial, time

historicoClima = "23 83 Rain"
condClima = {
    'Thunderstorm': 0,
    'Drizzle': 1,
    'Rain': 2,
    'Snow': 3,
    'Atmosphere': 4,
    'Clear': 5,
    'Clouds': 6
}

def getWeather():
    global historicoClima

    try:
        data = urllib2.urlopen('http://api.openweathermap.org/data/2.5/weather?q=Maringá,BR&units=metric&lang=pt&'
        + 'id=5bf53151077e9b00073c66c8'
        + '&APPID=8ade0ae7af63671f6c2527291b4aece0')
    except:
        print "Erro ao tentar recuperar informacoes climaticas!"
        print "Enviando ultima informacao recebida..."
    else:
        html = data.read()
        weather = json.loads(html)
        temperatura = str(weather['main']['temp'])
        umidade = str(weather['main']['humidity'])
        condicao = condClima[weather['weather'][0]['main']]

        historicoClima = "{} {} {}".format(temperatura, umidade, condicao)


    return  historicoClima

def getDate():
    return  (datetime.now().strftime('%d %m %Y %H %M %S') + 
                " " + str(datetime.today().weekday())) 

def arduinoConnect():
    connected = False

    while not connected:
        try:
            conexao = serial.Serial('/dev/ttyACM0', 9600)
        except:
            print "Aguardando conexao..."
            time.sleep(2)
        else:
            connected = True

    print "Conectado!"

    # espera a conecao acontecer
    while(not conexao.isOpen()):
        pass

    print "Conexao estabelecida!"
    while(conexao.isOpen()):
        request = conexao.readline()[:-2]

        if(request == "weather"):
            print "\nRequisicao de clima recebida"
            info = getWeather()
        elif (request == "data"):
            print "\nRequisicao de data recebida"
            info = getDate()

        if (request in ['weather','data']):
            print "Enviando Dados..."
            print info
            conexao.write(info)
            print "Done!"
    
    print "Conexao fechada!"
    conexao.close()

if __name__ == '__main__':
    arduinoConnect()
