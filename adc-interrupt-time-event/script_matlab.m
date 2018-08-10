clear all
close all

dados = importdata('dados.log');

figure(1)
subplot(211), plot(dados)
subplot(212), plot([dados;dados])