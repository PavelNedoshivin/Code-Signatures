clc;
clear;
close all;
m = [21, 19, 15];
t = [10, 11, 12];
n = 2.^m - 1;
k = n - m.*t;
result = zeros(1, 3);
for count = 1:3
    for i = 0:t(count)
        result(count) = result(count) + nchoosek(n(count), i);
    end
    result(count) = result(count) / 2^(n(count) - k(count));
    str = strcat('m=', num2str(m(count)), ', t=', num2str(t(count)));
    str = strcat(str, '    function=', num2str(result(count)));
    disp(str);
end