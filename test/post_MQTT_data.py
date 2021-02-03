import paho.mqtt.client as mqtt
import sys

MQTT_ADDRESS = '192.168.46.198'
MQTT_USER = 'mickael'
MQTT_PASSWORD = 'mickael'



def on_publish(client, userdata, result):
    print('data published')
    pass


def main():

    print('Number of arguments:', len(sys.argv), 'arguments.')
    print('Argument List:', str(sys.argv))
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_publish = on_publish

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.publish("home/kfee",str(sys.argv[1])+str(sys.argv[2])+str(sys.argv[3]))
  


if __name__ == '__main__':
    print('post MQTT')
    main()