

from matplotlib import pyplot as plt
import numpy as np


def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press ⌘F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    x = np.arange(1, 10, 0.1)
    y = x * 0.2
    y2 = np.sin(x)

    plt.plot(x, y, 'b', label='first')
    plt.plot(x, y2, 'r', label='second')
    plt.xlabel('x axis')
    plt.ylabel('y axis')
    plt.title('matplotlib sample')
    plt.legend(loc='upper right')
    plt.show()

