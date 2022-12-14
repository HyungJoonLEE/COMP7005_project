from matplotlib import pyplot as plt
import numpy as np
import os


# Need script or instruction that will install matplotlib in Linux


def draw_graph_data(proxy_value_list):
    x = np.arange(0, len(proxy_value_list), 1)
    y_value = []
    temp = 0

    for val in proxy_value_list:
        if val == '1':
            temp += 1
            y_value.append(temp)
        else:
            y_value.append(temp)

    plt.xlabel('total data packets sent')
    plt.ylabel('actual data packets arrived')
    plt.plot(x, y_value, 'b')
    plt.show()
    os.remove("./../../python_src/proxy_packet_send.txt")


def draw_graph_ack(ack_value_list):
    x = np.arange(0, len(ack_value_list), 1)
    y_value = []
    temp = 0

    for val in ack_value_list:
        if val == '1':
            temp += 1
            y_value.append(temp)
        else:
            y_value.append(temp)

    plt.xlabel('total ack packets sent')
    plt.ylabel('actual ack packets arrived')
    plt.plot(x, y_value, 'r')
    plt.show()
    os.remove("./../../python_src/proxy_ack_send.txt")



def process_file(file_pointer1, file_pointer2):
    value_string = file_pointer1.read()
    value_string2 = file_pointer2.read()
    draw_graph_ack(list(value_string2))
    draw_graph_data(list(value_string))


if __name__ == '__main__':
    proxy_packet_send = open("./../../python_src/proxy_packet_send.txt")
    proxy_ack_send = open("./../../python_src/proxy_ack_send.txt")

    process_file(proxy_packet_send, proxy_ack_send)
    # process_file(sender_ack_receive)
