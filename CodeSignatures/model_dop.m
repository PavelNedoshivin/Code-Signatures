clc;
clear;
close all;
nfig = 1;
exp = 100;
disp("exp = " + num2str(exp));
%[~, func] = test_first(exp);
%[decTry, p, nfig, t] = test_second(nfig, exp);
%[decTry, p, nfig] = test_third(nfig, t, decTry, p, exp);
%[t] = test_fourth(exp);

function [decTry, func] = test_first(exp)
    disp("FIRST");
    n = 7;
    k = 4;
    t = 1;
    decTry = 0;
    for i = 1:exp
        while (true)
            seq = zeros(1, n);
            seq(1:n) = randi([0 1], n, 1);
            [~, res] = bchdec(gf(seq), n, k);
            if res ~= -1
                break;
            end
            decTry = decTry + 1;
        end
    end
    decTry = decTry / exp;
    disp("attempts = " + num2str(decTry));
    func = 0;
    for i = 0:t
        func = func + nchoosek(n, i);
    end
    func = func / 2^(n - k);
    disp("function = " + num2str(func));
end

function [decTry, p, nfig, t] = test_second(nfig, exp)
    disp("SECOND");
    n = 15;
    table = bchnumerr(n);
    k = table(:, 2);
    t = table(:, 3);
    figure(nfig);
    hold on;
    decTry = zeros(size(t));
    for i = 1:size(t)
       for j = 1:exp
           while (true)
               seq = zeros(1, n);
               seq(1:n) = randi([0 1], n, 1);
               [~, res] = bchdec(gf(seq), n, k(i));
               if res ~= -1
                   break;
               end
               decTry(i) = decTry(i) + 1;
           end
       end
       decTry(i) = decTry(i) / exp;
    end
    plot(t, decTry);
    title("Dependece of decoding attempts on correcting ability (exp = " + num2str(exp) + ")");
    xlabel('t');
    ylabel('Attempts');
    legend('n = 15');
    nfig = nfig + 1;
    p = zeros(size(t));
    for i = 1:size(t)
        func = 0;
        for j = 0:t(i)
            func = func + nchoosek(n, j);
        end
        func = func / 2^(n - k(i));
        disp("t = " + num2str(t(i)) + "    f(t) = " + num2str(func));
        p(i) = func;
    end
end

function [decTry, p, nfig] = test_third(nfig, t, decTry, p, exp)
    disp("THIRD");
    figure(nfig);
    hold on;
    plot(t, decTry);
    p1 = 1 ./ p;
    p2 = (1 - p) ./ p;
    plot(t, p1);
    plot(t, p2);
    title("Dependence of decoding attempts (n = 15, exp = " + num2str(exp) + ")");
    xlabel('t');
    ylabel('Attempts');
    legend('model', '1 / p', '(1 - p) / p');
    nfig = nfig + 1;
    for i = 1:size(t)
        str = strcat("t = ", num2str(t(i)), "    attempts = ", num2str(decTry(i)));
        str = strcat(str, "    1 / p = ", num2str(p1(i)), "    (1 - p) / p = ", num2str(p2(i)));
        disp(str);
    end
    figure(nfig);
    hold on;
    n = 31;
    table = bchnumerr(n);
    k = table(:, 2);
    t = table(:, 3);
    decTry = zeros(size(t));
    for i = 1:size(t)
        for j = 1:exp
            seq = zeros(1, n);
            seq(1:n) = randi([0 1], n, 1);
            [~, res] = bchdec(gf(seq), n, k(i));
            if res ~= -1
                break;
            end
            decTry(i) = decTry(i) + 1;
        end
        decTry(i) = decTry(i) / exp;
    end
    p = zeros(size(t));
    for i = 1:size(t)
        for j = 0:t(i)
            p(i) = p(i) + nchoosek(n, j);
        end
        p(i) = p(i) / 2^(n - k(i));
    end
    plot(t, decTry);
    p1 = 1 ./ p;
    p2 = (1 - p) ./ p;
    plot(t, p1);
    plot(t, p2);
    title("Dependence of decoding attempts (n = 31, exp = " + num2str(exp) + ")");
    xlabel('t');
    ylabel('Attempts');
    legend('model', '1 / p', '(1 - p) / p');
    for i = 1:size(t)
        str = strcat("t = ", num2str(t(i)), "    attempts = ", num2str(decTry(i)));
        str = strcat(str, "    1 / p = ", num2str(p1(i)), "    (1 - p) / p = ", num2str(p2(i)));
        disp(str);
    end
    nfig = nfig + 1;
end

function [t] = test_fourth(exp)
    disp("FOURTH");
    n = 15;
    table = bchnumerr(n);
    k = table(:, 2);
    t = table(:, 3);
    disp("n = 15");
    for i = 1:size(k)
        disp("t = " + num2str(t(i)));
        for j = 0:(2^k(i) - 1)
            str = dec2bin(j, k(i));
            seq = str2num(str, k(i));
            enc = bchenc(gf(seq), n, k(i));
            disp(enc);
        end 
    end
    n = 31;
    table = bchnumerr(n);
    k = table(:, 2);
    t = table(:, 3);
    disp("n = 31");
    for i = 1:size(k)
        disp("t = " + num2str(t(i)));
        for j = 0:(2^k(i) - 1)
            str = dec2bin(j, k(i));
            seq = str2num(str);
            enc = bchenc(gf(seq), n, k(i));
            disp(enc);
        end 
    end
end
