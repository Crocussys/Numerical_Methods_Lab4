import sys

N_DIGITS = 6


def usage():
    print("simple_iterations [path_a] [path_b]\n")
    print("path_a - Файл коэфициентов")
    print("path_b - Файл свободных членов")


def error(a, b, size, e):
    for i in range(size):
        if abs(a[i] - b[i]) > e:
            return True
    return False


def main():
    if len(sys.argv) >= 2:
        if sys.argv[1] == "-h":
            usage()
            return 0
    if len(sys.argv) < 3:
        usage()
        print("\nError 1: Not enough arguments")
        return -1
    size = 0
    a = []
    b = []
    try:
        with open(sys.argv[1], 'r') as file:
            count_lines = 0
            for line in file:
                count_lines += 1
                elems = line.split(" ")
                count_elems = len(elems)
                if size == 0:
                    size = count_elems
                if count_elems != size:
                    raise ValueError
                elems = list(map(float, elems))
                a.append(elems)
            if count_lines != size:
                raise ValueError

        with open(sys.argv[2], 'r') as file:
            count_elems = 0
            for line in file:
                elems = line.split(" ")
                elems = list(map(float, elems))
                count_elems += len(elems)
                for elem in elems:
                    b.append(elem)
            if count_elems != size:
                raise ValueError
    except OSError as e:
        print("Error 2: " + str(e))
        return -2
    except ValueError:
        print("Error 3: Invalid value")
        return -3

    x = [0] * size
    new_x = [b[i]/a[i][i] for i in range(size)]
    e = pow(10, -N_DIGITS - 1)
    while error(x, new_x, size, e):
        x = new_x.copy()
        for i in range(size):
            new_x[i] = b[i]
            for j in range(size):
                if j != i:
                    new_x[i] -= a[i][j] * x[j]
            new_x[i] /= a[i][i]

    for i in range(size):
        print(f"x{i + 1} = {round(x[i], N_DIGITS)}")

    return 0


if __name__ == "__main__":
    main()
