N_DIGITS = 6

function usage()
    mprintf("simple_iterations [path_a] [path_b]\n\n")
    mprintf("path_a - Файл коэфициентов\n")
    mprintf("path_b - Файл свободных членов\n")
endfunction

function ValueError()
    mprintf("Error 3: Invalid value\n")
    return -3
endfunction

function [txt, err]=txt_in_file(file_name)
    [fl, err] = mopen(file_name, 'r')
    if err ~= 0 then
        mprintf("Error 2: File opening error\n")
        return
    end
    txt = mgetl(fl)
    mclose(fl)
endfunction

argv = sciargs()
[m, argc] = size(argv)
if argc >= 3 then
    if argv(3) == "-h" then
        usage()
        return 0
    end
end
if argc < 4 then
    usage()
    mprintf("\nError 1: Not enough arguments")
    return -1
end

len = 0
a= []
b= []
txt = []
[txt, err] = txt_in_file(argv(3))
if err ~= 0 then
    return -2
end
[n, m] = size(txt)
for i = 1:n do
    elems = strsplit(txt(i), " ")
    [count_elems, m] = size(elems)
    if len == 0 then
        len = count_elems
    end
    if count_elems ~= len then
        return ValueError()
    end
    line = strtod(elems)
    for j = 1:count_elems do
        if line(j) == %nan then
            return ValueError()
        else
            a(i, j) = line(j)
        end
    end
end
[txt, err] = txt_in_file(argv(4))
if err ~= 0 then
    return -2
end
count_elems = 0
[n, m] = size(txt)
for i = 1:n do
    elems = strsplit(txt(i), " ")
    [count, m] = size(elems)
    count_elems = count_elems + count
    digits = strtod(elems)
    for j = 1:count do
        if digits(j) == %nan then
            return ValueError()
        end
        b($+1) = digits(j)
    end
end
if count_elems ~= len then
    return ValueError()
end

x = []
new_x = []
for i = 1:len do
    x(i) = 0
    new_x(i) = b(i) / a(i, i)
end
e = 10 ** (-N_DIGITS - 1)
while 1 do
    flag = 0
    for i = 1:len do
        if abs(x(i) - new_x(i)) > e then
            flag = 1
            break
        end
    end
    if flag == 0 then
        break
    end
    x = new_x
    for i = 1:len do
        new_x(i) = b(i)
        for j = 1:len do
            if j ~= i then
                new_x(i) = new_x(i) - a(i, j) * x(j)
            end
        end
        new_x(i) = new_x(i) / a(i, i)
    end
end

for i = 1:len do
    mprintf("x%d = %f\n", i, x(i))
end

return 0
