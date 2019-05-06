%% Paso alto amplitud
datos=xlsread('piTankGo.xlsx','Paso AltoF');
in=datos(:,1);
f=datos(:,2);
out=datos(:,3);
ph=datos(:,5);
semilogx(f,20*log10(out/in(1)),'rx-');
hold
semilogx([0.159 15.9 33500],[-40.4455 -0.4455 -0.4455],'k','LineWidth',2);
legend('Datos experimentales','Diagrama de Bode');
title('Respuesta en amplitud del filtro paso alto');
xlabel('Frecuencia(Hz)');
ylabel('Amplitud(dB)');
axis([1 3.4e4 -15 1]);
hold off
%% Paso alto fase
ph(ph>250)=-(ph(ph>250)-360);
ph=[[89.9 88.9 87.5]'; ph];
f=[[0.17 0.5 2.2]'; f]; 
semilogx(f,ph,'rx-');
hold;
semilogx([0.159 1.59 15.9 159.9 3400],[90 90 45 0 0],'k','LineWidth',2);
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
semilogx(f,20*log10(out/in),'rx-');
hold
semilogx([0.001 12e3 12e5],[0.001 00.001 -80],'k','LineWidth',2);
hold off
legend('Datos experimentales','Diagrama de Bode');
title('Respuesta en amplitud del filtro paso bajo');
xlabel('Frecuencia(Hz)');
ylabel('Amplitud(dB)');
axis([0 3e6 -80 1]);
hold off;
%% Paso bajo fase
ph(ph>300)=ph(ph>300)-360;
semilogx(f,-ph,'rx-');
hold;
semilogx([0.001 1.2e3 12e3 120000 240000], [0 0 -90 -180 -180],'k','LineWidth',2);
legend('Datos experimentales','Diagrama de Bode');
title('Respuesta en fase del filtro paso bajo');
xlabel('Frecuencia(Hz)');
ylabel('Fase en grados')


