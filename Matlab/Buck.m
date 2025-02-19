L = 68e-6; %68uH
C = 2001e-6; %2000uf
r = 0;
R = 10;
Vi = 16;
Vo = 3;
Ts  = 1/50e3;
D = 0.1875;
s = tf('s'); %传递函数变量
Hs = 1/(L/R*s+L*C*s^2+r*(C*s+1/R)+1);
% bode(Hs);
step(Hs*3,50e-2); 