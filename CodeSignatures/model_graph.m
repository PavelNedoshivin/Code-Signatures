clc;
clear;
close all;
n = zeros(1, 1);
n(1) = 31;
n(2) = 63;
n(3) = 127;
n(4) = 511;
for N = 1:2   
    table = bchnumerr(n(N));
    k = zeros(size(table, 1), 1);
    t = zeros(size(table, 1), 1);
    for i = 1:size(table)
        k(i) = table(i, 2);
        t(i) = table(i, 3);
    end
    result = zeros(size(t, 1), 1);
    for count = 1:size(t)
        result(count) = 0;
        for i = 0:t(count)
            result(count) = result(count) + nchoosek(n(N), i);
        end
        result(count) = result(count) / (2.^(n(N) - k(count)));
    end
    str = strcat('n=', num2str(n(N)));
    disp(str);
    for i = 1:size(t)
        str = strcat('t=', num2str(t(i)), ' f(t)=', num2str(result(i)));
        disp(str);
    end
    disp(' ');
end