clc;
clear;
close all;
N = 255;
K = 239;
S = 63;
gp = bchgenpoly(255, 239);
bchEncoder = comm.BCHEncoder(N, K, gp, S);
bchDecoder = comm.BCHDecoder(N, K, gp, S);
errorRate = comm.ErrorRate('ComputationDelay', 3);
for counter = 1:20
    data = randi([0 1], 630, 1);
    encodedData = bchEncoder(data);
    modSignal = pskmod(encodedData, 2);
    receivedSignal = awgn(modSignal, 5);
    demodSignal = pskdemod(receivedSignal, 2);
    receivedBits = bchDecoder(demodSignal);
    errorStats = errorRate(data, receivedBits);
end
fprintf('Error rate = %f\nNumber of errors = %d\n', errorStats(1), errorStats(2));