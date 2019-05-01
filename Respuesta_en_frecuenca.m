%% Paso alto amplitud
datos=xlsread('piTankGo.xlsx','Paso AltoF');
in=datos(:,1);
f=datos(:,2);
out=datos(:,3);
ph=datos(:,5);
semilogx(f,20*log10(out),'rx-');
hold
semilogx([0.159 15.9 33500],[-34.75 5.25 5.25],'b');
legend('Datos experimentales','Diagrama de Bode');
title('Respuesta en amplitud del filtro paso alto');
xlabel('Frecuencia(Hz)');
ylabel('Amplitud(dB)');
axis([2 4e4 -9 8]);
hold off
%% Paso alto fase
ph(ph>250)=-(ph(ph>250)-360);
ph=[[89.9 88.9 87.5]'; ph];
f=[[0.17 0.5 2.2]'; f]; 
semilogx(f,ph,'rx-');
hold;
semilogx([0.159 1.59 15.9 159.9 3400],[90 90 45 0 0],'b');
legend('Datos experimentales','Diagrama de Bode');
title('Respuesta en fase del filtro paso alto');
xlabel('Frecuencia(Hz)');
ylabel('Fase en grados');

%% Paso bajo amplitud
datos=xlsread('piTankGo.xlsx','Paso bajo');
in=datos(:,1);
f=datos(:,2);
out=datos(:,3);
ph=datos(:,5);
semilogx(f,20*log10(out),'rx-');
hold
semilogx([10 12e3 12e5],[10.37 10.37 10.37-80]);
hold off
legend('Datos experimentales','Diagrama de Bode');
title('Respuesta en amplitud del filtro paso bajo');
xlabel('Frecuencia(Hz)');
ylabel('Amplitud(dB)');
axis([10 31000 0 11.5]);
hold off;
%% Paso bajo fase
semilogx(f,-ph,'rx-');
hold;
semilogx([10 1.2e3 12e3 120000 240000], [0 0 -90 -180 -180]);



