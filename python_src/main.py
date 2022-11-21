from matplotlib import pyplot as plt
import numpy as np


def draw_graph(value_list, size):
    x = np.arange(0, len(value_list), 1)
    y_value = []
    temp = 0

    for val in value_list:
        if val == '1':
            temp += 1
            y_value.append(temp)
        else:
            y_value.append(temp)

    plt.plot(x, y_value, 'b', label='first')
    # plt.legend(loc='upper right')
    plt.show()


def process_file(file_pointer):
    value_string = file_pointer.read()
    print(list(value_string))
    draw_graph(list(value_string), len(value_string))


if __name__ == '__main__':
    sender_packet_send = open("../cmake-build-debug/sender_packet_send.txt")
    sender_ack_receive = open("../cmake-build-debug/sender_ack_receive.txt")
    proxy_packet_send = open("../cmake-build-debug/proxy_packet_send.txt")
    proxy_ack_send = open("../cmake-build-debug/proxy_ack_send.txt")
    receiver_packet_receive = open(
        "../cmake-build-debug/receiver_packet_receive.txt")

    process_file(sender_packet_send)
    process_file(sender_ack_receive)
    process_file(sender_ack_receive)



    # plt.plot(x, y, 'b', label='first')
    # plt.plot(x, y2, 'r', label='second')

