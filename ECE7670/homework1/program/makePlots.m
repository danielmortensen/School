% values for simulation with p=0.5 in BPSK
theoreticalErrorp50n2Psk = [7.865e-02 5.628e-02 3.751e-02 2.288e-02 1.250e-02 5.954e-03 2.388e-03 7.727e-04 1.909e-04 3.363e-05 3.872e-06];
actualErrorp50n2psk = [7.826e-02 5.739e-02 3.598e-02 2.198e-02 1.328e-02 5.453e-03 2.412e-03 8.493e-04 2.003e-04 3.290e-05 4.036e-06];
snr = 0:10;
color1 = [0 0.4470 0.7410];
color2 = [0.8500 0.3250 0.0980];
color3 = [0.9290 0.6940 0.1250];
color4 = [0.4940 0.1840 0.5560];
figure; 

% plot theoretical vs actual for BPSK with P0 = 0.5
subplot(1,3,1);
p1 = semilogy(snr,theoreticalErrorp50n2Psk); hold on; 
p2 = semilogy(snr, actualErrorp50n2psk); 
p1.Color = color1;
p1.LineStyle = '--';
p2.Color = color1; 
p2.LineStyle = '-';
set(gca, 'YGrid','on');
title("BPSK Simulation with P_0 = 0.5");
legend("Theoretical Probability of Error","Observed Probability of Error");
xlabel('E_b/N_0(dB)');
ylabel('Probability of bit Error');

% plot theoretical vs actual for BPSK with P0 = 0.25
subplot(1,3,2);
actualErrorP25n2Psk = [ 6.452e-02 4.641e-02 3.258e-02 1.894e-02 1.040e-02 4.822e-03 1.987e-03 6.523e-04 1.721e-04 3.023e-05 3.599e-06];
theoreticalError = [6.492e-02 4.681e-02 3.140e-02 1.926e-02 1.058e-02 5.058e-03 2.036e-03 6.605e-04 1.636e-04 2.887e-05 3.330e-06];
p3 = semilogy(snr, theoreticalError); hold on; 
p3.Color = color2;
p3.LineStyle = '--';
p4 = semilogy(snr,actualErrorP25n2Psk);
p4.Color = color2;
p4.LineStyle = '-';
set(gca, 'YGrid','on');
title("BPSK Simulation with P_0 = 0.25");
legend("Theoretical Probability of Error","Observed Probability of Error");
xlabel('E_b/N_0(dB)');
ylabel('Probability of bit Error');

% plot theoretical vs actual for BPSK with P0 = 0.1
subplot(1,3,3);
theoreticalErrorP10nPsk2 = [3.900e-02 2.875e-02 1.966e-02 1.227e-02 6.832e-03 3.307e-03 1.345e-03 4.402e-04 1.098e-04 1.950e-05 2.260e-06];
actualErrorP10nPsk2 = [ 3.681e-02 3.109e-02 1.973e-02 1.240e-02 6.835e-03 3.256e-03 1.451e-03 4.416e-04 1.053e-04 1.941e-05 2.490e-06];
p5 = semilogy(snr, theoreticalErrorP10nPsk2); hold on; 
p5.Color = color3;
p5.LineStyle = '--';
p6 = semilogy(snr, actualErrorP10nPsk2);
p6.Color = color3;
p6.LineStyle = '-';
set(gca, 'YGrid','on');
title("BPSK Simulation with P_0 = 0.1");
legend("Theoretical Probability of Error","Observed Probability of Error");
xlabel('E_b/N_0(dB)');
ylabel('Probability of bit Error');

% create plot with the three probability values together.
figure; 
p7 = semilogy(snr, actualErrorp50n2psk); hold on; 
p8 = semilogy(snr, actualErrorP25n2Psk);
p9 = semilogy(snr, actualErrorP10nPsk2);
p7.Color = color1;
p8.Color = color2;
p9.Color = color3;
legend("Probability of One: 0.5","Probability of One: 0.25","Probability of One: 0.10");
set(gca, 'YGrid','on');
title("Comparison of Error Probability for Priors of 0.5, 0.25, and 0.1");
ylabel("Probability of Error");
xlabel("E_b/N_0(dB)");
%% 
figure;
theoreticalErrorP50n8Psk = [2.262e-01 2.140e-01 2.007e-01 1.862e-01 1.705e-01 1.538e-01 1.362e-01 1.179e-01 9.938e-02 8.101e-02 6.341e-02];
theoreticalErrorP50n8PskSymbol = [6.787e-01 6.421e-01 6.020e-01 5.585e-01 5.115e-01 4.614e-01 4.085e-01 3.538e-01 2.981e-01 2.430e-01 1.902e-01];
actualErrorP50n8Psk = [4.667e-01 4.562e-01 4.509e-01 4.344e-01 4.287e-01 4.180e-01 4.050e-01 3.915e-01 3.728e-01 3.669e-01 3.543e-01];
actualErrorP50n8PskSymbol = [5.841e-01 5.415e-01 4.977e-01 4.407e-01 3.860e-01 3.329e-01 2.869e-01 2.282e-01 1.671e-01 1.284e-01 8.662e-02];
p10 = semilogy(snr, theoreticalErrorP50n8Psk); hold on; 
p11 = semilogy(snr, actualErrorP50n8Psk);
p10.Color = color1;
p11.Color = color1;
p11.LineStyle = '--';

p12 = semilogy(snr, theoreticalErrorP50n8PskSymbol);
p13 = semilogy(snr, actualErrorP50n8PskSymbol);
p12.Color = color2;
p13.Color = color2;
p12.LineStyle = '--';
xlabel('E_b/N_0(dB)');
ylabel('Probability of Error');
legend('Estimated Bound on Bit Error','Actual Bit Error','Upper Bound on Symbol Error','Actual Symbol Error');
title("Comparison of Measured vs Error Bounds for Bits and Symbols");
set(gca, 'YGrid','on');

%% Plots for Hamming Code
theoreticalP50n2Psk = [7.865e-02 5.628e-02 3.751e-02 2.288e-02 1.250e-02 5.954e-03 2.388e-03 7.727e-04 1.909e-04 3.363e-05];
actualP50n2PskHamming74 = [5.556e-02 1.276e-02 5.176e-03 2.688e-03 8.897e-04 1.674e-04 4.741e-05 9.129e-06 2.408e-07 5.701e-09];
actualP50n2PskHamming1511 = [8.807e-02 4.097e-02 2.736e-02 1.032e-02 2.795e-03 7.247e-04 1.663e-04 1.501e-05 6.785e-07 1.816e-08];
figure; 
p14 = semilogy(snr,theoreticalErrorp50n2Psk); hold on;
p15 = semilogy(snr(1:10),actualP50n2PskHamming74);
p14.Color = color1;
p15.Color = color1;
p14.LineStyle = '--';
xlabel('E_b/N_0(dB)');
ylabel('Probability of Error');
legend('Estimated Bound for Uncorrected Bit Error','Bit Error with (7,4) Hamming Code');
title("Results for a (7,4) Hamming Code");
set(gca, 'YGrid','on');
figure; 
p16 = semilogy(snr,theoreticalErrorp50n2Psk); hold on;
p17 = semilogy(snr(1:10),actualP50n2PskHamming1511);
p16.Color = color2;
p17.Color = color2;
p16.LineStyle = '--';
xlabel('E_b/N_0(dB)');
ylabel('Probability of Error');
legend('Estimated Bound for Uncorrected Bit Error','Bit Error with (15,11) Hamming Code');
title("Results for a (15,11) Hamming Code");
set(gca, 'YGrid','on');
%% Sandbox
N0 = 1; Eb = 1;
sigma = sqrt(N0/2);
p1 = [0.5, 0.25, 0.1];
t = log(p1./(1 - p1))*(sigma^2)/(2*sqrt(Eb));...0.25*log(p1./(1 - p1)).*sqrt(N0/(2*Eb));
pErrorGiven0 = normcdf((t - sqrt(Eb))/sqrt(N0/2));
pErrorGiven1 = 1 - normcdf((t + sqrt(Eb))/sqrt(N0/2));
pError = pErrorGiven0.*(1 - p1) + pErrorGiven1.*p1;

%% Sandbox 2
% G = [1 1 0 1 0 0 0; 
%      0 1 1 0 1 0 0;
%      0 0 1 1 0 1 0;
%      0 0 0 1 1 0 1];
% H = [1 0 1 1 1 0 0; 
%      0 1 0 1 1 1 0;
%      0 0 1 0 1 1 1];

% G = rref(G);
% G = abs(G);
% G = mod(G,2);
% 
% H = rref(H);
% H = abs(H);
% H = mod(H,2);
% badIdx = 1:7;
% input = [1 1 0 1]
% code = mod(input*G,2)
% for ii = 1:numel(badIdx)
%     badCode = code;
%     badCode(badIdx(ii)) = mod(abs(code(badIdx(ii)) + 1),2);
%     syndrom = mod(H*badCode',2)
% end

P = [1 1 0 0;...
     1 0 1 0;...
     0 1 1 0;...
     1 1 1 0;...
     1 0 0 1;...
     0 1 0 1;...
     1 1 0 1;...
     0 0 1 1;...
     1 0 1 1;...
     0 1 1 1;...
     1 1 1 1];
G = [eye(11) P];
H = [P' eye(4)];
