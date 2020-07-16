clc;
clear;
close all;
nfig = 1;
numExperiment = 1000;
n = 31;
table = bchnumerr(n);
k = zeros(size(table, 1), 1);
t = zeros(size(table, 1), 1);
for i = 1:size(table)
    k(i) = table(i, 2);
    t(i) = table(i, 3);
end
statsTry = zeros(size(t));
tmax = t(size(t));
errCorr = zeros(size(k, 1), n + 1);
figure(nfig);
hold on;
decAmount = 0;
for count = 1:size(k)
    curK = k(count);
    decTry = 0;
    for exp = 1:numExperiment
        i = 0;
        while (true)
            seq = zeros(1, n);
            seq(1:n) = randi([0 1], n, 1);
            [decoded, res] = bchdec(gf(seq), n, curK);
            decAmount = decAmount + 1;
            if res ~= -1
               decTry = decTry + i;
               errCorr(count, res + 1) = errCorr(count, res + 1) + 1;
               break;
            end
            i = i + 1;
        end
    end
    statsTry(count) = decTry / numExperiment;
    strLeg = strcat('t = ', num2str(t(count)));
    plot(0:n, errCorr(count, :), 'DisplayName', strLeg);
end
disp(decAmount);
title('Corrected errors spectre');
xlabel('t');
ylabel('Amount');
legend();
nfig = nfig + 1;
figure(nfig);
hold on;
plot(t, statsTry);
title('Dependece of decoding attempts on correcting ability');
xlabel('t');
ylabel('Attempts');
strN = strcat('n = ', num2str(n));
legend(strN);
nfig = nfig + 1;