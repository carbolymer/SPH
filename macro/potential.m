h=5;
magnitude=1;

x = 0.02:0.01:h;
% y = (-((h)^2-(x-h).^2).^3)/32/h.^7*35*magnitude;
p0                        =  2.35769e-06;
p1                        =     -7.82904;
p2                        =      6.18333;
p3                        =     -1.51929;
p4                        =     0.109357;
x = x./h.*3;
y = p0+p1.*x+p2.*x.*x+p3.*x.*x.*x+p4.*x.*x.*x.*x;
x=x./3.*h;
yrepul=-magnitude.*sqrt(x).+y.*magnitude;
dx = diff(x);
dy = -diff(yrepul);
plot(x,y,x(2:end),dy./dx,x,yrepul);
grid on;
ylim([-10,10]);

% plot(x,y);
% plot(x(2:end),dy);