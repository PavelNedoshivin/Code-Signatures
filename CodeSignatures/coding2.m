clc;
clear;
close all;
n = 63;
k = 57;
t = bchnumerr(n, k);
s = RandStream('swb2712', 'Seed', 9973);
msg = gf(randi(s, [0 1], 1, k));
code = bchenc(msg, n, k);
cnumerr2 = zeros(nchoosek(n, 2), 1);
nErrs = zeros(nchoosek(n, 2), 1);
cnumerrIdx = 1;
for idx1 = 1:n-1
    sprintf('idx1 for 2 errors = %d', idx1);
    for idx2 = idx1+1:n
        errors = zeros(1, n);
        errors(idx1) = 1;
        errors(idx2) = 1;
        erroredCode = code + gf(errors);
        [decoded2, cnumerr2(cnumerrIdx)] = bchdec(erroredCode, n, k);
        if cnumerr2(cnumerrIdx) == 1
            code2 = bchenc(decoded2, n, k);
            nErrs(cnumerrIdx) = biterr(double(erroredCode.x), double(code2.x));
        end
        cnumerrIdx = cnumerrIdx + 1;
    end
end
plot(nErrs);
title('Number of Actual Errors');